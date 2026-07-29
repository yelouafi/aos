---
title: AOS Kernel Tutorials
description: English translations of a practical operating-system development tutorial series.
---

# AOS Kernel Tutorials

Learn how a small x86 operating-system kernel is built, one stage at a time.

This site publishes clear English editions of the original Arabic AOS tutorials.
Each lesson preserves the technical intent of the source while improving code
formatting, diagrams, and explanations for the web.

!!! info "Published progressively"

    Lessons appear here as soon as their translation and technical review are
    complete. Follow the [translation status](translation-status.md) to see what is
    published and what comes next.

## Start learning

### [Lesson 00 - Writing Your First Boot Sector](tutorials/00-writing-a-boot-sector/index.md)

Write 512 bytes of 16-bit x86 machine code that the BIOS can load directly, then use
a BIOS video service to print your first message.

### [Lesson 01 - Loading the Kernel from a Floppy Disk](tutorials/01-loading-the-kernel/index.md)

Build on that boot sector by reading a tiny kernel from disk and transferring
execution to it with a far jump.

## About the archive

The repository also preserves the original PDFs, HTML exports, assembly files, and
later kernel source code. The website contains only the curated English tutorials;
historical source material remains available in the
[GitHub repository](https://github.com/yelouafi/aos).
