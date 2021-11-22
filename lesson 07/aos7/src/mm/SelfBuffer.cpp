#include "./include/SelfBuffer.h"

SelfBuffer::SelfBuffer(DWORD objSize, DWORD maxSize):
size((objSize < sizeof(freeObj))?sizeof(freeObj):objSize)
{	
	// offset of the first object
	firstFree = (freeObj *)((DWORD)this+sizeof(SelfBuffer));
	
	// nb of free objects
	nbFree = maxFree = (maxSize-sizeof(SelfBuffer))/size;
	// init free list
	freeObj *curFree = firstFree;
	for (int i = 1; i < maxFree; ++i) {
		freeObj *nextFree = (freeObj*)((DWORD)curFree + size);
		curFree->next = nextFree;
		curFree = nextFree;
	}
	info = 0;
}

void *SelfBuffer::alloc() {
	if(nbFree) {
		freeObj *ret = firstFree;
		firstFree = firstFree->next;
		nbFree--; nbAlloc++;
		return ret;
	}
	return (void*)0;
};

void SelfBuffer::free(void *object) {
	freeObj *toFree = (freeObj *)object;
	toFree->next = firstFree;
	firstFree = toFree;
	nbFree++; nbAlloc--;
};
