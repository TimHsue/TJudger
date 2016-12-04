#include "judger.h"


Result :: Result() {}
Result :: Result(int sc, char* st, char* ci, char* i, char* o, char* a, int tu, int mu) {
	score = sc;
	status = st;
	compile_info = ci;
	in = i;
	out = o;
	ans = a;
	use_time = tu;
	use_memory = mu;
}


Config :: Config() {}
Config :: Config(char* lg, char* sn, char* ifl, char* ofl, char* afl, 
		int tl, int ml, char** cop) {
	language = lg;
	source_name = sn;
	file_name = (char*)"";
	in_file = ifl;
	out_file = ofl;
	ans_file = afl;
	time_limit = tl;
	memory_limit = ml;
	compile_option = cop;
	special_judge = NULL;
	spj_language = NULL;
}
Config :: Config(char* lg, char* sn, char* ifl, char* ofl, char* afl, 
		int tl, int ml, char** cop, char* spj, char* sl) {
	language = lg;
	source_name = sn;
	file_name = (char*)"";
	in_file = ifl;
	out_file = ofl;
	ans_file = afl;
	time_limit = tl;
	memory_limit = ml;
	compile_option = cop;
	special_judge = spj;
	spj_language = sl;
}



// in case of execve("exe");
void generate_name(const char* in_name, char* out_name) {
	srand((unsigned)time(NULL) + 970830);
	int len = sizeof(in_name) / sizeof(char);
	
	for (int i = 0; i < len; i++) out_name[i] = rand() % 26 + 65;
}


int compile(const char* lan, char* file_name, char* out_name, char** compile_opt) {
	char compiler[32] = {0};
	char out_com[32] = {0};
	
	if (strcmp(lan, (char*)"C++") == 0) {
		strcpy(compiler, "g++");
	} else if (strcmp(lan, (char*)"C") == 0) {
		strcpy(compiler, "gcc");
	} else if (strcmp(lan, (char*)"Pascal") == 0) {
		strcpy(compiler, "fpc");
	} else {
		REPORTER((char*)"No Langue");
		return -1;
	}
	
	generate_name(file_name, out_name);
	sprintf(out_com, "-o%s", out_name);
	
	char ** argv = new char*[32];
	argv[0] = compiler;
	argv[1] = file_name;
	argv[2] = out_com;

	int len = 3;
	while(compile_opt[len - 3] != NULL) argv[len++] = compile_opt[len - 3];
	argv[len] = NULL;
	
	SandBox com_sdb;
	RunConfig RRCF(true, false, true, compiler, (char*)"", (char*)"compile.out", argv, LimitList(10000, 128, 512 * 1024 * 1024));
	RunResult RRES;
	
	if (com_sdb.runner(RRCF, RRES)) {
		REPORTER((char*)"Compile fail.");
		delete argv;
		return -1;
	}

	delete argv;
	return 0;
}

int get_result(Config& CFG, Result& RES) {
	if (CFG.special_judge != NULL) {
		char file_name[64] = {0};
		char* argv[] = {NULL};

		if (compile(CFG.spj_language, CFG.special_judge, file_name, argv)) {
			RES.status = (char*)"System Error";
			return -1;
		}
		if (access(file_name, 0) != 0) {
			RES.status = (char*)"Compile Special Judge Error";
			return 0;
		}
		
		char* spjargv[] = {RES.in, RES.out, RES.ans, NULL};
		char spj_tmp_out[] = "spj_tmp.out";
		char* spj_res;
		RunResult RRES;
		RunConfig RCFG(false, true, true, file_name, CFG.in_file, spj_tmp_out, spjargv, 
					LimitList(10, 512, 16 * 1024 * 1024));
		SandBox spj_sdb;
		if (spj_sdb.runner(RCFG, RRES)) {
			REPORTER((char*)"Run progream fail");
			return -1;
		}
		if (remove(CFG.file_name)) {
			REPORTER((char*)"Delete program fail");
			return -1;
		}
		
		if (RRES.judger_error) {
			RES.status = (char*)"Judger Error";
			return 0;
		} else if (RRES.return_value | RRES.run_signal) {
			RES.status = (char*)"Run Special Judge Error";
			return 0;
		}
		
		if ((spj_res = READFILE(spj_tmp_out, 1024)) == NULL) {
			REPORTER((char*)"Read special judge result file fail");
			return -1;
		}
		
		int score = 0;
		for (int i = 0; i < 7 and spj_res[i]; i++) score = (score << 3) + (score << 1) + spj_res[i] - '0';
		RES.score = score;
		if (score == 100) {
			RES.status = (char*)"Accept";
		} else if (score == 0) {
			RES.status = (char*)"Wrong Answer";
		} else {
			RES.status = (char*)"Partly Correct";
		}
		
	} else {
		int tmp_res = compare(RES.out, RES.ans);
		if (tmp_res == -1) {
			REPORTER((char*)"Run comparator fail");
			return -1;
		}
		
		if (not tmp_res) {
			RES.score = 100;
			RES.status = (char*)"Accept";
		} else {
			RES.status = (char*)"Wrong Answer";
		}
	}
	return 0;
} 

