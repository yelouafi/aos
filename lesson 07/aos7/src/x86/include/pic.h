#ifndef PIC_H_
#define PIC_H_

#include "../../include/system.h"
#include "Machine.h"


class PIC {

public:
	static void init();
	static void setPIC1Mask(BYTE mask);
	static void setPIC2Mask(BYTE mask);
	static void enableIRQ(int IRQ);
	static void disableIRQ(int IRQ);
	static void acknowledgePIC1();
	static void acknowledgePIC2();
};

#endif /*PIC_H_*/
