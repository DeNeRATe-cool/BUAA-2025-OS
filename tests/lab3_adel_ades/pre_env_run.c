static inline void pre_env_run(struct Env *e) {
	struct Trapframe *tf = (curenv == e) ? (struct Trapframe *)KSTACKTOP - 1 : &e->env_tf;
	u_int epc = tf->cp0_epc;
	if (epc == 0x400180) {
		u_int ret = tf->regs[2];
		printk("env %08x reached end pc: 0x%08x, $v0=0x%08x\n", e->env_id, epc,
		       tf->regs[2]);
		if (ret == -1) {
			printk("\nerror! Don't take epc + 4 to skip the lw exception!\n");
		} else if (ret == -2) {
			printk(
			    "\nerror! You get the value in wrong address! Please check your imm\n");
		} else if (ret == -3) {
			printk("\nerror! Don't take epc + 4 to skip the sw exception!\n");
		} else if (ret == -4) {
			printk("\nerror! You store the value into wrong address! Please check your "
			       "imm\n");
		}
		env_destroy(e);
		schedule(0);
	}
}
