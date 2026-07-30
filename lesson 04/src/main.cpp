#include"video.h"
#include "interrupt.h"
#include "exception.h"
#include "timer.h"
#include "pic.h"
#include "isr.h"


//functions defined in isrs.s
extern "C" void initISRS();


extern "C" void __cxa_pure_virtual() {};

// functions called by gcc cygwin compiler
void __main() {};
void _alloca() {};

/* Copy 'count' bytes of data from 'src' to
    *  'dest', finally return 'dest' */
BYTE* memcpy(BYTE *dest, const BYTE *src, int count) 
{
    int i;
	for(i=0; i<count; i++)
		dest[i] = src[i];
	return dest;
}


/* set 'coun' bytes from 'dest' to 'val' */
BYTE* memset(BYTE *dest, BYTE val, int count)
{
	int i;
	for(i=0; i<count; i++)
		dest[i] = val;
	return dest;
}


 /* Convert an integer to a decimal (base = 'd')
	or to an hexadecimal (if base = 'x'). */
 void intToString (char *buf, char base, int number)
{	
	static char digits[] = "0123456789abcdef";
	char *p = buf;
	unsigned long uns = number;
	int divisor = 10;
 
   // If we are in decimal base invert the signe if necessary
	if (base == 'd' && number < 0) {
		*p = '-';
		p++;	buf++;
		uns = -number;
	} else if (base == 'x') { 
		divisor = 16;
		*p = '0'; 
		*(p+1) = 'x';
		p +=2;
		buf +=2;
	}	else if(base != 'u')
		*buf = 0;
	
	/* We will extract digits from the right by dividing
		by base and fetching the remainder */
    do {
		long remainder = uns % divisor;
		*p = digits[remainder];
		p++;
	}  while (uns /= divisor);
 
   // add a zero to terminate string
   *p = 0;
 
	// No we must revert the generated string
	char *head, *tail;
	head	= buf;   // head will point to the first char in buf
	tail = p - 1;		// and tail to the last char
	
	while (head < tail) {
		char tmp = *head;
		*head = *tail;
		*tail = tmp;
		head++;
		tail--;
	}
}

static DWORD usableMemoryKilobytes(const multiboot_info *information)
{
	DWORD total = 0;
	DWORD cursor = information->mmap_addr;
	DWORD end = cursor + information->mmap_length;

	while (cursor < end) {
		const multiboot_memory_map_entry *entry =
			reinterpret_cast<const multiboot_memory_map_entry *>(cursor);

		if (entry->size < 20)
			break;

		if (entry->type == 1 && entry->length_high == 0)
			total += entry->length_low / 1024;

		cursor += entry->size + sizeof(entry->size);
	}

	return total;
}

void printMemInfo(Video &v, DWORD magic, multiboot_info *mbi) {
	const char *devices[] = { "floppy A", "hard disk" };

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		v.printf("Assalamou Alaikoum without Multiboot\n");
		v.printf("Invalid magic number %x\n", magic);
		return;
	}

	v.printf("Assalamou Alaikoum from Multiboot\n");

	if (mbi->flags & 1) {
		v.printf("Lower memory = %uKB\n", mbi->mem_lower);
		v.printf("Upper memory = %uKB\n", mbi->mem_upper);
	} else if (mbi->flags & (1 << 6)) {
		v.printf(
			"Usable memory = %uKB (memory map)\n",
			usableMemoryKilobytes(mbi));
	} else {
		v.printf("Memory information = not provided\n");
	}

	if (mbi->flags & 2) {
		char bootDevice = (mbi->boot_device >> 24) & 0xF;
		const char *device = bootDevice ? devices[1] : devices[0];
		v.printf("Boot device = %s\n", device);
	} else {
		v.printf("Boot device = not provided\n");
	}
}


/*this function will be called from an assmebler file
  * so we tell the compiler to preserve its name when
  * compiling to assmebly*/
extern "C" void kmain(DWORD magic, multiboot_info *mbi) {
	cli();
	GDTSetup();
	IDTSetup();

	initPIC();
	initISRS();
	Video v;
	v.clear();

	printMemInfo(v, magic, mbi);
	
	Exception exc(&v);
	for (int i = 0; i < 32; ++i) {
		registerHandler(i, &exc);
	}
	//volatile int a = 5/0;
	Timer t(&v);
	t.setPhase(20);
	registerHandler(32,&t);
	setPIC1Mask(0xFE);
	sti();

	while (1)
		asm volatile("hlt");
}

