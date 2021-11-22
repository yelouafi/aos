#include "./include/system.h"
#include "./x86/include/Machine.h"
#include "./include/Kernel.h"
#include "./include/video.h"
#include "./x86/include/pic.h"
#include "./include/IntManager.h"
#include "./include/Interrupt.h"
#include "./x86/include/Exception.h"
#include "./x86/include/Timer.h"
#include "./include/FontMap.h"
#include "./mm/include/ObjCache.h"
#include "./mm/include/KHeapStorage.h"

//function defined in isrs.s
extern "C" void initISRS();

DWORD *pageTest;	
void testPhysMem();


/** Prints boot Info using the multiboot struct **/
/** provided by Grib boot loader *****************/

void printMemInfo(multiboot_info *mbi) {
	//{ "Floppy  A", "Floppy B"};
	char *device[] = { "«·ﬁ—’ «·„—‰ A", "«·ﬁ—’ «·„—‰ B"};
	
	// MSG: boot from grub
	printf("«·”·«„ ⁄·Ìﬂ„ «· Õ„Ì·  „ „‰ GRUB Ê«·Õ„œ ··Â\n");
	
	// if bit 1 of flags is set, then memory info is available

	if (mbi->flags & 1) {
	// MSG: Memory size = %u kB
       printf ("ÕÃ„ «·–«ﬂ—… = %u ﬂÌ·Ê»«Ì \n", Kernel::getMemorysize());
    }
    
	// if bit 2 of flags is set, then boot device info is available
	char bootdvc = ((unsigned) mbi->boot_device >> 24) & 0xf;
	char *boot = (bootdvc)?device[1]:device[0];
    if (mbi->flags & 2)
       	// MSG: Boot device %s
        printf (" „ «·≈ﬁ·«⁄ „‰  %s\n", boot);	
}

class TestMalloc {
public:
	int nb;
	char name[5000];	
	
	TestMalloc(int n, char *aname) {
		nb = n;
		strncpy(name, aname, 5000);
	};
};

ObjCache *cache;
class TestObjCache {
public:
	int nb;
	char name[20];	
	
	TestObjCache(int n, char *aname) {
		nb = n;
		strncpy(name, aname, 19);
	};
	
	
	void* operator new(DWORD size) {
		if(!cache)
			cache = new ObjCache("TestObjCache", 
						sizeof(	TestObjCache),
						PAGE_SIZE);
		return cache->alloc(0);
	};
	
	void operator delete(void *loc) {
		cache->free(loc);
	};
};

#define MAX_TEST	1000
TestObjCache *objCaches[1000];
TestMalloc *objMalloc[1000];

/** this function will be called from an assmebler file **/
/** so we tell the compiler to preserve its name when ****/
/** compiling to assmebly ********************************/
extern "C" void kmain(DWORD magic, multiboot_info *mbi) { 	
	Kernel::init(magic, mbi);
	//printMemInfo(mbi);
	
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
	PIC::enableIRQ(0);

	// MSG: initializing Physical Allocator
	printf("≈⁄œ«œ «·–«ﬂ—…...........");
	Kernel::initPhysMManager();
	Kernel::initPaging();
	Kernel::initHeap();
	// MSG: OK
	printf("‰ÃÕ\n");
	
	printf(" Ã—Ì» „Œ’’ «·”·«»\n");
	printf(" Œ’Ì’ «·⁄‰«’—\n");
	for (int i = 0; i < MAX_TEST; ++i) {
		objCaches[i] = new TestObjCache(i, "Ì«”Ì‰");;
		if(!objCaches[i])
			Kernel::FATAL_ERROR("·«  ÊÃœ –«ﬂ—… ﬂ«›Ì…");
	}
	printf(" Õ—Ì— «·⁄‰«’—\n");
	for (int i = 0; i < MAX_TEST; ++i) {
		delete objCaches[i];
	}
	printf("‰ÃÕ  Ã—Ì» „Œ’’ «·”·«»\n");
	
	
	printf(" Ã—Ì» „Œ’’ Malloc\n");
	printf(" Œ’Ì’ «·⁄‰«’—\n");
	for ( int i = 0; i < MAX_TEST; ++i) {
		objMalloc[i] = new TestMalloc(i, "Ì«”Ì‰");;
		if(!objMalloc[i])
			Kernel::FATAL_ERROR("·«  ÊÃœ –«ﬂ—… ﬂ«›Ì…");
	}
	printf(" Õ—Ì— «·⁄‰«’—\n");
	for (int i = 0; i < MAX_TEST; ++i) {
		delete objMalloc[i];
	}
	printf("‰ÃÕ  Ã—Ì» „Œ’’ Malloc\n");
	
	while(1);
}


