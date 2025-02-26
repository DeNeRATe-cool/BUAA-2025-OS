#include <stdio.h>

int check(int n) {
	int bit[5];
	int len = 0;
	while(n) bit[len++] = n % 10, n /= 10;
	if(len == 0) return 1;
	for(int i = 0; i < len; i++) {
		if(bit[i] != bit[len - i - 1])
			return 0;
	}
	return 1;

}

int main() {
	int n;
	scanf("%d", &n);

	if (check(n)) {
		printf("Y\n");
	} else {
		printf("N\n");
	}
	return 0;
}
