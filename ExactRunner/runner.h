#ifndef RUNNER_H
#define RUNNER_H


#include "../src/judger.c"


int read_config(Config*);

int write_result(Result*);

void free_cfg(Config*);


#endif
