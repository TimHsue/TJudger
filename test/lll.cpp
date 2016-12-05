#include <stdlib.h>

int a[10000000];

int main() {
	for (int i = 0; i < 10000000; i++) a[i] = i;
	return 0;
}
