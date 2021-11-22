#ifndef VREGION_H_
#define VREGION_H_

#include "../../include/system.h"
#include "../../x86/include/Pager.h"
#include "PhysMManager.h"
#include "SelfBuffer.h"

class VRegion
{	
	
private:
	virtAddr_t _start;
	DWORD _size;
	DWORD _attribs;
	SelfBuffer *buffer;
	

	VRegion(virtAddr_t start, DWORD size, DWORD attribs, SelfBuffer *buf):
		_start(start), _size(size), _attribs(attribs), buffer(buf)
	{};
	
	~VRegion();
	friend class KHeapStorage;
public:
	void* operator new(size_t size, void *location){
		return location;	
	};
	
	void operator delete(void *location) {};
	
	DWORD start() { return _start; };
	DWORD size() {  return _size; };
	DWORD end () { return _start + _size; };
	
	VRegion *prev;
	VRegion *next;
};

#endif /*VREGION_H_*/
