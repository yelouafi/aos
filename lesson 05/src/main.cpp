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
	
	video->printf("«·”·«„ ⁄·Ìﬂ„ 326.25 «· Õ„Ì·  „ „‰ GRUB Ê«·Õ„œ ··Â\n");
	
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
	video->setWriteMode(Latin);
	
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
	
	// allocate space for the memory test array
	pageTest = (DWORD *)bootAlloc(
			(memorySize/PAGE_SIZE)*sizeof(DWORD));
			
	Exception *exc = new Exception();
	for (int i = 0; i < 32; ++i) {
		IntManager::setHandler(i, exc);
	}
	
	Timer *t = new Timer();
	t->setPhase(20);
	IntManager::setHandler(32,t);
	PIC::setPIC1Mask(0xFE);
	
	// MSG: initializing Physical Allocator
	video->printf("≈⁄œ«œ «·„Œ’’ «·ÕﬁÌﬁÌ ··–«ﬂ—….............");
	Kernel::initPhysMem();
	// MSG: OK
	video->printf("[‰ÃÕ]\n");
	
	// begins the memory test
	testPhysMem();
	
	while(1);
}


static DWORD nbAllocated;
static DWORD nbFree;

inline void state() {
	int y = video->y();
	video->moveTo(0, 10);
	// MSG: %u Frames allocated
	video->printf(" „  Œ’Ì’ %u ’Ê—… ’›Õ…               \n", nbAllocated);
	// MSG: %u free Frames
	video->printf(" „  Õ—Ì— %u ’Ê—… ’›Õ…                \n",nbFree);
	video->moveTo(0, y);
}

extern DWORD kernelEnd;
void testPhysMem() {
	nbAllocated = 0;
	PhysMManager *physMManager = Kernel::physMManager;
	//init the array test
	nbFree = physMManager->freePages();	
	// MSG: Testing Physical allocator		
	video->printf("»œ¡  Ã—Ì» «·„Œ’’ «·ÕﬁÌﬁÌ\n", nbFree);
	
	DWORD *addr;
	// MSG: Testing allocation	
	video->printf(" Ã—Ì» «· Œ’Ì’\n");
	while(1) {
		addr = (DWORD *)physMManager->allocFrame(0);
		if(!addr) {
			nbFree = 0;
			break;
		}
		
		if(*addr == (DWORD)addr && addr[(PAGE_SIZE-1)/4] == 0x55) {
			// MSG: Error, page %u already allocated
			video->printf("Œÿ√° «·’›Õ… %u Œ’’  √ﬂÀ— „‰ „—…\n", nbAllocated);	

			Kernel::FATAL_ERROR("");
		}
		addr[0] = (DWORD)addr;
		addr[(PAGE_SIZE-1)/4] = 0x55;
		pageTest[nbAllocated] = (DWORD)addr;
		nbAllocated++;
		nbFree--;
		state();
	} 
	
	// MSG: Testing freeing
	video->printf(" Ã—Ì» «· Õ—Ì—\n");
	while(1) {
		if(!nbAllocated)
			break;
		addr = (DWORD *)pageTest[nbAllocated-1];
		if(addr[0] != (DWORD)addr || addr[(PAGE_SIZE-1)/4] != 0x55) {
			// MSG: Error, page %u corrupted
			video->printf("Œÿ√° «·’›Õ… %u  ÷„ »Ì«‰«  Œ«ÿ∆…\n", nbAllocated-1);	
			Kernel::FATAL_ERROR("");
		}
		physMManager->freeFrame(((DWORD)(addr)>>PAGE_SHIFT));
		nbFree++;
		nbAllocated--;
		state();
	}
	// MSG: Phsical allocator test Ok
	video->printf(" „  Ã—Ì» „Œ’’ «·–«ﬂ—… «·ÕﬁÌﬁÌ… »‰Ã«Õ\n");
}

