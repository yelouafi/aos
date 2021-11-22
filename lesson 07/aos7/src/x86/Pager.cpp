#include "./include/Pager.h"
#include "../include/Kernel.h"

/*****************Helper functions to manipulate paging ***********/

// Pages tables and page Dir virt adresses
const DWORD selfIndex = 255;
pte_t *pageTables = (pte_t *)PAGES_TABLES;
pde_t *pageDir = (pde_t *)PAGE_DIR;
pde_t *selfPde = (pde_t *)SELF_PDE_MAP;

// make a page dir entry, assumes paddr is Page aligned
inline void makePde(pde_t *pde, physAddr_t paddr, DWORD flags) {
	*pde = paddr | (flags & PDE_FLAGS_MASk);
}

// make a page table entry, assumes paddr is Page aligned
inline void makePte(pte_t *pte, physAddr_t paddr, DWORD flags) {
	*pte = paddr | (flags & PTE_FLAGS_MASk);
}

inline DWORD pdeIndex(virtAddr_t vaddr) {
	return	(vaddr >> PDE_SHIFT);
}

inline DWORD pteIndex(virtAddr_t vaddr) {
	return	((vaddr >> PTE_SHIFT)& 0x3ff);
}

inline DWORD pageOffset(virtAddr_t vaddr) {
	return	(vaddr & VPAGE_MASK);
}

inline pde_t* pdeAddr(virtAddr_t vaddr) {
	return	(pageDir+pdeIndex(vaddr));
}

inline pte_t* pageTableAddr(virtAddr_t vaddr) {
	return	pageTables+(pdeIndex(vaddr)*VPAGE_SIZE);
}

inline pte_t* pteAddr(virtAddr_t vaddr) {
	return	pageTables+(vaddr>>VPAGE_SHIFT);
}

/********************end of  helper functins sectin********************/
/**********************************************************************/
/**********************************************************************/


Pager::Pager(PhysMManager *pmm)
{
	pmem = pmm;
}

inline DWORD criticalAlloc(PhysMManager *pmm, DWORD flgs) {
		DWORD frame = pmm->allocFrame(flgs);
		if(!frame)
			// MSG: no enough memory
			Kernel::FATAL_ERROR("·«  ÊÃœ –«ﬂ—… ﬂ«›Ì…\n");
		return frame;
}

pde_t* Pager::initPaging(physAddr_t kbase, physAddr_t ktop) {
	// 1- build the initial Page dir	
	// get a new fram for the global page dir
	pde_t *pd = (pde_t*)criticalAlloc(pmem, 0);
	for (int i = 0; i < 1024; ++i) {
		pd[i] = 0;
	}
	
	//ident map the kernel image
	pde_t *kpde;
	pte_t *kpte;
	for (physAddr_t paddr = kbase; paddr < ktop; paddr+=PAGE_SIZE) {
		// get the kernel page dir entry index
		kpde = pd + pdeIndex(paddr);
		if(!(*kpde & PRESENT)) {
			// new Page dir entry, get and Map a new page table
			kpte = (pte_t*)criticalAlloc(pmem, 0);
			for (int i = 0; i < 1024; ++i) {
				kpte[i] = 0;
			}
			makePde(kpde, (physAddr_t)kpte, PRESENT|WRITE);
		}
		makePte(kpte+pteIndex(paddr), paddr, PRESENT|WRITE);
	}
	
	// self mapping for the page dir
	makePde(pd+selfIndex, (physAddr_t)pd, PRESENT|WRITE); 
	
	// init the page base dir register value
	pbdr = ((DWORD)pd);
	asm volatile (
		 "movl %0, %%cr3	\n\t\
		 movl %%cr0, %%eax \n\t\
		 orl $0x80010000, %%eax \n\t\
		 movl %%eax, %%cr0	\n\t\
		 jmp	1f	\n\t\
		 1: \n\t\
		 " ::"r"(pbdr):"memory","eax");
	baseDeneid = kbase;
	topDeneid = ktop;
	return pd;
	
}

int Pager::map(virtAddr_t vaddr, physAddr_t paddr, DWORD attribs) {
	// align to a page boundary
	vaddr = pageBase(vaddr);
	paddr = pageBase(paddr);
	
	// Check if vaddr is valid
	if(isSystemAddr(vaddr) || isMapAddr(vaddr))
		return INVPARAM;
	
	//get the page dir entry
	pde_t *pde = pdeAddr(vaddr);
	
	// if not the page dir entry is not present
	// create a new one	
	if(!(*pde & PRESENT)) {
		pte_t *pt = (pte_t*)(PAGES_TABLES+(pdeIndex(vaddr)*PAGE_SIZE)); 
		physAddr_t ppt = pmem->allocFrame(0);
		if(!ppt)
			return NOMEM;
		DWORD flags = PRESENT | KERNEL_WRITE;
		makePde(pde, (physAddr_t)ppt, flags);
		invlPage((DWORD)pt);
		for (int i = 0; i < 1024; ++i) {
			pt[i] = 0;
		}
	}
		
	// get the page table entry, and map paddr
	pte_t *pte = pteAddr(vaddr);
	makePte(pte, (physAddr_t)paddr, PRESENT | attribs);
	invlPage(vaddr);
	return SUCCESS;
}


int Pager::unmap(virtAddr_t vaddr, bool releaseFrame) {
	// align to a page boundary
	vaddr = pageBase(vaddr);
	
	// Check if vaddr is valid
	if(isSystemAddr(vaddr))
		return INVPARAM;
	
	//get the page dir entry
	pde_t *pde = pdeAddr(vaddr);
	
	// make sure the page table is present
	if((*pde & PRESENT)) {
		if(releaseFrame)
			pmem->freeFrame(*pteAddr(vaddr));
		*(pteAddr(vaddr)) = 0;
	}
	return SUCCESS;
}
