#pragma once

#include "types.h"

// Minimal framebuffer abstraction. Actual framebuffer address/format
// should be provided by the bootloader (multiboot framebuffer info) or
// by early assembly that sets a graphics mode.

int framebuffer_init(void *addr, u32 width, u32 height, u32 pitch, u32 bpp);
void framebuffer_putpixel(u32 x, u32 y, u32 color);
void framebuffer_clear(u32 color);

// Query functions
u32 framebuffer_width(void);
u32 framebuffer_height(void);
int framebuffer_available(void);
