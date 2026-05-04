#include "serial.h"
#include "vga.h"
#include "panic.h"

void panic(const char *message)
{
    serial_write("PANIC: ");
    serial_write(message);
    serial_write("\n");

    vga_write("PANIC: ");
    vga_write(message);
    vga_write("\n");

    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}
