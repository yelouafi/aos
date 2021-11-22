#include "./include/Machine.h"

// Declare a GDT with 3 entries, and the GDT pointer 

static GDTEntry gdt[MAX_GDT_ENTRIES];
static GDTR gptr;

// Declare a GDT with 3 entries, and the GDT pointer 
static IDTEntry idt[MAX_IDT_ENTRIES];
static IDTR idtr;


// this function set a handler for that vector
extern "C" void IDTsetHandler(int vector, DWORD handler, int dpl) {
	if(vector < 0 || vector >= MAX_IDT_ENTRIES) {
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

void setGDTEntry(int num,  bool isCode, int dpl)
{	if(num>= MAX_GDT_ENTRIES)
		return;
    // Setup the descriptor base address: 0
    gdt[num].base_1 = 0;
    gdt[num].base_2 = 0;
    gdt[num].base_3 = 0;

    // Setup the descriptor limits : 0xFFFFF (*4 ko see granularity flag)
    gdt[num].limit_1 = 0xFFFF;
    gdt[num].limit_2 = 0xF;

    // setup flags
	gdt[num].type = (isCode)?0xb:0x3;	// if isCode=true -> type=code (0xb) oterwise type=data(3)
    gdt[num].system = 1;							// decriptor type = code/data
    gdt[num].dpl = (dpl & 3);					// same as (dpl modulo 3+1) to ensure dpl always <= 3
    gdt[num].present = 1;
	
	gdt[num].avl = 0;
    gdt[num].db = 1;								// D/B: 32 bit segment type
	gdt[num].zero = 0;
	gdt[num].granularity = 1;
}



void Machine::setupIDT() {
	//clear all enries for the moment
	memset((BYTE*)&idt, 0, sizeof(IDTEntry)*MAX_IDT_ENTRIES);
	
	//initialzes and load the idtr register
	idtr.base = (DWORD)&idt;
	idtr.limit = sizeof(IDTEntry)*MAX_IDT_ENTRIES-1;
	asm volatile ("lidt %0"	:: "m"(idtr));
}

/* This will setup our proeper GDT
*  setup the first 3 entries, and then setup the GDTR stucture
*  finally load the gdt int the GDTR using inline assembly  */
void Machine::setupGDT()
{
    /* Setup the GDT pointer and limit */
    gptr.limit = (sizeof(GDTEntry) * MAX_GDT_ENTRIES)-1 ;
    gptr.base = (DWORD)&gdt;
	
	
	//First gdt entry must be null
	BYTE *entry = reinterpret_cast<BYTE*>(gdt);
	memset(entry, 0, sizeof(GDTEntry));

    // setup a code segment with dpl=0
    setGDTEntry(1, true, 0);

    // setup a data segment with dpl=0
    setGDTEntry(2, false, 0);
	
	//load the new GDT into GDTR and then update the segment registers
	asm volatile ("lgdt %0          \n\
			 ljmp $8,$1f      \n\
			 1:               \n\
			 movw $16, %%ax    \n\
			 movw %%ax,  %%ss \n\
			 movw %%ax,  %%ds \n\
			 movw %%ax,  %%es \n\
			 movw %%ax,  %%fs \n\
			 movw %%ax,  %%gs"
		:: "m"(gptr));
}

void Machine::init() {
	Machine::setupGDT();
	Machine::setupIDT();
}


