#include "./include/PhysMManager.h"
#include "../include/Kernel.h"
#include "../x86/include/Machine.h"

int PhysMManager::init() {
	memTop = pageBase(Kernel::getMemorysize()+1);
	nbFrames = memTop / PAGE_SIZE;
	
	_pfStack = new PFrameStack(nbFrames);
	
	kernelTop = pageTop(Kernel::kend());
	
	//mark low memory as free
	for (int i = START_PAGE; i < START_VGA; i+=PAGE_SIZE) {
		_pfStack->push(i>>PAGE_SHIFT);
	}
	
	//mark high memory (afetr the kernel) as free
	for (int i = kernelTop; i < memTop; i+=PAGE_SIZE) {
		_pfStack->push(i>>PAGE_SHIFT);
	}
	return SUCCESS;
}

physAddr_t PhysMManager::allocFrame(DWORD flags) {
	return _pfStack->pop()<<PAGE_SHIFT;	
}
	
void PhysMManager::freeFrame(physAddr_t paddr) {
	_pfStack->push(paddr>>PAGE_SHIFT);	
}

DWORD PhysMManager::freePages() {
	return _pfStack->size();	
}


