#include"video.h"


// function's called by gcc cygwin compiler
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
		*p++;
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



/*this function will be called from an assmebler file
  * so we tell the compiler to preserve its name when
  * compiling to assmebly*/
extern "C" void kmain(DWORD magic, multiboot_info *mbi) {
	GDTSetup();
	Video v;
	char *device[] = { "floppy A", "hard disk"};
	v.clear();
	// is magic number valid?
	if(magic != MULTIBOOT_MAGIC) {
		v.printf("Assalamou Alaikoum but not from grub\n");
		v.printf("Invalid magic number %x\n", magic);
	} else {
		v.printf("Assalamou Alaikoum from grub\n");
	}
	
	// if bit 1 of flags is set, then memory info is available
	if (mbi->flags & 1) {
       v.printf ("Lower memory = %uKB\n",
                 (unsigned) mbi->mem_lower);
		v.printf ("Upper memory = %uKB\n",
                 (unsigned) mbi->mem_upper);
    }
    
	// if bit 2 of flags is set, then boot device info is available
	char bootdvc = ((unsigned) mbi->boot_device >> 24) & 0xf;
	char *boot = (bootdvc)?device[1]:device[0];
    if (mbi->flags & 2)
        v.printf ("Boot device = %s\n", boot);	
	while(1);
}
