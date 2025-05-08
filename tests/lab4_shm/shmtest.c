#include <lib.h>

int main() {
	volatile char *va1 = (char *)0x500000;
	volatile char *va2 = (char *)0x600000;
	int key = shm_new(2);

	shm_bind(key, (void *)va1);
	shm_bind(key, (void *)va2);

	va1[0] = va2[0] + 10;
	debugf("va1[0]: %d, va2[0]: %d, they shall be same\n", va1[0], va2[0]);

	shm_unbind(key, (void *)va1);
	shm_unbind(key, (void *)va2);

	va1[0] = va2[0] + 10;
	debugf("va1[0]: %d, va2[0]: %d, they shall be different\n", va1[0], va2[0]);

	shm_free(key);

	return 0;
}
