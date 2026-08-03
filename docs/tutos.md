# Guides
## Index
  - [Get UART Access](#how-to-get-uart)
  - [Access to a U-Boot Shell](#how-to-get-a-u-boot-shell)
  - [Boot into a rooted linux shell](#get-a-linux-shell)
  - [Dump firmware manually with UART](#how-to-dump-the-flash-to-uart)
## How to get UART
The UART on the blu ray player is 3.3v, here is the pinout:
  - RX: TP100
  - TX: TP200
  - GND: Any metal pad except the heatsink and some parts of the PSU (dont touch the PSU.)  
  *Note: Both testpoints are located above the hynix RAM & much behind the RCA connectors*
Just connect the two pads on a FTDI or other serial to usb converter (that is 3.3v) and you're all set.  
## How to get a U-Boot Shell
On this device the bootloader is U-Boot. It can be accessed by:
  1. **first** spam enter on the serial terminal
  2. **then**, and **only then**, power on the device.
When you get a shell, it'll look like this:  
```
Hit any key to stop autoboot:  0
mt8560_base #
```

If your Serial Mon. looks like this, then **congrats**, you're in (partially) !  
## Get a linux shell
Getting a linux shell is actually pretty simple once you get access to the bootloader, you just need to modify the kernel bootargs, then load the kernel.  
First of all, let's modify the cmdline to:
  - Not set the root as the initrd (it automatically launches init.)
  - Mount the mtdblock16 (the first SquashFS filesystem (with /res, a lot of programs and with sh) as the root.
  - Set the init to /bin/sh (using **init=**):
All of this can be done using this command to run inside the Uboot bootloader shell:
```
setenv bootargs root=/dev/mtdblock16 rootfstype=squashfs ro console=ttyMT0 mem=384M mt85xx_reserve=367M,17M drvmem=182M,136M BL_Ver=3847 init=/bin/sh
```
From here, we can use the proprietary command `mt85xx_boot nand` or directly load the kernel at address `0x0d9fffc0` using `bootm 0x0d9fffc0` (They boot are looking to do the same thing.)
After boot, it should look like this:
```
devblksize is 4096
VFS: Mounted root (squashfs filesystem) readonly on device 31:16.
Freeing init memory: 104K, pages: 26
sh-3.2#
```
Here you go, you have a shell. It is very restricted as there isn't any devtmpfs, proc or tmpfs mounted, to mount such services, refer to [Mounting on restricted shell](#Mounting-on-restricted-shells).
## Mounting on restricted shells
By default, the filesystem where we're located (rootfs-normal-1) isn't complete, it has been designed to be mounted from the initrd (Still analysing the init scripts). It also is read-only.

To get access to normal linux devices, cpuinfo, partitions, usb devices (if we load the driver on rootfs_enc) on a read-only partition, well have to find an empty folder and put whats called a TMPFS, it's simply a filesystem running on RAM, where its not readonly.

In the tmpfs well put our sysfs (the /sys on desktop) and procfs (/proc on desktop) we will manually populate /dev for our need (map some partitions for example).

The only available empty folder for our setup is `/res`.

Paste those commands in your terminal to mount everything:

```sh
mount -t tmpfs tmpfs /res

mkdir /res/dev
mkdir /res/proc
mkdir /res/sys
mkdir /res/mnt

mount -t proc proc /res/proc
mount -t sysfs sysfs /res/sys
```
Then, let's populate /res/dev (adding our partitions)
Character devices (`/dev/mtdX`) are used for raw flash access for example to dump the flash:

```sh
mknod /res/dev/mtd15 c 90 30
```

The second number is simply:

```
partition_number × 2
```

For example:

| Partition | Minor |
|-----------|------:|
| mtd0 | 0 |
| mtd1 | 2 |
| mtd2 | 4 |
| mtd15 | 30 |

To mount a filesystem stored on an MTD partition, you'll also need the corresponding **block device**:
For example, for `mtd15` (initrd), you'll have to write this.

```sh
mknod /res/dev/mtdblock15 b 31 15
mount -t squashfs /res/dev/mtdblock15 /res/mnt
```

The `mtdblock` device is what `mount` expects.

To fully populate /res/dev with block and non block devices without writing by hand the required `mknod` commands for each mtd device, we'll use a loop to speed up the process:
```
for i in {0..29}; do
    mknod /res/dev/mtd$i c 90 $((i*2))
    mknod /res/dev/mtdblock$i b 31 $i
done
```
When done, **congratulations, you now have a usable linux environment (While not very enjoyable) !**
## How to dump the flsh to uart
TODO: Write this section, but it involves using a formatted hexdump command in conjuction with my hextobin python script in the tools directory, and 921600 baudrate. If someone really wants that (why? the dump is uploaded, see technical specs), create a, issue, and this'll be done ASAP.