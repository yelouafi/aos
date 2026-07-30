NASM ?= nasm

BUILD_DIR := build
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
OBJECTS := \
	$(BUILD_DIR)/multiboot.o \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/video.o \
	$(BUILD_DIR)/gdt.o \
	$(BUILD_DIR)/isr.o \
	$(BUILD_DIR)/isrs.o \
	$(BUILD_DIR)/exception.o \
	$(BUILD_DIR)/pic.o \
	$(BUILD_DIR)/timer.o

HOST_OS := $(shell uname -s)

ifeq ($(HOST_OS),Darwin)
CXX := clang++
CXX_TARGET := --target=i686-none-elf
RUST_SYSROOT := $(shell rustc --print sysroot 2>/dev/null)
RUST_HOST := $(shell rustc -vV 2>/dev/null | sed -n 's/^host: //p')
ELF_LD := $(RUST_SYSROOT)/lib/rustlib/$(RUST_HOST)/bin/rust-lld
ELF_LD_FLAGS := -flavor gnu -m elf_i386
else
CXX := g++
CXX_TARGET := -m32
ELF_LD := ld
ELF_LD_FLAGS := -m elf_i386
endif

CXXFLAGS := \
	$(CXX_TARGET) \
	-std=gnu++98 \
	-ffreestanding \
	-fno-builtin \
	-fno-exceptions \
	-fno-rtti \
	-fno-pic \
	-fno-pie \
	-fno-stack-protector \
	-fno-threadsafe-statics \
	-Wall \
	-Wextra

.PHONY: all kernel inspect clean

all: $(KERNEL_ELF)

kernel: $(KERNEL_ELF)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/multiboot.o: src/multiboot.s | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/isrs.o: src/isrs.s src/asm.inc | $(BUILD_DIR)
	$(NASM) -f elf32 -Isrc/ $< -o $@

$(BUILD_DIR)/%.o: src/%.cpp src/system.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/video.o: src/video.h
$(BUILD_DIR)/main.o: src/video.h src/interrupt.h src/exception.h src/timer.h src/pic.h src/isr.h
$(BUILD_DIR)/isr.o: src/interrupt.h
$(BUILD_DIR)/exception.o: src/exception.h src/interrupt.h src/video.h
$(BUILD_DIR)/timer.o: src/timer.h src/interrupt.h src/video.h src/pic.h
$(BUILD_DIR)/pic.o: src/pic.h

$(KERNEL_ELF): $(OBJECTS) src/link.lds
	@command -v "$(ELF_LD)" >/dev/null 2>&1 || test -x "$(ELF_LD)" || \
		{ echo "error: an ELF32 linker is required (GNU ld, ld.lld, or rust-lld)"; exit 1; }
	$(ELF_LD) $(ELF_LD_FLAGS) -T src/link.lds -o $@ $(OBJECTS)

inspect: $(KERNEL_ELF)
	@format="$$(file $(KERNEL_ELF))"; \
		printf "kernel: %s bytes\n" "$$(wc -c < $(KERNEL_ELF) | tr -d '[:space:]')"; \
		printf "format: %s\n" "$$format"; \
		echo "$$format" | grep -q "ELF 32-bit.*Intel 80386" || \
		{ echo "error: $(KERNEL_ELF) must be a 32-bit i386 ELF"; exit 1; }
	@od -An -tx4 -N 8192 $(KERNEL_ELF) | grep -q "1badb002" || \
		{ echo "error: Multiboot header is missing from the first 8192 bytes"; exit 1; }
	@printf "multiboot header: 1badb002\n"

clean:
	rm -f $(OBJECTS) $(KERNEL_ELF)
	-rmdir $(BUILD_DIR)
