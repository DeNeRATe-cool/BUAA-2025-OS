static void printk_dict_test_var() {
	int num = 123;
	int multiply = num * num;
	printk("We have a variable  {%7k}\n", "num", num);
	printk("The neg' of it is   (%-7k)\n", "-num", -num);
	printk("And another is      [%07k]", "multiply", multiply);
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	printk_dict_test_var();
	halt();
}
