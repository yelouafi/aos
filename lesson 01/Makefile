NASM ?= nasm
QEMU ?= qemu-system-i386
DD ?= dd

BUILD_DIR := build
BOOT_BIN := $(BUILD_DIR)/boot.bin
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
FLOPPY_IMAGE := $(BUILD_DIR)/lesson-01.img

.PHONY: all image inspect run clean

all: $(BOOT_BIN) $(KERNEL_BIN)

image: $(FLOPPY_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): boot.s | $(BUILD_DIR)
	$(NASM) -f bin $< -o $@
	@test "$$(wc -c < $@ | tr -d '[:space:]')" -eq 512 || \
		{ echo "error: $@ must be exactly 512 bytes"; exit 1; }

$(KERNEL_BIN): kernel.s | $(BUILD_DIR)
	$(NASM) -f bin $< -o $@
	@test "$$(wc -c < $@ | tr -d '[:space:]')" -eq 512 || \
		{ echo "error: $@ must be exactly 512 bytes"; exit 1; }

$(FLOPPY_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(BOOT_BIN) of=$@ conv=notrunc
	$(DD) if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

inspect: $(BOOT_BIN) $(KERNEL_BIN) $(FLOPPY_IMAGE)
	@size="$$(wc -c < $(BOOT_BIN) | tr -d '[:space:]')"; \
		test "$$size" -eq 512 || \
		{ echo "error: $(BOOT_BIN) must be exactly 512 bytes"; exit 1; }; \
		printf "boot size: %s bytes\n" "$$size"
	@size="$$(wc -c < $(KERNEL_BIN) | tr -d '[:space:]')"; \
		test "$$size" -eq 512 || \
		{ echo "error: $(KERNEL_BIN) must be exactly 512 bytes"; exit 1; }; \
		printf "kernel size: %s bytes\n" "$$size"
	@size="$$(wc -c < $(FLOPPY_IMAGE) | tr -d '[:space:]')"; \
		test "$$size" -eq 1474560 || \
		{ echo "error: $(FLOPPY_IMAGE) must be exactly 1474560 bytes"; exit 1; }; \
		printf "image size: %s bytes\n" "$$size"
	@signature="$$(od -An -tx1 -j 510 -N 2 $(BOOT_BIN) | tr -d '[:space:]')"; \
		test "$$signature" = "55aa" || \
		{ echo "error: expected boot signature 55 aa, found $$signature"; exit 1; }; \
		printf "signature: 55 aa\n"

run: $(FLOPPY_IMAGE)
	@command -v $(QEMU) >/dev/null 2>&1 || \
		{ echo "error: $(QEMU) is not installed"; exit 1; }
	$(QEMU) -drive file=$(FLOPPY_IMAGE),format=raw,if=floppy -boot order=a

clean:
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(FLOPPY_IMAGE)
	-rmdir $(BUILD_DIR)
