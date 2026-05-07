#include "types.h"
#include "keyboard.h"

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

static inline u8 inb(u16 port)
{
    u8 value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static int shift_down;

static char scancode_to_char(u8 scancode)
{
    static const char unshifted[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
    };

    static const char shifted[128] = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0
    };

    if (scancode >= 128) {
        return 0;
    }

    return shift_down ? shifted[scancode] : unshifted[scancode];
}

void keyboard_init(void)
{
    shift_down = 0;
}

char keyboard_getchar_nonblock(void)
{
    u8 status;
    u8 scancode;
    char out;

    status = inb(KBD_STATUS_PORT);
    if ((status & 0x01) == 0) {
        return 0;
    }

    scancode = inb(KBD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) {
        shift_down = 1;
        return 0;
    }

    if (scancode == 0xAA || scancode == 0xB6) {
        shift_down = 0;
        return 0;
    }

    if ((scancode & 0x80) != 0) {
        return 0;
    }

    out = scancode_to_char(scancode);
    if (out != 0) {
        return out;
    }

    return 0;
}

char keyboard_getchar(void)
{
    for (;;) {
        char c = keyboard_getchar_nonblock();
        if (c != 0) {
            return c;
        }
    }
}
