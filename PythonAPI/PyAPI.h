#ifndef PYAPI_H
#define PYAPI_H


#include "Python.h"
#include "../src/judger.cpp"


char** generate_argv(PyObject* pyoargv);

int generate_config(Config& CFG, PyObject* pyoCFG);

PyObject* python_run(PyObject* self, PyObject* argv);

extern "C" void initTJudger();


#endif
