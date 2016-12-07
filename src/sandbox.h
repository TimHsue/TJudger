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

#define TIMETOP (60)
#define MEMTOP (1024 * 1024 * 1024)

typedef struct LimitList {
	int time_lim;
	int memory_lim;
	int output_lim;
} LimitList;


typedef struct RunConfig {
	int is_compilation;
	int use_sandbox;
	int is_limited;
	char* run_program;
	char* in_file;
	char* out_file;
	char** argv;
	LimitList lims;
} RunConfig;


typedef struct RunResult {
	int use_time;
	int use_memory;
	int run_signal;
	int return_value;
	int judger_error;
} RunResult;


typedef struct Sandbox {
	int syscal_white_list[32];
} Sandbox;

int load_syscal_list(const RunConfig*);
	
int load_limit(const RunConfig*);
	
int runner(const RunConfig*, RunResult*);


int load_syscal_list(const RunConfig *RCFG) {
	Sandbox sbx = {{16,
			SCMP_SYS(close), SCMP_SYS(read),
			SCMP_SYS(write), SCMP_SYS(tgkill),
			SCMP_SYS(brk), SCMP_SYS(rt_sigprocmask),
			SCMP_SYS(access), SCMP_SYS(mmap),
			SCMP_SYS(mprotect), SCMP_SYS(fstat),
			SCMP_SYS(arch_prctl), SCMP_SYS(munmap),
			SCMP_SYS(exit_group), SCMP_SYS(writev)
		}};
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
	
	if (! ctx) {
		REPORTER("Initialize system call fail");
		return -1;
	}
	
	for (int i = 1; i <= (sbx.syscal_white_list[0]); i++) {
		if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, (sbx.syscal_white_list[i]), 0) != 0) {
			REPORTER("Add system call fail.");
			return -1;
		}
	}
	
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, 
		SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)RCFG -> run_program)) != 0) {
		REPORTER("Add system call fail.");
		return -1;
	}
	
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 2, 
		SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)""),
		SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t)(O_RDONLY|O_CLOEXEC))) != 0) {
		REPORTER("Add system call fail.");
		return -1;
	}
	
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 1,
		SCMP_A0(SCMP_CMP_EQ, 0)) != 0) {
		REPORTER("Add system call fail.");
		return -1;
	}
	
	if (seccomp_load(ctx) != 0) {
		REPORTER("Load system call fail.");
		return -1;
	}

	return 0;
}

	
int load_limit(const RunConfig *RCFG) {
	struct rlimit rtmp;
	
	rtmp.rlim_max = TIMETOP;
	rtmp.rlim_cur = (unsigned long int)(RCFG -> lims.time_lim * 1.3 / 1000) + 1 < rtmp.rlim_max ?
					(unsigned long int)(RCFG -> lims.time_lim * 1.3 / 1000) + 1 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_CPU, &rtmp) != 0) {
		REPORTER("Load cpu time limit fail.");
		return -1;
	}
	
	// need 32-bit system solotion
	rtmp.rlim_max = MEMTOP;
	rtmp.rlim_cur = (unsigned long int)2 * RCFG -> lims.memory_lim * 1024 * 1024 < rtmp.rlim_max ?
					(unsigned long int)2 * RCFG -> lims.memory_lim * 1024 * 1024 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_AS, &rtmp) != 0) {
		REPORTER("Load memory limit fail.");
		return -1;
	}

	rtmp.rlim_max = 512 * 1024 * 1024;
	rtmp.rlim_cur = (unsigned long int)2 * RCFG -> lims.output_lim < rtmp.rlim_max ? 
					(unsigned long int)2 * RCFG -> lims.output_lim : rtmp.rlim_max;
	if (setrlimit(RLIMIT_FSIZE, &rtmp) != 0) {
		REPORTER("Load output size limit fail.");
		return -1;
	}
	
	rtmp.rlim_max = MEMTOP;
	rtmp.rlim_cur = (unsigned long int)2 * RCFG -> lims.memory_lim * 1024 * 1024 < rtmp.rlim_max ?
					(unsigned long int)2 * RCFG -> lims.memory_lim * 1024 * 1024 : rtmp.rlim_max;
	if (setrlimit(RLIMIT_STACK, &rtmp) != 0) {
		REPORTER("Load stack size limit fail.");
		return -1;
	}

	return 0;
}


int runner(const RunConfig *RCFG, RunResult *RES) {
	struct rusage Ruse;
	int status_val;
	
	pid_t s_pid = fork();
	
	if (s_pid < 0) {
		REPORTER("Main fork fail.");
		return -1;
		
	} else if (s_pid == 0) {
		if ((RCFG -> is_compilation == 0) && (freopen(RCFG -> in_file, "r", stdin) == NULL)) {
			REPORTER("Freopen in fail.");
			return -1;
		}
		
		if ((RCFG -> is_compilation == 0) && (freopen(RCFG -> out_file, "w", stdout) == NULL)) {
			REPORTER("Freopen out fail.");
			return -1;
		}
		
		if ((RCFG -> is_compilation != 0) && (freopen(RCFG -> out_file, "w", stderr) == NULL)) {
			REPORTER("Freopen err fail.");
			return -1;
		}
		
		if (RCFG -> is_limited != 0) if (load_limit(RCFG) != 0) return -1;
		if (RCFG -> use_sandbox != 0) if (load_syscal_list(RCFG) != 0) return -1;
		
		if (RCFG -> is_compilation != 0) execvp(RCFG -> run_program, RCFG -> argv);
		else execve(RCFG -> run_program, RCFG -> argv, NULL);
		
		REPORTER("Execve or execvp fail");
		exit(0);
		
	} else {		
		pid_t surveillant = fork();
		
		if (surveillant < 0) {
			REPORTER("Run surveillant fail.");
			return -1;
			
		} else if (surveillant == 0) {
			int time_lim = (RCFG -> lims.time_lim * 1.3 / 1000) + 1 < TIMETOP ? 
							(RCFG -> lims.time_lim * 1.3 / 1000) + 1 : TIMETOP;
			sleep(time_lim);
			kill(s_pid, SIGKILL);
			exit(0);
		} else {
			if (wait4(s_pid, &status_val, 0, &Ruse) == -1) {
				REPORTER("Wait child fail.");
				return -1;
			}
			
			int sstatus;
			kill(surveillant, SIGKILL);
			if (waitpid(surveillant, &sstatus, 0) == -1) {
				REPORTER("Wait surveillant fail.");
				return -1;
			}
			
			if (WIFSIGNALED(status_val) != 0) RES -> run_signal = WTERMSIG(status_val);
			RES -> return_value = WEXITSTATUS(status_val);
			RES -> judger_error = 0;
			RES -> use_time += (int)(Ruse.ru_utime.tv_sec * 1000);
			RES -> use_time += (int)(Ruse.ru_utime.tv_usec / 1000);
			RES -> use_memory = (int)(Ruse.ru_maxrss);
			
		}
	}
	return 0;
}


#endif
