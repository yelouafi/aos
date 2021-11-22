#include "PFrameStack.h"

PFrameStack::PFrameStack(DWORD nbFrames) {
		top = 0;
		frames = (DWORD *) bootAlloc(nbFrames*sizeof(DWORD));
};
