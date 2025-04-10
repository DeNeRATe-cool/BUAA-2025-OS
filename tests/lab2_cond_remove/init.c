#include <mmu.h>
#include <pmap.h>
#include <print.h>

#undef NDEBUG

void test_cond_remove() {
	struct Page *p;
	assert(page_alloc(&p) == 0);
	Pde *pgdir = (Pde *)page2kva(p);
	cur_pgdir = pgdir;
	u_int va[4] = {UTEXT, UTEXT + PAGE_SIZE, UTEXT + 1024 * PAGE_SIZE,
		       UTEXT + 1025 * PAGE_SIZE};
	u_int perm[4] = {PTE_V, PTE_V | PTE_D | PTE_G, PTE_V | PTE_G, PTE_V | PTE_D};

	struct Page *pp;
	assert(page_alloc(&pp) == 0);
	assert(page_insert(pgdir, 0, pp, va[0], perm[0]) == 0);
	assert(page_insert(pgdir, 0, pp, va[1], perm[1]) == 0);
	assert(page_insert(pgdir, 0, pp, va[2], perm[2]) == 0);
	assert(page_insert(pgdir, 0, pp, va[3], perm[3]) == 0);
	int r = page_conditional_remove(pgdir, 0, PTE_D | PTE_G, 0, UTEXT + 1025 * PAGE_SIZE);

	assert(r == 2);
	assert(pp->pp_ref == 2);
	assert(page_lookup(pgdir, va[0], 0));
	assert(page_lookup(pgdir, va[3], 0));
	assert(page_lookup(pgdir, va[1], 0) == 0);
	assert(page_lookup(pgdir, va[2], 0) == 0);
	printk("test succeeded!\n");
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();
	test_cond_remove();
	halt();
}
