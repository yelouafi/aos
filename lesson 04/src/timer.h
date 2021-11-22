#ifndef TIMER_H_
#define TIMER_H_

#include "system.h"
#include "interrupt.h"
#include "video.h"

class Timer: public Interrupt
{	WORD _phase;
	Video* out;
	
public:
	Timer(Video *v);	
	void handle(regs* r, int vector, int errorCode);
	void setPhase(WORD phase);
};

#endif /*TIMER_H_*/
