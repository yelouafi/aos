#include "./include/exception.h"

static const char *messages[] =
	{
	    "Division By Zero",
	    "Debug",
	    "Non Maskable Interrupt",
	    "Breakpoint",
	    "Into Detected Overflow",
	    "Out of Bounds",
	    "Invalid Opcode",
	    "No Coprocessor",
	
	    "Double Fault",
	    "Coprocessor Segment Overrun",
	    "Bad TSS",
	    "Segment Not Present",
	    "Stack Fault",
	    "General Protection Fault",
	    "Page Fault",
	    "Unknown Interrupt",
	
	    "Coprocessor Fault",
	    "Alignment Check",
	    "Machine Check",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved",
	    "Reserved"
	};

Exception::Exception()
{
}

void Exception::handle(regs* r, int vector, int errorCode) {
	DWORD cr2;
	asm volatile ("mov %%cr2, %0":"=d"(cr2):);
	printf("\nException: %s\n", messages[vector]);
	printf("eax: %x, ebx: %x, ecx: %x, edx: %x, cr2=%x\n",
		r->eax, r->ebx, r->ecx, r->edx, cr2);
	while(1);
}
