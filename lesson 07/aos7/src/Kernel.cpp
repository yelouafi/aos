#include "./include/Kernel.h"
#include "./include/video.h"
#include "./x86/include/Machine.h"
#include "./include/DLList.h"
#include "./mm/include/VRegion.h"


PhysMManager* Kernel::physMManager = null;
Pager *Kernel::pager = null;
KHeapStorage *Kernel::kheap = null;
DWORD Kernel::memorySize = 0;
DWORD Kernel::kernelEnd = 0;
Video *Kernel::video = null;
Malloc *Kernel::malloc = 0; 
Allocator *Kernel::_alloc;

void Kernel::init(DWORD magic, multiboot_info *mbi) {
	extern char end;
	kernelEnd = (DWORD) &end;
	memorySize = mbi->mem_upper*1024+0x100000;
	// is magic number valid?
	if(magic != MULTIBOOT_MAGIC) { 
		Machine::hlt();
	}
	
	_alloc = new((void *)kernelEnd) BootAllocator(&kernelEnd);
	video = new Video();
	malloc = new Malloc();
	physMManager = new PhysMManager();
	pager = new Pager(physMManager);
	kheap = new KHeapStorage();
}

void Kernel::initPhysMManager() {
	physMManager->init();
	_alloc = null;
}

void Kernel::initPaging() {
	pager->initPaging(START_VGA, physMManager->kTop());
}

void Kernel::initHeap() {
	if(kheap->initHeap(physMManager->kTop(), 
					PAGES_TABLES-physMManager->kTop(), 
					physMManager, 
					pager) || 
						ObjCache::init(kheap)) 
		FATAL_ERROR("ÝÔá ÅÚÏÇÏ ÇáÐÇßÑÉ. ÊæÞíÝ ÇáäÙÇã\n");
	malloc->init(kheap);
	_alloc = malloc;
}

	
void Kernel::FATAL_ERROR(const char *format, ...) {
	va_list ap;
	
	va_start(ap, format);
	video->vprintf(format, ap);
	va_end(ap);
	Machine::hlt();
}
