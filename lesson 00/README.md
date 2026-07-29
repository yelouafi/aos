# Lesson 00 source

This directory contains the source code for
[Writing Your First Boot Sector](../docs/tutorials/00-writing-a-boot-sector/index.md).

## Build

Install NASM, then run:

```sh
make
make inspect
```

`make inspect` verifies that `build/boot.bin` is 512 bytes and ends with the boot
signature bytes `55 aa`.

## Create and run a floppy image

```sh
make image
make run
```

`make run` requires `qemu-system-i386`. Generated files are written under `build/`.
