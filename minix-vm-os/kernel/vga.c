#include "types.h"
#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_LIGHT_GREY 0x07
#define VGA_COLOR_WHITE 0x0F

static volatile u16 *const VGA_MEMORY = (u16 *)0xB8000;

static u8 vga_row;
static u8 vga_column;
static u8 vga_color = VGA_COLOR_WHITE;

static u16 vga_entry(unsigned char c, u8 color)
{
    return (u16)c | ((u16)color << 8);
}

static void vga_clear_row(u8 row)
{
    u16 x;
    for (x = 0; x < VGA_WIDTH; ++x) {
        VGA_MEMORY[row * VGA_WIDTH + x] = vga_entry(' ', vga_color);
    }
}

static void vga_scroll(void)
{
    u16 row;
    u16 col;

    for (row = 1; row < VGA_HEIGHT; ++row) {
        for (col = 0; col < VGA_WIDTH; ++col) {
            VGA_MEMORY[(row - 1) * VGA_WIDTH + col] = VGA_MEMORY[row * VGA_WIDTH + col];
        }
    }

    vga_clear_row(VGA_HEIGHT - 1);
}

static void vga_newline(void)
{
    vga_column = 0;
    if (vga_row < VGA_HEIGHT - 1) {
        ++vga_row;
        return;
    }

    vga_scroll();
}

void vga_init(void)
{
    u8 row;

    vga_row = 0;
    vga_column = 0;
    vga_color = VGA_COLOR_LIGHT_GREY;

    for (row = 0; row < VGA_HEIGHT; ++row) {
        vga_clear_row(row);
    }
}

void vga_write_char(char c)
{
    if (c == '\n') {
        vga_newline();
        return;
    }

    if (c == '\r') {
        vga_column = 0;
        return;
    }

    if (c == '\b') {
        if (vga_column > 0) {
            --vga_column;
        } else if (vga_row > 0) {
            --vga_row;
            vga_column = (VGA_WIDTH - 1);
        } else {
            return;
        }

        VGA_MEMORY[vga_row * VGA_WIDTH + vga_column] = vga_entry(' ', vga_color);
        return;
    }

    VGA_MEMORY[vga_row * VGA_WIDTH + vga_column] = vga_entry((unsigned char)c, vga_color);

    ++vga_column;
    if (vga_column >= VGA_WIDTH) {
        vga_newline();
    }
}

void vga_write(const char *s)
{
    while (*s != '\0') {
        vga_write_char(*s++);
    }
}

void vga_write_hex(u32 value)
{
    int shift;

    vga_write("0x");
    for (shift = 28; shift >= 0; shift -= 4) {
        u8 digit = (u8)((value >> shift) & 0xF);
        vga_write_char((char)(digit < 10 ? ('0' + digit) : ('A' + (digit - 10))));
    }
}
