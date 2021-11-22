#ifndef KERNEL_H_
#define KERNEL_H_

#include "system.h"
#include "../mm/include/PhysMManager.h"
#include "../x86/include/Pager.h"
#include "../mm/include/Allocator.h"
#include "../mm/include/KHeapStorage.h"
#include "../mm/include/ObjCache.h"
#include "../mm/include/BootAllocator.h"
#include "../mm/include/Malloc.h"
#include "Video.h"


struct multiboot_info
{
	unsigned long flags;
	unsigned long mem_lower;
	unsigned long mem_upper;
	unsigned long boot_device;
};

class Kernel
{	static DWORD memorySize;
	static DWORD kernelEnd;
	
	static PhysMManager *physMManager;
	static Pager *pager;
	static Allocator *_alloc;
	static Malloc *malloc;
	static Video *video;
	static KHeapStorage *kheap; 
	
public:
	static void FATAL_ERROR(const char *format, ...);
	static void init(DWORD magic, multiboot_info *mbi);
	static void initPhysMManager();
	static void initPaging();
	static void initHeap();
	static void *alloc(size_t size, DWORD attribs) {
		if(_alloc)
			return _alloc->alloc(size, attribs);
		else
			// MSG: no memory allocator 
			FATAL_ERROR("áÇ íæÌÏ ÈÚÏ ãÎÕÕ ááÐÇßÑÉ");
	};
	
	static void *free(void *location) {
		if(_alloc)
			_alloc->free(location);
		else
			// MSG: no memory allocator 
			FATAL_ERROR("áÇ íæÌÏ ÈÚÏ ãÎÕÕ ááÐÇßÑÉ");
	};
	
	static Video *getVideo() {
		return video;
	};
	
	static void vprintf(const char* format, va_list ap) {
		video->vprintf(format, ap);
		video->updateCursor();
	};
	
	
	static DWORD getMemorysize() {
		return memorySize;
	};
	
	static DWORD kend() {
		return kernelEnd;
	};
};

#endif /*KERNEL_H_*/
