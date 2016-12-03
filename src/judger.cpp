#include "judger.h"


Result :: Result() {}
Result :: Result(char* st, char* ci, char* i, char* o, char* a, int tu, int mu) {
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
		int tl, int ml, char* spj, char** cop) {
	language = lg;
	source_name = sn;
	file_name = (char*)"";
	in_file = ifl;
	out_file = ofl;
	ans_file = afl;
	time_limit = tl;
	memory_limit = ml;
	special_judge = spj;
	compile_option = cop;
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

void run(Config &CFG, Result &RES) {
	FILE* stream;
	RES = Result((char*)"System Error", (char*)"", (char*)"", (char*)"", (char*)"", 0, 0);
	char file_name[64] = {0};

	if (compile(CFG.language, CFG.source_name, file_name, CFG.compile_option)) {
		RES.status = (char*)"Compile Error";
		return;
	}
	if ((RES.compile_info = READFILE((char*)"compile.out", 256)) == NULL) {
		REPORTER((char*)"Read compile result fail");
		return;
	}
	if (access(file_name, 0) != 0) {
		RES.status = (char*)"Compile Error";
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
	run_sdb.runner(RCFG, RRES);
	remove(CFG.file_name);
	
	RES.use_time = RRES.use_time;
	RES.use_memory = RRES.use_memory;
	if (RRES.judger_error) {
		RES.status = (char*)"Judger Error";
	} else if (RRES.run_status == 0) {
		RES.status = (char*)"Run Successfully";
		if (RRES.use_time > CFG.time_limit) RES.status = (char*)"Time Limit Exceed";
		if (RRES.use_memory > CFG.memory_limit) RES.status = (char*)"Memory Limit Exceed";
	} else if (RRES.run_status == 31) {
		RES.status = (char*)"Dangerous System Call";
	} else if (RRES.run_status == 9) {
		if (RRES.use_time > CFG.time_limit) RES.status = (char*)"Time Limit Exceed";
	} else if (RRES.run_status == 11) {
		if (RRES.use_memory > CFG.memory_limit) RES.status = (char*)"Memory Limit Exceed";
		else RES.status = (char*)"Runtime Error";
	} else if (RRES.run_status) {
		RES.status = (char*)"Runtime Error";
	}
	
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
}


