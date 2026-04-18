# Guides

## How to get UART
The UART on the blu ray player is 3.3v, here is the pinout:
  - RX: TP100
  - TX: TP200
  - GND: Any metal pad except the heatsink and some parts of the PSU (dont touch the PSU.)

Just connect the two pads on a FTDI or other serial to usb converter (that is 3.3v) and you're all set.  
## How to get a U-Boot Shell
On this device the bootloader is U-Boot. It can be accessed by:
  1. **first** spam enter on the serial terminal
  2. **then**, and **only then**, power on the device.
When you get a shell, it'll look like this:  
```
 0Hit any key to stop autoboot:  0
mt8560_base #
mt8560_base #
mt8560_base #
mt8560_base #
```

If your Serial Mon. looks like this, then **congrats**, you're in (partially) !  
## Get a linux shell
Getting a linux shell is actually pretty simple once you get access to the bootloader, you just need to modify the kernel bootargs, then load the kernel.  
First of all, let's modify the cmdline to:
  - Do not set the root as the initrd (it automatically launches init.)
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
Here you go, you have a shell. It is very restricted as there isn't any devtmpfs, proc or tmpfs mounted.
