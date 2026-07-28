# Editorial Notes for Lesson 01

These notes accompany the English translation,
[Loading the Kernel from a Floppy Disk](../docs/tutorials/01-loading-the-kernel/index.md).
They document translation choices, technical corrections, and presentation changes
without interrupting the tutorial itself.

## Translation approach

The English edition preserves the original lesson's technical content and sequence,
but does not imitate its page layout or translate awkward phrasing literally.
Sentences were rewritten where necessary to make the explanation natural and clear
in English.

The source greeting, `Salam Alikom!`, is translated as `Peace be upon you!`.
The kernel message is normalized to `Kernel loaded.` because that is the message
described in the PDF, even though the repository's historical `kernel.s` file uses
`Kernel is speaking !`.

## Technical corrections

| Source problem | Correction |
|---|---|
| The physical address was calculated as `segment * 4 + offset`. | Real mode uses `segment * 16 + offset`. |
| `bootdrv` was written before `DS` was initialized. | Initialize `DS = 0x07C0` first, then save `DL`. |
| The reset call relied on whatever value remained in `DL`. | Reload the saved boot drive before every `INT 13h` call. |
| The jump was written as `jmp dword 0x100:0`. | Use an explicit 16:16 far jump: `jmp 0x0100:0x0000`. |
| The kernel label appeared as both `krnlMsg` and `krnlMasg`. | Use one label, `kernelMsg`, consistently. |
| `[BITS 16]` was described as "16 bytes." | It selects 16-bit instruction mode. |
| `ORG 0` was said to prove that `CS` contains zero. | It tells NASM how to calculate offsets; it does not set `CS`. |
| The NASM commands omitted or misplaced `-o`. | Use `nasm -f bin source.s -o output.bin`. |
| The kernel could be shorter than the full sector read by the boot loader. | Pad `kernel.bin` to exactly 512 bytes. |
| "Boschs" was used as the emulator name. | The correct name is **Bochs**. |
| Stack and string state were assumed. | Initialize `SS:SP`, protect the update with `CLI`/`STI`, and clear `DF` with `CLD`. |

## Presentation changes

- The PDF's page-oriented formatting was replaced with semantic Markdown sections.
- Register descriptions and code explanations were converted into tables.
- Long code samples were normalized and consistently formatted.
- The original bitmap drawings were replaced by two accessible SVG diagrams:
  [boot flow](../docs/tutorials/01-loading-the-kernel/assets/boot-flow.svg) and
  [disk geometry](../docs/tutorials/01-loading-the-kernel/assets/disk-geometry.svg).
- Repository-relative SVG references are used so the diagrams render on GitHub.
- Warnings and important details use Material for MkDocs admonitions.

## Validation

The Markdown structure, local asset links, SVG XML, and rendered SVG layouts were
checked. The NASM forms used for `BITS`, `ORG`, flat binary output, and far jumps
were cross-checked against the official NASM documentation.

NASM was not installed in the working environment, so the listings were not assembled
as part of this translation pass.

## Source

Original Arabic PDF:
[تحميل النواة من القرص المرن.pdf](./تحميل%20النواة%20من%20القرص%20المرن.pdf)
