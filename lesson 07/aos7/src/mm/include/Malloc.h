#ifndef MALLOC_H_
#define MALLOC_H_

#include "../../include/system.h"
#include "KHeapStorage.h"
#include "ObjCache.h"
#include "Allocator.h"

struct mallocCache;

class Malloc: public Allocator
{	KHeapStorage *heap;
public:
	int init(KHeapStorage *kheap);
	
	void *alloc(size_t size, DWORD attribs);
	void free(void *address);	
};

#endif /*MALLOC_H_*/
