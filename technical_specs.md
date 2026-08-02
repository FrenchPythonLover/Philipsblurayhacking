# Technical specifications
The BDP 3280 is a very robust system, here are the specs:  
SoC: Mediatek MT8553.  
DRAM: 3 GB.   
Storage:  Toshiba TC58NVG0S3ETA00 - GBIT (128M × 8 BIT) CMOS NAND E²PROM. 

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
