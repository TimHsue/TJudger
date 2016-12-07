#ifndef PYAPI_H
#define PYAPI_H


#include "Python.h"
#include "../src/judger.c"


char** generate_argv(PyObject*);

int generate_config(Config*, PyObject*);

PyObject* python_run(PyObject*, PyObject*);

PyObject* generate_result(Result*);

void initTJudger(void);


#endif
