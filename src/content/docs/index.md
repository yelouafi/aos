---
title: AOS Kernel Tutorials
description: English translations of a practical operating-system development tutorial series.
---

Learn how a small x86 operating-system kernel is built, one stage at a time.

This site publishes clear English editions of the original Arabic AOS tutorials.
Each lesson preserves the technical intent of the source while improving code
formatting, diagrams, and explanations for the web.

:::note[Published progressively]

Lessons appear here as soon as their translation and technical review are
complete. Follow the [translation status](./translation-status/) to see what is
published and what comes next.

:::

## Start learning

### [Lesson 00 - Writing Your First Boot Sector](./tutorials/00-writing-a-boot-sector/)

Start with the parts of a PC, how the CPU uses memory and registers, and how assembly
becomes machine code. Then build 512 bootable bytes that print your first message.

### [Lesson 01 - Loading the Kernel from a Floppy Disk](./tutorials/01-loading-the-kernel/)

Build on that boot sector by reading a tiny kernel from disk and transferring
execution to it with a far jump.

### [Lesson 02 - Entering 32-bit Protected Mode](./tutorials/02-entering-protected-mode/)

Build a Global Descriptor Table, switch the processor into protected mode, and
print directly through VGA text memory from a 32-bit kernel.

## About the archive

The repository also preserves the original PDFs, HTML exports, assembly files, and
later kernel source code. The website contains only the curated English tutorials;
historical source material remains available in the
[GitHub repository](https://github.com/yelouafi/aos).
