#include "./include/ObjCache.h"
#include "../include/assert.h"

ObjCache* ObjCache::ccache = 0;
KHeapStorage* ObjCache::heap = 0;

int ObjCache::init(KHeapStorage *kheap) {
	heap = kheap;
	VRegion *rg = heap->alloc(VRBUF_SIZE, VR_MAP);
	if(!rg)
		return NOMEM;
	
	SelfBuffer *sb = new((void *)rg->start()) SelfBuffer(sizeof(ObjCache)+
						sizeof(objHead), VRBUF_SIZE);
	sb->info = (DWORD) rg;
	
	void *tmp = sb->alloc();
	KASSERT(tmp);
	ccache = new(tmp) ObjCache("ccache", sizeof(ObjCache), VRBUF_SIZE);
	
	ccache->sbPart.push(sb);
	ccache->_nbFree += sb->freeObjs();
	return SUCCESS;
}

ObjCache::ObjCache(const char *name, DWORD objSize,  DWORD bufSize):
	_bufSize(bufSize), _totObjSize(alignSup(objSize+sizeof(objHead),4))
{
	KASSERT(objSize>0);
	KASSERT(bufSize>0); 
	KASSERT(bufSize>(_totObjSize+sizeof(SelfBuffer)));
	strncpy(this->name, name, 20);
}

void *ObjCache::alloc(DWORD attribs) {
	SelfBuffer *buf;
	void *ret;
	if(!sbPart.isEmpty()) {
		buf = sbPart.Head();
		ret = buf->alloc();
		if(buf->isFull()) {
			sbPart.remove(buf);
			sbFull.push(buf);	
		}	
	} else if(!sbEmpty.isEmpty() || grow()) {
		buf = sbEmpty.pop();
		ret = buf->alloc();
		if(!buf->isFull())
			sbPart.push(buf);	
		else
			sbFull.push(buf);
	} else
		return null;
	_nbFree--;
	KASSERT(ret);
	((objHead *)ret)->owner = buf;
	return (void *) ((DWORD)ret+sizeof(objHead));
}

int ObjCache::free(void *object) {
	objHead *head = (objHead *)((DWORD)object-sizeof(objHead));
	SelfBuffer *buf = head->owner;
	if(!buf)
		return INVPARAM;
	DLList<SelfBuffer> *list;
	if(buf->isFull()) 
		list = &sbFull;
	else
		list = &sbPart;
		
	buf->free(object);
	_nbFree ++;
	if(buf->isEmpty()) {
		list->remove(buf);
		sbEmpty.push(buf);
	} else if(list != (&sbPart)) {
		list->remove(buf);
		sbPart.push(buf);
	}	 
}

