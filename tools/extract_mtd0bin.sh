#!/bin/bash

dd_part()  { dd if=mtd0.bin of=mtd${1}.bin bs=4096 skip=$(( $2 / 4096 )) count=$(( $3 / 4096 )) status=none; }


dd_part 1  0x200000  0x200000
dd_part 2  0x400000  0x200000
dd_part 3  0x600000  0x200000
dd_part 4  0x800000  0x100000
dd_part 5  0x900000  0x100000
dd_part 6  0xA00000  0x100000
dd_part 7  0xB00000  0x100000
dd_part 8  0xC00000  0x200000
dd_part 9  0xE00000  0x200000
dd_part 10 0x1000000 0x100000
dd_part 11 0x1100000 0x100000
dd_part 12 0x1200000 0x100000
dd_part 13 0x1100000 0x200000   # kernel_1
dd_part 14 0x1300000 0x200000   # kernel_2
dd_part 15 0x1500000 0x300000   # initrd_1
dd_part 16 0x1800000 0x1900000  # rootfs_normal_1
dd_part 17 0x3100000 0x1900000  # rootfs_enc_1
dd_part 18 0x4A00000 0x100000   # rootfs_it_1
dd_part 19 0x4B00000 0x100000   # rootfs_enc_it_1
dd_part 20 0x4C00000 0x200000   # initrd_2
dd_part 21 0x4E00000 0xC00000   # rootfs_normal_2
dd_part 22 0x5A00000 0x400000   # rootfs_enc_2
dd_part 23 0x5E00000 0x100000   # rootfs_it_2
dd_part 24 0x5F00000 0x100000   # rootfs_enc_it_2
dd_part 25 0x6000000 0x100000   # upg_status
dd_part 26 0x6100000 0x200000   # fast_init_logo
dd_part 27 0x6300000 0x100000   # fast_init_param
dd_part 28 0x6400000 0x100000   # log_raw
dd_part 29 0x6500000 0x1620000  # ubi0