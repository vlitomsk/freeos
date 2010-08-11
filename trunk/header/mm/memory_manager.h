#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define NULL             0
// !!чтобы старт и енд обязательно делились на sizeof(u64) !! и аддишн!!
#define HEAP_START       0x10000
#define HEAP_END         0x100000
#define MAX_HEAP_PARTS   512
//#define REALLOC_ADDITION 0x100 /*Прослойка для realloc'а*/
#define REALLOC_ADDITION 0x0

#define OSOB_START 1
#define OSOB_END   2

struct __heap_part {
    char used;
	void* addr;
	unsigned int size;
	unsigned char osob;
	struct __heap_part* next;
	struct __heap_part* prev;
};
extern void print_debug(void);
extern void clean_heap(void);
extern void reset_heap(void);
extern void* __kmalloc(unsigned int);
extern void* __kcalloc(unsigned int, unsigned int);
extern void __kfree(void*);
extern void* __krealloc(void*, unsigned int);

#endif
