#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "../../include/system.h"
#include "../../include/interrupt.h"

class Exception : public Interrupt
{		
public:
	Exception();	
	void handle(regs* r, int vector, int errorCode);
};

#endif /*EXCEPTION_H_*/
