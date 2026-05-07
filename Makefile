AS=nasm
CC=i686-elf-gcc

CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostdinc -Wall -Wextra -Werror -Iinclude
LDFLAGS=-T linker.ld -ffreestanding -nostdlib -Wl,--build-id=none

ifeq ($(OS),Windows_NT)
SHELL := cmd
.SHELLFLAGS := /C

MKDIR_BUILD=if not exist build mkdir build
MKDIR_ISODIR=if not exist build\isodir\boot\grub mkdir build\isodir\boot\grub
COPY_FILE=copy /Y
DEVNULL=>NUL 2>&1
RM_RF=if exist build rmdir /S /Q build
ISO_CMD=grub-mkrescue -o build\os.iso build\isodir >NUL 2>&1
QEMU_RUN=qemu-system-i386 -cdrom build\os.iso -serial mon:stdio -nographic -no-reboot -no-shutdown
QEMU_RUN_VGA=qemu-system-i386 -cdrom build\os.iso -no-reboot -no-shutdown
TOOLCHAIN_CHECK=powershell -NoProfile -ExecutionPolicy Bypass -File scripts\check-toolchain.ps1 -Root .
else
MKDIR_BUILD=mkdir -p build
MKDIR_ISODIR=mkdir -p build/isodir/boot/grub
COPY_FILE=cp
DEVNULL=>/dev/null 2>&1
RM_RF=rm -rf build
ISO_CMD=grub-mkrescue -o build/os.iso build/isodir >/dev/null 2>&1
QEMU_RUN=qemu-system-i386 -cdrom build/os.iso -serial mon:stdio -nographic -no-reboot -no-shutdown
QEMU_RUN_VGA=qemu-system-i386 -cdrom build/os.iso -no-reboot -no-shutdown
TOOLCHAIN_CHECK=:
endif

OBJS= \
build/boot.o \
build/main.o \
build/serial.o \
build/vga.o \
build/keyboard.o \
build/machine.o \
build/panic.o \
build/memory.o \
build/framebuffer.o \
build/gui_nuklear.o

all: build/os.iso

build/kernel.bin: $(OBJS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

build/boot.o: boot/boot.asm
	@$(MKDIR_BUILD)
	$(AS) -f elf32 $< -o $@

build/main.o: kernel/main.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/serial.o: kernel/serial.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/vga.o: kernel/vga.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/keyboard.o: kernel/keyboard.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/machine.o: kernel/machine.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/panic.o: kernel/panic.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/memory.o: kernel/memory.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/framebuffer.o: kernel/framebuffer.c
	@$(MKDIR_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

build/gui_nuklear.o: kernel/gui_nuklear.c
	@$(MKDIR_BUILD)
	$(CC) $(filter-out -Werror,$(CFLAGS)) -Wno-unused-function -Wno-unused-parameter -Wno-sign-compare -Wno-implicit-fallthrough -c $< -o $@

build/os.iso: build/kernel.bin grub/grub.cfg
	@$(MKDIR_ISODIR)
	$(COPY_FILE) build/kernel.bin build/isodir/boot/kernel.bin $(DEVNULL)
	$(COPY_FILE) grub/grub.cfg build/isodir/boot/grub/grub.cfg $(DEVNULL)
	$(ISO_CMD)

run: build/os.iso
	$(QEMU_RUN_VGA)

run-serial: build/os.iso
	$(QEMU_RUN)

run-vga: build/os.iso
	$(QEMU_RUN_VGA)

run-vnc: build/os.iso
	@echo "Starting QEMU with VNC on :1 (port 5901)"
	qemu-system-i386 -cdrom build/os.iso -vnc :1 -no-reboot -no-shutdown -serial mon:stdio &

kill-qemu:
	@echo "Stopping any running qemu-system-i386 processes (WSL/Linux)"
	-pkill -f qemu-system-i386 || true

toolchain-check:
	@if [ "$(OS)" = "Windows_NT" ]; then \
		$(TOOLCHAIN_CHECK); \
	else \
		command -v $(AS) >/dev/null || { echo "Missing nasm"; exit 1; }; \
		command -v $(CC) >/dev/null || { echo "Missing i686-elf-gcc"; exit 1; }; \
		command -v grub-mkrescue >/dev/null || { echo "Missing grub-mkrescue"; exit 1; }; \
		command -v qemu-system-i386 >/dev/null || { echo "Missing qemu-system-i386"; exit 1; }; \
		echo "Toolchain OK"; \
	fi

clean:
	$(RM_RF)

.PHONY: all run run-serial run-vga clean toolchain-check
