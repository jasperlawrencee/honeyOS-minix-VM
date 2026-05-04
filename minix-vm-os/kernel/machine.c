#include "types.h"
#include "machine.h"

static inline void outw(u16 port, u16 value)
{
    __asm__ __volatile__("outw %0, %1" : : "a"(value), "Nd"(port));
}

void machine_shutdown(void)
{
    /* Common ACPI power-off paths for QEMU/Bochs/VirtualBox variants. */
    outw(0x604, 0x2000);
    outw(0xB004, 0x2000);
    outw(0x4004, 0x3400);

    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}
