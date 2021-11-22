#include "Kernel.h"
#include "video.h"
#include "Machine.h"

extern Video *video;

PhysMManager* Kernel::physMManager = (PhysMManager*) 0;

void Kernel::init(DWORD magic, multiboot_info *mbi) {
	
	// is magic number valid?
	if(magic != MULTIBOOT_MAGIC) { 
		video->printf("Invalid magic number %x\n", magic);
		Machine::hlt();
	}
}


void Kernel::initPhysMem() {
	physMManager = new PhysMManager();
}

	
void Kernel::FATAL_ERROR(const char *s) {
	video->printf(s);
	Machine::hlt();
}
