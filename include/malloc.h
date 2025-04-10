#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <pmap.h>

#define HEAP_BEGIN 0x80400000
#define HEAP_SIZE 0x400000

#define MBLOCK_SIZE sizeof(struct MBlock)

#define MBLOCK_PREV(elm, field) (struct MBlock *)((elm)->field.le_prev) // place entry at first

LIST_HEAD(MBlock_list, MBlock);
typedef LIST_ENTRY(MBlock) MBlock_LIST_entry_t;

struct MBlock {
	MBlock_LIST_entry_t mb_link;

	u_int size;    // size of the available space, if allocated, is the size of allocated space
	void *ptr;     // pointer to the begin of block, as a magic number to check for validity
	u_int free;    // 1 if block is free, 0 if the block is allocated
	u_int padding; // padding to make size of block multiple of 8
	char data[];   // data of the block, allocated for user
};

void malloc_init(void);
void *malloc(size_t size);
void free(void *p);

#endif
