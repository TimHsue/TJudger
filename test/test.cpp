#include "../src/judger.cpp"

int main() {
	char* argv[] = {(char*)"-lm", NULL};
	compile((char*)"C++", (char*)"lll.cpp", argv);
	return 0;
}
