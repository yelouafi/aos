%define MULTIBOOT_HEADER_MAGIC 0x1BADB002
%define MULTIBOOT_HEADER_FLAGS 0x00000003
%define MULTIBOOT_CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[BITS 32]

SECTION .multiboot
ALIGN 4
multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

SECTION .text
GLOBAL entry
EXTERN kmain

entry:
    mov esp, stack

    ; A Multiboot loader provides its runtime magic in EAX and the
    ; multiboot_info pointer in EBX. Forward both as cdecl arguments.
    push ebx
    push eax
    call kmain

.hang:
    cli
    hlt
    jmp .hang

SECTION .stack nobits
ALIGN 16
resb 0x10000
stack:
