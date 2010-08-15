#ifndef STDLIB_H
#define STDLIB_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

extern void* malloc(unsigned int byte_count);
extern void* calloc(unsigned int member_count, unsigned int member_size);
extern void  free(void* addr);

#define NULL 0

#endif
