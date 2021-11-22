#ifndef BOOTALLOCATOR_H_
#define BOOTALLOCATOR_H_

#include "Allocator.h"
#include "../../include/system.h"

class BootAllocator: public Allocator
{	DWORD *krnEnd;
public:
	BootAllocator(DWORD *kernelEnd): krnEnd(kernelEnd) {
		(*kernelEnd) += sizeof(BootAllocator);
	};
	
	void* operator new(size_t size, void *loc){
		return loc;
	};
	
	void *alloc(size_t size, DWORD attribs);
	void free(void *);
};

#endif /*BOOTALLOCATOR_H_*/
