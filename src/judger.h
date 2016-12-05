#ifndef JUDGER_H
#define JUDGER_H


#include <string.h>
#include <sys/stat.h>
#include "sandbox.h"
#include "comparator.h"


class Result {
public:
	int score;
	char* status;
	char* compile_info;
	char* in;
	char* out;
	char* ans;
	int use_time;
	int use_memory;
	
	Result();
	Result(int, char*, char*, char*, char*, char*, int, int);
};


class Config {
public:
	char* language;
	char* source_name;
	char* file_name;
	char* in_file;
	char* out_file;
	char* ans_file;
	int time_limit;
	int memory_limit;
	char** compile_option;
	char* special_judge;
	char* spj_language;
	
	Config();
	Config(char*, char*, char*, char*, char*, int, int, char**);
	Config(char*, char*, char*, char*, char*, int, int, char**, char*, char*);
};



void generate_name(const char*);

int compile(const char*, char*, char*, char**);

int get_result(char*, Result&);

void run(Config&, Result&);

void delete_all(Result&);


#endif
