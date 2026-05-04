# Minimal Minix-derived VM OS (x86, QEMU)

A minimal, bootable, freestanding OS scaffold inspired by small, low-level pieces from MINIX Appendix B. This project is intentionally tiny so you can extend it safely.

## Scope (V1)
- Multiboot boot path
- 32-bit kernel entry
- COM1 serial logging
- VGA text-mode logging fallback
- Keyboard input polling and echo
- Interactive command prompt with shutdown command
- Panic path and stable idle loop
- Small memory primitives module
- Provenance log for Appendix B-derived elements

## Current status
- Scaffold complete
- First implementation complete
- Ready for build on a cross toolchain host

## Build prerequisites

### Native Windows (PowerShell/cmd)
Install and add to `PATH`:
- nasm
- i686-elf-gcc toolchain
- grub-mkrescue
- qemu-system-i386

The repo includes a Windows checker script at `scripts/check-toolchain.ps1` and the `make toolchain-check` target now uses it automatically on Windows.

### WSL2 Ubuntu (optional)
If you prefer Linux tooling on Windows, install the same tools in WSL:
- nasm
- grub-mkrescue (grub-pc-bin, xorriso)
- qemu-system-i386
- i686-elf-gcc toolchain

If `i686-elf-gcc` is not available but `i686-linux-gnu-gcc` is installed, build with:
- `make clean`
- `make CC=i686-linux-gnu-gcc`
- `make run CC=i686-linux-gnu-gcc`
- `make run-vga CC=i686-linux-gnu-gcc`

## Build and run
```bash
make toolchain-check
make
make run
make run-vga
```

On native Windows, if `make` is not available but `mingw32-make` is installed, use:
```powershell
mingw32-make toolchain-check
mingw32-make
mingw32-make run
mingw32-make run-vga
```

## Runtime note
- `make run` uses QEMU in `-nographic` mode and routes monitor/serial to the terminal so kernel text is visible directly.
- `make run-vga` starts QEMU with the display window so VGA text output is visible on screen.
- Runtime UI now shows a `HONEY OS` banner, accepts typed input, echoes with `You typed: ...`, and powers off on `shutdown`.

## Output
- Kernel ELF: build/kernel.bin
- Bootable ISO: build/os.iso

## Minix Appendix B usage
This project uses only a minimal subset of concepts/constants and tiny routine behavior patterns. See docs/PROVENANCE.md for strict attribution and traceability.
