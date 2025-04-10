#include <malloc.h>
#include <pmap.h>

u_int address[1000];
u_int end[1000];
int pos = 0;

void out_of_range() {
	printk("Invalid address: out of range\n");
}

void overlap() {
	printk("Invalid address: address overlap\n");
}

void should_null() {
	printk("Invalid alloc: address should be NULL\n");
}

void not_null() {
	printk("Invalid alloc: address should not be NULL\n");
}

void not_aligned() {
	printk("Invalid address: not aligned to 8\n");
}

char check_null(void *p) {
	if (p != NULL) {
		should_null();
		return 0;
	}
	return 1;
}

char check(u_int a, u_int b) {

	if (a == 0) {
		not_null();
		return 0;
	}

	if (a < (u_int)HEAP_BEGIN || b > (u_int)(HEAP_BEGIN + HEAP_SIZE)) {
		out_of_range();
		return 0;
	}

	if ((a & 7) != 0) {
		not_aligned();
		return 0;
	}

	for (int i = 0; i < pos; i++) {
		if ((a >= address[i] && a <= end[i]) || (b >= address[i] && b <= end[i])) {
			overlap();
			return 0;
		}
	}

	address[pos] = a;
	end[pos++] = b;

	return 1;
}

void rem(u_int a, u_int b) {
	for (int i = 0; i < pos; i++) {
		if (address[i] == a && end[i] == b) {
			address[i] = 0;
			end[i] = 0;
		}
	}
}

void malloc_test() {
	void *p1 = malloc(0x100000);
	assert(check((u_int)p1, (u_int)p1 + 0x100000));

	void *p2 = malloc(0x100000);
	assert(check((u_int)p2, (u_int)p2 + 0x100000));

	void *p3 = malloc(0x100000);
	assert(check((u_int)p3, (u_int)p3 + 0x100000));

	void *p4 = malloc(0x100000);
	assert(check_null(p4));

	void *p5 = malloc(100);
	assert(check((u_int)p5, (u_int)p5 + 100));

	printk("malloc_test() is done\n");
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	printk("init.c:\tmips_init() is called\n");

	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();
	malloc_init();

	malloc_test();
	halt();
}
