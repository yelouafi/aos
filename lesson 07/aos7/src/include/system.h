#ifndef SYSTEM_H_
#define SYSTEM_H_

typedef unsigned int		size_t;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned int	DWORD;
typedef unsigned long long 	QWORD;
typedef unsigned char 		CHAR;
typedef unsigned int		physAddr_t;
typedef unsigned int		virtAddr_t;

#define	SUCCESS			0
#define	NOMEM			-1	//no enough memory
#define	INVPARAM		-2	//invalid parameter


void* operator new(size_t size);
void operator delete(void *loc);
void* operator new[](size_t size);
void operator delete[](void *loc);

/* Copy 'count' bytes of data from 'src' to
*  'dest', finally return 'dest' */
inline BYTE* memcpy(BYTE *dest, const BYTE *src, int count) 
{
    int i;
	for(i=0; i<count; i++)
		dest[i] = src[i];
	return dest;
}


// word and dword versions
inline WORD* memcpy(WORD *dest, const WORD *src, int count) 
{
    int i;
	for(i=0; i<count; i++)
		dest[i] = src[i];
	return dest;
}

inline DWORD* memcpy(DWORD *dest, const DWORD *src, int count) 
{
    int i;
	for(i=0; i<count; i++)
		dest[i] = src[i];
	return dest;
}



/* set 'coun' bytes from 'dest' to 'val' */
inline BYTE* memset(BYTE *dest, BYTE val, int count)
{
	int i;
	for(i=0; i<count; i++)
		dest[i] = val;
	return dest;
}

inline WORD* memset(WORD *dest, WORD val, int count)
{
	int i;
	for(i=0; i<count; i++)
		dest[i] = val;
	return dest;
}


inline DWORD* memset(DWORD *dest, DWORD val, int count)
{
	int i;
	for(i=0; i<count; i++)
		dest[i] = val;
	return dest;
}

// impotant, align must be a power of 2*
inline DWORD alignInf(DWORD value, DWORD align) {
	return (value & ~(align-1));	
}

// impotant, align must be a power of 2*
inline DWORD alignSup(DWORD value, DWORD align) {
	return alignInf(value+align-1, align);	
}

inline bool isAligned(DWORD value, DWORD align) {
	return !(value % align);
}

void intToString (char *buf, char base, int number);
char* strncpy(char *dest, const char *src, DWORD nb);
void printf(const char* format, ...);


/* Basic stdarg.h macros. Taken from gcc support files */
#define __GNUC_VA_LIST 
typedef void *__gnuc_va_list;
typedef __gnuc_va_list va_list;
#define __va_rounded_size(TYPE) \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))
#define va_start(AP, LASTARG) \
  (AP = ((__gnuc_va_list) __builtin_next_arg (LASTARG)))
#define va_end(AP) \
  ((void)0)
#define va_arg(AP, TYPE) \
  (AP = (__gnuc_va_list) ((char *) (AP) + __va_rounded_size (TYPE)),  \
   *((TYPE *) (void *) ((char *) (AP) - __va_rounded_size (TYPE))))
#define __va_copy(dest, src) \
  (dest) = (src)

#endif /*SYSTEM_H_*/
