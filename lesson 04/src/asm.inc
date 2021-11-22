;; this macro pushs all the args 
;; example: PUSHS eax, ebx, ecx...
%macro PUSHS 1-*
	%rep %0
		push %1
		%rotate 1
	%endrep
%endmacro

;; this is the pop equivalent of the above macro
;; attention!! args must be provided in the same
;; order in PUSHS
%macro POPS 1-*
	%rep %0
		%rotate -1
		pop %1
	%endrep
%endmacro


;; common declaration for a procedure
%macro proc 1
global %1, _%1
%1: 
_%1:
%endmacro

;; just for readability
%define endproc	

;; generates the correct call instruction
;; to an extern c function
%macro c_call 1
	%ifdef LEADING_USCORE
		call _%1
  	%else
		call %1
  	%endif
%endmacro


;; generates the correct extern symbol
%macro c_extern 1-*
%rep %0
	%ifdef LEADING_USCORE
		extern _%1
  	%else
		extern %1
  	%endif
	%rotate 1
%endrep
%endmacro
