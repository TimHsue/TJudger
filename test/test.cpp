#include "../src/judger.cpp"
#include <string>
#include <cstring>
#include <iostream>

int main() {
	char* argv[] = {NULL};
	Config CFG((char*)"C++", (char*)"lll.cpp", (char*)"lll.in", (char*)"lll.out", 
	(char*)"lll.ans", 1000, 64, argv);
	Result RES;
	
	char* test[] = {(char*)"AC", (char*)"RE", (char*)"TLE", (char*)"MLE", (char*)"CE", (char*)"WA", (char*)"DSC", (char*)"OLE"};
	char tmp[32];
	for (int i = 0; i < 8; i++) {
		strcpy(tmp, test[i]);
		strcat(tmp, (char*)"/lll.cpp");
		CFG.source_name = tmp;
		printf("%s Config:\n", test[i]);
		printf("lan: %s\nname: %s\n", CFG.language, CFG.source_name);
		printf("in: %s\nout: %s\nans: %s\n", CFG.in_file, CFG.out_file, CFG.ans_file);
		printf("time lim: %d\nmem lim: %d\n", CFG.time_limit, CFG.memory_limit);
		run(CFG, RES);
		printf("%s Result:\n", test[i]);
		printf("statu: %s\ntime: %d\nmemory: %d\n", RES.status, RES.use_time, RES.use_memory);
		printf("in: %s\nout: %s\nans: %s\n", RES.in, RES.out, RES.ans);
		printf("\n\n");
	}
	
	return 0;
}
