[BITS 16]
[ORG 0x0]

%define DESC_ENTRIES 3
%define DESC_LENGTH 8


start:
	mov ax, 0x07c0
	mov ds, ax
	mov es, ax
	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xf000
	
	mov si, msg
	call prints
	
	xor ax, ax
	int 0x13
	
	mov ax, 0x100   ; memory location to move data
	mov es, ax
	mov bx, 0
	mov dl, 0
	mov dh, 0  ; head
	mov ch, 0 ; track
	mov cl, 2   ; sector
	mov al, 1  ; number of sectors to read
	mov ah, 2
	int 0x13

setup_gdt:	
	cli
	lgdt[gdtptr]
	mov eax, cr0
	or ax, 1
	mov cr0, eax
	
	jmp next
	
next:
	mov esp,0x9F000	
	mov ax,0x10		; updating segment registers
	mov ds,ax
	mov fs,ax
	mov gs,ax
	mov es,ax
	mov ss,ax
	jmp dword 0x8:0x1000   ; long jump (to update cs) to kernel code
	
	jmp $

	
prints:
	push ax
	push bx
   .debut
	lodsb
	cmp al, 0
	jz .fin
	mov ah, 0x0e
	mov bx, 0x07
	int 0x10
	inc bx
	jmp .debut
	
  .fin:
	pop bx
	pop ax
	ret

;-----GDT table descriptors---------
gdt:
gdt_null:
	dw 0,0,0,0
gdt_cs:
	db 0xFF, 0xFF,  0,0,   0,     10011011b,     11011111b,     0
gdt_ds:
	db 0xFF, 0xFF,  0,0,    0,     10010011b,    11011111b,     0
gdtend:

gdtptr:
	dw	(DESC_ENTRIES  * DESC_LENGTH)-1          ; limite =( number_entries x entry_length)-1
	dd	0x7C00+gdt		; base, linear address = (segment_adressx16) + offset

;-------------------------------------------------------------
msg db "Loading the kernel",13,10,0

;-------------------------------------------------------------

times 510-($-$$) db 144
dw 0xAA55
