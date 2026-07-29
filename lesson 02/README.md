# Lesson 02 source

This directory contains the source code for
[Entering 32-bit Protected Mode](../docs/tutorials/02-entering-protected-mode/index.md).

## Build

Install NASM, then run:

```sh
make
make inspect
```

## Create and run a floppy image

```sh
make image
make run
```

`make run` requires `qemu-system-i386`. Generated files are written under `build/`.
