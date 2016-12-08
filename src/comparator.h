#ifndef COMPARATOR_H
#define COMPARATOR_H


#include <string.h>
#include "tools.h"


int ignore_space_linefeed(char*, char*, int);

int compare(char*, char*);


int ignore_space_linefeed(char* adj, char* ori, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (ori[i] == '\n' || ori[i] == '\r') ori[i] = ' ';
		adj[i] = ori[i];
	}
	adj[size++] = ' ';
	int resize = 0;
	char last = ' ';
	for (i = 0; i < size; i++) {
		if (adj[i] == ' ' && last == ' ') continue;
		adj[resize] = adj[i];
		resize += 1;
		last = adj[i];
	}
	adj[resize] = 0;
	return resize;
}


int compare(char* out, char* ans) {
	int out_size = strlen(out);
	int ans_size = strlen(ans);
	char* adjusted_out = (char *)malloc(sizeof(char) * (out_size + 2));
	char* adjusted_ans = (char *)malloc(sizeof(char) * (ans_size + 2));
	out_size = ignore_space_linefeed(adjusted_out, out, out_size);
	ans_size = ignore_space_linefeed(adjusted_ans, ans, ans_size);
	
	if (out_size == -1 || ans_size == -1) {
		REPORTER("Adjust file fail");
		free(adjusted_out);
		free(adjusted_ans);
		return -1;
	}
	
	if (out_size != ans_size) {
		free(adjusted_out);
		free(adjusted_ans);
		return 1;
	}
	
	int i;
	for (i = 0; i < out_size; i++) {
		if (adjusted_out[i] != adjusted_ans[i]) {
			free(adjusted_out);
			free(adjusted_ans);
			return 1;
		}
	}
	
	free(adjusted_out);
	free(adjusted_ans);
	return 0;
}


#endif
