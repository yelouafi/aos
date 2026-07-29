; AOS Lesson 02 - Entering 32-bit protected mode

[BITS 16]
[ORG 0]

%define BOOT_SEGMENT    0x07C0
%define BOOT_ADDRESS    0x7C00
%define KERNEL_SEGMENT  0x0100
%define KERNEL_ADDRESS  0x1000

%define CODE_SELECTOR   0x08
%define DATA_SELECTOR   0x10

start:
    ; Establish known data segments and a safe stack.
    cli

    mov ax, BOOT_SEGMENT
    mov ds, ax
    mov es, ax

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF

    cld
    sti

    ; Preserve the drive selected by the BIOS.
    mov [bootDrive], dl

    mov si, loadingMessage
    call printString

    ; Reset the boot drive.
    mov dl, [bootDrive]
    xor ax, ax
    int 0x13
    jc diskError

    ; Read sector 2 into physical address 0x1000.
    mov ax, KERNEL_SEGMENT
    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [bootDrive]
    int 0x13
    jc diskError

    ; Install the descriptor table and enable protected mode.
    cli
    lgdt [gdtPointer]

    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    ; Reload CS through the 32-bit code descriptor.
    jmp dword CODE_SELECTOR:(BOOT_ADDRESS + protectedEntry)

diskError:
    mov si, diskErrorMessage
    call printString

.hang:
    cli
    hlt
    jmp .hang

printString:
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

; ---------------------------------------------------------------------------
; Global Descriptor Table
; ---------------------------------------------------------------------------

gdtStart:
gdtNull:
    dq 0

gdtCode:
    dw 0xFFFF               ; limit bits 0-15
    dw 0x0000               ; base bits 0-15
    db 0x00                 ; base bits 16-23
    db 10011010b            ; present, ring 0, code, readable
    db 11001111b            ; 4 KiB granularity, 32-bit, limit bits 16-19
    db 0x00                 ; base bits 24-31

gdtData:
    dw 0xFFFF               ; limit bits 0-15
    dw 0x0000               ; base bits 0-15
    db 0x00                 ; base bits 16-23
    db 10010010b            ; present, ring 0, data, writable
    db 11001111b            ; 4 KiB granularity, 32-bit, limit bits 16-19
    db 0x00                 ; base bits 24-31
gdtEnd:

gdtPointer:
    dw gdtEnd - gdtStart - 1
    dd BOOT_ADDRESS + gdtStart

loadingMessage   db "Loading the 32-bit kernel...", 13, 10, 0
diskErrorMessage db "Error: unable to read the kernel.", 13, 10, 0
bootDrive        db 0

; ---------------------------------------------------------------------------
; First instructions after CR0.PE becomes 1
; ---------------------------------------------------------------------------

[BITS 32]
protectedEntry:
    mov ax, DATA_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x0009F000

    ; The kernel was loaded at linear address 0x1000.
    jmp dword CODE_SELECTOR:KERNEL_ADDRESS

times 510 - ($ - $$) db 0
dw 0xAA55
