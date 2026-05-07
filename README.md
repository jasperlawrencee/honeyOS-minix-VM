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
make run-serial

make run-vnc    # start QEMU with VNC on :1 (port 5901)
make kill-qemu  # stop running qemu-system-i386 processes (WSL/Linux)
```

On native Windows, if `make` is not available but `mingw32-make` is installed, use:
```powershell
mingw32-make toolchain-check
mingw32-make
mingw32-make run
mingw32-make run-vga
mingw32-make run-serial
```

## Runtime note
- `make run` starts QEMU with a display window (recommended for Nuklear UI).
- `make run-vga` is the same graphical mode as `make run`.
- `make run-serial` uses QEMU in `-nographic` mode and routes monitor/serial to the terminal.
 - `make run-vnc` starts QEMU and exposes the VM display via VNC on `:1` (TCP port `5901`). Useful when running inside WSL or headless environments; connect a VNC client to `localhost:5901`.
 - `make kill-qemu` attempts to stop any running `qemu-system-i386` processes using `pkill` (WSL/Linux). On native Windows use Task Manager or `taskkill` if required.
- The Nuklear UI is framebuffer graphics and is only visible in `run-vga` (or any graphical QEMU launch), not in `-nographic` terminal mode.
- Runtime UI now shows a `HONEY OS` banner, accepts typed input, echoes with `You typed: ...`, and powers off on `shutdown`.

## Output
- Kernel ELF: build/kernel.bin
- Bootable ISO: build/os.iso

## Minix Appendix B usage
This project uses only a minimal subset of concepts/constants and tiny routine behavior patterns. See docs/PROVENANCE.md for strict attribution and traceability.
