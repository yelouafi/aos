#include "Kernel.h"
#include "video.h"
#include "Machine.h"

extern Video *video;

PhysMManager* Kernel::physMManager = (PhysMManager*) 0;
Pager *Kernel::pager = (Pager*)0;
pde_t *Kernel::kpageDir = (pde_t *)0;

void Kernel::init(DWORD magic, multiboot_info *mbi) {
	
	// is magic number valid?
	if(magic != MULTIBOOT_MAGIC) { 
		video->printf("Invalid magic number %x\n", magic);
		Machine::hlt();
	}
}

void Kernel::initPmem() {
	physMManager = new PhysMManager();
}

void Kernel::initPaging() {
	pager = new Pager(physMManager);
	kpageDir = pager->initPaging(START_VGA, pageTop(kernelEnd));
}



	
void Kernel::FATAL_ERROR(const char *s) {
	video->printf(s);
	Machine::hlt();
}
