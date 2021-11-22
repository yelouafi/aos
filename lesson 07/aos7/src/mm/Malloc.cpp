#include "./include/Malloc.h"
#include "./include/SelfBuffer.h"
#include "../include/assert.h"


struct cacheHead {
	DWORD size;	
};

struct vrMallocHead {
	VRegion *region;
	DWORD zero;	
};

#define MAX_CACHE	12288

static struct {
	const char *name;
	DWORD objSize;
	DWORD bufSize;	
	ObjCache *cache;
} mallocCache[] = {
	{ "malloc8b", 8, PAGE_SIZE, null}, 	
	{ "malloc16b", 16, PAGE_SIZE, null},
	{ "malloc32b", 32, PAGE_SIZE, null},
	{ "malloc64b", 64, PAGE_SIZE, null},
	{ "malloc128b", 128, PAGE_SIZE, null},
	{ "malloc256b", 256, PAGE_SIZE, null},
	{ "malloc512b", 512, PAGE_SIZE, null},
	{ "malloc1024b", 1024, PAGE_SIZE, null},
	{ "malloc2048b", 2048, 2*PAGE_SIZE, null},
	{ "malloc4096b", 4096, 3*PAGE_SIZE, null},
	{ "malloc8192b", 8192, 3*PAGE_SIZE, null},
	{ "mallocMAXb", MAX_CACHE, 4*PAGE_SIZE, null},
	{ null, 0, 0, null}
};

int Malloc::init(KHeapStorage *kheap)
{	
	heap = kheap;
	for (int i = 0; mallocCache[i].objSize != 0; ++i) {
		ObjCache *tmp = new ObjCache(mallocCache[i].name,
			mallocCache[i].objSize+sizeof(cacheHead),
			mallocCache[i].bufSize);
		KASSERT(tmp);
		mallocCache[i].cache = tmp;
	}
	return SUCCESS;
}


void* Malloc::alloc(size_t size, DWORD attribs) {
	static DWORD i =0;
	if(size <= MAX_CACHE) {
		for (int i = 0; mallocCache[i].objSize != 0; ++i) {
			if(size <= mallocCache[i].objSize) {
				cacheHead *ret = (cacheHead *)mallocCache[i].cache->alloc(0);
				if(ret) {
					ret->size = size;
					void *t = (void *) (ret+1);
					return (void *) (ret+1);
				}
			}
		}
	} else {
		VRegion *vr = heap->alloc(size+sizeof(vrMallocHead), VR_MAP);
		if(vr) {
			vrMallocHead *h = (vrMallocHead *) vr->start();
			h->region = vr;
			/* important to find wich method was used by malloc
			 * from the cache or directly from the heap */
			h->zero = 0;
			return (void *)(h+1);		
		}
	}
	return null;	
}

inline DWORD allocSize(void *addr) {
	return *((DWORD *)addr-1);		
}

inline vrMallocHead *vrHead(void *addr) {
	return (vrMallocHead *) ((DWORD)addr - sizeof(vrMallocHead));
}

inline cacheHead *chead(void *addr) {
	return (cacheHead *) ((DWORD)addr - sizeof(cacheHead));
}

void Malloc::free(void *address) {
	DWORD size = allocSize(address);
	if(size) {
		KASSERT(size <= MAX_CACHE);
		for (int i = 0; mallocCache[i].objSize != 0; ++i) {
			if(size <= mallocCache[i].objSize) {
				mallocCache[i].cache->free((void *)chead(address));
				return;
			}
		}
	} else {
		vrMallocHead *h = vrHead(address);
		KASSERT(h->region);
		heap->free(h->region);
	}
}


