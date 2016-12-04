#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%d \n\n", a + b);
	return 0;
}
