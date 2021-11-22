#ifndef INTERRUPT_
#define INTERRUPT_

#include"../x86/include/Machine.h"

class Interrupt {
	public:
		virtual void handle(regs *r, int vector, int errorCode)=0;
};

#endif /*INTERRUPT_*/
