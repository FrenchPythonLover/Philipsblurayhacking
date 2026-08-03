# Technical Infos, Notes...
## Index
 - [Specifications](#specs)
 - [Bootflow & Boot logs](#bootflow)
 - [U-Boot Commands](#available-u-boot-commands)
 - [U-Boot environment](#u-boot-environment)
 - [Partition Table](#partition-table)
 - [Dumps](#dumps)
## Specs
The BDP 3280 is a classic bluray embedded system, here are the specs:  
SoC: Mediatek MT8553 (Same core as RPI1).  
RAM: 384MB DDR3 SDRAM.   
Storage:  Toshiba TC58NVG0S3ETA00 - GBIT (128M × 8 BIT) CMOS NAND E²PROM (128M NAND). 
## Bootflow
see [here](https://github.com/FrenchPythonLover/Philipsblurayhacking/blob/main/bootflow.md)

Full boot logs are available [here](https://github.com/FrenchPythonLover/Philipsblurayhacking/blob/main/bootlog.txt)  

## Available U-Boot commands
```
?       - alias for 'help'
base    - print or set address offset
bdinfo  - print Board Info structure
bootm   - boot application image from memory
bootp   - boot image via network using BOOTP/TFTP protocol
chpart  - change active partition
cmp     - memory compare
cp      - memory copy
crc32   - checksum calculation
dis_log - log_enable
fatinfo - print information about filesystem
fatload - load binary file from a dos filesystem
fatls   - list files in a directory (default /)
go      - start application at address 'addr'
help    - print online help
loop    - infinite loop on address range
matchfile- match a file  in a directory
md      - memory display
mm      - memory modify (auto-incrementing address)
mt85xx_boot- mt85xx_boot   - boot command for mt85xx platform
mtdparts- define flash/nand partitions
mtest   - simple RAM read/write test
mw      - memory write (fill)
nand    - NAND sub-system
nboot   - boot from NAND device
nm      - memory modify (constant address)
ping    - send ICMP ECHO_REQUEST to network host
printenv- print environment variables
rarpboot- boot image via network using RARP/TFTP protocol
reset   - Perform RESET of the CPU
setenv  - set environment variables
tftpboot- boot image via network using TFTP protocol
ubi     - ubi commands
upg     - upg     - Upgrade image sub-system
usb     - usb     - BDP USB sub-system
usbboot - usbboot - boot from USB device
version - print monitor version
```
## U-Boot Environment
```
bootcmd=mt85xx_boot nand
autostart=yes
verify=no
bootdelay=0
baudrate=115200
ethaddr=00:0C:E7:00:00:00
ipaddr=172.22.82.167
serverip=172.21.69.23
gatewayip=172.22.82.254
netmask=255.255.255.0
loadaddr=0x2000000
upg_be_name=8560_linux_demo_dbg.bin
upg_fe_name=DVD.bin
upg_usb_path=UPG
upg_usb_fw_path=UPG
upg_bsm_be_name=UPG
upg_bsm_fe_name=UPG
stdin=serial
stdout=serial
stderr=serial
bootargs=root=/dev/ram0 rw initrd=0x16700000,0x00020173 console=ttyMT0 kgdboc=ttyMT0 mem=384M mt85xx_reserve=367M,17M drvmem=182M,136M BL_Ver=3847
Environment size: 533/8188 bytes
```

## Partition Table
| dev | size | eraseblock | name | part_type | role |
|---|---|---|---|---|---|
| mtd0 | 0x08000000 | 0x20000 | `NAND 128MiB 3.3V 8-bit` | Raw NAND | Whole NAND (Toshiba TC58NVG0S3ETA00) |
| mtd1 | 0x00200000 | 0x20000 | `boot_1` | U-Boot image | U-Boot |
| mtd2 | 0x00200000 | 0x20000 | `part_info_1` | Raw / U-Boot environment | U-Boot environment & MTD info |
| mtd3 | 0x00200000 | 0x20000 | `part_info_2` | Raw / U-Boot environment | U-Boot environment & MTD info (backup) |
| mtd4 | 0x00100000 | 0x20000 | `mac_addr_1` | Raw data | MAC address |
| mtd5 | 0x00100000 | 0x20000 | `mac_addr_2` | Raw data | MAC address (backup) |
| mtd6 | 0x00100000 | 0x20000 | `key_block_1` | Raw data | Crypto info |
| mtd7 | 0x00100000 | 0x20000 | `key_block_2` | Raw data | Crypto info (backup) |
| mtd8 | 0x00200000 | 0x20000 | `fe_bin_1` | Proprietary binary | Front-end binary (servo) |
| mtd9 | 0x00200000 | 0x20000 | `fe_bin_2` | Proprietary binary | Front-end binary (servo, backup) |
| mtd10 | 0x00100000 | 0x20000 | `fe_test_data` | Raw data | Factory test data |
| mtd11 | 0x00100000 | 0x20000 | `fe_parameters` | Raw data | FE calibration |
| mtd12 | 0x00100000 | 0x20000 | `fe_power_curve` | Raw data | Laser power curve |
| mtd13 | 0x00200000 | 0x20000 | `kernel_1` | uImage (Linux kernel) | Kernel |
| mtd14 | 0x00200000 | 0x20000 | `kernel_2` | uImage (Linux kernel) | Kernel (backup) |
| mtd15 | 0x00300000 | 0x20000 | `initrd_1` | gzip-compressed newc cpio archive | Initramfs — gzip'd cpio (newc), contains `etc/init.d/*`, `boot/*.ko` (bsp, decry, tz, drv_mem, dbg_utility) |
| mtd16 | 0x01900000 | 0x20000 | `rootfs_normal_1` | SquashFS v4 (gzip) | 1st SquashFS — `bin/lib/usr/res`. `lib/modules` on this rootfs is **empty**; presumed symlink to `rootfs_enc_1` by symmetry with mtd21/22 (**not yet verified**) |
| mtd17 | 0x01900000 | 0x20000 | `rootfs_enc_1` | SquashFS v4 (gzip) | Presumed: contains `lib/modules/<kver>/BDP/*.ko` for rootfs_normal_1 (**not yet tested**) |
| mtd18 | 0x00100000 | 0x20000 | `rootfs_it_1` | SquashFS v4 (gzip) | BD-IT |
| mtd19 | 0x00100000 | 0x20000 | `rootfs_enc_it_1` | SquashFS v4 (gzip) | Encrypted BD-IT |
| mtd20 | 0x00200000 | 0x20000 | `initrd_2` | gzip-compressed newc cpio archive | Initramfs (backup) |
| mtd21 | 0x00c00000 | 0x20000 | `rootfs_normal_2` | SquashFS v4 (gzip) | Backup SquashFS — `bin/lib/usr`. **Confirmed**: `lib/modules` is a symlink → `/mnt/rootfs_enc/lib/modules` |
| mtd22 | 0x00400000 | 0x20000 | `rootfs_enc_2` | SquashFS v4 (gzip) | Backup. **Confirmed**: contains `lib/modules/2.6.35/BDP/*.ko` — usbcore, mtk_hcd, usb-storage, fat/vfat/msdos, nls_*, isofs, udf, fuse, ntfs/tntfs, libata, sata_mt85xx_mod, cdrom, sr_mod, Wi-Fi drivers (`hst_*`, `if_ath_usb`, `hif_usb`, `musb_hdrc`), bdpdrv, cbagent, drvcli, drvuart, osai, pdown, rm, kmem, adf, ext3/jbd/mbcache |
| mtd23 | 0x00100000 | 0x20000 | `rootfs_it_2` | SquashFS v4 (gzip) | Backup BD-IT |
| mtd24 | 0x00100000 | 0x20000 | `rootfs_enc_it_2` | SquashFS v4 (gzip) | Backup encrypted BD-IT |
| mtd25 | 0x00100000 | 0x20000 | `upg_status` | Raw data | Upgrade status (read at boot) |
| mtd26 | 0x00200000 | 0x20000 | `fast_init_logo` | PNG image | Fast boot logo |
| mtd27 | 0x00100000 | 0x20000 | `fast_init_param` | Raw data | Fast boot parameters |
| mtd28 | 0x00100000 | 0x20000 | `log_raw` | Raw data | Logs |
| mtd29 | 0x01620000 | 0x20000 | `ubi0` | UBI / UBIFS | UBIFS — persistent storage. Volume `ubi_boot` mounted at `/mnt/ubi_boot`, holds `log`, `APDA`, `browser`, `acfg`, `misc_data`, `CPS_manager`, `cust_part_1` (symlinked from their usual root-level paths) |
**General notes:**
- All `_1`/`_2` partitions are primary/backup pairs (except `rootfs_normal_2`/`rootfs_enc_2`, which are smaller — likely a lightweight rescue image).
- `rootfs_normal_X` is incomplete standalone: `lib/modules` is a broken symlink that depends on `rootfs_enc_X` being mounted alongside it.
- `rootfs_enc` naming is misleading — not necessarily encrypted, more of an extension/modules partition for the main rootfs.

## Dumps
I successfully dumped the entire flash using uart. For more info about this topic, please see [dumping flash to uart](https://github.com/FrenchPythonLover/Philipsblurayhacking/blob/main/tutos.md#How-to-dump-the-flash-to-uart)  

The entire dump is available on the [Internet Archive](https://archive.org/details/mtd0_20260803).  