# Binary Decompilation & Analysis Index

This folder contains documentation and analysis for binaries, kernel modules, and scripts extracted from the Philips BDP 3280 flash dump.

---

## Progress Overview

| Target | Source Partition | Path / Component | Type | Status | Exploitation / Priority Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`init`** | `mtd15` (initrd) | `/init` | ELF Executable | 🟢 Complete | Easy to decompile |
| **`sbin/init`** | `mtd16` (rootfs_normal_1) | `/sbin/init` | ELF Executable | 🟢 Complete | SysV Init |
| **`Main firmware program`** | ? | `?` | ELF Executable | 🔴 Unanalyzed | **Very High Priority:** Might contain stock shell password |
| **`upg_prog`** | `mtd16` (rootfs_normal_1) | `/etc/init.d/upg_prog` | ELF Executable | 🔴 Unanalyzed | **Medium Priority:** Handles firmware updates & validation |

**Status Legend:**
- 🔴 **Unanalyzed:** Extracted, not yet opened in Ghidra/IDA.
- 🟡 **In Progress:** Base entry points identified, mapping functions.
- 🟢 **Complete:** Fully documented, core algorithms understood.
- ⚠️ **Vulnerable:** Confirmed vector for execution / shell access.

---

## Detailed Notes Index

### Userland Executables (`docs/decompiling/userland/`)
* [Initrd Init Process (`initrd_init.md`)](./userland/initrd_init.md) — Analysis of early boot `/init` process.


### Kernel Modules (`docs/decompiling/modules/`)
Nothing for now

---

## Decompilation Guidelines & Setup

* **Architecture:** ARM v6 / Little Endian (32-bit)
* **Compiler:** GCC 4.5.1
* **Ghidra Language ID:** `ARM:LE:32:v6` or `ARM:LE:32:default`
* **Base Load Address:** Standard 32-bit ELF (`0x00008000` default unless non-PIE kernel module)