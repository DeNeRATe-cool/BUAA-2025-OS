static void printk_check_position(void) {
	printk("### check_position ###\n");
	printk("%s%P%s", "This is a testcase: ", 2, 5, "\n");
	printk("the position is %P, size = %d\n", 1, 9, 9 - 1);
}

static void printk_check_range(void) {
	printk("### check_range ###\n");
	printk("%s%z%s", "This is a testcase: ", 2023, 2023, "\n");
	printk("the range is %z, size = %d\n", 1, 9, 9 - 1);
	printk("the range is %-5lz\n", 66, 88);
	printk("the range is %05z\n", 1, 100000);
}

typedef struct {
	int a; char b; char c; int d;
} test_s1;

typedef struct {
	int size; int c[10];
} test_s2;

static void printk_check_jJ(void) {
	test_s1 s11 = {10, '*', 'd', 100000};
	test_s1 s12 = {-10, 'r', '$', -100000};
	test_s2 s21 = {3, {1, 2, 3}};
	test_s2 s22 = {5, {-1, -100000, 23, -6, 88888}};
	printk("### check_struct ###\n");
	printk("%5j\n", &s11);
	printk("%5j\n", &s12);
	printk("%5J\n", &s21);
	printk("%5J\n", &s22);
} 

static void printk_1_check(void) {
	printk("%5d\n", 999999);
	printk("%5d\n", 12);
	printk("%5d\n", -12);
	printk("%5d\n", -1234);
	printk("%05d\n", 12);
	printk("%-5d\n", 12);
	printk("%-5d\n", -12);
	printk("%05d\n", -12);
	printk("%05b\n", -8);
	printk("%05b\n", 2147483647);
	printk("%05d\n", -100);
	printk("%05o\n", -100);
	printk("%05O\n", -100);
	printk("%05u\n", -100);
	printk("%05U\n", -100);
	printk("%05x\n", 1194684);
	printk("%05X\n", 1194684);
	printk("%05x\n", -1194684);
	printk("%05X\n", -1194684);
	int asc = 65;
	for (asc = 65; asc <= 90; asc++) {
		printk("%c", asc);
	}
	printk("\n");
	char str[] = "I love buaa scse!";
	char *strptr = str;
	printk("%s\n", str);
	printk("%c", *strptr++);
	printk("%c", *strptr++);
	printk("%c", *strptr);
	printk("\n");
}

static void printk_2_check(void) {
	int a = 97;
	int b = -97;

	printk("%d\n", a);
	printk("%c\n", a);
	printk("%x\n", a);
	printk("%u\n", a);
	printk("%b\n", a);

	printk("i love buaa\n");
	printk("abcdefghijklmnopqrst\n");
	printk("i love os\n");
	printk("string\n");
	printk("good luck\n");

	printk("%dand%d\n", b, a);
	printk("%dand%c\n", b, a);
	printk("%dand%x\n", b, a);
	printk("%dand%b\n", b, a);
	printk("%dand%u\n", b, a);

	printk("%04dend\n", a);
	printk("%04xend\n", a);
	printk("%-4dend\n", a);
	printk("%-4xend\n", a);
	printk("%ldend\n", a);

	printk("%04ldend\n", a);
	printk("%04lxend\n", b);
}

static void printk_3_check(void) {
	int b = 98;
	int c = -98;
	int d = 2024;

	printk("%b\n", b);
	printk("%d\n", b);
	printk("%D\n", b);
	printk("%o\n", b);
	printk("%u\n", b);
	printk("%U\n", b);
	printk("%x\n", d);
	printk("%X\n", d);
	printk("%d%\0You shall not see this!\n", d);
	printk("%a%%\n");

	char str[] = "printk_3";
	printk("printk_3\n");
	printk("This letter is %c\n", *str);
	printk("start- %s -end\n", str);

	printk("%d and %d\n", c, b);
	printk("%d and %c\n", c, b);
	printk("%d and %x\n", c, b);
	printk("%d and %b\n", c, b);
	printk("%d and %u\n", c, b);
}

extern char bss_end[];

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	if ((u_long)mips_init < KERNBASE || (u_long)mips_init >= KSTACKTOP) {
		panic("bad address of kernel code: %x", mips_init);
	}
	if ((u_long)bss_end < KERNBASE || (u_long)bss_end > KSTACKTOP) {
		panic("bad address of bss_end: %x", (u_long)bss_end);
	}

	printk_1_check();
	printk_2_check();
	printk_3_check();
	printk_check_jJ();
	printk_check_range();
	printk_check_position();
	halt();
}
