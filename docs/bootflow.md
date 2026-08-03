# Philips BDP 3280 Bootflow
[ Hardware Power On / ROM ]
           │
           ▼
[ U-Boot ] (mtd1)
   │  Loads kernel from mtd13 & initrd from mtd15 into RAM
   │  Passes bootargs: "root=/dev/ram0 rw initrd=0x16700000,... console=ttyMT0"
   ▼
[ Linux Kernel ] (mtd13)
   │  Initializes CPU, RAM, serial console (ttyMT0)
   │  Mounts initrd CPIO archive as root filesystem /
   ▼
[ Early Init ] (/init on mtd15)
   │  Mounts /proc, /sys, /dev/shm, /dev/pts
   │  Loads early modules from /boot/*.ko (drv_mem, decry, tz, dbg_utility)
   │  Executes /sbin/init
   ▼
[ BusyBox init / System Init ] (/sbin/init on mtd16)
   │  Parses /etc/inittab
   │  Runs system initialization script: /etc/init.d/rcS
   ▼
[ Master Boot Script ] (/etc/init.d/rcS)
   │  Calls /etc/init.d/rcSinit
   │  Executes /etc/init.d/mtd_init.sh (mounts SquashFS rootfs_normal & rootfs_enc)
   │  Calls /etc/init.d/rc5
   ▼
[ Master app boot script ] (/etc/init.d/rc5)
   │  Spawns main Blu-ray application stack, network daemons...