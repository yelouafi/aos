#ifndef KERNEL_H_
#define KERNEL_H_

#include "system.h"
#include "PhysMManager.h"



struct multiboot_info
{
	unsigned long flags;
	unsigned long mem_lower;
	unsigned long mem_upper;
	unsigned long boot_device;
};

class Kernel
{
	
public:
	static PhysMManager *physMManager;
	
	static void FATAL_ERROR(const char *s);
	static void init(DWORD magic, multiboot_info *mbi);
	static void initPhysMem();
};

#endif /*KERNEL_H_*/
