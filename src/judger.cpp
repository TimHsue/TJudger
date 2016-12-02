#include "judger.h"


// in case of execve("exe");
void generate_name(const char* in_name, char* out_name) {
	srand((unsigned)time(NULL) + 970830);
	int len = sizeof(in_name) / sizeof(char);
	
	for (int i = 0; i < len; i++) out_name[i] = rand() % 26 + 65;
}


int compile(const char* lan, char* file_name, char** compile_opt) {
	char compiler[32] = {0};
	char out_com[32] = {0};
	char out_name[32] = {0};
	
	if (strcmp(lan, (char*)"C++") == 0) {
		strcpy(compiler, "g++");
	} else if (strcmp(lan, (char*)"C") == 0) {
		strcpy(compiler, "gcc");
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

	printf("%s\n", argv[0]); // del
	printf("%s\n", argv[1]); // del;

	int len = 3;
	while(compile_opt[len] != NULL) argv[len++] = compile_opt[len - 3];
	argv[len] = NULL;
	
	SandBox com_sdb;
	RunConfig RCF(true, false, true, compiler, (char*)"", (char*)"compile.out", argv, LimitList(10000, 128));
	RunResult RES;
	com_sdb.runner(RCF, RES);
	delete argv;
	
	if (RES.run_status) {
		REPORTER((char*)"Compile fail.");
		return -1;
	}
	
	return 0;
}

void run(const Config &CFG, Result &RES) {
	
}
