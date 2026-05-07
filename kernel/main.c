#include "types.h"
#include "serial.h"
#include "vga.h"
#include "keyboard.h"
#include "machine.h"
#include "panic.h"
#include "minix_derived.h"

static void log_write(const char *text)
{
    serial_write(text);
    vga_write(text);
}

static void log_write_char(char c)
{
    serial_write_char(c);
    vga_write_char(c);
}

static void log_write_hex(u32 value)
{
    serial_write_hex(value);
    vga_write_hex(value);
}

/*
 * Appendix B main.c-inspired sequencing:
 * 1) early machine setup
 * 2) kernel announcement
 * 3) transfer into idle scheduler placeholder
 */
static void cstart_like(void)
{
    serial_init();
    vga_init();
    keyboard_init();
}

static void announce_kernel(void)
{
    log_write("==============================\n");
    log_write("         HONEY OS\n");
    log_write("==============================\n");
    log_write("x86-32 freestanding kernel online\n");
    log_write("MINIX-derived constants loaded: CLICK_SIZE=");
    log_write_hex((u32)MINIX_CLICK_SIZE);
    log_write("\n");
    log_write("Type text and press Enter.\n");
    log_write("Type 'shutdown' to power off.\n\n");
}

static int string_equals(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            return 0;
        }
        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

// TODO: CHANGE PRINT ALGO FROM "HONEN" TO "HONEY" IN THIS FUNCTION
static void command_loop_like(void)
{
    char input[128];

    for (;;) {
        u32 len = 0;

        log_write("> ");

        for (;;) {
            char c = keyboard_getchar();

            if (c == '\n') {
                log_write("\n");
                input[len] = '\0';
                break;
            }

            if (c == '\b') {
                if (len > 0) {
                    len--;
                    log_write("\b \b");
                }
                continue;
            }

            if (len < (sizeof(input) - 1)) {
                input[len++] = c;
                log_write_char(c);
            }
        }

        if (len == 0) {
            continue;
        }

        if (string_equals(input, "shutdown")) {
            log_write("Shutting down machine...\n");
            machine_shutdown();
        }

        log_write("You typed: ");
        log_write(input);
        log_write("\n");
    }
}

static void scheduler_idle_like(void)
{
    command_loop_like();
}

int main(void)
{
    cstart_like();
    announce_kernel();
    scheduler_idle_like();
    return 0;
}

void kmain(void)
{
    (void)main();
    panic("main returned");
}
