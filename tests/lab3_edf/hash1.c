#define MOD 19260817

int strlen(char *s) {
	int i;
	for (i = 0; s[i]; i++) {
	}
	return i;
}

int main() {
	int i;
	char str[] = "Hello World";
	int len = strlen(str);
	int hash = 0;
	for (i = 0; i < 1000000; i++) {
		hash = hash * 79 + str[i % len] - 'A' + 10;
		hash %= MOD;
	}
	return hash;
}
