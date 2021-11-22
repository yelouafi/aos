#ifndef PHYSMMANAGER_H_
#define PHYSMMANAGER_H_

#include"PFrameStack.h"

#define PAGE_SHIFT	12				
#define PAGE_SIZE	4096
#define PAGE_MASK	~(PAGE_SIZE-1)   //extract the n-12 bytes of address

inline DWORD pageBase(DWORD address) {
		return (address & PAGE_MASK);
}

inline DWORD pageTop(DWORD address) {
		return pageBase(address+PAGE_SIZE-1);
}

inline bool isPageAligned(DWORD adress) {
	return !(adress & (PAGE_SIZE-1));	
}

inline DWORD pageIndex(DWORD address) {
		return (address & PAGE_MASK)>> PAGE_SHIFT;
}

class PhysMManager
{	
	PFrameStack *_pfStack;
	DWORD nbFrames;
	
	DWORD memTop;
	DWORD kernelTop;
	DWORD kernlBase;
	
	
public:
	static const DWORD ZEROED = 1;
	
	int init();
	//allocate a single page Frame
	physAddr_t allocFrame(DWORD flags);
	
	//free page frame at address 
	void freeFrame(physAddr_t address);	
	
	// get Total free page
	DWORD freePages();
	
	DWORD kTop() {
		return kernelTop;
	};
	
	DWORD kbase() {
		return kernlBase;
	};
};

#endif /*PHYSMMANAGER_H_*/
