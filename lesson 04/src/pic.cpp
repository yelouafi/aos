#include "pic.h"


const int pic1= 0x20;
const int pic2= 0xA0;

void initPIC() {
	
	//ICW1: need ICW4, pic2 present, edge detetction
	outb(pic1, 0x11);
	outb(pic2, 0x11);
	
	/*ICW2: remap pic1 to use vectors from 0x20 (32)
	***		remap pic2 to use vectors from 0x28 (40)
	* */
	outb(pic1+1, 0x20);
	outb(pic2+1, 0x28);
	
	//ICW3: pic2 uses IRQ2
	outb(pic1+1, 4);
	outb(pic2+1, 2);
	
	//ICW4
	outb(pic1+1, 1);
	outb(pic2+1, 1);
	
	//mask all irqs initially
	setPIC1Mask(0xFF);
	setPIC2Mask(0xFF);
}	

// disable irqs specified in mask
void setPIC1Mask(BYTE mask) {
	/* to set the int mask, we use the OCW1
	 * we write it to 0x21 port, each bit 
	 * correspond to an irq line */
	outb(pic1+1, mask);
}

//same as bove, but for the salve PIC
void setPIC2Mask(BYTE mask) {
	outb(pic2+1, mask);
}

/* we must send an EOI to PIC, to reenable
 * the interrupted irq line */
void acknowledgePIC1() {
	/* EOI is sent by writing the OCW2 (=0x20
	 * non specific EOI in our case)to 0x20 port*/
	outb(pic1, 0x20);
}

void acknowledgePIC2() {
	// we must send EOI ti both controllers
	outb(pic1, 0x20);
	outb(pic2, 0x20);
} 
