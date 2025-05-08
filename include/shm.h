#ifndef _SHM_H_
#define _SHM_H_

#include <mmu.h>
#include <types.h>

#define N_SHM_PAGE 8
#define N_SHM 8

struct Shm {
	u_int npage;
	struct Page *pages[N_SHM_PAGE];
	u_int open;
};

#endif
