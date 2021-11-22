CPPFLAGS = -c -fno-builtin -fno-rtti -fno-exceptions

.PHONY:  clean

OBJS = multiboot.o system.o Machine.o pic.o Kernel.o IntManager.o\
 main.o  FontMap.o video.o isrs.o \
 exception.o timer.o PFrameStack.o PhysMManager.o errors.o\
 Pager.o SelfBuffer.o VRegion.o KHeapStorage.o ObjCache.o \
 BootAllocator.o Malloc.o


all: kernel.elf

kernel.elf: $(OBJS) link.lds
	ld -T link.lds $(OBJS) -o kernel.tmp
	objcopy -O elf32-i386 kernel.tmp kernel.elf
	$(RM) kernel.tmp
	"C:\Program Files\WinImage\winimage.exe" floppy.flp /i kernel.elf /h
	 $(RM) $(OBJS)
	
clean:
	$(RM) $(OBJS)
	
	
%.o: ../src/%.cpp
	g++ $(CPPFLAGS)  $< -o $@

%.o: ../src/x86/%.cpp
	g++ $(CPPFLAGS)  $< -o $@
	
%.o: ../src/mm/%.cpp
	g++ $(CPPFLAGS)  $< -o $@
	
%.o: ../src/x86/%.s
	nasm -f elf -I../src/x86/include/ -DLEADING_USCORE $< -o $@