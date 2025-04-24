int __attribute__((optimize("O0"))) main() {
	unsigned int src1[2]; // 数组 src1

	src1[0] = 1;
	src1[1] = 2;
	unsigned int dst = 3;
	unsigned int *ptr = src1;

	// lw test
	asm volatile("lw %0, 5(%1)\n\t"
		     : "=r"(dst) /* 输出操作数，也是第0个操作数 %0 */
		     : "r"(ptr)	 /* 输入操作数，也是第1个操作数 %1 */
	);
	if (dst == 3) {
		return -1;
	} else if (dst != 2) {
		return -2;
	}

	// sw test
	unsigned int tmp = 4;
	ptr = (unsigned int *)((unsigned int)ptr | 0x1);
	asm volatile("sw %0, 1(%1)\n\t" : : "r"(tmp), "r"(ptr));
	dst = src1[0];
	if (dst == 2) {
		return -3;
	} else if (dst != 4) {
		return -4;
	}

	return dst;
}
