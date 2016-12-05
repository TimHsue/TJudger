#ifndef TOOLS_H
#define TOOLS_H


#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>


void REPORTER(char*);

char* READFILE(char*, int);


void REPORTER(char* log_info) {
	FILE* stream;
	stream = fopen("TJudger.log", "a");
	time_t now_time = time(NULL);
	char output_time[64];
	strftime(output_time, sizeof(output_time), "%F %R:%S",localtime(&now_time));
	fprintf(stream, "[%s] : %s. ERRORCODE : %d\n", output_time, log_info, errno);
	fclose(stream);
}

// todo : need access checker
char* READFILE(char* file_name, int length) {
	FILE* stream;
	struct stat statbuf;
	
	if (stat(file_name, &statbuf)) {
		REPORTER((char*)"Get file size fail");
		return NULL;
	}
	
	int size = length < statbuf.st_size ? length : statbuf.st_size;
	if (size > 512 * 1024 * 1024) {
		REPORTER((char*)"File size too large");
		return NULL;
	}
	
	if (not (stream = fopen(file_name, "rb"))) {
		REPORTER((char*)"Open file fail");
		return NULL;
	}
	
	char* text = new char[size + 1];
	if (fread(text, 1, size, stream) != size) {
		REPORTER((char*)"Read file fail");
		return NULL;
	}
	text[size] = 0;
	
	return text;
}


#endif
