#ifndef SYSTEM_H_
#define SYSTEM_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long long QWORD;
typedef unsigned char CHAR;

extern DWORD memorySize;
extern DWORD kernelEnd;

void *bootAlloc(DWORD size);

/* this will overide the builtin new operator */
inline void* operator new(DWORD size) {
	void *tmp = bootAlloc(size); 
	return tmp;	
}

/* Copy 'count' bytes of data from 'src' to
*  'dest', finally return 'dest' */
inline BYTE* memcpy(BYTE *dest, const BYTE *src, int count) 
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


void intToString (char *buf, char base, int number);
		

#endif /*SYSTEM_H_*/
