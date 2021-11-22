#ifndef KERNEL_H_
#define KERNEL_H_

#include "system.h"
#include "PhysMManager.h"
#include "Pager.h"



struct multiboot_info
{
	unsigned long flags;
	unsigned long mem_lower;
	unsigned long mem_upper;
	unsigned long boot_device;
};

class Kernel
{	static pde_t *kpageDir;	
public:
	static PhysMManager *physMManager;
	static Pager *pager;
	
	static void FATAL_ERROR(const char *s);
	static void init(DWORD magic, multiboot_info *mbi);
	static void initPmem();
	static void initPaging();
};

#endif /*KERNEL_H_*/
