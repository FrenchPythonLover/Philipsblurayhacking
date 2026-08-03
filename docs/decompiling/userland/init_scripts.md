# Analysis: /etc/init.d Startup Scripts

- **Location:** `mtd15` (initrd) & `mtd16` (rootfs_normal) `/etc/init.d/`
- **Execution Engine:** Standard Sysv-init `/sbin/init`

---

## Script Execution Order & Map

| Order | Script Path | Source MTD | Purpose | Key Sub-processes |
| :---: | :--- | :---: | :--- | :--- |
| **1** | `/etc/inittab` | `mtd15` | Defines system runlevels & console | Spawns `rcS`, sets up `ttyMT0` |
| **2** | `/etc/init.d/rcS` | `mtd15` | Master boot sequence dispatcher | Runs `rcSinit`, `mtd_init.sh` |
| **3** | `/etc/init.d/rcSinit` | `mtd15` | Network & basic directory setup | Preps `/var`, `/tmp`, network interfaces |
| **4** | `/etc/init.d/mtd_init.sh` | `mtd15` | Mounts system flash partitions | Mounts `mtd16` (rootfs) & `mtd17` (rootfs_enc) |
| **5** | `/etc/init.d/usb_init.sh` | `mtd15` | USB host & wireless stack | Invokes `/etc/wifi.script/*` drivers |
| **6** | `/etc/init.d/upg_prog.sh` | `mtd16` | Firmware recovery & upgrade check | Runs `/etc/init.d/upg_prog` binary |

---

## Script Deep-Dives
None for now
