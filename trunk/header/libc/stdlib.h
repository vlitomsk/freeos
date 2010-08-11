#ifndef STDLIB_H
#define STDLIB_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define LOW_BYTE(x) (x & 0xFF)
#define HIGH_BYTE(x) ((x & 0xFF00) >> 8)

#include <mm/memory_manager.h>

// Леонид, думаю вы не заметите три макроса :)

#define malloc(x) __kmalloc(x)
#define calloc(x) __kcalloc(x)
#define free(x) __kfree(x)
#define realloc(x) __krealloc(x)

#endif
