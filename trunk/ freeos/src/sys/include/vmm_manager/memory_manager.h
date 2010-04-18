#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define HEAP_START 0x10000
#define HEAP_END   0xFFFFFF
#define MAX_HEAP_PARTS 100
#define REALLOC_ADDITION 10 /*prosloika dlya realloc*/

#define OSOB_START 1
#define OSOB_END   2

struct __heap_part {
	void* addr;
	unsigned int size;
	unsigned char osob;
	struct __heap_part* next;
	struct __heap_part* prev;
};

//extern struct __heap_part first_part;
//extern int heap_part_count;

extern void clean_heap(void);
extern void* malloc(unsigned int byte_count);
extern void* calloc(unsigned int member_count, unsigned int member_size);
extern void free(void* addr);
extern void* realloc(void* ptr, unsigned int byte_count);

#endif
