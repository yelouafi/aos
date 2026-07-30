; AOS Lesson 01 - Load a kernel from a floppy disk

[BITS 16]
[ORG 0]

EntryPoint:
    ; Establish a known stack and string direction.
    cli
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    cld
    sti

    ; This source uses offsets relative to segment 0x07C0.
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax

    ; The BIOS tells us which drive booted in DL.
    mov [bootdrv], dl

    mov si, bootMsg
    call ShowMsg

    ; Reset the boot drive before reading.
    mov dl, [bootdrv]
    xor ax, ax
    int 0x13
    jc readFail

    ; Read cylinder 0, head 0, sector 2 into 0x0100:0x0000.
    mov ax, 0x0100
    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [bootdrv]
    int 0x13
    jc readFail

    ; Transfer control to the kernel loaded in the second sector.
    jmp 0x0100:0x0000

readFail:
    mov si, readError
    call ShowMsg

.hang:
    jmp .hang

ShowMsg:
    push ax
    push bx
    push si

.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    mov bx, 0x0007
    int 0x10
    jmp .loop

.done:
    pop si
    pop bx
    pop ax
    ret

bootdrv db 0
bootMsg db "Peace be upon you!", 13, 10, 0
readError db "Error: unable to read the kernel.", 13, 10, 0

times 510 - ($ - $$) db 0
dw 0xAA55
