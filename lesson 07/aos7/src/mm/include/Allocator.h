#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "../../include/system.h"

class Allocator {
	public:
		virtual void *alloc(size_t size, DWORD attribs)=0;
		virtual void free(void *)=0;
};

#endif /*ALLOCATOR_H_*/
