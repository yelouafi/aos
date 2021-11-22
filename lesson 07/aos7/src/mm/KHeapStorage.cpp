#include "./include/KHeapStorage.h"
#include "../include/Kernel.h"
#include "../include/assert.h"



int KHeapStorage::initHeap(virtAddr_t heapBase, DWORD heapSize, 
			PhysMManager *pmem, Pager *pager)
{	KASSERT(heapBase && heapSize && pmem && pager);
	_heapBase = heapBase;
	_heapSize = heapSize; 
	_pmem = pmem;
	_pager = pager;
	_nbFree = 0;
	// init the heap space management structs
	// first aquires a physical page frame
	physAddr_t paddr = _pmem->allocFrame(0);
	if(!paddr)
		return NOMEM;
	int err = _pager->map(_heapBase, paddr, KERNEL_WRITE);
	if(err) {
		_pmem->freeFrame(paddr);
		return INVPARAM;
	}
	_heapFree = _heapSize;
	selfSplit(_heapBase, _heapSize, null);
	return SUCCESS;
}

VRegion *KHeapStorage::alloc(DWORD size, DWORD attribs) {
	KASSERT(size != 0);
	size = pageTop(size);

	VRegion *rg = findFirstFree(size);

	if(rg) {
		rg->_attribs |= (VR_USED | attribs);
		// if flag VR_MAP turned on, create underlying
		// pages frames
		if(attribs & VR_MAP) {
			for (DWORD start = rg->_start; start < rg->end(); 
							start += PAGE_SIZE) {
				physAddr_t paddr = _pmem->allocFrame(0);
				
				/* if either we fail to get a new page frame
				 * or to map it at the given address, we must
				 * undo all preceding allocation and mappings */
				if(!paddr || _pager->map(start, paddr, KERNEL_WRITE)) {
					free(rg);
					if(paddr)
						_pmem->freeFrame(paddr);
					start -= PAGE_SIZE;
					while(start >= rg->_start) {
						_pager->unmap(start, true);
						start -= PAGE_SIZE;
					}
					return null;
				} 
			}
		}
	}
	if(_nbFree < 2) {
			VRegion *vr = findFirstFree(VRBUF_SIZE);
			if(!rg)
				goto fin;
			physAddr_t paddr = _pmem->allocFrame(0);
			if(!paddr)
				goto fin;
			int err = _pager->map(vr->_start, paddr, KERNEL_WRITE);
			if(err) {
				_pmem->freeFrame(paddr);	
				goto fin;
			}
			SelfBuffer *sb = new ((void *)vr->_start) 
						SelfBuffer(sizeof(VRegion), VRBUF_SIZE);
			sb->info = (DWORD) vr;
			_heapFree -= VRBUF_SIZE;
		
			sbEmpty.push(sb);
			_nbFree += sb->freeObjs(); 
		}
fin:
	_heapFree -= rg->_size;
	return rg;
}

void KHeapStorage::free(VRegion *region) {
	KASSERT(region);
	DWORD size = region->_size;
	regUsed.remove(region);
	region->_attribs &= ~(VR_USED);
	if(region->_attribs & VR_MAP) {
		/* if region has an underlying phys frames
		 * unmap and free them */
		for(DWORD start = region->_start; start < region->end();
					start += PAGE_SIZE)
				_pager->unmap(start, true);
		// clear the VR_MAP flag
		region->_attribs &= ~(VR_MAP);
	}
	insertVRFree(region);
	_heapFree += size; 
} 

/* free space occuped by empty buffers an empty buffer
 *  contains on used object (its own region) plus optionnally
 *  another used obj holding the adjacent free region*/
 
void KHeapStorage::reclaim() {
	do {
		if(sbEmpty.isEmpty())
			return;
		SelfBuffer *cur = sbEmpty.Head();
		// ensure we can still get a region object without
		if(_nbFree < (2+cur->freeObjs())) {
			break;
		}
		// get the next buffer
		sbEmpty.pop();
		_nbFree -= cur->freeObjs();
		/* remove the region occuped by the buffer (stored in 
		 * info) from the used list*/
		VRegion *owner = (VRegion *)cur->info;
		free(owner);
	} while(1);
}

void KHeapStorage::printStat() {
	printf("\nFree heap size %x\n", _heapFree);	
	printf("used regions number %d\n", regUsed.Count());
	printf("free regions number %d\n", regFree.Count());
	printf("full slabs number %d\n", sbFull.Count());
	printf("part slabs number %d\n", sbPart.Count());
	printf("free slabs number %d\n", sbEmpty.Count());
	
}
