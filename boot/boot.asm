[BITS 32]

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

section .text
global start
extern kmain

global boot_multiboot_magic
global boot_multiboot_info_ptr

section .data
align 4
boot_multiboot_magic:    dd 0
boot_multiboot_info_ptr: dd 0

section .text

start:
    cli
    mov [boot_multiboot_magic], eax
    mov [boot_multiboot_info_ptr], ebx
    mov esp, stack_top
    call kmain

.halt:
    hlt
    jmp .halt

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
