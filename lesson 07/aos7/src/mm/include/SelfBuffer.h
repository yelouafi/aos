#ifndef SELFBUFFER_H_
#define SELFBUFFER_H_

#include "../../include/system.h"

/********************SelfBuffer Class ***********************/
/** this class represents a sef managed buffer, ie this *****/
/** buffer can hold a free list of objets with same size ****/
/** self managed means it does not need another allocator ***/
/** to work, this will avoid us the Chiken/odd problem when */
/** implementing a memory allocator. ************************/
/** usage : create an instance and supplie it object size ***/
/** plus the buffer to manage. and call alloc/free to allocate*/
/** or free an object. when the buffer is full it returns null*/
/** you may then create another self buffer. for list maintenance*/
/** you can set next and prev members of the buffer*/

struct freeObj {
	freeObj *next;
};




class SelfBuffer
{	DWORD size;
	DWORD nbFree;
	DWORD maxFree;
	DWORD nbAlloc;
	freeObj *firstFree;
	
public:
	SelfBuffer(DWORD objSize, DWORD maxSize);
	
	void *operator new(size_t size, void *location){
		return location;	
	};
	
	
	void *alloc();
	void free(void *object);
		
	inline bool isEmpty() { return (nbFree == maxFree); };
	inline bool isFull() { return (nbFree == 0); };
	inline DWORD freeObjs() { return nbFree; };
	inline DWORD usedObjs() { return nbAlloc; };
	
	SelfBuffer *next, *prev;
	DWORD info;
};

#endif /*SELFBUFFER_H_*/
