# Editorial Notes for Lesson 04

These notes accompany the English translation,
[Managing Interrupts and Exceptions](../src/content/docs/tutorials/04-managing-interrupts/index.md).
They document translation choices, technical corrections, and presentation changes
without interrupting the tutorial itself.

## Translation approach

The English edition preserves the original lesson's technical content and sequence -
exceptions and IRQs, PIC remapping, the IDT, the assembly dispatch stubs, an
`Exception` handler, then a PIT-driven `Timer` - but does not imitate the PDF's page
layout or translate its phrasing literally. Sentences were rewritten where necessary
for clarity in English.

Code listings were cross-checked against the repository's actual working sources in
`lesson 04/src/` rather than retyped from the PDF's screenshots, since the two differ
in a few places (see below). The exception message table remains faithful to the
source. Runtime messages were corrected where the browser uses a direct Multiboot
loader rather than GRUB, and the elapsed-time line now uses grammatical English.

## Technical corrections

| Source problem | Correction |
|---|---|
| `multiboot.s` pushed the literal Multiboot *header* magic (`0x1BADB002`) onto the stack in place of `EAX`, instead of forwarding the real value the Multiboot loader places in `EAX` at handoff (as Lesson 03's `multiboot.s` does). This silently defeated the loader-magic check in `kmain` - the "invalid magic" branch could never be reached, valid loader or not. | `multiboot.s` now pushes `eax`, forwarding the loader's real value, exactly as Lesson 03 does. |
| `system.h` defined `MULTIBOOT_MAGIC` as `0x1BADB002`, the Multiboot *header* magic, and compared the loader's runtime value against it in `kmain`. The value a Multiboot loader actually places in `EAX` is the different, documented constant `0x2BADB002`. Combined with the previous bug, the two mistakes canceled out into an always-true check that validated nothing. | Renamed to `MULTIBOOT_BOOTLOADER_MAGIC` and corrected to `0x2BADB002`, matching Lesson 03's constant of the same name and the actual Multiboot specification. |
| `multiboot.s`'s post-`kmain` fallback was a bare `jmp $` (a spinning busy loop). `kmain` itself never returns, so this path is unreachable, but Lesson 03's equivalent stub uses a `cli`/`hlt` loop instead. | Replaced with the same `cli; hlt; jmp` hang loop Lesson 03 uses, for consistency and so an unreachable path fails safe rather than busy-spinning. |
| `multiboot.s` declared `SECTION .stack` without the `nobits` attribute NASM needs to treat it as uninitialized space. The linker script's `.bss` output section therefore had to preserve real, explicit zero bytes for the full 64 KiB reservation, inflating `kernel.elf` from roughly 17 KB to 85 KB for no functional reason. | Added `nobits` to `SECTION .stack`, matching Lesson 03's `multiboot.s`; the reservation now costs no file space. |
| `Timer`'s tick counter, `nb`, was declared `long long` and used with `%` (modulo). On the freestanding, no-libgcc toolchain this series builds with, 64-bit modulo lowers to a call to `__moddi3`, which is not linked in and left `kernel.elf` failing to link with an undefined-symbol error. | Changed `nb` to a 32-bit `DWORD`. A 32-bit counter ticking at 20 Hz does not wrap for over two years of continuous uptime, and 32-bit division/modulo is a native x86 instruction requiring no runtime support. |
| Lesson 04 still read only the legacy Multiboot lower/upper-memory and boot-device fields, while v86's direct loader supplies a memory map and no disk device. It also described that direct loader as GRUB. | Reused Lesson 03's flag-aware memory-map handling and loader-neutral messages: `from Multiboot` and `Boot device = not provided`. |
| Every IDT gate was created at DPL 3, allowing future user-mode code to invoke exception and hardware-IRQ entry points with `int`. | Installed this lesson's kernel-only gates at DPL 0. Hardware IRQ delivery is unaffected because the privilege check applies to explicit software interrupts. |
| `kmain` enabled interrupts before registering its C++ handlers, then ended in an empty busy loop. The fatal exception path busy-spun too. | Kept interrupts disabled through initialization, enabled them only after handler registration and IRQ0 unmasking, used `hlt` while idle, and used `cli; hlt` after fatal exceptions. |
| The browser test accepted the static boot greeting, so it could pass before the first timer interrupt. | Changed the required output to `1 second elapsed`, making both the widget and CI wait for the complete IDT/PIC/PIT/EOI path. |

## Presentation changes

- The PDF's page-by-page Arabic prose, inline code screenshots, and a printed
  appendix on installing Bochs under Cygwin/Windows were restructured into semantic
  Markdown sections, tables, and admonitions. The Bochs/VFD appendix itself is not
  reproduced - the browser-based v86 playground below supersedes it as this site's
  way of actually running the kernel, the same choice Lesson 03 made.
- Three SVG diagrams replace the PDF's plain-text and bitmap figures:
  [ISR dispatch flow](../src/content/docs/tutorials/04-managing-interrupts/assets/isr-dispatch-flow.svg),
  [IDT entry layout](../src/content/docs/tutorials/04-managing-interrupts/assets/idt-entry.svg), and
  [PIC remapping](../src/content/docs/tutorials/04-managing-interrupts/assets/pic-remap.svg).
- A top-level `lesson 04/Makefile` was added (the PDF's own Makefile targets a
  Windows/Cygwin `g++`/`ld`/`objcopy`/WinImage pipeline). It follows Lesson 03's
  Makefile shape: `clang++`/`rust-lld` on macOS, `g++`/`ld` elsewhere, with `kernel`
  and `inspect` targets so `scripts/build-playgrounds.mjs` can build it the same way
  as every other registered playground.

## Validation

The kernel was built with the new `lesson 04/Makefile` (`make -C "lesson 04" kernel
inspect`), producing a 32-bit i386 ELF with a valid Multiboot header in the first
8192 bytes. The corrected build was registered in `playgrounds.json` as `lesson-04`
and driven interactively through the same v86-based browser playground Lesson 03
uses: the kernel boots, prints the Multiboot banner, and the `Timer` handler's
elapsed-seconds line kept updating once per second over a run of more than 20
seconds, confirming the IDT, the remapped PIC, and the EOI handshake all work
end to end rather than firing once and hanging.

`playgrounds.json`'s `expectedOutput` for `lesson-04` is
`"1 second elapsed"`. v86's `wait_until_vga_screen_contains` helper checks
whether a screen line starts with the requested text, so the complete first
timer update is a valid target. This makes `"Boot verified"` evidence that the
IDT, remapped PIC, PIT handler, C++ dispatch, and EOI handshake all ran, rather
than evidence only that `kmain` printed its greeting.

The full `tests/e2e/playgrounds.spec.js` suite (all five registered playgrounds)
was run against `astro build` output through `astro preview` and passes.
Markdown structure, local asset links, and SVG XML were checked, and the site
was built with `astro build`.

Cross-compiling with the exact Windows/Cygwin toolchain the PDF's own Makefile
targets was not attempted in this environment, since it is not installed here;
Lesson 03's cross-platform Makefile approach was reused instead.

## Source

Original Arabic PDF: [interrupt.pdf](./interrupt.pdf)
