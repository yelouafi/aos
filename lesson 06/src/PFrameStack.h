#ifndef PFRAMESTACK_H_
#define PFRAMESTACK_H_

#include"system.h"

/* Stack for managin free page frames */
 
class PFrameStack
{	DWORD top;			// the stack pointer
	DWORD *frames;		// Points to frames's array
	
public:
	
	PFrameStack(DWORD nbFrames);
	
	inline DWORD pop() {
		return (top)?frames[--top]:0;
	};
	
	inline void push(DWORD index) {
		frames[top++] = index;
	};
	
	inline DWORD size() {
		return top;		
	}
	
};

#endif /*PFRAMESTACK_H_*/
