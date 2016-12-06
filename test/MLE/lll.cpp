#include <stdio.h>

int c[30000000];

int main() {
	int a, b;
	for (int i = 1; i < 30000000; i++) c[i] = i;
	scanf("%d %d", &a, &b);
	printf("%d\n", a + b);
	return 0;
}
