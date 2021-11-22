#include "timer.h"
#include "pic.h"

static long long  nb=0;
static int sec=0;

static int base = 1193180;

Timer::Timer(Video* v)
{
	out = v;
	_phase = 20;
	setPhase(_phase);
}


void Timer::handle(regs* r, int vector, int errorCode) {
	nb++;
	if( (nb % _phase) == 0) {
		int x = out->x();
		int y = out->y();
		out->moveTo(0,24);
		out->printf("%u secondes elapsed", ++sec);
		out->moveTo(x,y);
	}
	acknowledgePIC1();
}

void Timer::setPhase(WORD phase) {
	_phase = phase;
	int divisor = base / phase;
	
	cli();
	outb(0x43, 0x34);	// counter 0, square wave
	outb(0x40, divisor & 0xff);
	outb(0x40, divisor>>8);
	sti(); 
}


