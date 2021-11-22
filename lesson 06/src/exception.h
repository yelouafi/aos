#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "system.h"
#include "interrupt.h"

class Exception : public Interrupt
{		
public:
	Exception();	
	void handle(regs* r, int vector, int errorCode);
};

#endif /*EXCEPTION_H_*/
