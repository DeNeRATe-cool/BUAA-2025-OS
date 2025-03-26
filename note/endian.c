#define REVERSE_32(n_n) \
	((((n_n)&0xff) << 24) | (((n_n)&0xff00) << 8) | (((n_n) >> 8) & 0xff00) | (((n_n) >> 24) & 0xff))

#define REVERSE_16(n_n) \
	((((n_n)&0xff) << 8) | (((n_n) >> 8) & 0xff))
