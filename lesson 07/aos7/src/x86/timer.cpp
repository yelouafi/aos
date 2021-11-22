#include "./include/timer.h"
#include "./include/pic.h"

static int nb=0;
static long long sec=0;

static int base = 1193180;

Timer::Timer()
{
	_phase = 20;
	setPhase(_phase);
}


void Timer::handle(regs* r, int vector, int errorCode) {
	nb++;
	
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
