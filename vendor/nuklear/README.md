Nuklear vendor note

This workspace includes a Nuklear integration scaffold but does not bundle the
`nuklear.h` single-header library due to licensing and size considerations.

To enable full Nuklear support inside the kernel image:

1. Download `nuklear.h` from the official repository (https://github.com/Immediate-Mode-UI/Nuklear).
2. Place `nuklear.h` at `vendor/nuklear/nuklear.h`.
3. Implement the draw backend in `kernel/gui_nuklear.c` by converting Nuklear draw
   commands into pixels written to the framebuffer via `framebuffer_putpixel`.
4. Provide a framebuffer at early boot (e.g. via GRUB framebuffer/multiboot info)
   and call `framebuffer_init` with its address, width, height, pitch and bpp.

Notes:
- Nuklear produces draw commands composed of filled rectangles, triangles and
  text glyphs. You will need a small triangle/rect rasterizer and a bitmap font
  blitter to render text.
- Alternatively, consider using a tiny immediate-mode UI like `microui` if you
  prefer even smaller footprint.
