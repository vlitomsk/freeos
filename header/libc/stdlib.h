#ifndef STDLIB_H
#define STDLIB_H

#include <memory_manager.h>

// Леонид, думаю вы не заметите три макроса :)

#define malloc(x) __kmalloc(x)
#define calloc(x) __kcalloc(x)
#define free(x) __kcalloc(x)
#define realloc(x) __krealloc(x)

#endif
