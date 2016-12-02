#include "sandbox.h"


LimitList :: LimitList() {}	
LimitList :: LimitList(int tl, int ml) {
	time_lim = tl;
	memory_lim = ml;
}


RunConfig :: RunConfig() {}	
RunConfig :: RunConfig(bool ic, bool us, bool il, char* rp, char* ifl, char* ofl, char** ag, LimitList lims_) {
	is_compilation = ic;
	use_sandbox = us;
	is_limited = il;
	run_program = rp;
	in_file = ifl;
	out_file = ofl;
	argv = ag;
	lims = lims_;
}


RunResult :: RunResult() {}
RunResult :: RunResult(int ut, int um, int rs, bool je) {
	use_time = ut;
	use_memory = um;
	run_status = rs;
	judger_error = je;
}


int SandBox :: load_syscal_list(const RunConfig &RCFG) {
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
	
	if (! ctx) {
		REPORTER((char*)"Initialize system call fail");
		return -1;
	}
	
	for (int i = 1; i <= syscal_white_list[0]; i++) {
		if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, syscal_white_list[i], 0)) {
			REPORTER((char*)"Add system call fail.");
			return -1;
		}
	}
	
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, 
		SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)RCFG.run_program))) {
		REPORTER((char*)"Add system call fail.");
		return -1;
	}
	
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 2, 
		SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)""),
		SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t)(O_RDONLY|O_CLOEXEC)))) {
		REPORTER((char*)"Add system call fail.");
		return -1;
	}
	
	if (seccomp_load(ctx)) {
		REPORTER((char*)"Load system call fail.");
		return -1;
	}

	return 0;
}
	
int SandBox :: load_limit(const RunConfig &RCFG) {
	rlimit rtmp;
	
	rtmp.rlim_max = TIMETOP;
	rtmp.rlim_cur = (long int)(RCFG.lims.time_lim * 1.3 / 1000) + 1 < rtmp.rlim_max ?
					(long int)(RCFG.lims.time_lim * 1.3 / 1000) + 1 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_CPU, &rtmp)) {
		REPORTER((char*)"Load cpu time limit fail.");
		return -1;
	}
	
	// need 32-bit system solotion
	rtmp.rlim_max = MEMTOP;
	rtmp.rlim_cur = 2 * RCFG.lims.memory_lim * 1024 * 1024 < rtmp.rlim_max ?
					2 * RCFG.lims.memory_lim * 1024 * 1024 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_AS, &rtmp)) {
		REPORTER((char*)"Load memory limit fail.");
		return -1;
	}

	rtmp.rlim_max = 100 * 1024 * 1024;
	rtmp.rlim_cur = 100 * 1024 * 1024;
	if (setrlimit(RLIMIT_FSIZE, &rtmp)) {
		REPORTER((char*)"Load output size limit fail.");
		return -1;
	}
	
	rtmp.rlim_max = MEMTOP;
	rtmp.rlim_cur = 2 * RCFG.lims.memory_lim * 1024 * 1024 < rtmp.rlim_max ?
					2 * RCFG.lims.memory_lim * 1024 * 1024 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_STACK, &rtmp)) {
		REPORTER((char*)"Load stack size limit fail.");
		return -1;
	}

	return 0;
}

void SandBox :: runner(const RunConfig &RCFG, RunResult &RES) {
	RES = RunResult(0, 0, 0, SIGABRT);
	rusage Ruse;
	int status_val;
	
	pid_t s_pid = vfork();
	
	if (s_pid < 0) {
		REPORTER((char*)"Main vfork fail.");
		return;
		
	} else if (s_pid == 0) {
		if ((not RCFG.is_compilation) and freopen(RCFG.in_file, "r", stdin) == NULL) {
			REPORTER((char*)"Freopen in fail.");
			return;
		}
		
		if (freopen(RCFG.out_file, "w", stdout) == NULL) {
			REPORTER((char*)"Freopen out fail.");
			return;
		}
		
		if (RCFG.is_compilation and freopen(RCFG.out_file, "w", stderr) == NULL) {
			REPORTER((char*)"Freopen out fail.");
			return;
		}
		
		if (RCFG.is_limited) if (load_limit(RCFG)) return;
		if (RCFG.use_sandbox) if (load_syscal_list(RCFG)) return;
	
		if (RCFG.is_compilation) execvp(RCFG.run_program, RCFG.argv);
		else execve(RCFG.run_program, RCFG.argv, NULL);
		
		REPORTER((char*)"Execve fail");
		return;
		
	} else {
		pid_t surveillant = fork();
		
		if (surveillant < 0) {
			REPORTER((char*)"Run surveillant fail.");
			return;
			
		} else if (surveillant == 0) {
			int time_lim = (RCFG.lims.time_lim * 1.3 / 1000) + 1 < TIMETOP ? 
							(RCFG.lims.time_lim * 1.3 / 1000) + 1 : TIMETOP;
			sleep(time_lim);
			kill(s_pid, SIGKILL);
			exit(0);
			
		} else {
			if (wait4(s_pid, &status_val, 0, &Ruse) == -1) {
				REPORTER((char*)"Wait child fail.");
				return;
			}
			
			kill(surveillant, SIGKILL);
			
			RES.run_status = WTERMSIG(status_val);
			RES.judger_error = 0;
			RES.use_time += (int)((Ruse.ru_stime.tv_sec + Ruse.ru_utime.tv_sec) * 1000);
			RES.use_time += (int)((Ruse.ru_stime.tv_usec + Ruse.ru_utime.tv_usec) / 1000);
			RES.use_memory = (int)(Ruse.ru_maxrss / 1024);
		}
	}
}

/*----------------------for test--------------------------
int main() {
	RunConfig RCF(true, true, (char*)"./lll", (char*)"lll.in", (char*)"lll.out", LimitList(1000, 64));
	RunResult RES;
	SandBox SB;
	SB.runner(RCF, RES);
	printf("time_use = %d, mem_use = %d, return_val = %d judge_error = %d\n" , RES.use_time, RES.use_memory, RES.run_status, RES.judger_error);
	return 0;
}
*/

