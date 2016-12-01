#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int main() {
	fork();
	printf("I shouldnt be here!");
	return 0;
}
