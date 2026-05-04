#include "types.h"
#include "serial.h"

#define COM1 0x3F8

static inline void outb(u16 port, u8 value)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline u8 inb(u16 port)
{
    u8 value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static int serial_ready_to_send(void)
{
    return (inb(COM1 + 5) & 0x20) != 0;
}

void serial_init(void)
{
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

void serial_write_char(char c)
{
    while (!serial_ready_to_send()) {
    }
    outb(COM1, (u8)c);
}

void serial_write(const char *s)
{
    while (*s != '\0') {
        if (*s == '\n') {
            serial_write_char('\r');
        }
        serial_write_char(*s++);
    }
}

void serial_write_hex(u32 value)
{
    int shift;
    serial_write("0x");
    for (shift = 28; shift >= 0; shift -= 4) {
        u8 digit = (u8)((value >> shift) & 0xF);
        serial_write_char((char)(digit < 10 ? ('0' + digit) : ('A' + (digit - 10))));
    }
}
