#include "judger.h"


// in case of execve("exe");
void generate_name(const char *in_name, char *out_name) {
	srand((unsigned)time(NULL) + 970830);
	int len = sizeof(in_name) / sizeof(char);
	
	int i;
	for (i = 0; i < len; i++) out_name[i] = rand() % 26 + 65;
}


int compile(const char *lan, char *file_name, char *out_name, char **compile_opt) {
	char compiler[32] = {0};
	char out_com[32] = {0};
	
	if (strcmp(lan, "C++") == 0) {
		strcpy(compiler, "g++");
	} else if (strcmp(lan, "C") == 0) {
		strcpy(compiler, "gcc");
	} else if (strcmp(lan, "Pascal") == 0) {
		strcpy(compiler, "fpc");
	} else {
		REPORTER("No Langue");
		return -1;
	}
	
	generate_name(file_name, out_name);
	sprintf(out_com, "-o%s", out_name);
	
	char **argv = (char **)malloc(sizeof(char*) * 32);
	argv[0] = compiler;
	argv[1] = file_name;
	argv[2] = out_com;
	if (compile_opt != NULL) {
		int len = 3;
		while(compile_opt[len - 3] != NULL) argv[len] = compile_opt[len - 3], len += 1;
		argv[len] = NULL;
	} else argv[3] = NULL;
	
	RunConfig RRCF = {1, 0, 1, compiler, "", "compile.out", argv, {10000, 128, 512 * 1024 * 1024}};
	RunResult RRES = {0, 0, 0, 0, 1};
	
	if (runner(&RRCF, &RRES) != 0) {
		REPORTER("Compile fail.");
		if (argv != NULL) free(argv);
		return -1;
	}

	if (argv != NULL) free(argv);
	return 0;
}


int get_result(Config *CFG, Result *RES) {
	if (CFG -> special_judge != NULL) {
		char file_name[64] = {0};
		char* argv[] = {NULL};

		if (compile(CFG -> spj_language, CFG -> special_judge, file_name, argv) != 0) {
			strcpy(RES -> status, "System Error");
			return -1;
		}
		if (access(file_name, 0) != 0) {
			strcpy(RES -> status, "Compile Special Judge Error");
			return 0;
		}
		
		char* spjargv[] = {RES -> in, RES -> out, RES -> ans, NULL};
		char spj_tmp_out[] = "spj_tmp.out";
		char* spj_res;
		RunResult RRES = {0, 0, 0, 0, 1};
		RunConfig RCFG = {0, 1, 1, file_name, CFG -> in_file, spj_tmp_out, spjargv, 
					{10, 512, 16 * 1024 * 1024}};

		if (runner(&RCFG, &RRES) != 0) {
			REPORTER("Run progream fail");
			if (remove(file_name)) {
				REPORTER("Delete program fail");
				return -1;
			}
			return -1;
		}
		if (remove(file_name) != 0) {
			REPORTER("Delete program fail");
			return -1;
		}
		
		if (RRES.judger_error != 0) {
			strcpy(RES -> status, "Judger Error");
			return 0;
		} else if ((RRES.return_value | RRES.run_signal) != 0) {
			strcpy(RES -> status, "Run Special Judge Error");
			return 0;
		}
		
		if ((spj_res = READFILE(spj_tmp_out, 1024)) == NULL) {
			REPORTER("Read special judge result file fail");
			return -1;
		}
		
		int score = 0;
		int i;
		for (i = 0; i < 7 && spj_res[i]; i++) {
			if('0' <= spj_res[i] && spj_res[i] <= '9')
				score = (score << 3) + (score << 1) + spj_res[i] - '0';
		}
		RES -> score = score;
		if (score == 100) {
			strcpy(RES -> status, "Accepted");
		} else if (score == 0) {
			strcpy(RES -> status, "Wrong Answer");
		} else {
			strcpy(RES -> status, "Partly Correct");
		}
		
		if (spj_res != NULL) free(spj_res);
	} else {
		int tmp_res = compare(RES -> out, RES -> ans);
		if (tmp_res == -1) {
			REPORTER("Run comparator fail");
			return -1;
		}
		
		if (! tmp_res) {
			RES -> score = 100;
			strcpy(RES -> status, "Accepted");
		} else {
			strcpy(RES -> status, "Wrong Answer");
		}
	}
	return 0;
} 


