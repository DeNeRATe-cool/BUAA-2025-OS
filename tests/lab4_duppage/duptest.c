#include <lib.h>

int main() {
	int r;

	// test new syscall
	r = fork();
	if (r == 0) {
		int this = syscall_getenvid();
		int that = syscall_get_parent_envid();
		debugf("^@^@^ I am the child %x, my parent is %x\n", this, that);
		ipc_send(that, 0, 0, 0);
		return 0;
	} else if (r > 0) {
		int that = syscall_get_parent_envid();
		ipc_recv(0, 0, 0);
		debugf("^@^@^ I am the parent, my 'parent' shall be zero: %x\n", that);
	}

	// test PTE_PROTECT
	volatile char *mem_protected = (char *)0x410000;
	syscall_mem_alloc(0, (void *)mem_protected, PTE_D | PTE_PROTECT);
	mem_protected[0] = 0x7c;
	r = fork();
	if (r == 0) {
		// passive alloc
		debugf("^@^@^ I am the child, I read from mem: %x\n", mem_protected[0]);
		mem_protected[0] = 0x3c;
		debugf("^@^@^ I am the child, I read from mem again: %x\n", mem_protected[0]);
		ipc_send(syscall_get_parent_envid(), 0, 0, 0);
		return 0;
	} else if (r > 0) {
		ipc_recv(0, 0, 0);
		debugf("^@^@^ I am the parent, I read from mem: %x\n", mem_protected[0]);
	}
	return 0;
}
