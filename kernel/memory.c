#include "types.h"
#include "memory.h"

/* These routines are intentionally tiny and freestanding for kernel bring-up. */
void *memcpy(void *dst, const void *src, size_t count)
{
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;
    size_t i;

    for (i = 0; i < count; i++) {
        d[i] = s[i];
    }

    return dst;
}

void *memset(void *dst, int value, size_t count)
{
    u8 *d = (u8 *)dst;
    size_t i;

    for (i = 0; i < count; i++) {
        d[i] = (u8)value;
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t count)
{
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;

    if (d == s || count == 0) {
        return dst;
    }

    if (d < s) {
        return memcpy(dst, src, count);
    }

    while (count > 0) {
        count--;
        d[count] = s[count];
    }

    return dst;
}
