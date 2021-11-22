#include "./include/pic.h"


const int pic1= 0x20;
const int pic2= 0xA0;
static BYTE _mask1 = 0xF;
static BYTE _mask2 = 0xF;

void PIC::init() {
	
	//ICW1: need ICW4, pic2 present, edge detetction
	Machine::outb(pic1, 0x11);
	Machine::outb(pic2, 0x11);
	
	/*ICW2: remap pic1 to use vectors from 0x20 (32)
	***		remap pic2 to use vectors from 0x28 (40)
	* */
	Machine::outb(pic1+1, 0x20);
	Machine::outb(pic2+1, 0x28);
	
	//ICW3: pic2 uses IRQ2
	Machine::outb(pic1+1, 4);
	Machine::outb(pic2+1, 2);
	
	//ICW4
	Machine::outb(pic1+1, 1);
	Machine::outb(pic2+1, 1);
	
	//mask all irqs initially
	setPIC1Mask(_mask1);
	setPIC2Mask(_mask2);
}	

// disable irqs specified in mask
void PIC::setPIC1Mask(BYTE mask) {
	/* to set the int mask, we use the OCW1
	 * we write it to 0x21 port, each bit 
	 * correspond to an irq line */
	Machine::outb(pic1+1, mask);
	_mask1 = mask;
}

//same as bove, but for the salve PIC
void PIC::setPIC2Mask(BYTE mask) {
	Machine::outb(pic2+1, mask);
	_mask2 = mask;
}

void PIC::enableIRQ(int IRQ) {
	if(IRQ >= 0 && IRQ < 8) {
		_mask1 &= ~(1U<<IRQ);
		Machine::outb(pic1+1, _mask1);
	} else if(IRQ < 16) {
		_mask2 &= ~(1U<<(IRQ-8));
		Machine::outb(pic2+1, _mask2);
	}
}

void PIC::disableIRQ(int IRQ) {
	if(IRQ >= 0 && IRQ < 8) {
		_mask1 |= (1U<<IRQ);
		Machine::outb(pic1+1, _mask1);
	} else if(IRQ < 16) {
		_mask2 |= (1U<<(IRQ-8));
		Machine::outb(pic2+1, _mask2);
	}
}

/* we must send an EOI to PIC, to reenable
 * the interrupted irq line */
void PIC::acknowledgePIC1() {
	/* EOI is sent by writing the OCW2 (=0x20
	 * non specific EOI in our case)to 0x20 port*/
	Machine::outb(pic1, 0x20);
}

void PIC::acknowledgePIC2() {
	// we must send EOI ti both controllers
	Machine::outb(pic1, 0x20);
	Machine::outb(pic2, 0x20);
} 
