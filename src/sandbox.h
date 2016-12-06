#ifndef SANDBOX_H
#define SANDBOX_H


#include <seccomp.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h> 
#include "tools.h"

const int TIMETOP = 60;
const long long MEMTOP = 1024 * 1024 * 1024;


const int syscal_white_list[32] = {16,
	SCMP_SYS(close), SCMP_SYS(read),
	SCMP_SYS(write), SCMP_SYS(tgkill),
	SCMP_SYS(brk), SCMP_SYS(rt_sigprocmask),
	SCMP_SYS(access), SCMP_SYS(mmap),
	SCMP_SYS(mprotect), SCMP_SYS(fstat),
	SCMP_SYS(arch_prctl), SCMP_SYS(munmap),
	SCMP_SYS(exit_group), SCMP_SYS(writev)
};


class LimitList {
public:
	int time_lim;
	int memory_lim;
	int output_lim;
	
	LimitList();	
	LimitList(int, int, int);
};


class RunConfig {
public:
	bool is_compilation;
	bool use_sandbox;
	bool is_limited;
	char* run_program;
	char* in_file;
	char* out_file;
	char** argv;
	LimitList lims;
	
	RunConfig();
	RunConfig(bool, bool, bool, char*, char*, char*, char**, LimitList);
};


class RunResult {
public:
	int use_time;
	int use_memory;
	int run_signal;
	int return_value;
	bool judger_error;
	
	RunResult();
	RunResult(int, int, int, int, bool);
};


class SandBox {
private:
	int load_syscal_list(const RunConfig&);
	
	int load_limit(const RunConfig&);
public:
	int runner(const RunConfig&, RunResult&);
};


LimitList :: LimitList() {}	
LimitList :: LimitList(int tl, int ml, int opl) {
	time_lim = tl;
	memory_lim = ml;
	output_lim = opl;
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
RunResult :: RunResult(int ut, int um, int rs, int rv, bool je) {
	use_time = ut;
	use_memory = um;
	run_signal = rs;
	return_value = rv;
	judger_error = je;
}


int SandBox :: load_syscal_list(const RunConfig &RCFG) {
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
	
	if (not ctx) {
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
	
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 1,
		SCMP_A0(SCMP_CMP_EQ, 0))) {
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
	rtmp.rlim_cur = (unsigned long int)(RCFG.lims.time_lim * 1.3 / 1000) + 1 < rtmp.rlim_max ?
					(unsigned long int)(RCFG.lims.time_lim * 1.3 / 1000) + 1 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_CPU, &rtmp)) {
		REPORTER((char*)"Load cpu time limit fail.");
		return -1;
	}
	
	// need 32-bit system solotion
	rtmp.rlim_max = MEMTOP;
	rtmp.rlim_cur = (unsigned long int)2 * RCFG.lims.memory_lim * 1024 * 1024 < rtmp.rlim_max ?
					(unsigned long int)2 * RCFG.lims.memory_lim * 1024 * 1024 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_AS, &rtmp)) {
		REPORTER((char*)"Load memory limit fail.");
		return -1;
	}

	rtmp.rlim_max = 512 * 1024 * 1024;
	rtmp.rlim_cur = (unsigned long int)2 * RCFG.lims.output_lim < rtmp.rlim_max ? 
					(unsigned long int)2 * RCFG.lims.output_lim : rtmp.rlim_max;
	if (setrlimit(RLIMIT_FSIZE, &rtmp)) {
		REPORTER((char*)"Load output size limit fail.");
		return -1;
	}
	
	rtmp.rlim_max = MEMTOP;
	rtmp.rlim_cur = (unsigned long int)2 * RCFG.lims.memory_lim * 1024 * 1024 < rtmp.rlim_max ?
					(unsigned long int)2 * RCFG.lims.memory_lim * 1024 * 1024 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_STACK, &rtmp)) {
		REPORTER((char*)"Load stack size limit fail.");
		return -1;
	}

	return 0;
}


int SandBox :: runner(const RunConfig &RCFG, RunResult &RES) {
	RES = RunResult(0, 0, 0, 0, SIGABRT);
	rusage Ruse;
	int status_val;
	
	pid_t s_pid = fork();
	
	if (s_pid < 0) {
		REPORTER((char*)"Main fork fail.");
		return -1;
		
	} else if (s_pid == 0) {
		if ((not RCFG.is_compilation) and freopen(RCFG.in_file, "r", stdin) == NULL) {
			REPORTER((char*)"Freopen in fail.");
			return -1;
		}
		
		if ((not RCFG.is_compilation) and freopen(RCFG.out_file, "w", stdout) == NULL) {
			REPORTER((char*)"Freopen out fail.");
			return -1;
		}
		
		if (RCFG.is_compilation and freopen(RCFG.out_file, "w", stderr) == NULL) {
			REPORTER((char*)"Freopen err fail.");
			return -1;
		}
		
		if (RCFG.is_limited) if (load_limit(RCFG)) return -1;
		if (RCFG.use_sandbox) if (load_syscal_list(RCFG)) return -1;
	
		if (RCFG.is_compilation) execvp(RCFG.run_program, RCFG.argv);
		else execve(RCFG.run_program, RCFG.argv, NULL);
		
		REPORTER((char*)"Execve or execvp fail");
		exit(0);
		
	} else {
		pid_t surveillant = fork();
		
		if (surveillant < 0) {
			REPORTER((char*)"Run surveillant fail.");
			return -1;
			
		} else if (surveillant == 0) {
			int time_lim = (RCFG.lims.time_lim * 1.3 / 1000) + 1 < TIMETOP ? 
							(RCFG.lims.time_lim * 1.3 / 1000) + 1 : TIMETOP;
			sleep(time_lim);
			kill(s_pid, SIGKILL);
			exit(0);
			
		} else {
			if (wait4(s_pid, &status_val, 0, &Ruse) == -1) {
				REPORTER((char*)"Wait child fail.");
				return -1;
			}
			
			kill(surveillant, SIGKILL);
			
			if (WIFSIGNALED(status_val)) RES.run_signal = WTERMSIG(status_val);
			RES.return_value = WEXITSTATUS(status_val);
			RES.judger_error = 0;
			RES.use_time += (int)(Ruse.ru_utime.tv_sec * 1000);
			RES.use_time += (int)(Ruse.ru_utime.tv_usec / 1000);
			RES.use_memory = (int)(Ruse.ru_maxrss);
		}
	}
	return 0;
}


#endif
