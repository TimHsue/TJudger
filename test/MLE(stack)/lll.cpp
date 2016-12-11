#include <stdio.h>

int res;

int dfs(int de) {
	int i = de;
	if (de < 10000000) {
		res = dfs(de + 1);
	} else {
		return 0;
	}
	return i;
}

int main() {
	dfs(0);
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%d\n", a + b);
	return 0;
}
