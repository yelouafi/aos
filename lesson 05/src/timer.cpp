#include "timer.h"
#include "video.h"
#include "pic.h"

static int nb=0;
static long long sec=0;

static int base = 1193180;

Timer::Timer()
{
	_phase = 20;
	setPhase(_phase);
}


extern Video *video;
void Timer::handle(regs* r, int vector, int errorCode) {
	nb++;
	if( (nb % _phase) == 0) {
		int x = video->x();
		int y = video->y();
		video->moveTo(0,24);
		// MSG: %u seconds elapsed
		video->printf("%u ËÇäíÉ ãÑÊ ãäÐ ÇáÅÞáÇÚ", ++sec);
		video->moveTo(x,y);
		
		block--;
	}
	PIC::acknowledgePIC1();
}

void Timer::setPhase(WORD phase) {
	_phase = phase;
	int divisor = base / phase;
	Machine::cli();
	Machine::outb(0x43, 0x34);	// counter 0, square wave
	Machine::outb(0x40, divisor & 0xff);
	Machine::outb(0x40, divisor>>8);
	Machine::sti(); 
}

void Timer::wait(int clocks){
	block = clocks;
	
	do {} while(block);	
}
