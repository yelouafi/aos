%define MULTIBOOT_HEADER_MAGIC 0x1BADB002        ; magic header
%define MULTIBOOT_HEADER_FLAGS 0x00010003 
                  ;; flags:
                     ;; align to 4 kb        
                     ;; give us memory info
                     ;; header contains address fields
%define MULTIBOOT_CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
                       ;; remember: sum of this field and the tow first fields must be zero


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; code section

SECTION .text
[BITS 32]

GLOBAL entry	; must be global to be visible to the linker
entry: 
	jmp start
	
; extern symbols defined in the linker script file
EXTERN code, bss, end
ALIGN 4   ; mutiboot header must be 4 bytes aligned

multiboot_header:
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM
  dd multiboot_header
  dd code
  dd bss
  dd end
  dd entry

; program entry point, the code of kernel start here

EXTERN _kmain, kmain

start:
  mov esp, stack
  
  
  push ebx
  push MULTIBOOT_HEADER_MAGIC
  %ifdef LEADING_USCORE
	call _kmain
  %else
	call kmain
  %endif
  jmp $

SECTION .stack
resb 0x10000   ; we reserve space for our stack
stack:
