#ifndef JUDGER_H
#define JUDGER_H


#include <string.h>
#include "sandbox.cpp"


class Result {
public:
	char* status;
	char* compile_info;
	char* in;
	char* out;
	char* ans;
	int time_use;
	int memory_use;
	
	Result();
	Result(char*, char*, char*, char*, char*, int, int);
};

void run()


class Config {
public:
	bool use_
	char* language;
	char* program_name;
	char* in_file;
	char* out_file;
	char* ans_file;
	int time_limit;
	int memory_limit;
	char* special_judge;
	
	Config();
	Config(char*, char*, char*, char*, char*);
}


void compile(const char*, const char*);

void run(const Config&, Result&);

void get_rusult(const Config&, Result&);


#endif
