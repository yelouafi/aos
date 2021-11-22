#ifndef SYSTEM_H_
#define SYSTEM_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long QWORD;


#define MULTIBOOT_MAGIC      0x1BADB002

#define	null	((void*)0)

struct multiboot_info
{
	unsigned long flags;
	unsigned long mem_lower;
	unsigned long mem_upper;
	unsigned long boot_device;
};


//this struct represents the stack format
// when switching to c interrupt handler
struct regs {
	DWORD ss, gs, fs, es, ds;
	DWORD ebp, esi, edi, edx, ecx, ebx, eax;
} __attribute__((packed));


inline void cli() {
	asm volatile ("cli");	
}

inline void sti() {
	asm volatile ("sti");	
}

inline unsigned char inb (unsigned short port)
{
    unsigned char rv;
   	asm volatile("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
inline void outb (unsigned short port, unsigned char data)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (data));
}

extern "C" void __cxa_pure_virtual();
BYTE *memcpy(BYTE *dest, const BYTE *src, int count);
BYTE *memset(BYTE *dest, const BYTE val, int count);
void intToString (char *buf, char base, int number);
void GDTSetup();

#endif /*SYSTEM_H_*/
