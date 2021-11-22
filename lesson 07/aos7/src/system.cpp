#include "./include/system.h"
#include "./x86/include/Machine.h"
#include "./include/Kernel.h"

// needed by GCC when declaring abstract class
extern "C" void __cxa_pure_virtual(){};

// functions called by gcc cygwin compiler
void __main(){};
void _alloca(){};

/* this will overide the builtin new operator */
void *operator new(size_t size)
{
	return Kernel::alloc(size, 0);
}

//overload the operator "new[]"
void *operator new[](size_t size)
{
	return Kernel::alloc(size, 0);
}

void operator delete(void *loc)
{
	Kernel::free(loc);
}

void operator delete[](void *loc)
{
	Kernel::free(loc);
}

/* Convert an integer to a decimal (base = 'd')
	or to an hexadecimal (if base = 'x'). */
void intToString(char *buf, char base, int number)
{
	static char digits[] = "0123456789abcdef";
	char *p = buf;
	unsigned long uns = number;
	int divisor = 10;

	// If we are in decimal base invert the signe if necessary
	if (base == 'd' && number < 0)
	{
		*p = '-';
		p++;
		buf++;
		uns = -number;
	}
	else if (base == 'x')
	{
		divisor = 16;
		*p = '0';
		*(p + 1) = 'x';
		p += 2;
		buf += 2;
	}
	else if (base != 'u')
		*buf = 0;

	/* We will extract digits from the right by dividing
		by base and fetching the remainder */
	do
	{
		long remainder = uns % divisor;
		*p = digits[remainder];
		*p++;
	} while (uns /= divisor);

	// add a zero to terminate string
	*p = 0;

	// No we must revert the generated string
	char *head, *tail;
	head = buf;		// head will point to the first char in buf
	tail = p - 1; // and tail to the last char

	while (head < tail)
	{
		char tmp = *head;
		*head = *tail;
		*tail = tmp;
		head++;
		tail--;
	}
}

char *strncpy(char *dest, const char *src, DWORD nb)
{
	for (int i = 0; i < nb && src[i] != 0; ++i)
	{
		dest[i] = src[i];
	}
	return dest;
}

void printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	Kernel::vprintf(format, ap);
	va_end(ap);
}
