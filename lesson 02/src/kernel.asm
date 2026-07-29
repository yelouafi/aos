; AOS Lesson 02 - 32-bit kernel

[BITS 32]
[ORG 0x1000]

%define VIDEO_MEMORY  0x000B8000
%define SCREEN_CELLS  (80 * 25)
%define TEXT_ATTRIBUTE 0x07

start:
    cld

    ; Clear the 80 x 25 text screen.
    mov ebx, VIDEO_MEMORY
    mov ecx, SCREEN_CELLS

.clearScreen:
    mov byte [ebx], ' '
    mov byte [ebx + 1], TEXT_ATTRIBUTE
    add ebx, 2
    loop .clearScreen

    ; Print the null-terminated message.
    mov esi, message
    mov ebx, VIDEO_MEMORY

.printCharacter:
    lodsb
    test al, al
    jz .hang

    mov byte [ebx], al
    mov byte [ebx + 1], TEXT_ATTRIBUTE
    add ebx, 2
    jmp .printCharacter

.hang:
    cli
    hlt
    jmp .hang

message db "Protected mode is active.", 0
