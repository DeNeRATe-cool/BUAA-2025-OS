static inline void pre_env_run(struct Env *e) {
	static int count = 0;
	if (count > MOS_SCHED_MAX_TICKS) {
		printk("%4d: ticks exceeded the limit %d\n", count, MOS_SCHED_MAX_TICKS);
		halt();
	}
	printk("%4d: %08x\n", count, e->env_id);
	count++;

	static int flag[4] = {0};
	struct Trapframe *tf = (curenv == e) ? (struct Trapframe *)KSTACKTOP - 1 : &e->env_tf;
	u_int epc = tf->cp0_epc;
	if (epc == MOS_SCHED_END_PC && !flag[ENVX(e->env_id)]) {
		printk("env %08x reached end pc: 0x%08x, $v0=0x%08x\n", e->env_id, epc,
		       tf->regs[2]);
		flag[ENVX(e->env_id)] = 1;
	}

	if (count > MOS_SCHED_MIN_TICKS) {
		for (int i = 0; i < sizeof(flag) / sizeof(flag[0]); i++) {
			if (!flag[i]) {
				return;
			}
		}
		printk("test finished, halt mos!\n");
		halt();
	}
}
