#include "framebuffer.h"
#include "serial.h"

static void *fb_addr = 0;
static u32 fb_width = 0;
static u32 fb_height = 0;
static u32 fb_pitch = 0;
static u32 fb_bpp = 0;

int framebuffer_init(void *addr, u32 width, u32 height, u32 pitch, u32 bpp)
{
    if (!addr || width == 0 || height == 0) {
        serial_write("framebuffer_init: invalid parameters\n");
        return -1;
    }

    fb_addr = addr;
    fb_width = width;
    fb_height = height;
    fb_pitch = pitch;
    fb_bpp = bpp;

    serial_write("framebuffer_init: initialized\n");
    return 0;
}

void framebuffer_putpixel(u32 x, u32 y, u32 color)
{
    if (!fb_addr) return;
    if (x >= fb_width || y >= fb_height) return;

    // Support 32bpp packed pixel (0xAARRGGBB or 0x00RRGGBB)
    if (fb_bpp == 32) {
        u8 *base = (u8*)fb_addr + y * fb_pitch + x * 4;
        base[0] = (color & 0x000000FF);
        base[1] = (color & 0x0000FF00) >> 8;
        base[2] = (color & 0x00FF0000) >> 16;
        base[3] = (color & 0xFF000000) >> 24;
    }
    // For other bpp values, do nothing for now.
}

void framebuffer_clear(u32 color)
{
    if (!fb_addr) return;
    for (u32 y = 0; y < fb_height; ++y) {
        for (u32 x = 0; x < fb_width; ++x) {
            framebuffer_putpixel(x, y, color);
        }
    }
}

u32 framebuffer_width(void) { return fb_width; }
u32 framebuffer_height(void) { return fb_height; }
int framebuffer_available(void) { return fb_addr != 0; }
