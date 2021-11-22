[BITS 32]
[ORG 0x1000]

%define VIDEO_MEM 0xB8000  ; Adress of text mode video memory
%define VIDEO_LENGTH 80*25  ; Adress of text mode video memory

	mov ebx, 0xB8000
	mov ecx, VIDEO_LENGTH
	
clear_screen:
	mov byte [ebx], ' '
	inc ebx
	mov byte [ebx], 7
	inc ebx
	loop clear_screen
	
	

prints:
	xor eax, eax
	xor edi, edi
	mov ebx, 0xB8000
	
begin:
	mov al, [msg+edi]	; mov the char number 'ebx' in msg
	cmp al, 0			
	je end							; reached end of msg
	mov byte [ebx], al		; for putting a charachter in screen, we must put 1-the char ascii cpde
	inc ebx												; and 2- the char color attribute:
	mov byte [ebx], 0x7; here 0x7 means : foreground blank & background white
	inc ebx
	inc edi
	jmp begin
	
end:
	jmp end;	infinite loop



msg  db "Now, we are in proected mode", 0