void run(Config &CFG, Result &RES) {
	FILE* stream;
	RES = Result(0, (char*)"System Error", (char*)"", (char*)"", (char*)"", (char*)"", 0, 0);
	char file_name[64] = {0};

	if (compile(CFG.language, CFG.source_name, file_name, CFG.compile_option)) {
		RES.status = (char*)"Compile porgram Error";
		return;
	}
	if ((RES.compile_info = READFILE((char*)"compile.out", 256)) == NULL) {
		REPORTER((char*)"Read compile result fail");
		return;
	}
	if (access(file_name, 0) != 0) {
		RES.status = (char*)"Compile porgram Error";
		return;
	}
	CFG.file_name = file_name;
		
	struct stat statbuf;
	if (stat(CFG.ans_file, &statbuf)) {
		REPORTER((char*)"Get answer file size fail");
		return;
	}
	int size = statbuf.st_size;
	
	RunResult RRES;
	RunConfig RCFG(false, true, true, CFG.file_name, CFG.in_file, CFG.out_file, NULL, 
					LimitList(CFG.time_limit, CFG.memory_limit, size));
	SandBox run_sdb;
	if (run_sdb.runner(RCFG, RRES)) {
		REPORTER((char*)"Run progream fail");
		return;
	}
	if (remove(CFG.file_name)) {
		REPORTER((char*)"Delete program fail");
		return;
	}
	
	RES.use_time = RRES.use_time;
	RES.use_memory = RRES.use_memory;
	if (RRES.judger_error) {
		RES.status = (char*)"Judger Error";
	} else if (RRES.run_signal == 0) {
		if (RRES.return_value == 0) RES.status = (char*)"Run Successfully";
		else RES.status = (char*)"Runtime Error";
		if (RRES.use_time > CFG.time_limit) RES.status = (char*)"Time Limit Exceed";
		if (RRES.use_memory > CFG.memory_limit) RES.status = (char*)"Memory Limit Exceed";
	} else if (RRES.run_signal == 31) {
		RES.status = (char*)"Dangerous System Call";
	} else if (RRES.run_signal == 9) {
		if (RRES.use_time > CFG.time_limit) RES.status = (char*)"Time Limit Exceed";
	} else if (RRES.run_signal == 11) {
		if (RRES.use_memory > CFG.memory_limit) RES.status = (char*)"Memory Limit Exceed";
		else RES.status = (char*)"Runtime Error";
	} else if (RRES.run_signal == 25) {
		RES.status = (char*)"Output Limit Exceed";
	}
	
	// limit singel file size below 512MB
	if ((RES.in = READFILE(CFG.in_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER((char*)"Read in file fail");
		return;
	}
	if ((RES.out = READFILE(CFG.out_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER((char*)"Read out file fail");
		return;
	}
	if ((RES.ans = READFILE(CFG.ans_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER((char*)"Read ans file fail");
		return;
	}
	
	if (strcmp(RES.status, (char*)"Run Successfully") == 0) {
		if (get_result(CFG, RES)) {
			REPORTER((char*)"Get Result fail");
			return;
		}
	}
}



