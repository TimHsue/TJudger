#ifndef SANDBOX_H
#define SANDBOX_H


#include <seccomp.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h> 


void REPORTER(char* log_info) {
	FILE* stream;
	stream = fopen("../TJudger.log", "a");
	time_t now_time = time(NULL);
	char output_time[64];
	strftime(output_time, sizeof(output_time), "%F %R:%S",localtime(&now_time));
	fprintf(stream, "[%s] : %s\n", output_time, log_info);
	fclose(stream);
	exit(0);
}


const int TIMETOP = 60;
const long long MEMTOP = 1024 * 1024 * 1024;


const int syscal_white_list[32] = {16,
	SCMP_SYS(close), SCMP_SYS(read),
	SCMP_SYS(write), SCMP_SYS(tgkill),
	SCMP_SYS(brk), SCMP_SYS(rt_sigprocmask),
	SCMP_SYS(access), SCMP_SYS(mmap),
	SCMP_SYS(mprotect), SCMP_SYS(fstat),
	SCMP_SYS(arch_prctl), SCMP_SYS(munmap),
	SCMP_SYS(exit_group), SCMP_SYS(vfork),
	SCMP_SYS(writev), SCMP_SYS(lseek)
};


class LimitList {
public:
	int time_lim;
	int memory_lim;
	
	LimitList();	
	LimitList(int, int);
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
	int run_status;
	bool judger_error;
	
	RunResult();
	RunResult(int, int, int, bool);
};


class SandBox {
private:
	int load_syscal_list(const RunConfig&);
	
	int load_limit(const RunConfig&);
public:
	void runner(const RunConfig&, RunResult&);
};


#endif

