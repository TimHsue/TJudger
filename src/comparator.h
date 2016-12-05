#ifndef COMPARATOR_H
#define COMPARATOR_H


#include <string.h>
#include "tools.h"


int ignore_space_linefeed(char*, char*, int);

int compare(char*, char*);


int ignore_space_linefeed(char* adj, char* ori, int size) {
	for (int i = 0; i < size; i++) {
		if (ori[i] == '\n' or ori[i] == '\r') ori[i] = ' ';
		adj[i] = ori[i];
	}
	adj[size++] = ' ';
	int resize = 0;
	char last = ' ';
	for (int i = 0; i < size; i++) {
		if (adj[i] == ' ' and last == ' ') continue;
		adj[resize++] = adj[i];
		last = adj[i];
	}
	adj[resize] = 0;
	return resize;
}


int compare(char* out, char* ans) {
	int out_size = strlen(out);
	int ans_size = strlen(ans);
	char* adjusted_out = new char[out_size + 2];
	char* adjusted_ans = new char[ans_size + 2];
	out_size = ignore_space_linefeed(adjusted_out, out, out_size);
	ans_size = ignore_space_linefeed(adjusted_ans, ans, ans_size);
	
	if (out_size == -1 or ans_size == -1) {
		REPORTER((char*)"Adjust file fail");
		delete adjusted_out;
		delete adjusted_ans;
		return -1;
	}
	
	if (out_size != ans_size) {
		delete adjusted_out;
		delete adjusted_ans;
		return 1;
	}
	for (int i = 0; i < out_size; i++) {
		if (adjusted_out[i] != adjusted_ans[i]) {
			delete adjusted_out;
			delete adjusted_ans;
			return 1;
		}
	}
	
	delete adjusted_out;
	delete adjusted_ans;
	return 0;
}


#endif
