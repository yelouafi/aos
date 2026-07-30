#include "timer.h"
#include "pic.h"

static DWORD nb=0;
static int sec=0;

static int base = 1193180;

Timer::Timer(Video* v)
{
	out = v;
	_phase = 20;
}
void Timer::handle(regs* r, int vector, int errorCode) {
	(void)r;
	(void)vector;
	(void)errorCode;
	nb++;
	if( (nb % _phase) == 0) {
		int x = out->x();
		int y = out->y();
		out->moveTo(0,24);
		++sec;
		if (sec == 1)
			out->printf("%u second elapsed", sec);
		else
			out->printf("%u seconds elapsed", sec);
		out->moveTo(x,y);
	}
	acknowledgePIC1();
}

void Timer::setPhase(WORD phase) {
	if (!phase)
		return;

	_phase = phase;
	int divisor = base / phase;

	outb(0x43, 0x34);	// counter 0, square wave
	outb(0x40, divisor & 0xff);
	outb(0x40, divisor>>8);
}


