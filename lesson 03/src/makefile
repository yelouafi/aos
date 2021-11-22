CPPFLAGS = -c -fno-builtin 

.PHONY:  clean

OBJS = multiboot.o main.o video.o gdt.o


all: kernel.elf

kernel.elf: $(OBJS) link.lds
	#if you are on linux you can put directly  ld -T link.lds $(OBJS) -o kernel.elf
	ld -T link.lds $(OBJS) -o kernel.tmp
	#remove the following if you are using linux
	objcopy -O elf32-i386 kernel.tmp kernel.elf
	$(RM) kernel.tmp
	
clean:
	$(RM) $(OBJS)
	
	
%.o: %.cpp
	g++ $(CPPFLAGS) $< -o $@
	
video.cpp: system.h video.h
main.cpp: system.h

	
multiboot.o: multiboot.s
	#remove the -DLEADING_USCORE option if your compiler doesn't insert the '_' char 
	#at symbol'begining
	nasm -f elf  -DLEADING_USCORE $< -o $@