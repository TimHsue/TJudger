#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main() {
	char ** argv = new char*[32];
	char *a = (char *)"1asdf";
	char b[] = "adsfa";
	char c[] = "asdfasdf";
	argv[0] = a;
	argv[1] = b;
	argv[2] = c;
	for (int i = 0; i <= 2; i++) {
		printf("%s\n", argv[i]);
	}
	return 0;
}
