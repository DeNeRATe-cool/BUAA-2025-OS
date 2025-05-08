#include <lib.h>
#include <mmu.h>

int shm_new(u_int npage) {
	return syscall_shm_new(npage);
}

int shm_bind(u_int key, void *va) {
	return syscall_shm_bind(key, (u_int)va, PTE_D);
}

int shm_unbind(u_int key, void *va) {
	return syscall_shm_unbind(key, (u_int)va);
}

int shm_free(u_int key) {
	return syscall_shm_free(key);
}
