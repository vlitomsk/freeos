#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define NULL             0
#define HEAP_START       0x10000
#define HEAP_END         0xFFFFFF
#define MAX_HEAP_PARTS   512
#define REALLOC_ADDITION 0x0 /*Прослойка для realloc'а*/

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

extern void clean_heap(void);
extern void reset_heap(void);
extern void* __kmalloc(unsigned int);
extern void* __kcalloc(unsigned int, unsigned int);
extern void __kfree(void*);
extern void* __krealloc(void*, unsigned int);

#endif
