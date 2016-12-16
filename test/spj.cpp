#include <iostream>
#include <cstdio>
using namespace std;

#define is_num(x) (x <= '9' and x >= '0')
int get_num(char **pos_pointer) {
	char *now_pos = *pos_pointer;
	char tmp = *now_pos;
	int res = 0;
	
	while (not is_num(tmp)) {
		if (tmp == EOF) return res;
		tmp = *(++now_pos);
	}
	while (    is_num(tmp)) {
		res = (res << 3) + (res << 1) + tmp - '0';
		tmp = *(++now_pos);
	}
	
	*pos_pointer = now_pos;
	return res;
}

int main(int argc, char *argv[]) {
	char *in = argv[0];
	char *out = argv[1];
	char *ans = argv[2];
	int a, b, c1, c2;
	a = get_num(&in);
	b = get_num(&in);
	c1 = get_num(&ans);
	c2 = get_num(&out);
	if (a + b != c2) printf("%d\n", 0);
	else printf("%d\n", 50);
}
