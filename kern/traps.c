#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <trap.h>

extern void handle_int(void);
extern void handle_tlb(void);
extern void handle_sys(void);
extern void handle_mod(void);
extern void handle_reserved(void);
extern void handle_adel(void);
extern void handle_ades(void);

void (*exception_handlers[32])(void) = {
    [0 ... 31] = handle_reserved,
    [0] = handle_int,
    [2 ... 3] = handle_tlb,
    [4] = handle_adel,
    [5] = handle_ades,
#if !defined(LAB) || LAB >= 4
    [1] = handle_mod,
    [8] = handle_sys,
#endif
};

/* Overview:
 *   The fallback handler when an unknown exception code is encountered.
 *   'genex.S' wraps this function in 'handle_reserved'.
 */
void do_reserved(struct Trapframe *tf) {
	print_tf(tf);
	panic("Unknown ExcCode %2d", (tf->cp0_cause >> 2) & 0x1f);
}

void do_adel(struct Trapframe *tf) {
 	// 在此实现相应操作以使修改后指令符合要求
	u_long va = tf -> cp0_epc;
	Pte * pte;
	page_lookup(curenv -> env_pgdir, va, &pte);
	u_long pa = PTE_ADDR(*pte) | (va & 0xfff);
	u_long kva = KADDR(pa);
	u_int * instr = (u_int *) kva;
	u_int base = ((*instr) >> 21) & 0x1F;
	u_int rt = ((*instr) >> 16) & 0x1F;
	u_int imm = (*instr) & 0xffff;
	u_int immm = imm;
	if(imm & 0x8000) immm |= 0xFFFF0000;
	u_int loc = tf -> regs[base] + immm;
	// printk("base = %u rt = %u immm = %u loc = %u\n", base, rt, immm, loc);
	u_int offset = loc & 0x3;
	// printk("offset = %u\n", offset);
	u_int ans = imm - offset;
	// u_int ans = ((short)(imm) - (short)(offset)) & 0xFFFF;
	u_int new_inst = ((*instr) & (~0xffff)) | ans;
	*instr = new_inst;
	printk("AdEL handled, new imm is : %04x\n", new_inst & 0xffff); // 这里的 new_inst 替换为修改后的指令
	tf -> regs[rt] = (*((u_int *)(loc & (~0x3))));
	tf->cp0_epc += 4;
}

void do_ades(struct Trapframe *tf) {
 	u_long va = tf -> cp0_epc;
	Pte * pte;
	page_lookup(curenv -> env_pgdir, va, &pte);
	u_long pa = PTE_ADDR(*pte) | (va & 0xfff);
	u_long kva = KADDR(pa);
	u_int * instr = (u_int *) kva;
	u_int base = ((*instr) >> 21) & 0x1F;
	u_int rt = ((*instr) >> 16) & 0x1F;
	u_int imm = (*instr) & 0xffff;
	u_int immm = imm;
	if(imm & 0x8000) immm |= 0xFFFF0000;
	u_int loc = tf -> regs[base] + immm;
	// printk("base = %u rt = %u immm = %u loc = %u\n", base, rt, immm, loc);
	u_int offset = loc & 0x3;
	// printk("offset = %u\n", offset);
	u_int ans = imm - offset;
	// u_int ans = ((short)(imm) - (short)(offset)) & 0xFFFF;
	u_int new_inst = ((*instr) & (~0xffff)) | ans;
	*instr = new_inst;
	printk("AdES handled, new imm is : %04x\n", new_inst & 0xffff); // 这里的 new_inst 替换为修改后的指令
	*((u_int *)(loc & (~0x3))) = tf -> regs[rt];
	tf->cp0_epc += 4;	// 在此实现相应操作以使修改后指令符合要求
}
