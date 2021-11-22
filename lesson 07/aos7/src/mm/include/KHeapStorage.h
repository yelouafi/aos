#ifndef KHEAPSTORAGE_H_
#define KHEAPSTORAGE_H_

#include "../../include/system.h"
#include "../../x86/include/Pager.h"
#include "../../include/DLList.h"
#include "PhysMManager.h"
#include "VRegion.h"
#include "SelfBuffer.h"


#define	VRBUF_SIZE	VPAGE_SIZE
#define VR_USED		1
#define VR_MAP		2


class KHeapStorage
{	
	virtAddr_t _heapBase;
	DWORD _heapSize;
	DWORD _heapFree;
	PhysMManager *_pmem;
	Pager *_pager;
	
	DLList<SelfBuffer> sbEmpty;
	DLList<SelfBuffer> sbFull;
	DLList<SelfBuffer> sbPart;
	DWORD _nbFree;

	DLList<VRegion> regUsed;
	DLList<VRegion> regFree;
	friend class Kernel;
	inline void delVR(VRegion *rg) {
		SelfBuffer *buf = rg->buffer;
		DLList<SelfBuffer> *list;
		
		if(buf->isFull()) 
			list = &sbFull;
		else
			list = &sbPart;
			
		buf->free((void *)rg);
		_nbFree ++;
		if(buf->isEmpty()) {
			list->remove(buf);
			sbEmpty.push(buf);
		} else if(list != (&sbPart)) {
			list->remove(buf);
			sbPart.push(buf);
		}	 
	};
	
	// insert a region into the free list 
	// so the list is kept adress-ordered
	inline void insertVRFree(VRegion *rg) {
		VRegion *cur = regFree.reset();
		while(!regFree.atEnd()) {
			if(cur->_start > rg->_start) {
				// test if we can merge with next
				if(rg->end()==cur->_start) {
					cur->_start = rg->_start;
					cur->_size += rg->_size;
					delVR(rg);
					if(cur->prev->end() == cur->_start) {
						cur->prev->_size += cur->_size;
						regFree.remove(cur);
						delVR(cur);
					}
				} else if(cur->prev->end() == rg->_start) {
					cur->prev->_size += rg->_size;
					delVR(rg);
				} else
					regFree.insertBefore(cur, rg);
				return;	
			}
			cur = regFree.next();
		}
		regFree.queue(rg);
	};
	
	// this create a new buffer into the region 
	// pointed by start and create a region object
	// representing the same buffer into itself
	inline int selfSplit(virtAddr_t start, DWORD size, VRegion *prev) {
		SelfBuffer *sb;
		void *tmp;
		if(size > VRBUF_SIZE) {
			// we need t split the region
			sb = new ((void *)start) 
						SelfBuffer(sizeof(VRegion), VRBUF_SIZE);
			
			// first create region representing the buffer			
			tmp = sb->alloc();
			VRegion *sbuf = new(tmp) VRegion(start, VRBUF_SIZE, 0, sb);
			regUsed.push(sbuf);
			sbuf->_attribs |= VR_USED;
			_heapFree -= VRBUF_SIZE;
			// then create the region representing the remainder
			tmp = sb->alloc();
			VRegion *remaind = new(tmp) VRegion(
				start + VRBUF_SIZE,
				size - VRBUF_SIZE, 0, sb);
			sb->info = (DWORD) sbuf;
			// insert the remainder in the free list	
			if(prev)
				regFree.insertAfter(prev, remaind);
			else
				insertVRFree(remaind);
			
			// the buffer is pseudo empty, since it holds its own 
			// region and an adjacent region
			sbPart.push(sb);
			_nbFree += sb->freeObjs();
			return SUCCESS;
		} else if(size == VRBUF_SIZE) {
			// dont need split the region
			sb = new ((void *)start) 
						SelfBuffer(sizeof(VRegion), VRBUF_SIZE);
			void *tmp = sb->alloc();
			// create the region representing the selfBuffer
			VRegion *sbuf = new(tmp) VRegion(start, VRBUF_SIZE, 0, sb);
			sb->info = (DWORD) sbuf;
			regUsed.push(sbuf);
			sbuf->_attribs |= VR_USED;
			_heapFree -= VRBUF_SIZE;
			
			sbPart.push(sb);
			_nbFree += sb->freeObjs();
			return SUCCESS;
		} else
			return NOMEM;
	};
	
	
public:
	KHeapStorage()
	{};
	
	int initHeap(virtAddr_t heapBase, DWORD heapSize, 
			PhysMManager *pmem, Pager *pager);
	DWORD freeHeap() { return _heapFree; };
	
	
	VRegion *alloc(DWORD size, DWORD attribs);
	void free(VRegion *region);
	
	// to reclaim space used by free buffers
	void reclaim();
	
	void printStat();
	
private:
	// check the partial and the free list
	inline VRegion* createVRegion(virtAddr_t start, DWORD size, DWORD attribs) {
		void *tmp;
		VRegion *rg = null;
		SelfBuffer *sb;
		
		// first, check the partial list
		if(!_nbFree) {
			return null;
		}
		if(!sbPart.isEmpty()) {
			sb = sbPart.Head();
			tmp = sb->alloc();
			// if the buffer become full move to the full list
			if(sb->isFull()) {
				sbPart.pop();
				sbFull.push(sb);
			}
			_nbFree--;
			rg = new(tmp) VRegion(start, size, attribs, sb);
		// Partial list is empty, check the empty list
		} else if(!sbEmpty.isEmpty()) {
			sb = sbEmpty.Head();
			tmp = sb->alloc();
			sbEmpty.pop();
			
			if(sb->isFull()) {
				sbFull.push(sb);
			} else
				sbPart.push(sb);
			_nbFree--;
			rg = new(tmp) VRegion(start, size, attribs, sb);
		} 
		return rg;
	};
	
	inline VRegion* findFirstFree(DWORD size) {
		VRegion *free = regFree.reset();
		// iterate over free regions until finding
		// a fit for this size
		while(!regFree.atEnd()) {
			if(free->_size == size) {
				// Good luck, the region has exactly
				// the required size
				regFree.remove(free);
				regUsed.push(free); 
				return free;	
			} else if(free->_size > size) {
				DWORD newStart = free->_start+size;
				DWORD newSize = free->_size - size;
				// the free region is larger than the required size
				// we need to split it 
				VRegion *rg = createVRegion(free->_start, size, 0);
				if(rg) {
					free->_start = newStart;
					free->_size = newSize;
					regUsed.push(rg);
					return rg;
				} else
					return null;
			} else {
				free = regFree.next();	
			}
		}
		return null;
	};
};

#endif /*KHEAPSTORAGE_H_*/
