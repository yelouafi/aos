<div align="center">

<sub>AOS TUTORIALS · LESSON 01</sub>

<h1>Loading the Kernel from a Floppy Disk</h1>

<p><strong>English translation of the original Arabic lesson</strong></p>

<p>
  <kbd>16-bit x86</kbd>
  <kbd>BIOS</kbd>
  <kbd>NASM</kbd>
  <kbd>Real mode</kbd>
</p>

</div>

## What you will build

By the end of the lesson, you will have two 512-byte programs:

| Disk sector | Program | Loaded at | Responsibility |
|---:|---|---:|---|
| 1 | Boot sector | `0x07C0:0x0000` (physical `0x7C00`) | Ask the BIOS to read the kernel |
| 2 | Kernel | `0x0100:0x0000` (physical `0x1000`) | Print a message to the screen |

The lesson continues an earlier introduction to writing a boot sector. That lesson
stopped after printing a message from the boot sector. Here we take the next natural
step: load another program from disk and transfer control to it.

You should already be comfortable with basic 16-bit x86 assembly and with how a BIOS
loads a boot sector.

<p align="center">
  <img src="./assets/boot-flow.svg" alt="Boot flow: BIOS loads the boot sector, the boot sector reads the kernel, then execution jumps to the kernel." width="100%">
</p>

## 1. Starting point

The previous lesson produced a boot sector that displayed a line of text with BIOS
video service `INT 10h`, function `AH = 0Eh`.

<details>
<summary><strong>Show the previous lesson's boot-sector program</strong></summary>

```nasm
[BITS 16]

EntryPoint:
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF

    mov si, Salam
    call ShowMsg

k_loop:
    jmp k_loop

ShowMsg:
    push ax
    push bx

.loop_start:
    lodsb
    cmp al, 0
    je .loop_end
    mov ah, 0x0E
    mov bx, 0x0007
    int 0x10
    jmp .loop_start

.loop_end:
    pop bx
    pop ax
    ret

Salam db "Salam Alikom!", 13, 10, 0

times 510 - ($ - $$) db 0
dw 0xAA55
```

</details>

We will extend that program in two stages:

1. Use BIOS disk service `INT 13h` to load the kernel.
2. Write a tiny kernel that prints a second message.

## 2. How a disk is organized

Inside a floppy's plastic shell is a flexible disk coated with magnetic material,
similar in principle to magnetic tape. The drive encodes stored data as magnetic
patterns and decodes those patterns back into binary data when reading.

The drive contains:

- A motor that spins the disk at a fixed speed.
- Two read/write heads, one for each side of the floppy.
- A mechanical arm that moves the heads between the outer edge and the center.

Each disk surface is divided into concentric **tracks**. Each track is divided into
**sectors**, and every sector on a standard floppy holds the same amount of data.
For this lesson, one sector is **512 bytes**.

Hard disks follow the same general idea, but use rigid platters that can rotate much
faster. A hard disk normally contains several platter surfaces stacked above one
another. Tracks at the same radius on all surfaces form a **cylinder**.

<p align="center">
  <img src="./assets/disk-geometry.svg" alt="A disk surface divided into tracks and sectors, beside stacked platter surfaces forming a cylinder." width="100%">
</p>

### CHS addressing

The BIOS interface used in this lesson identifies a sector with three values:

1. **Cylinder** - the track position from the outer edge toward the center.
2. **Head** - the selected disk surface.
3. **Sector** - the selected sector within that track.

This scheme is called **CHS**, for cylinder-head-sector.

Data is ordered first by sector, then by head, then by cylinder. On a floppy, the
sequence begins with sector 1 on head 0 of cylinder 0. After the last sector on that
track, it continues on head 1 of the same cylinder, then moves to cylinder 1.

> [!IMPORTANT]
> Sector numbers begin at **1**. Cylinder and head numbers begin at **0**.

## 3. Reading sectors with BIOS INT 13h

In real mode, BIOS interrupt `INT 13h` provides disk services. Function `AH = 02h`
reads one or more sectors with CHS addressing.

### Inputs

| Register | Meaning in this lesson |
|---|---|
| `ES:BX` | Destination buffer in memory. `ES` is the segment and `BX` is the offset. |
| `AH` | Function number: `02h` means "read sectors." |
| `AL` | Number of sectors to read. |
| `CH` | Low eight bits of the cylinder number. We use cylinder `0`. |
| `CL` | Sector number in bits 0-5; the top two cylinder bits are stored in bits 6-7. We use sector `2`. |
| `DH` | Head number. We use head `0`. |
| `DL` | BIOS drive number. Preserve the value supplied by the BIOS instead of assuming drive `0`. |

### Outputs

| Carry flag | Result |
|---|---|
| `CF = 0` | The read succeeded and the requested data is in memory. |
| `CF = 1` | The read failed; `AH` contains a BIOS status code. |

At this point the disk layout is simple:

```text
+----------------------+----------------------+
| Sector 1             | Sector 2             |
| boot.bin (512 bytes) | kernel.bin (512 B)   |
+----------------------+----------------------+
```

The boot sector asks the BIOS to copy sector 2 into `ES:BX = 0x0100:0x0000`.

<table>
  <tr>
    <td><strong>Real-mode address rule</strong></td>
    <td><code>physical address = segment * 16 + offset</code></td>
  </tr>
  <tr>
    <td><strong>Kernel destination</strong></td>
    <td><code>0x0100 * 0x10 + 0x0000 = 0x1000</code></td>
  </tr>
</table>

