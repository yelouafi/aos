#ifndef TIMER_H_
#define TIMER_H_

#include "system.h"
#include "interrupt.h"


class Timer: public Interrupt
{	WORD _phase;
	DWORD block;
		
public:
	Timer();	
	void handle(regs* r, int vector, int errorCode);
	void setPhase(WORD phase);
	void wait(int clocks);
};

#endif /*TIMER_H_*/
