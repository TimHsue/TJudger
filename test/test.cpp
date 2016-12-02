#include "../src/sandbox.cpp"

int main() {
	RunConfig RCF(true, true, (char*)"./lll", (char*)"lll.in", (char*)"lll.out", LimitList(1000, 64));
	RunResult RES;
	SandBox SB;
	SB.runner(RCF, RES);
	printf("time_use = %d, mem_use = %d, return_val = %d judge_error = %d\n" , RES.use_time, RES.use_memory, RES.run_status, RES.judger_error);
	return 0;
}
