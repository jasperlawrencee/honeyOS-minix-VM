#ifndef VGA_H
#define VGA_H

#include "types.h"

void vga_init(void);
void vga_write_char(char c);
void vga_write(const char *s);
void vga_write_hex(u32 value);

#endif
