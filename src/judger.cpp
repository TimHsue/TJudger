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
		strcpy(compiler, (char*)"g++");
	} else if (strcmp(lan, (char*)"C") == 0) {
		strcpy(compiler, (char*)"gcc");
	} else if (strcmp(lan, (char*)"Pascal") == 0) {
		strcpy(compiler, (char*)"fpc");
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
	if (compile_opt != NULL) {
		int len = 3;
		while(compile_opt[len - 3] != NULL) argv[len] = compile_opt[len - 3], len += 1;
		argv[len] = NULL;
	} else argv[3] = NULL;
	
	SandBox com_sdb;
	RunConfig RRCF(true, false, true, compiler, (char*)"", (char*)"compile.out", argv, LimitList(10000, 128, 512 * 1024 * 1024));
	RunResult RRES;
	
	if (com_sdb.runner(RRCF, RRES)) {
		REPORTER((char*)"Compile fail.");
		if (argv != NULL) delete[] argv;
		return -1;
	}

	if (argv != NULL) delete[] argv;
	return 0;
}


int get_result(Config& CFG, Result& RES) {
	if (CFG.special_judge != NULL) {
		char file_name[64] = {0};
		char* argv[] = {NULL};

		if (compile(CFG.spj_language, CFG.special_judge, file_name, argv)) {
			strcpy(RES.status, (char*)"System Error");
			return -1;
		}
		if (access(file_name, 0) != 0) {
			strcpy(RES.status, (char*)"Compile Special Judge Error");
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
			if (remove(file_name)) {
				REPORTER((char*)"Delete program fail");
				return -1;
			}
			return -1;
		}
		if (remove(file_name)) {
			REPORTER((char*)"Delete program fail");
			return -1;
		}
		
		if (RRES.judger_error) {
			strcpy(RES.status, (char*)"Judger Error");
			return 0;
		} else if (RRES.return_value | RRES.run_signal) {
			strcpy(RES.status, (char*)"Run Special Judge Error");
			return 0;
		}
		
		if ((spj_res = READFILE(spj_tmp_out, 1024)) == NULL) {
			REPORTER((char*)"Read special judge result file fail");
			return -1;
		}
		
		int score = 0;
		for (int i = 0; i < 7 and spj_res[i]; i++) {
			if('0' <= spj_res[i] and spj_res[i] <= '9')
				score = (score << 3) + (score << 1) + spj_res[i] - '0';
		}
		RES.score = score;
		if (score == 100) {
			strcpy(RES.status, (char*)"Accepted");
		} else if (score == 0) {
			strcpy(RES.status, (char*)"Wrong Answer");
		} else {
			strcpy(RES.status, (char*)"Partly Correct");
		}
		
		if (spj_res != NULL) delete[] spj_res;
	} else {
		int tmp_res = compare(RES.out, RES.ans);
		if (tmp_res == -1) {
			REPORTER((char*)"Run comparator fail");
			return -1;
		}
		
		if (not tmp_res) {
			RES.score = 100;
			strcpy(RES.status, (char*)"Accepted");
		} else {
			strcpy(RES.status, (char*)"Wrong Answer");
		}
	}
	return 0;
} 


Result run(Config &CFG) {
	char* status = new char[32];
	strcpy(status, (char*)"System Error");
	Result RES(0, status, NULL, NULL, NULL, NULL, 0, 0);
	char file_name[64] = {0};

	if (compile(CFG.language, CFG.source_name, file_name, CFG.compile_option)) {
		strcpy(status, (char*)"Compile Error");
		RES.status = status;
		return RES;
	}
	if ((RES.compile_info = READFILE((char*)"compile.out", 256)) == NULL) {
		REPORTER((char*)"Read compile result fail");
		return RES;
	}
	if (access(file_name, 0) != 0) {
		strcpy(status, (char*)"Compile Error");
		RES.status = status;
		return RES;
	}
	
	struct stat statbuf;
	if (stat(CFG.ans_file, &statbuf)) {
		strcpy(status, (char*)"No Answers");
		RES.status = status;
		REPORTER((char*)"Get answer file size fail");
		if (remove(file_name)) {
			REPORTER((char*)"Delete program fail");
			return RES;
		}
		return RES;
	}
	int size = statbuf.st_size;
	
	RunResult RRES;
	RunConfig RCFG(false, true, true, file_name, CFG.in_file, CFG.out_file, NULL, 
					LimitList(CFG.time_limit, CFG.memory_limit, size));
	SandBox run_sdb;
	if (run_sdb.runner(RCFG, RRES)) {
		REPORTER((char*)"Run progream fail");
		return RES;
	}
	if (remove(file_name)) {
		REPORTER((char*)"Delete program fail");
		return RES;
	}
	
	RES.use_time = RRES.use_time;
	RES.use_memory = RRES.use_memory;
	if (RRES.judger_error) {
		strcpy(status, (char*)"Judger Error");
	} else if (RRES.run_signal == 0) {
		if (RRES.return_value == 0) {
			strcpy(status, (char*)"Run Successfully");
		} else {
			strcpy(status, (char*)"Runtime Error");
		}
		if (RRES.use_time > CFG.time_limit) {
			strcpy(status, (char*)"Time Limit Exceed");
		} else if (RRES.use_memory > CFG.memory_limit * 1024) {
			strcpy(status, (char*)"Memory Limit Exceed");
		}
	} else if (RRES.run_signal == 31) {
		strcpy(status, (char*)"Dangerous System Call");
	} else if (RRES.run_signal == 9) {
		if (RRES.use_time > CFG.time_limit) {
			strcpy(status, (char*)"Time Limit Exceed");
		}
	} else if (RRES.run_signal == 11) {
		if (RRES.use_memory > CFG.memory_limit * 1024) {
			strcpy(status, (char*)"Memory Limit Exceed");
		} else {
			strcpy(status, (char*)"Runtime Error");
		}
	} else if (RRES.run_signal == 25) {
		strcpy(status, (char*)"Output Limit Exceed");
	}
	RES.status = status;
	
	// limit singel file size below 512MB
	if ((RES.in = READFILE(CFG.in_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER((char*)"Read in file fail");
		delete_files(RES);
		return RES;
	}
	if ((RES.out = READFILE(CFG.out_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER((char*)"Read out file fail");
		delete_files(RES);
		return RES;
	}
	if ((RES.ans = READFILE(CFG.ans_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER((char*)"Read ans file fail");
		delete_files(RES);
		return RES;
	}
	
	if (strcmp(RES.status, (char*)"Run Successfully") == 0) {
		if (get_result(CFG, RES)) {
			REPORTER((char*)"Get Result fail");
			delete_files(RES);
			return RES;
		}
	}
	
	return RES;
}


void delete_files(Result& RES) {
	if (RES.in != NULL) delete []RES.in;
	if (RES.out != NULL) delete []RES.out;
	if (RES.ans != NULL) delete []RES.ans;
	if (RES.compile_info != NULL) delete []RES.compile_info;
}


void delete_all(Result& RES) {
	if (RES.in != NULL) delete RES.in;
	if (RES.out != NULL) delete RES.out;
	if (RES.ans != NULL) delete RES.ans;
	if (RES.compile_info != NULL) delete RES.compile_info;
	if (RES.status != NULL) delete RES.status;
}

