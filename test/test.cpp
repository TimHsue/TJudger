#include "../src/judger.cpp"

int main() {
	char* argv[] = {NULL};
	Config CFG((char*)"C++", (char*)"lll.cpp", (char*)"lll.in", (char*)"lll.out", 
	(char*)"lll.ans", 1000, 512, argv);
	Result RES;
	run(CFG, RES);
	printf("statu: %s\ntime: %d\nmemory: %d\n", RES.status, RES.use_time, RES.use_memory);
	printf("in: %s\nout: %s\nans: %s\n", RES.in, RES.out, RES.ans);
	return 0;
}
