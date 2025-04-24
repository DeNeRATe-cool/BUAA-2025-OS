#define ENV_CREATE_EDF(x, y, z)                                                                    \
	({                                                                                         \
		extern u_char binary_##x##_start[];                                                \
		extern u_int binary_##x##_size;                                                    \
		env_create_edf(binary_##x##_start, (u_int)binary_##x##_size, y, z);                \
	})

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	printk("init.c:\tmips_init() is called\n");

	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();
	env_init();

	ENV_CREATE_PRIORITY(test_hash1, 1);
	ENV_CREATE_PRIORITY(test_hash2, 3);
	ENV_CREATE_EDF(test_hash3, 1, 5);
	ENV_CREATE_EDF(test_hash4, 2, 7);

	schedule(0);
	panic("init.c:\tend of mips_init() reached!");
}
