#ifndef PAGER_H_
#define PAGER_H_

#include "system.h"
#include "Machine.h"
#include "PhysMManager.h"

// Values for self mapping
#define SELF_MAP_INDEX	255
#define	ZONE_MAPPING	(SELF_MAP_INDEX<<22)
#define	ZONE_MAPPING_SIZE	(1UL<<22)
#define	PAGE_DIR		(ZONE_MAPPING+(SELF_MAP_INDEX<<12))
#define	SELF_PDE_MAP	(PAGE_DIR+(SELF_MAP_INDEX*4))
#define USER_ZONE		(ZONE_MAPPING+ZONE_MAPPING_SIZE)

#define VPAGE_SHIFT	12				
#define VPAGE_SIZE	4096
#define VPAGE_MASK	(PAGE_SIZE-1)   //extract the n-12 bytes of address
#define PDE_FLAGS_MASk	VPAGE_SIZE-1
#define PTE_FLAGS_MASk	VPAGE_SIZE-1


#define PDE_SHIFT	22
#define PTE_SHIFT	12


#define	PRESENT	1
#define	WRITE	2
#define	USER	4
#define	WRITE_THROUGH	8
#define	CACHE_DISABLE	16
#define	ACESSED	32
#define	DIRTY	64
#define	PT_ATTRIB	128
#define	GLOBAL	256

typedef	unsigned int	pde_t;
typedef	unsigned int	pte_t;
#define NULL_PDE	0
#define NULL_PTE	0


class Pager
{	PhysMManager *pmem;
	DWORD pbdr;	//inititial PDBR
	
	DWORD baseDeneid, topDeneid;
	
	inline bool isSystemAddr(virtAddr_t vaddr) {
		return ((vaddr >= START_VGA &&  vaddr < topDeneid) || !vaddr);
	};
	
	inline bool isMapAddr(virtAddr_t vaddr) {
		return (vaddr >= ZONE_MAPPING && vaddr < USER_ZONE);
	};
	
	void invlPage(virtAddr_t page) {
		asm volatile("invlpg %0" ::"m"(*(DWORD *)page));	
	}
	
	void invlAllPages() {
		DWORD tmp;
		asm volatile(
			"mov %cr3, %0 \n\t\
			mov %0, %cr3\n\t" 
			:"=r"(tmp)::"memory");	
	}
public:
	Pager(PhysMManager *pmm);
	
	
	// setup the paging subsystem
	pde_t* initPaging(physAddr_t kbase, physAddr_t ktop); 
	
	// map a physical frame to a new virtual address
	int map(virtAddr_t vaddr, physAddr_t paddr, DWORD flags);
	int unmap(virtAddr_t vaddr);
};

#endif /*PAGER_H_*/
