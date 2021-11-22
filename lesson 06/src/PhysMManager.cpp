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
	return _pfStack->pop()<<PAGE_SHIFT;	
}
	
void PhysMManager::freeFrame(DWORD index) {
	_pfStack->push(index);	
}

DWORD PhysMManager::freePages() {
	return _pfStack->size();	
}


