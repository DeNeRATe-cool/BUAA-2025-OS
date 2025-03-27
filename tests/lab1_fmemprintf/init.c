#include <stream.h>

void fmemprintf_1_check(void) {
	int r[3];
	FILE f;
	char buffer[64] = "abclo, ";
	FILE *stream = fmemopen(&f, buffer, "a");
	if (stream != NULL) {
		r[0] = fmemprintf(stream, "%s %d", "MOS", 2025);
		fseek(stream, 0, SEEK_SET);
		r[1] = fmemprintf(stream, "%s", "Hel");
		fseek(stream, 0, SEEK_END);
		r[2] = fmemprintf(stream, "%c", '!');
		fclose(stream);
		printk("%d %d %d\n", r[0], r[1], r[2]);
		printk("%s\n", buffer);
	}
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	fmemprintf_1_check();
	halt();
}
