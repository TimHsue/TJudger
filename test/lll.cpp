#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int c[30000000];

int main() {
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%d\n", a + b);
	for (int i = 1; i <= 50000000; i++) c[i] = i;
	// while (true) fork();
	return 0;
}
