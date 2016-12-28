#include "../src/judger.c"

// #include <mcheck.h>

int main() {
	// setenv("MALLOC_TRACE", "mtrace.out", 1);  
	// mtrace();
	char* argv[] = {NULL};
	Config CFG = {"C++", "MLE/lll.cpp", "lll.in", "lll.out", "lll.ans", 1000, 128, argv};
	Result RES;
	int i;
	/*
	for (i = 0; i < 1; i++) {
		RES = run(&CFG);
		printf("status: %s memory: %d\n", RES.status, RES.use_memory);
		delete_all(&RES);
	}
	*/
	
	char* test[9] = {"OLE", "RE", "TLE", "MLE", "MLE(stack)", "CE", "WA", "DSC", "AC"};
	char tmp[32];
	for (i = 0; i < 9; i++) {
		strcpy(tmp, test[i]);
		strcat(tmp, (char*)"/lll.cpp");
		CFG.source_name = tmp;
		printf("%s Config:\n", test[i]);
		printf("lan: %s\nname: %s\n", CFG.language, CFG.source_name);
		printf("in: %s\nout: %s\nans: %s\n", CFG.in_file, CFG.out_file, CFG.ans_file);
		printf("time lim: %d\nmem lim: %d\n", CFG.time_limit, CFG.memory_limit);
		RES = run(&CFG);
		printf("%s Result:\n", test[i]);
		printf("statu: %s\ntime: %d\nmemory: %d\n", RES.status, RES.use_time, RES.use_memory);
		printf("in: %s\nout: %s\nans: %s\n", RES.in, RES.out, RES.ans);
		printf("\n\n");
		delete_all(&RES);
	}
	
	/*
	for (i = 0; i < 30; i++) {
		RES = run(CFG);
		// printf("memory: %d\n", RES.use_memory);
		printf("time: %d memory: %d\n", RES.use_time, RES.use_memory);
		delete_all(RES);
	}
	*/
	
	return 0;
}
