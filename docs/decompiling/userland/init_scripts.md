# Analysis: /etc/init.d Startup Scripts

- **Location:** `mtd15` (initrd) 
- **Execution Engine:** Standard Sysv-init `/sbin/init @ mtd16`

---

## Script Execution Order & Map

| Order | Script Path | Source MTD | Purpose | Key Sub-processes / Notes|
| :---: | :--- | :---: | :--- | :--- |
| **1** | `/etc/inittab` | `mtd15` | Defines system runlevels & console | Spawns `rcS`, sets up `ttyMT0` |
| **2** | `/etc/init.d/rcSinit` | `mtd15` | Preps kernel modules then executes `rcS` | Practically twin of /init but different outcomes |
| **3** | `/etc/init.d/rcS` | `mtd15` | Master dispatcher | Runs `mtd_init` then `lircd_simulator in BG` |
| **4** | `/etc/init.d/mtd_init.sh` | `mtd15` | Mounter for complementary partitions | Mounts mtd29 (UBIFS, Persistent). Script suggests earlier models used YAFFS2. |
| **5** | `/etc/init.d/rc5` | `mtd15` | Main "Program Loop": Launches network daemons & bdpprog (main program) & when killed kills the IR Handler. |
---

