#include "PyAPI.h"


char **generate_argv(PyObject *pyoargv) {
	if (! PyList_Check(pyoargv)) {
		REPORTER("Argv type check fail");
		return NULL;
	}
	
	int len = PyList_GET_SIZE(pyoargv);
	char **argv = (char**)malloc(sizeof(char*) * (len + 1));
	
	for (int i = 0; i < len; i++) {
		argv[i] = PyString_AsString(PyList_GetItem(pyoargv, i));
	}
	argv[len] = NULL;
	
	return argv;
}


int generate_config(Config *CFG, PyObject *pyoCFG) {
	PyObject *tmp;
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "language")) == NULL) {
		REPORTER("Get languege fail");
		return -1;
	}
	CFG -> language = PyString_AsString(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "source_name")) == NULL) {
		REPORTER("Get source fail");
		return -1;
	}
	CFG -> source_name = PyString_AsString(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "in_file")) == NULL) {
		REPORTER("Get in fail");
		return -1;
	}
	CFG -> in_file = PyString_AsString(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "out_file")) == NULL) {
		REPORTER("Get out fail");
		return -1;
	}
	CFG -> out_file = PyString_AsString(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG,"ans_file")) == NULL) {
		REPORTER("Get ans fail");
		return -1;
	}
	CFG -> ans_file = PyString_AsString(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "time_limit")) == NULL) {
		REPORTER("Get time limit fail");
		return -1;
	}
	CFG -> time_limit = PyLong_AsLong(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "memory_limit")) == NULL) {
		REPORTER("Get memory limit fail");
		return -1;
	}
	CFG -> memory_limit = PyLong_AsLong(tmp);
	// Py_DECREF(tmp);
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "compile_option")) != NULL) {
		CFG -> compile_option = generate_argv(tmp);
		// Py_DECREF(tmp);
	} else CFG -> compile_option = NULL;
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "special_judge")) != NULL) {
		CFG -> special_judge = PyString_AsString(tmp);
		// Py_DECREF(tmp);
	} else CFG -> special_judge = NULL;
	
	if ((tmp = PyDict_GetItemString(pyoCFG, "spj_language")) != NULL) {
		CFG -> spj_language = PyString_AsString(tmp);
		// Py_DECREF(tmp);
	} else CFG -> spj_language = NULL;
	
	return 0;
}


PyObject *generate_result(Result *RES) {
	PyObject *tmp;
	PyObject *pyoRES;
	
	pyoRES = PyDict_New();
	
	tmp = PyLong_FromLong(RES -> score);
	if (PyDict_SetItemString(pyoRES, "score", tmp)) {
		REPORTER("Set score fail");
	}
	Py_DECREF(tmp);
	
	tmp = PyString_FromString(RES -> status);
	if (PyDict_SetItemString(pyoRES, "status", tmp)) {
		REPORTER("Set status fail");
	}
	Py_DECREF(tmp);
	
	if (RES -> compile_info != NULL) {
		tmp = PyString_FromString(RES -> compile_info);
		if (PyDict_SetItemString(pyoRES, "compile_info", tmp)) {
			REPORTER("Set compile information fail");
		}
		Py_DECREF(tmp);
	}
	
	if (RES -> in != NULL) {
		tmp = PyString_FromString(RES -> in);
		if (PyDict_SetItemString(pyoRES, "in", tmp)) {
			REPORTER("Set in fail");
		}
		Py_DECREF(tmp);
	}
	
	if (RES -> out != NULL) {
		tmp = PyString_FromString(RES -> out);
		if (PyDict_SetItemString(pyoRES, "out", tmp)) {
			REPORTER("Set out fail");
		}
		Py_DECREF(tmp);
	}
	
	if (RES -> ans != NULL) {
		tmp = PyString_FromString(RES -> ans);
		if (PyDict_SetItemString(pyoRES, "ans", tmp)) {
			REPORTER("Set ans fail");
		}
		Py_DECREF(tmp);
	}
	
	tmp = PyLong_FromLong(RES -> use_time);
	if (PyDict_SetItemString(pyoRES, "use_time", tmp)) {
		REPORTER("Set use time fail");
	}
	Py_DECREF(tmp);
	
	tmp = PyLong_FromLong(RES -> use_memory);
	if (PyDict_SetItemString(pyoRES, "use_memory", tmp)) {
		REPORTER("Set use memory fail");
	}
	Py_DECREF(tmp);
	
	return pyoRES;
}


PyObject *python_run(PyObject *self, PyObject *argv) {
	PyObject *pyoCFG;
	PyObject *pyoRES;
	Config CFG;
	Result RES = {0, "System Error", "", "", "", "", 0, 0};
	
	if (! PyArg_ParseTuple(argv, "O", &pyoCFG)) {
		REPORTER("Parse python tuple fail");
		return generate_result(&RES);
	}
	if (! PyDict_Check(pyoCFG)) {
		REPORTER("Parse python tuple fail");
		return generate_result(&RES);
	}
	
	if (generate_config(&CFG, pyoCFG)) {
		REPORTER("Generate config from python fail");
		return generate_result(&RES);
	}

	RES = run(&CFG);
	
	pyoRES = generate_result(&RES);
	delete_all(&RES);
	return pyoRES;
}


static PyMethodDef TJudger_Methods[] = {
    {"run", python_run, METH_VARARGS},
    {NULL, NULL, 0}
};


void initTJudger(void) {
    Py_InitModule("TJudger", TJudger_Methods);
}
