#include "PhysMManager.h"
#include "Kernel.h"
#include "Machine.h"
#include "video.h"

PhysMManager::PhysMManager() {
	memTop = pageBase(memorySize+1);
	nbFrames = memTop / PAGE_SIZE;
	_pfStack = new PFrameStack(nbFrames);
	
	
	kernelTop = pageTop(kernelEnd);
	//mark low memory as free
	for (int i = START_PAGE; i < START_VGA; i+=PAGE_SIZE) {
		_pfStack->push(i>>PAGE_SHIFT);
	}
	
	//mark high memory (afetr the kernel) as free
	for (int i = kernelTop; i < memTop; i+=PAGE_SIZE) {
		_pfStack->push(i>>PAGE_SHIFT);
	}
	
}

DWORD PhysMManager::allocFrame(DWORD flags) {
	BYTE *ind = (BYTE *)_pfStack->pop();	
	DWORD val = ((DWORD) ind) << PAGE_SHIFT;
	if(ind && (flags & ZEROED)) {
		memset((BYTE *)val, 0, PAGE_SIZE);
		
	}
	return val;
}
	
void PhysMManager::freeFrame(DWORD index) {
	_pfStack->push(index);	
}

DWORD PhysMManager::freePages() {
	return _pfStack->size();	
}


