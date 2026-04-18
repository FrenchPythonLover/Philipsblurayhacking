# The Boot Flow of the Philips BDP 3280

## 0 - BROM (silicium)
Hardcoded in the SoC, it reads the first NAND blocks, verify signatures, launch the **preloader**. Invisible on logs.
## 1 - Preloader
The preloader calibrates the DRAM, reads the PIT (partition table).  
On logs:
```
preloader v6062
CFG = 0x1
[0x00092000] [0x703fc000]
Jul 10 2011 14:59:10SSC (+-5/1000, 30kHz)
DDR3, data rate: 1242 MHz.
B0:DQS(19~54), 36, 36
B1:DQS(12~46), 35, 29
0x7388=0x2e2e2e2e.
B0:Gw( 1~127), 127, 62
B1:Gw( 1~127), 127, 62
0x7388=0x3e3e.
A 256MB, x16
B0:DQS(19~49), 31, 34
B1:DQS(11~47), 37, 29
0x7388=0x2e2e2e2e.
B0:Gw( 1~127), 127, 61
B1:Gw( 1~127), 127, 62
0x7388=0x3e3d.
B 128MB, x16

        [0x40000de0]
        [0x400026fc]
        [0x40004fb8]
LD *
VFP
aMTD Found at 0x300000
```  
Then passes the `r_args_to_uboot` struct via shared mem:  
On logs:  
```
head sig   : 0xa0b0ead1   
kern addr  : 0x0d9fffc0   ← kernel already on RAM
initrd addr: 0x16700000
initrd size: 0x00020173 (128 KB)
```
## 2 - U-Boot (2009.08)
It reads everything from the preloader, then boots  
On logs:  
```
U-Boot 2009.08 (Jul 13 2011 - 15:42:50)
NXP B.V. - MT85XX SoC with ARM1176JZF-S
DRAM:  384 MB
NAND:  128 MiB
u-boot adaptive mtd mechanism applied.
[_i_find_part_tbl]Part tbl info passed from preloader
[_i_find_part_tbl] version is 1!!
[_i_get_upg_status_in_nand]upg status passed from preloader
Using default environment
In:    serial
Out:   serial
Err:   serial
r_args_to_uboot:
        head sig        : 0xa0b0ead1
        version : 1
        boot type       : 0
        dram ch1        : 0x10000000
        dram ch2        : 0x08000000
        kern addr       : 0x0d9fffc0
        initrd addr     : 0x16700000
        initrd size     : 0x00020173
enable bim two way write.
boot type:[0]
Bootloader version 3847
PHY reset fail
n>)�B�T@#I
Hit any key to stop autoboot:  0
## Booting kernel from Legacy Image at 0d9fffc0 ...
   Image Name:
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1479464 Bytes =  1.4 MB
   Load Address: 0da00000
   Entry Point:  0da00000
   Loading Kernel Image ... OK
OK
Starting kernel ...
```
## 3 - Linux 2.6.35
The kernel boots, with the default bootargs it mounts the initrd then executes the init.  
On logs:
```
Linux version 2.6.35 (mtk70389@mszslx14) (gcc version 4.5.1 (GCC) ) #18 SMP PREEMPT Wed Jul 13 15:41:31 CST 2011
CPU: ARMv6-compatible processor [410fb767] revision 7 (ARMv7), cr=00c5387f
CPU: VIPT nonaliasing data cache, VIPT nonaliasing instruction cache
Machine: mt85xx
Memory policy: ECC disabled, Data cache writeback
PERCPU: Embedded 6 pages/cpu @c065b000 s3776 r8192 d12608 u65536
pcpu-alloc: s3776 r8192 d12608 u65536 alloc=16*4096
pcpu-alloc: [0] 0
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 97536
Kernel command line: root=/dev/ram0 rw initrd=0x16700000,0x00020173 console=ttyMT0 kgdboc=ttyMT0 mem=384M mt85xx_reserve=367M,17M drvmem=182M,136M BL_Ver=3847
PID hash table entries: 2048 (order: 1, 8192 bytes)
Dentry cache hash table entries: 65536 (order: 6, 262144 bytes)
Inode-cache hash table entries: 32768 (order: 5, 131072 bytes)
Memory: 384MB = 384MB total
Memory: 229508k/229508k available, 163708k reserved, 0K highmem
Virtual kernel memory layout:
    vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    fixmap  : 0xfff00000 - 0xfffe0000   ( 896 kB)
    DMA     : 0xffc00000 - 0xffe00000   (   2 MB)
    vmalloc : 0xd8800000 - 0xf8000000   ( 504 MB)
    lowmem  : 0xc0000000 - 0xd8000000   ( 384 MB)
    modules : 0xbf000000 - 0xc0000000   (  16 MB)
      .init : 0xc0008000 - 0xc0022000   ( 104 kB)
      .text : 0xc0022000 - 0xc02a6000   (2576 kB)
      .data : 0xc02ba000 - 0xc02d30c0   ( 101 kB)
SLUB: Genslabs=11, HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
Hierarchical RCU implementation.
        RCU-based detection of stalled CPUs is disabled.
        Verbose stalled-CPUs detection is disabled.
NR_IRQS:128
Console: colour dummy device 80x30
console [ttyMT0] enabled
_mt85xx_uart_console_init:register console ok
Calibrating delay loop... 800.76 BogoMIPS (lpj=400384)
pid_max: default: 32768 minimum: 301
Mount-cache hash table entries: 512
CPU: Testing write buffer coherency: ok
Brought up 1 CPUs
SMP: Total of 1 processors activated (800.76 BogoMIPS).
NET: Registered protocol family 16
=== mt8560_init ===
IC Version = B
Serial: MT85XX driver $Revision: #25 $
Serial : add uart port 0
ttyMT0 at MMIO 0x7000c000 (irq = 14) is a MT85XX Serial
Serial: driver_register 0
bio: create slab <bio-0> at 0
SCSI subsystem initialized
NET: Registered protocol family 2
IP route cache hash table entries: 4096 (order: 2, 16384 bytes)
TCP established hash table entries: 16384 (order: 5, 131072 bytes)
TCP bind hash table entries: 16384 (order: 5, 196608 bytes)
TCP: Hash tables configured (established 16384 bind 16384)
TCP reno registered
UDP hash table entries: 256 (order: 1, 8192 bytes)
UDP-Lite hash table entries: 256 (order: 1, 8192 bytes)
NET: Registered protocol family 1
Unpacking initramfs...
Freeing initrd memory: 128K, pages: 32
k_gpio module init success
squashfs: version 4.0 (2009/01/31) Phillip Lougher
msgmni has been set to 448
Block layer SCSI generic (bsg) driver version 0.4 loaded (major 254)
io scheduler noop registered
io scheduler deadline registered (default)
kgdb: Registered I/O driver kgdboc.
loop: module loaded
[_get_static_buffer]start...
[_get_static_buffer]start_addr:0xd59c0000, alloc_sz:0x28000, buf_sz:0x25000
[sata_bltin_init_buffer]sata_int_ldr_fw:0xd5a00000, sata_hw_seg_buf:0xd59c0000, sata_mtd_read_buf:0xd59c0000
mt85xx_nand_alloc_memo1 address = 0xd5860000
mt85xx_nand_alloc_memo2 address = 0xd5860020
NAND device: Manufacturer ID: 0x98, Chip ID: 0xd1 (Toshiba NAND 128MiB 3,3V 8-bit)
kernel adaptive mtd mechanism applied.
mt85xx_part_tbl_init start.
mt85xx_part_tbl_init success, v1 pit found !
Creating 29 MTD partitions on "NAND 128MiB 3,3V 8-bit":
0x000000000000-0x000000200000 : "boot_1"
0x000000200000-0x000000400000 : "part_info_1"
0x000000400000-0x000000600000 : "part_info_2"
0x000000600000-0x000000700000 : "mac_addr_1"
0x000000700000-0x000000800000 : "mac_addr_2"
0x000000800000-0x000000900000 : "key_block_1"
0x000000900000-0x000000a00000 : "key_block_2"
0x000000a00000-0x000000c00000 : "fe_bin_1"
0x000000c00000-0x000000e00000 : "fe_bin_2"
0x000000e00000-0x000000f00000 : "fe_test_data"
0x000000f00000-0x000001000000 : "fe_parameters"
0x000001000000-0x000001100000 : "fe_power_curve"
0x000001100000-0x000001300000 : "kernel_1"
0x000001300000-0x000001500000 : "kernel_2"
0x000001500000-0x000001800000 : "initrd_1"
0x000001800000-0x000003100000 : "rootfs_normal_1"
0x000003100000-0x000004a00000 : "rootfs_enc_1"
0x000004a00000-0x000004b00000 : "rootfs_it_1"
0x000004b00000-0x000004c00000 : "rootfs_enc_it_1"
0x000004c00000-0x000004e00000 : "initrd_2"
0x000004e00000-0x000005a00000 : "rootfs_normal_2"
0x000005a00000-0x000005e00000 : "rootfs_enc_2"
0x000005e00000-0x000005f00000 : "rootfs_it_2"
0x000005f00000-0x000006000000 : "rootfs_enc_it_2"
0x000006000000-0x000006100000 : "upg_status"
0x000006100000-0x000006300000 : "fast_init_logo"
0x000006300000-0x000006400000 : "fast_init_param"
0x000006400000-0x000006500000 : "log_raw"
0x000006500000-0x000007b20000 : "ubi0"
star: eth_base=0xd882a000
star: pinmux_base=0xd882e000
star: pdwnc_base=0xd8832100
star: eth_pdwnc_base=0xd8836c00
star: eth_chksum_base=0xd883a000
star: ipll_base1=0xd883e700
star: ipll_base2=0xd8842300
star: bsp_base=0xd8846000
star: desc addr: 0xffc00000(virtual)/0x15877000(physical)
Star Ethernet: Internal Vitesse PHY
star: PHY addr = 0x0000
star: Star MAC init done
mice: PS/2 mouse device common for all mice
TCP cubic registered
NET: Registered protocol family 17
PM Init
VFP support v0.3: implementor 41 architecture 1 part 20 variant b rev 5
_mt85xx_late_console_init:register console ok
Freeing init memory: 104K, pages: 26
dbg_utility: module license 'Proprietary' taints kernel.
Disabling lock debugging due to kernel taint
/proc/bl_info created
0xcb600000-0xd0000000 : channel1 continuous
0xcb600000-0xcfde9f00 : channel1 dynamic
0xd0000000-0xd3e00000 : channel2 continuous
0xd3142000-0xd3e00000 : channel2 dynamic
[nand part read-1], u4DevId = 25,                   u8Offset = 0x0, u4MemPtr = 0xd88bb000, u4MemLen = 0xac
[get_upg_status] success.
hard sector size is 512
devblksize is 4096
hard sector size is 512
devblksize is 4096
hard sector size is 512
devblksize is 4096
hard sector size is 512
devblksize is 4096
INIT: version 2.88 booting
```
Then, the rest is just the init and its different stages.
