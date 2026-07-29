# Lesson 02 editorial notes

These notes record corrections made while preparing the English web edition of
**Entering 32-bit Protected Mode**. They are intentionally kept outside the
tutorial.

| Source issue | English-edition treatment |
|---|---|
| The boot drive was hard-coded as drive `0`. | Preserve the drive number supplied by the BIOS and reload it for disk operations. |
| Disk reset and read failures were ignored. | Check the BIOS carry flag and display a clear error message. |
| The string loop used `LODSB` without establishing the direction flag. | Execute `CLD` during startup. |
| A local loop label lacked its colon, producing a NASM warning. | Use explicit, consistently terminated labels. |
| `INC BX` followed a BIOS character call even though `BX` was replaced on every iteration. | Remove the ineffective instruction. |
| Only `AX` was ORed before writing all of `EAX` back to `CR0`. | Set `CR0.PE` with `OR EAX, 1`. |
| A near jump and several setup instructions appeared before `CS` was reloaded. | Use a far jump immediately after enabling protected mode, then assemble the target as 32-bit code. |
| The GDT set the available and accessed bits despite not using them. | Use conventional flat code/data descriptors with access bytes `0x9A` and `0x92`, and flags byte `0xCF`. |
| The kernel message read “proected mode.” | Correct it to “Protected mode is active.” |
| The video attribute comment described `0x07` incorrectly. | Describe it as light gray text on a black background. |
| The final busy loop ran continuously. | Use `CLI`/`HLT` with a defensive loop. |
