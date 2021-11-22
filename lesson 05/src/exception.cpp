#include "exception.h"
#include "video.h"

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

extern Video *video;
void Exception::handle(regs* r, int vector, int errorCode) {
	video->printf("\nException: %s\n", messages[vector]);
	video->printf("eax: %x, ebx: %x, ecx: %x, edx: %x\n",
		r->eax, r->ebx, r->ecx, r->edx);
	while(1);
}
