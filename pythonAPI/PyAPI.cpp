#include "PyAPI.h"


PyObject* generate


PyObject* run(PyObject* self, PyObject* argv) {
	char* language, source_name, file_name, in_file, out_file, ans_file;
	int time_limit, memory_limit;
	char** compile_option;
	char* special_judge, spj_language;
	PyObject* tmp, pyoCFG, pyoRES;
	Config CFG;
	RES = Result(0, (char*)"System Error", (char*)"", (char*)"", (char*)"", (char*)"", 0, 0);
	
	if (not PyArg_ParseTuple(argv, (char*)"O", &pyocfg) {
		REPORTER((char*)"Parse python tuple fail");
		return RES;
	}
	if (not PyDict_check(pyocfg)) {
		REPORTER((char*)"Parse python tuple fail");
		return RES;
	}
}
