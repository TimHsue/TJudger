#include <stdio.h>

int a[1000];

int main() {
	for (int i = 1; i <= 100000; i++) a[i] = i;
	scanf("%d %d", &a[1], &a[2]);
	printf("%d\n", a[1] + a[2]);
	return 0;
}
