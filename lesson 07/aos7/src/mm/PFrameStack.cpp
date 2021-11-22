#include "./include/PFrameStack.h"
#include "../include/Kernel.h"

PFrameStack::PFrameStack(DWORD nbFrames) {
		top = 0;
		frames = (DWORD *) Kernel::alloc(nbFrames*sizeof(DWORD),0);
};