## 4. Writing the boot sector

The BIOS places the boot drive number in `DL`. Save it before performing any disk
operation, but only **after** initializing `DS`; the BIOS does not guarantee the
initial value of `DS`.

The completed boot sector:

```nasm
; boot.s
[BITS 16]
[ORG 0]

EntryPoint:
    ; Establish a known stack and string direction.
    cli
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    cld
    sti

    ; This source uses offsets relative to segment 0x07C0.
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax

    ; The BIOS tells us which drive booted in DL.
    mov [bootdrv], dl

    mov si, bootMsg
    call ShowMsg

    ; Reset the boot drive before reading.
    mov dl, [bootdrv]
    xor ax, ax                    ; AH = 00h: reset disk system
    int 0x13
    jc readFail

    ; Read cylinder 0, head 0, sector 2 into 0x0100:0x0000.
    mov ax, 0x0100
    mov es, ax
    xor bx, bx
    mov ah, 0x02                 ; BIOS read-sector function
    mov al, 0x01                 ; read one sector
    mov ch, 0x00                 ; cylinder 0
    mov cl, 0x02                 ; sector 2
    mov dh, 0x00                 ; head 0
    mov dl, [bootdrv]
    int 0x13
    jc readFail

    ; CS:IP becomes 0x0100:0x0000, where the kernel was loaded.
    jmp 0x0100:0x0000

readFail:
    mov si, readError
    call ShowMsg

.hang:
    jmp .hang

ShowMsg:
    push ax
    push bx
    push si

.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E                 ; BIOS teletype output
    mov bx, 0x0007               ; page 0, light-gray foreground
    int 0x10
    jmp .loop

.done:
    pop si
    pop bx
    pop ax
    ret

bootdrv  db 0
bootMsg  db "Peace be upon you!", 13, 10, 0
readError db "Error: unable to read the kernel.", 13, 10, 0

times 510 - ($ - $$) db 0
dw 0xAA55
```

The final word `0xAA55` is the boot signature. Because x86 stores words in
little-endian order, it appears on disk as bytes `55 AA` at offsets 510 and 511.

### What the new code does

| Code | Purpose |
|---|---|
| `mov [bootdrv], dl` | Preserve the drive selected by the BIOS. |
| `AH = 00h`, `INT 13h` | Reset the disk service for that drive. |
| `ES:BX = 0x0100:0` | Select physical destination `0x1000`. |
| `AH = 02h`, `AL = 1` | Read exactly one sector. |
| `CH = 0`, `CL = 2`, `DH = 0` | Select cylinder 0, sector 2, head 0. |
| `jc readFail` | Check the carry flag and report a disk error. |
| `jmp 0x0100:0` | Perform a 16:16 far jump to the loaded kernel. |

## 5. Writing the kernel

The kernel begins at offset 0 within segment `0x0100`, so its source uses `ORG 0`.
It initializes the data segments and stack, prints a message with the same BIOS
video service, and then stops in an infinite loop.

```nasm
; kernel.s
[BITS 16]
[ORG 0]

start:
    cli

    ; The boot sector loaded this code at segment 0x0100.
    mov ax, 0x0100
    mov ds, ax
    mov es, ax

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF

    cld
    sti

    mov si, kernelMsg
    call ShowMsg

.hang:
    jmp .hang

ShowMsg:
    push ax
    push bx
    push si

.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    mov bx, 0x0007
    int 0x10
    jmp .loop

.done:
    pop si
    pop bx
    pop ax
    ret

kernelMsg db "Kernel loaded.", 13, 10, 0

; The boot sector reads one complete 512-byte sector.
times 512 - ($ - $$) db 0
```

Because both programs use `ShowMsg`, you can move it to a shared include file:

```nasm
%include "showmsg.inc"
```

For a first experiment, keeping the routine in both source files makes each listing
self-contained.

## 6. Assemble the programs

Use NASM's flat-binary output format:

```sh
nasm -f bin boot.s -o boot.bin
nasm -f bin kernel.s -o kernel.bin
```

Both files should be exactly 512 bytes:

```sh
wc -c boot.bin kernel.bin
```

Expected result:

```text
512 boot.bin
512 kernel.bin
```

Create a two-sector image:

```sh
cat boot.bin kernel.bin > aos.img
```

The resulting `aos.img` is 1,024 bytes. It contains the boot sector followed
immediately by the kernel sector and can be attached as a raw floppy image in an x86
emulator such as Bochs.

> [!WARNING]
> The next command writes directly to a device. Confirm the device name before
> running it. Selecting the wrong target can destroy data.

To write the image to floppy drive A on a Unix-like system:

```sh
dd if=aos.img of=/dev/fd0 bs=512 conv=fsync
```

### Windows

Create the same two-sector image with:

```bat
copy /b boot.bin+kernel.bin aos.img
```

They then use `ntrawrite` to copy the image to a physical floppy:

```bat
ntrawrite -f aos.img
```

When prompted for the diskette drive, enter `A`.

## 7. Result

The machine now follows this sequence:

1. The BIOS loads and runs the boot sector.
2. The boot sector prints `Peace be upon you!`.
3. BIOS disk service `INT 13h` copies sector 2 to physical address `0x1000`.
4. A far jump transfers control to `0x0100:0x0000`.
5. The kernel prints `Kernel loaded.` and remains in its infinite loop.

The kernel does very little, but the important boundary has been crossed: code is no
longer limited to the 512-byte boot sector. Later lessons can grow the loader, load a
larger kernel, and switch the x86 processor into protected mode.
