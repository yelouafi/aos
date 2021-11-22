[BITS 16]

EntryPoint:
	mov [bootdrv], dl; dl contains the boot drive init,so we save it in [bootdrv]
	mov	ax,07c0h
	mov	ds,ax
	mov	es,ax
	mov	ax,09000h
	mov	ss,ax
	mov	sp,0ffffh

	mov si,Salam
	call ShowMsg
	
	;load kernel
	xor ax, ax
	int 0x13
	
	mov ax, 0x100
	mov es,ax
	mov bx,0
	mov ah,2
	mov al,1
	mov ch,0
	mov cl,2
	mov dh,0
	mov dl, [bootdrv]
	int 0x13
	jc readFail
	
	jmp dword 0x100:0 

readFail:
	mov si, readError
	call ShowMsg

k_loop:
	jmp k_loop

ShowMsg:
	push	ax
	push	bx
.loop_start:
	lodsb
	cmp	al,0
	je	.loop_end
	mov	ah,0Eh
	mov	bx,07h
	int	10h
	jmp	.loop_start
.loop_end:
	pop	bx
	pop	ax
	ret
	

bootdrv db 0

Salam	           db	"Salam Alikom !",13,10,0
readError db     "Error, unable to read from floppy",13,10,0

times 510-($-$$) db 0

DW 0AA55h

