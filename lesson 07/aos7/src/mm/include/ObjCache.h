#ifndef OBJCACHE_H_
#define OBJCACHE_H_

#include "../../include/system.h"
#include "KHeapStorage.h"

struct objHead {
	SelfBuffer *owner;
};

class ObjCache
{	char name[20];
	DWORD _objSize;
	DWORD _bufSize;	
	
	DLList<SelfBuffer> sbEmpty;
	DLList<SelfBuffer> sbFull;
	DLList<SelfBuffer> sbPart;
	
	DWORD _totObjSize;
	DWORD _nbFree;
	
	static ObjCache *ccache;
	static KHeapStorage *heap;
	static int init(KHeapStorage *kheap);
	
	friend class Kernel;
	
	inline SelfBuffer* grow() {
		VRegion *rg = heap->alloc(_bufSize, VR_MAP);
		if(!rg)
			return null;
		SelfBuffer *sb = new((void *)rg->start()) SelfBuffer(_totObjSize, _bufSize);
		sb->info = (DWORD) rg;
		sbEmpty.push(sb);
		_nbFree += sb->freeObjs();
		return sb;
	};
	
	
public:
	ObjCache(const char *name, DWORD bufSize, DWORD objSize);
	
	
	void *alloc(DWORD attribs);
	int free(void *object);
	
	inline void shrink() {
		SelfBuffer *buf;
		while(!sbEmpty.isEmpty()) {
			buf = sbEmpty.pop();
			_nbFree -= buf->freeObjs();
			VRegion *vr = (VRegion *)buf->info;
			heap->free(vr);	
		}	
	};
	
	inline void printStat() {
		printf("empty slabs %d\n", sbEmpty.Count());
		printf("partial slabs %d\n", sbPart.Count());
		printf("full slabs %d\n", sbFull.Count());
		printf("total free %d\n", _nbFree);
	};
	
	void *operator new(size_t size, void *loc){
		return loc;
	};
	
	void *operator new(size_t size){
		return ccache->alloc(0);
	};
	
	void operator delete(void *address){
		ccache->free(address);
	};

	
};

#endif /*OBJCACHE_H_*/
