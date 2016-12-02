#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

char tmp[2000] = "adsfasdf";

int main() {
	FILE* stream;
	stream = fopen("test.www", "w");
	fprintf(stream, "%s", tmp);
	fclose(stream);
	// printf("222");	
	return 0;
}
