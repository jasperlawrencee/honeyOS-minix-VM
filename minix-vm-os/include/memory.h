#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void *memcpy(void *dst, const void *src, size_t count);
void *memset(void *dst, int value, size_t count);
void *memmove(void *dst, const void *src, size_t count);

#endif
