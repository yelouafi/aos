#ifndef SYSTEM_H_
#define SYSTEM_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long long QWORD;

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

typedef struct multiboot_memory_map_entry
{
    DWORD size;
    DWORD address_low;
    DWORD address_high;
    DWORD length_low;
    DWORD length_high;
    DWORD type;
} multiboot_memory_map_entry;

typedef struct multiboot_info
{
    DWORD flags;
    DWORD mem_lower;
    DWORD mem_upper;
    DWORD boot_device;
    DWORD cmdline;
    DWORD mods_count;
    DWORD mods_addr;
    DWORD symbols[4];
    DWORD mmap_length;
    DWORD mmap_addr;
} multiboot_info;

inline unsigned char inb(unsigned short port)
{
    unsigned char value;
    asm volatile("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

inline void outb(unsigned short port, unsigned char data)
{
    asm volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

BYTE *memcpy(BYTE *destination, const BYTE *source, int count);
BYTE *memset(BYTE *destination, BYTE value, int count);
void intToString(char *buffer, char base, int number);
void GDTSetup();

#endif
