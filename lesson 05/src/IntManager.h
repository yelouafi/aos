#ifndef INTMANAGER_H_
#define INTMANAGER_H_

#include "Machine.h"
#include "interrupt.h"

class IntManager
{	
public:
	static void setHandler(BYTE vector, Interrupt *interrupt);
	static void unsetHandler(BYTE vector);
	
};

#endif /*INTMANAGER_H_*/
