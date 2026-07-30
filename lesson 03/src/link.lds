ENTRY(entry)

SECTIONS
{
    . = 0x00100000;

    .text : ALIGN(4096)
    {
        code = .;
        KEEP(*(.multiboot))
        *(.text*)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata*)
    }

    .data : ALIGN(4096)
    {
        data = .;
        *(.data*)
    }

    .bss : ALIGN(4096)
    {
        bss = .;
        *(COMMON)
        *(.bss*)
        *(.stack)
    }

    end = .;

    /DISCARD/ :
    {
        *(.note*)
        *(.indent)
        *(.comment)
        *(.stab)
        *(.stabstr)
        *(.eh_frame*)
    }
}
