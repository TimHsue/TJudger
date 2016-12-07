#ifndef TOOLS_H
#define TOOLS_H


#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

void REPORTER(char *);

char* READFILE(char *, int);


void REPORTER(char *log_info) {
	FILE* stream;
	stream = fopen("TJudger.log", "a");
	
	time_t now_time;
	time(&now_time);
	
	char output_time[64];
	strftime(output_time, sizeof(output_time), "%F %R:%S",localtime(&now_time));
	fprintf(stream, "[%s] : %s. ERRORCODE : %d\n", output_time, log_info, errno);
	fclose(stream);
}

// todo : need access checker
char *READFILE(char *file_name, int length) {
	FILE* stream;
	struct stat statbuf;
	
	if (stat(file_name, &statbuf) != 0) {
		REPORTER("Get file size fail");
		return NULL;
	}
	
	long size = length < statbuf.st_size ? length : statbuf.st_size;
	if (size > 512 * 1024 * 1024) {
		REPORTER("File size too large");
		return NULL;
	}
	
	if (! (stream = fopen(file_name, "rb"))) {
		REPORTER("Open file fail");
		return NULL;
	}
	
	char *text = (char *)malloc((size + 1) * sizeof(char));
	if (text == NULL) {
		REPORTER("Malloc text fail");
		return NULL;
	}
	
	if ((int)fread(text, 1, size, stream) != size) {
		REPORTER("Read file fail");
		fclose(stream);
		return NULL;
	}
	text[size] = 0;
	fclose(stream);
	
	return text;
}


#endif
