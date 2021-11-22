#include "system.h"
#include "interrupt.h"


struct IDTEntry {
	WORD offset_1; 		// offset 0..15
	WORD segment;     	// segment selector
	BYTE reserved:5;  
	BYTE flags:3;     	// 0 for interrupt/trap gates 
	BYTE type:5;       	// type of gate  
	BYTE dpl:2;        
	BYTE present:1;    
	WORD offset_2; 
} __attribute__((packed));


// Structure to be loaded into the IDTR
struct IDTR {
    WORD limit;
    DWORD base;
} __attribute__((packed));

// Declare a GDT with 3 entries, and the GDT pointer 
#define MAX_ENTRIES 256
IDTEntry idt[MAX_ENTRIES];
static Interrupt* handlers[MAX_ENTRIES];
IDTR idtr;

void IDTSetup() {
	//clear all enries for the moment
	memset((BYTE*)&idt, 0, sizeof(IDTEntry)*MAX_ENTRIES);
	
	//initialzes and load the idtr register
	idtr.base = (DWORD)&idt;
	idtr.limit = sizeof(IDTEntry)*MAX_ENTRIES-1;
	asm volatile ("lidt %0"	:: "m"(idtr));
}

// this function set a handler for that vector
extern "C" void IDTsetHandler(int vector, DWORD handler, int dpl) {
	if(vector < 0 || vector >= MAX_ENTRIES) {
		return;
	} else if(!handler) {
		memset((BYTE*)&(idt[vector]), 0, sizeof(IDTEntry));
	} else {
		idt[vector].segment = 8;
		idt[vector].offset_1 = handler & 0xFFFF;
		idt[vector].offset_2 = (handler>>16) & 0xFFFF;
		idt[vector].type = 14; //gate's type = interrupt gate
		idt[vector].dpl = dpl & 3;
		idt[vector].present = 1;
	}
}


//this is the default handler, it will call
// the handler registered with this vector
extern "C" void isrDispatch(regs r, int vector, int errorCode) {
	if(vector < 0 || vector >= MAX_ENTRIES)
		return;
	if(handlers[vector])
		handlers[vector]->handle(&r, vector, errorCode);
}

void registerHandler(BYTE vector, Interrupt *interrupt) {
	handlers[vector] = interrupt;	
}

void unregisterHandler(BYTE vector) {
	handlers[vector] = (Interrupt*)null;	
}

