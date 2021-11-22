#ifndef PIC_H_
#define PIC_H_

#include "system.h"
#include "Machine.h"


class PIC {

public:
	static void init();
	static void setPIC1Mask(BYTE mask);
	static void setPIC2Mask(BYTE mask);
	static void acknowledgePIC1();
	static void acknowledgePIC2();
};

#endif /*PIC_H_*/
