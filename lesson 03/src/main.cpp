#include "video.h"

void __main() {}
void _alloca() {}

BYTE *memcpy(BYTE *destination, const BYTE *source, int count)
{
    for (int index = 0; index < count; ++index)
        destination[index] = source[index];
    return destination;
}

BYTE *memset(BYTE *destination, BYTE value, int count)
{
    for (int index = 0; index < count; ++index)
        destination[index] = value;
    return destination;
}

void intToString(char *buffer, char base, int number)
{
    static char digits[] = "0123456789abcdef";
    char *cursor = buffer;
    unsigned long unsignedNumber = number;
    int divisor = 10;

    if (base == 'd' && number < 0)
    {
        *cursor++ = '-';
        ++buffer;
        unsignedNumber = -number;
    }
    else if (base == 'x')
    {
        divisor = 16;
        *cursor++ = '0';
        *cursor++ = 'x';
        buffer += 2;
    }
    else if (base != 'u')
    {
        *buffer = 0;
    }

    do
    {
        long remainder = unsignedNumber % divisor;
        *cursor++ = digits[remainder];
    } while (unsignedNumber /= divisor);

    *cursor = 0;

    char *head = buffer;
    char *tail = cursor - 1;
    while (head < tail)
    {
        char temporary = *head;
        *head++ = *tail;
        *tail-- = temporary;
    }
}

static DWORD usableMemoryKilobytes(const multiboot_info *information)
{
    DWORD total = 0;
    DWORD cursor = information->mmap_addr;
    DWORD end = cursor + information->mmap_length;

    while (cursor < end)
    {
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

extern "C" void kmain(DWORD magic, multiboot_info *information)
{
    GDTSetup();

    Video video;
    const char *devices[] = { "floppy A", "hard disk" };
    video.clear();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        video.printf("Assalamou Alaikoum without Multiboot\n");
        video.printf("Invalid magic number %x\n", magic);
    }
    else
    {
        video.printf("Assalamou Alaikoum from Multiboot\n");

        if (information->flags & 1)
        {
            video.printf("Lower memory = %uKB\n", information->mem_lower);
            video.printf("Upper memory = %uKB\n", information->mem_upper);
        }
        else if (information->flags & (1 << 6))
        {
            video.printf(
                "Usable memory = %uKB (memory map)\n",
                usableMemoryKilobytes(information));
        }
        else
        {
            video.printf("Memory information = not provided\n");
        }

        if (information->flags & 2)
        {
            char bootDevice = (information->boot_device >> 24) & 0xF;
            const char *device = bootDevice ? devices[1] : devices[0];
            video.printf("Boot device = %s\n", device);
        }
        else
        {
            video.printf("Boot device = not provided\n");
        }
    }

    while (1)
        asm volatile("cli; hlt");
}
