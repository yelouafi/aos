#include "system.h"
#include "Machine.h"
#include "Kernel.h"
#include "video.h"
#include "pic.h"
#include "IntManager.h"
#include "Interrupt.h"
#include "Exception.h"
#include "Timer.h"
#include "FontMap.h"

//function defined in isrs.s
extern "C" void initISRS();

DWORD *pageTest;	
void testPhysMem();

Video *video;


/** Prints boot Info using the multiboot struct **/
/** provided by Grib boot loader *****************/

void printMemInfo(multiboot_info *mbi) {
	//{ "Floppy  A", "Floppy B"};
	char *device[] = { "«·ﬁ—’ «·„—‰ A", "«·ﬁ—’ «·„—‰ B"};
	
	video->printf("«·”·«„ ⁄·Ìﬂ„ «· Õ„Ì·  „ „‰ GRUB Ê«·Õ„œ ··Â\n");
	
	// if bit 1 of flags is set, then memory info is available

	if (mbi->flags & 1) {
	// MSG: Memory size = %u kB
       video->printf ("ÕÃ„ «·–«ﬂ—… = %u ﬂÌ·Ê»«Ì \n", memorySize/1024);
    }
    
	// if bit 2 of flags is set, then boot device info is available
	char bootdvc = ((unsigned) mbi->boot_device >> 24) & 0xf;
	char *boot = (bootdvc)?device[1]:device[0];
    if (mbi->flags & 2)
       	// MSG: Boot device %s
        video->printf (" „ «·≈ﬁ·«⁄ „‰  %s\n", boot);	
}


extern void error(Video *v, int err);
void testVirtMem(DWORD *paddr, DWORD *vaddr);
/** this function will be called from an assmebler file **/
/** so we tell the compiler to preserve its name when ****/
/** compiling to assmebly ********************************/
void loadDefaultFonts();

extern "C" void kmain(DWORD magic, multiboot_info *mbi) { 
	extern char end;
	kernelEnd = (DWORD) &end;
	memorySize = mbi->mem_upper*1024+0x100000;
	
	video = new Video();
	FontMap *map = new FontMap();
	
	map->loadDefaultMap();
	video->map = map;
	video->clear();
	//video->setWriteMode(Latin);
	
	Kernel::init(magic, mbi);
	printMemInfo(mbi);
	
	// class Machine contains code specific to the machine
	// inb, outb, cli, sti ... + struct regs, GDTR, IDTR...
	// setupGDT, setupIDT
	Machine::init();
	
	// init and remap the Programmable interrupt controller
	PIC::init();
	
	// init interrupt handlers array	
	initISRS();
	Machine::sti();
	
				
	Exception *exc = new Exception();
	for (int i = 0; i < 32; ++i) {
		IntManager::setHandler(i, exc);
	}
	
	Timer *t = new Timer();
	t->setPhase(20);
	IntManager::setHandler(32,t);
	PIC::setPIC1Mask(0xFE);
	
	// MSG: initializing Physical Allocator
	video->printf("≈⁄œ«œ «·–«ﬂ—….............");
	Kernel::initPmem();
	DWORD *paddr = (DWORD *)Kernel::physMManager->allocFrame(PhysMManager::ZEROED);
	*paddr = 0x55AA;
	video->printf("«·ﬁÌ„… ›Ì «·⁄‰Ê«‰ %x = %x\n", (DWORD)paddr, *paddr);
	Kernel::initPaging();
	// MSG: OK
	video->printf("[‰ÃÕ]\n");
	video->printf(" Ã—Ì» «·„›Â—” «·«› —«÷Ì\n");
	DWORD *vad = (DWORD *) 0xA00000;
	DWORD *vad1 = (DWORD *) 0xB10000;
	
	testVirtMem(paddr, vad);
	testVirtMem(paddr, vad1);
	Kernel::pager->unmap((virtAddr_t)0xB10000);
	*(vad1+1) = 0x51;
	while(1);
}

void testVirtMem(DWORD *paddr, DWORD *vaddr) {
	int err = Kernel::pager->map((virtAddr_t)vaddr, 
				(physAddr_t)paddr, WRITE);
	if(err<0)
		error(video, err);
	
	if(*vaddr != 0x55AA)
		video->printf("Œÿ√° «·ﬁÌ„… ›Ì «·⁄‰Ê«‰ %x = %x €Ì— „ÿ«»ﬁ…\n",
					(DWORD)vaddr, *vaddr );
	else
		video->printf("‰ÃÕ°«·ﬁÌ„… ›Ì «·⁄‰Ê«‰ %x = %x „ÿ«»ﬁ…\n",
				(DWORD)vaddr, *vaddr);
}

