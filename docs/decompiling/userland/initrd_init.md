# Component Analysis: init

- **File Path:** `/init`
- **Source Partition:** `mtd15`

## Purpose
An early custom made init, executing before SYSVINIT (/sbin/init)

## Program explanation & Flow
The program executes using two paths;
 - User manually executed the program
 - Kernel executed the program (as init process)
### Manual path
The init first unload essential modules (located in /boot @ mtd15);
It then verifies the arguments, and accordingly:
 - Reboot ("-reboot")
 - Shutdown ("-shutdown")
As simple as that.
### Init (Kernel) Path
Opposed to the manual path, here the init first loads any essential modules @ /boot, like bsp.ko, tz.ko or decry.ko.

It then mounts the following:
 - `rootfs_normal_1` (`/dev/mtdblock16`) to `/` 
 - `rootfs_enc_1` (`/dev/mtdblock17`) to `/mnt/rootfs_enc`
 - `rootfs_it_1` (`/dev/mtdblock18`) to `/mnt/rootfs_it` 
 - `rootfs_enc_it_1` (`/dev/mtdblock19`) to `/mnt/rootfs_enc_it` 
 - `ubi0:ubi_boot` (`UBIFS` volume) to `/mnt/ubi_boot`

And then executes /sbin/init (from rootfs_normal_1) which is standard sysvinit, so it loads every scripts in /etc/init.d/

## Interesting findings & Notes
### Notes
Ghidra doesn't like ARM registers in its decompiler. It openly struggles with syscalls, _entry and others.

It was easy and hard to decompile this binary, easy because there was debug symbols & messages, which helped to get context and to deduct the functions faster, but hard because ghidra was missing on critical function arguments sometimes.