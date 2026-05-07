#include "types.h"
#include "serial.h"
#include "vga.h"
#include "keyboard.h"
#include "gui.h"
#include "machine.h"
#include "panic.h"
#include "minix_derived.h"

extern u32 boot_multiboot_magic;
extern u32 boot_multiboot_info_ptr;

#include "framebuffer.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002u
#define MULTIBOOT_INFO_FLAG_FRAMEBUFFER (1u << 12)

struct multiboot_info_fb {
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    u32 cmdline;
    u32 mods_count;
    u32 mods_addr;
    u32 syms[4];
    u32 mmap_length;
    u32 mmap_addr;
    u32 drives_length;
    u32 drives_addr;
    u32 config_table;
    u32 boot_loader_name;
    u32 apm_table;
    u32 vbe_control_info;
    u32 vbe_mode_info;
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;
    u64 framebuffer_addr;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    u8 color_info[6];
} __attribute__((packed));

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

    if (boot_multiboot_magic == MULTIBOOT_BOOTLOADER_MAGIC && boot_multiboot_info_ptr != 0) {
        const struct multiboot_info_fb *mbi = (const struct multiboot_info_fb *)(u32)boot_multiboot_info_ptr;
        if ((mbi->flags & MULTIBOOT_INFO_FLAG_FRAMEBUFFER) != 0 && mbi->framebuffer_width != 0 && mbi->framebuffer_height != 0) {
            framebuffer_init((void *)(u32)mbi->framebuffer_addr, mbi->framebuffer_width, mbi->framebuffer_height, mbi->framebuffer_pitch, mbi->framebuffer_bpp);
        }
    }
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
}

static void announce_text_mode_ui(void)
{
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

    if (gui_init() == 0 && gui_available()) {
        serial_write("Launching Nuklear framebuffer UI.\n");
        for (;;) {
            gui_poll_and_draw();
        }
    } else {
        log_write("Framebuffer GUI unavailable; falling back to text UI.\n");
        announce_text_mode_ui();
        scheduler_idle_like();
    }
    return 0;
}

void kmain(void)
{
    (void)main();
    panic("main returned");
}
