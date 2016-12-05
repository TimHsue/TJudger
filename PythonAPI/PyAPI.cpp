#include "PyAPI.h"


char** generate_argv(PyObject* pyoargv) {
	if (not PyList_Check(pyoargv)) {
		REPORTER((char*)"Argv type check fail");
		return NULL;
	}
	
	int len = PyList_GET_SIZE(pyoargv);
	char** argv = new char* [len + 1];
	
	for (int i = 0; i < len; i++) {
		argv[i] = PyString_AsString(PyList_GetItem(pyoargv, i));
	}
	argv[len] = NULL;
	
	return argv;
}


int generate_config(Config& CFG, PyObject* pyoCFG) {
	PyObject* tmp;
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"language")) == NULL) {
		REPORTER((char*)"Get languege fail");
		return -1;
	}
	CFG.language = PyString_AsString(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"source_name")) == NULL) {
		REPORTER((char*)"Get source fail");
		return -1;
	}
	CFG.source_name = PyString_AsString(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"in_file")) == NULL) {
		REPORTER((char*)"Get in fail");
		return -1;
	}
	CFG.in_file = PyString_AsString(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"out_file")) == NULL) {
		REPORTER((char*)"Get out fail");
		return -1;
	}
	CFG.out_file = PyString_AsString(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"ans_file")) == NULL) {
		REPORTER((char*)"Get ans fail");
		return -1;
	}
	CFG.ans_file = PyString_AsString(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"time_limit")) == NULL) {
		REPORTER((char*)"Get time limit fail");
		return -1;
	}
	CFG.time_limit = PyLong_AsLong(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"memory_limit")) == NULL) {
		REPORTER((char*)"Get memory limit fail");
		return -1;
	}
	CFG.memory_limit = PyLong_AsLong(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"compile_option")) != NULL) {
		CFG.compile_option = generate_argv(tmp);
	} else CFG.compile_option = NULL;
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"special_judge")) != NULL) {
		CFG.special_judge = PyString_AsString(tmp);
	} else CFG.special_judge = NULL;
	
	if ((tmp = PyDict_GetItemString(pyoCFG, (char*)"spj_language")) != NULL) {
		CFG.spj_language = PyString_AsString(tmp);
	} else CFG.spj_language = NULL;
	
	return 0;
}


PyObject* generate_result(Result& RES) {
	PyObject* tmp;
	PyObject* pyoRES;
	
	pyoRES = PyDict_New();
	
	tmp = PyLong_FromLong(RES.score);
	if (PyDict_SetItemString(pyoRES, "score", tmp)) {
		REPORTER((char*)"Set score fail");
	}
	
	tmp = PyString_FromString(RES.status);
	if (PyDict_SetItemString(pyoRES, "status", tmp)) {
		REPORTER((char*)"Set status fail");
	}
	
	if (RES.compile_info != NULL) {
		tmp = PyString_FromString(RES.compile_info);
		if (PyDict_SetItemString(pyoRES, "compile_info", tmp)) {
			REPORTER((char*)"Set compile information fail");
		}
	}
	
	if (RES.in != NULL) {
		tmp = PyString_FromString(RES.in);
		if (PyDict_SetItemString(pyoRES, "in", tmp)) {
			REPORTER((char*)"Set in fail");
		}
	}
	
	if (RES.out != NULL) {
		tmp = PyString_FromString(RES.out);
		if (PyDict_SetItemString(pyoRES, "out", tmp)) {
			REPORTER((char*)"Set out fail");
		}
	}
	
	if (RES.ans != NULL) {
		tmp = PyString_FromString(RES.ans);
		if (PyDict_SetItemString(pyoRES, "ans", tmp)) {
			REPORTER((char*)"Set ans fail");
		}
	}
	
	tmp = PyLong_FromLong(RES.use_time);
	if (PyDict_SetItemString(pyoRES, "use_time", tmp)) {
		REPORTER((char*)"Set use time fail");
	}
	
	tmp = PyLong_FromLong(RES.use_memory);
	if (PyDict_SetItemString(pyoRES, "use_memory", tmp)) {
		REPORTER((char*)"Set use memory fail");
	}
	
	return pyoRES;
}


PyObject* python_run(PyObject* self, PyObject* argv) {
	PyObject* pyoCFG;
	PyObject* pyoRES;
	Config CFG;
	Result RES = Result(0, (char*)"System Error", (char*)"", (char*)"", (char*)"", (char*)"", 0, 0);
	
	if (not PyArg_ParseTuple(argv, (char*)"O", &pyoCFG)) {
		REPORTER((char*)"Parse python tuple fail");
		return generate_result(RES);
	}
	if (not PyDict_Check(pyoCFG)) {
		REPORTER((char*)"Parse python tuple fail");
		return generate_result(RES);
	}
	
	if (generate_config(CFG, pyoCFG)) {
		REPORTER((char*)"Generate config from python fail");
		return generate_result(RES);
	}

	run(CFG, RES);
	
	pyoRES = generate_result(RES);
	delete_all(RES);
	delete CFG.compile_option;
	return pyoRES;
}


static PyMethodDef TJudger_Methods[] = {
    {"run", python_run, METH_VARARGS},
    {NULL, NULL, 0}
};


extern "C" void initTJudger() {
    Py_InitModule("TJudger", TJudger_Methods);
}
