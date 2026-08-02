# Partition Table — Philips BDP3280

| dev | size | eraseblock | name | role |
|---|---|---|---|---|
| mtd0 | 0x08000000 | 0x20000 | `NAND 128MiB 3,3V 8-bit` | Whole NAND (Toshiba TC58NVG0S3ETA00) |
| mtd1 | 0x00200000 | 0x20000 | `boot_1` | U-Boot |
| mtd2 | 0x00200000 | 0x20000 | `part_info_1` | U-Boot env & MTD info |
| mtd3 | 0x00200000 | 0x20000 | `part_info_2` | U-Boot env & MTD info (backup) |
| mtd4 | 0x00100000 | 0x20000 | `mac_addr_1` | MAC address |
| mtd5 | 0x00100000 | 0x20000 | `mac_addr_2` | MAC address (backup) |
| mtd6 | 0x00100000 | 0x20000 | `key_block_1` | Crypto info |
| mtd7 | 0x00100000 | 0x20000 | `key_block_2` | Crypto info (backup) |
| mtd8 | 0x00200000 | 0x20000 | `fe_bin_1` | Front-end binary (servo) |
| mtd9 | 0x00200000 | 0x20000 | `fe_bin_2` | Front-end binary (servo, backup) |
| mtd10 | 0x00100000 | 0x20000 | `fe_test_data` | Factory test data |
| mtd11 | 0x00100000 | 0x20000 | `fe_parameters` | FE calibration |
| mtd12 | 0x00100000 | 0x20000 | `fe_power_curve` | Laser curve |
| mtd13 | 0x00200000 | 0x20000 | `kernel_1` | Kernel |
| mtd14 | 0x00200000 | 0x20000 | `kernel_2` | Kernel (backup) |
| mtd15 | 0x00300000 | 0x20000 | `initrd_1` | Initramfs — gzip'd cpio (newc), contains `etc/init.d/*`, `boot/*.ko` (bsp, decry, tz, drv_mem, dbg_utility) |
| mtd16 | 0x01900000 | 0x20000 | `rootfs_normal_1` | 1st SquashFS — `bin/lib/usr/res`. `lib/modules` on this rootfs is **empty**; presumed symlink to `rootfs_enc_1` by symmetry with mtd21/22 (**not yet verified**) |
| mtd17 | 0x01900000 | 0x20000 | `rootfs_enc_1` | Presumed: contains `lib/modules/<kver>/BDP/*.ko` for rootfs_normal_1 (**not yet tested**) |
| mtd18 | 0x00100000 | 0x20000 | `rootfs_it_1` | BD-IT |
| mtd19 | 0x00100000 | 0x20000 | `rootfs_enc_it_1` | Encrypted BD-IT |
| mtd20 | 0x00200000 | 0x20000 | `initrd_2` | Initramfs (backup) |
| mtd21 | 0x00c00000 | 0x20000 | `rootfs_normal_2` | Backup SquashFS — `bin/lib/usr`. **Confirmed**: `lib/modules` is a symlink → `/mnt/rootfs_enc/lib/modules` |
| mtd22 | 0x00400000 | 0x20000 | `rootfs_enc_2` | Backup. **Confirmed**: contains `lib/modules/2.6.35/BDP/*.ko` — usbcore, mtk_hcd, usb-storage, fat/vfat/msdos, nls_*, isofs, udf, fuse, ntfs/tntfs, libata, sata_mt85xx_mod, cdrom, sr_mod, wifi drivers (hst_*, if_ath_usb, hif_usb, musb_hdrc), bdpdrv, cbagent, drvcli, drvuart, osai, pdown, rm, kmem, adf, ext3/jbd/mbcache |
| mtd23 | 0x00100000 | 0x20000 | `rootfs_it_2` | Backup BD-IT |
| mtd24 | 0x00100000 | 0x20000 | `rootfs_enc_it_2` | Backup encrypted BD-IT |
| mtd25 | 0x00100000 | 0x20000 | `upg_status` | Upgrade status (read at boot) |
| mtd26 | 0x00200000 | 0x20000 | `fast_init_logo` | Fast boot logo |
| mtd27 | 0x00100000 | 0x20000 | `fast_init_param` | Fast boot parameters |
| mtd28 | 0x00100000 | 0x20000 | `log_raw` | Logs |
| mtd29 | 0x01620000 | 0x20000 | `ubi0` | UBIFS — persistent storage. Volume `ubi_boot` mounted at `/mnt/ubi_boot`, holds `log`, `APDA`, `browser`, `acfg`, `misc_data`, `CPS_manager`, `cust_part_1` (symlinked from their usual root-level paths) |

**⚠️ Open item:** active kernel `uname -r` reports `2.6.27-mt85xx`, but the `.ko` files found in `rootfs_enc_2` live under a folder named `2.6.35`. Version mismatch not yet resolved — `insmod` may either load fine (stale/unrelated folder name) or fail on ABI mismatch (`version magic`). Not tested yet.

**General notes:**
- All `_1`/`_2` partitions are primary/backup pairs (except `rootfs_normal_2`/`rootfs_enc_2`, which are smaller — likely a lightweight rescue image).
- `rootfs_normal_X` is incomplete standalone: `lib/modules` is a broken symlink that depends on `rootfs_enc_X` being mounted alongside it.
- `rootfs_enc` naming is misleading — not necessarily encrypted, more of an extension/modules partition for the main rootfs.
