#include "IntManager.h"

static Interrupt *handlers[MAX_IDT_ENTRIES];

//this is the default handler, it will call
// the handler registered with this vector
extern "C" void isrDispatch(regs r, int vector, int errorCode) {
	if(vector < 0 || vector >= MAX_IDT_ENTRIES)
		return;
	if(handlers[vector])
		handlers[vector]->handle(&r, vector, errorCode);
}

void IntManager::setHandler(BYTE vector, Interrupt *interrupt) {
	handlers[vector] = interrupt;	
}

void IntManager::unsetHandler(BYTE vector) {
	handlers[vector] = (Interrupt*)null;	
}
