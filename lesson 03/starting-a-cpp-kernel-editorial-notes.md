# Editorial Notes for Lesson 03

These notes accompany the English translation,
[Starting a C++ Kernel](../docs/tutorials/03-starting-a-cpp-kernel/index.md).
They document translation choices, technical corrections, and presentation changes
without interrupting the tutorial itself.

## Translation approach

The English edition preserves the original lesson's technical content and sequence,
but does not imitate its page layout or translate awkward phrasing literally.
Sentences were rewritten where necessary to make the explanation natural and clear
in English.

Code listings were cross-checked against the repository's actual working sources in
`lesson 03/src/` rather than retyped from the PDF's screenshots, since the two differ
in a few places (see below). The kernel's own literal string output -
`"Assalamou Alaikoum from grub"`, `"Assalamou Alaikoum but not from grub"`, and the
GRUB menu title `Arabian OS` - is left exactly as it appears in the source, since it
is program output the kernel actually prints, not prose being translated.

## Technical corrections

| Source problem | Correction |
|---|---|
| `Video::scroll(int y)` declared its parameter `y` and then reused the same name for the inner loop variable: `for (int y = (height - y) * 2; ...)`. In C++, a variable's scope begins at its own declarator, so this initializer read the inner `y` before it was ever assigned - undefined behavior, not a reference to the outer parameter. | Renamed the parameter to `dy` and rewrote the blanking loop with a single non-shadowing index, `i`, computed directly as a byte offset. |
| The same `scroll` function's blanking loop bound only advanced by `dy * 2` regardless of screen width, so it cleared a single character cell per scrolled line instead of the full row. | The corrected loop bound is `dy * width * 2`, matching the number of bytes actually made stale by the scroll. Verified against a small standalone reproduction of the corrected logic. |
| The PDF's transcription of `multiboot.s` reserves `resb 0x4000` (16 KiB) for the stack, with a comment describing it as such. | The repository's actual `lesson 03/src/multiboot.s` reserves `resb 0x10000` (64 KiB); the tutorial follows the real source. |
| The PDF's transcription of `Video::Video()` initializes `_attribute(0x07)` (light gray on black). | The repository's actual `lesson 03/src/video.cpp` initializes `_attribute(0x0f)` (bright white on black); the tutorial follows the real source. |
| `setGDTEntry`'s source comment describes `dpl & 3` as "the same as dpl modulo 3." | `& 3` masks to two bits, i.e. modulo 4 (the range 0-3); the tutorial describes the mask directly instead of repeating the comment's inexact phrasing. |

## Presentation changes

- The PDF's page-by-page Arabic prose and inline code screenshots were restructured
  into semantic Markdown sections, tables, and admonitions.
- The `one.c` / `tow.c` translation-unit example was kept but given English variable
  names consistent with the rest of the lesson.
- Three SVG diagrams replace the PDF's plain-text and bitmap figures:
  [build pipeline](../docs/tutorials/03-starting-a-cpp-kernel/assets/build-pipeline.svg),
  [memory layout](../docs/tutorials/03-starting-a-cpp-kernel/assets/memory-layout.svg), and
  [Multiboot handoff](../docs/tutorials/03-starting-a-cpp-kernel/assets/multiboot-handoff.svg).
- `main.cpp`'s listing in the tutorial omits the bodies of `memcpy`, `memset`, and
  `intToString`, since they are identical to the ones already shown in full while
  discussing `system.h` and `video.cpp`; the surrounding prose says so explicitly.

## Validation

The corrected `scroll()` logic was extracted into a standalone host-side C++
reproduction and run under `g++` to confirm the fix actually blanks every cell of
the scrolled-in rows, not just one cell per row. The Markdown structure, local
asset links, and SVG XML were checked, and the site was built with
`mkdocs build --strict`.

Cross-compiling the full freestanding kernel (`nasm -f elf`, a 32-bit `g++`, and
`objcopy -O elf32-i386`) was not attempted in this environment, since it targets a
Windows/Cygwin or Linux i386 toolchain that is not installed here.

## Source

Original Arabic PDF: [kernelcpp.pdf](./kernelcpp.pdf)
