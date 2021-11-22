[BITS 16]
[ORG 0x0]

start:
	mov ax,0x100
	mov ds,ax
	mov es,ax
	mov ax,0x9000
	mov ss,ax
	mov sp, 0xffff

	mov si,krnlMsg
	call ShowMsg
	
end:
	jmp end
	
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

krnlMsg: db 'Kernel is speaking !',10,0