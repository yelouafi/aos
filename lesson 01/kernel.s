; AOS Lesson 01 - Tiny real-mode kernel

[BITS 16]
[ORG 0]

start:
    cli

    ; The boot sector loaded this code at segment 0x0100.
    mov ax, 0x0100
    mov ds, ax
    mov es, ax

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF

    cld
    sti

    mov si, kernelMsg
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

kernelMsg db "Kernel loaded.", 13, 10, 0

; The boot sector reads one complete 512-byte sector.
times 512 - ($ - $$) db 0
