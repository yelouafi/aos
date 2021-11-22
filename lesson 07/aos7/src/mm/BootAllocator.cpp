#include "./include/BootAllocator.h"
#include "../include/Kernel.h"

void *BootAllocator::alloc(size_t size, DWORD attribs) {
	DWORD addr = *krnEnd;
	DWORD newEnd = addr + size;
	if(Kernel::getMemorysize() <= newEnd)
		//MSG: Boot Allocator: no enough memory
		Kernel::FATAL_ERROR("ãÎÕÕ ÇáÅÞáÇÚ: áÇÊæÌÏ ÐÇßÑÉ ßÇÝíÉ");
	(*krnEnd) += size;
	return (void *)addr;	
}

void BootAllocator::free(void *) {
	// not supported
}