Result run(Config *CFG) {
	char* status = (char*)malloc(sizeof(char) * 32);
	strcpy(status, "System Error");
	Result RES = {0, status, NULL, NULL, NULL, NULL, 0, 0};
	char file_name[64] = {0};

	if (compile(CFG -> language, CFG -> source_name, file_name, CFG -> compile_option) != 0) {
		strcpy(status, "Compile Error");
		RES.status = status;
		return RES;
	}
	if ((RES.compile_info = READFILE("compile.out", 256)) == NULL) {
		REPORTER("Read compile result fail");
		return RES;
	}
	if (access(file_name, 0) != 0) {
		strcpy(status, "Compile Error");
		RES.status = status;
		return RES;
	}
	
	struct stat statbuf;
	if (stat(CFG -> ans_file, &statbuf)) {
		strcpy(status, "No Answers");
		RES.status = status;
		REPORTER("Get answer file size fail");
		if (remove(file_name)) {
			REPORTER("Delete program fail");
			return RES;
		}
		return RES;
	}
	int size = statbuf.st_size;
	
	RunResult RRES = {0, 0, 0, 0, SIGABRT};
	RunConfig RCFG = {0, 1, 1, file_name, CFG -> in_file, CFG -> out_file, NULL, 
					{CFG -> time_limit, CFG -> memory_limit, size}};

	if (runner(&RCFG, &RRES) != 0) {
		REPORTER("Run progream fail");
		return RES;
	}
	if (remove(file_name) != 0) {
		REPORTER("Delete program fail");
		return RES;
	}
	
	RES.use_time = RRES.use_time;
	RES.use_memory = RRES.use_memory;
	if (RRES.judger_error) {
		strcpy(status, "Judger Error");
	} else if (RRES.run_signal == 0) {
		if (RRES.return_value == 0) {
			strcpy(status, "Run Successfully");
		} else {
			strcpy(status, "Runtime Error");
		}
		if (RRES.use_time > CFG -> time_limit) {
			strcpy(status, "Time Limit Exceed");
		} else if (RRES.use_memory > CFG -> memory_limit * 1024) {
			strcpy(status, "Memory Limit Exceed");
		}
	} else if (RRES.run_signal == 31) {
		strcpy(status, "Dangerous System Call");
	} else if (RRES.run_signal == 9) {
		if (RRES.use_time > CFG -> time_limit) {
			strcpy(status, "Time Limit Exceed");
		}
	} else if (RRES.run_signal == 11) {
		if (RRES.use_memory > CFG -> memory_limit * 1024) {
			strcpy(status, "Memory Limit Exceed");
		} else {
			strcpy(status, "Runtime Error");
		}
	} else if (RRES.run_signal == 25) {
		strcpy(status, "Output Limit Exceed");
	}
	RES.status = status;
	
	// limit singel file size below 512MB
	if ((RES.in = READFILE(CFG -> in_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER("Read in file fail");
		delete_files(&RES);
		return RES;
	}
	if ((RES.out = READFILE(CFG -> out_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER("Read out file fail");
		delete_files(&RES);
		return RES;
	}
	if ((RES.ans = READFILE(CFG -> ans_file, 512 * 1024 * 1024)) == NULL) {
		REPORTER("Read ans file fail");
		delete_files(&RES);
		return RES;
	}
	
	if (strcmp(RES.status, "Run Successfully") == 0) {
		if (get_result(CFG, &RES) != 0) {
			REPORTER("Get Result fail");
			delete_files(&RES);
			return RES;
		}
	}
	
	return RES;
}


void delete_files(Result *RES) {
	if (RES -> in != NULL) free(RES -> in);
	if (RES -> out != NULL) free(RES -> out);
	if (RES -> ans != NULL) free(RES -> ans);
	if (RES -> compile_info != NULL) free(RES -> compile_info);
}


void delete_all(Result *RES) {
	if (RES -> in != NULL) free(RES -> in);
	if (RES -> out != NULL) free(RES -> out);
	if (RES -> ans != NULL) free(RES -> ans);
	if (RES -> compile_info != NULL) free(RES -> compile_info);
	if (RES -> status != NULL) free(RES -> status);
}

