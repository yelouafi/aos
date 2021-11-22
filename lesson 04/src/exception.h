#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "interrupt.h"
#include "video.h"

class Exception : public Interrupt
{	Video* out;
	
public:
	Exception(Video *v);	
	void handle(regs* r, int vector, int errorCode);
};

#endif /*EXCEPTION_H_*/
