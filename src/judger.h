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


class Config {
public:
	char* language;
	char* file_name;
	char* in_file;
	char* out_file;
	char* ans_file;
	int time_limit;
	int memory_limit;
	char* special_judge;
	char* compile_option;
	
	Config();
	Config(char*, char*, char*, char*, char*, int, int, char*);
};


void generate_name(const char*);

int compile(const char*, char*, char**);

void run(const Config&, Result&);

void get_rusult(const Config&, Result&);


#endif
