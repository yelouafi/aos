%include "asm.inc"

;; some macros will facilitate out coding
;; INTR generates the code for interruptions
;; that dont push an error code
%macro INTR 1
isr%1:
	push dword 0		; pushes dummy error code
	push dword %1		; push the vector
	jmp isrCommon
%endmacro

;; interrupts that push error code
%macro INTR_EC 1
isr%1:
	push dword %1		; push the vector
	jmp isrCommon
%endmacro

;; generates code in cascade for multiples
;; isrs using the above INTR macro
%macro MAKE_INTR 2
%assign i %1
%rep (%2-%1+1)
	INTR i
	%assign i i+1
%endrep
%endmacro

;; set a handler on the idt table by calling
;; IDTsetHandler(int vector, DWORD handler, int dpl)
;; (defined in isr.cpp)
%macro setHandler 1
	push dword 3		;; dpl=3
	push dword isr%1	;; handler = isr%1 {isr0, isr1...}
	push dword %1		;; interrupt vector
	c_call IDTsetHandler
	add esp, 12
%endmacro

SECTION .text
[BITS 32]

c_extern isrDispatch, IDTsetHandler

MAKE_INTR 0,7

INTR_EC 8	
INTR 9		
INTR_EC 10	
INTR_EC 11	
INTR_EC 12	
INTR_EC 13	
INTR_EC 14

MAKE_INTR 15, 47


isrCommon:			
	; save registers on the stack
	PUSHS eax,ebx,ecx,edx,edi,esi,ebp,ds,es,fs,gs,ss
	
	; ensures segment registers loaded with 
	; correct values
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
	
		%ifdef LEADING_USCORE
			mov eax, _isrDispatch
	  	%else
			mov eax, isrDispatch
	  	%endif
	  
	  	call eax
	POPS  eax,ebx,ecx,edx,edi,esi,ebp,ds,es,fs,gs,ss
	add esp, 8	; discard [dummy] error code & vector number
	iret
	
;; this procedure will fill the idt entries
;; with our handlers: isr0, isr1...
proc	initISRS
	push ebp
	mov ebp, esp
	
	%assign i 0
	%rep 48
		setHandler i
		%assign i i+1
	%endrep
	
	leave
	ret
endproc 

