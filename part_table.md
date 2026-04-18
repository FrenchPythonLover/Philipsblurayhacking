# Partition table
dev:    size   erasesize  name role   
mtd0: 08000000 00020000 "NAND 128MiB 3,3V 8-bit" - -  
mtd1: 00200000 00020000 "boot_1" U-Boot  
mtd2: 00200000 00020000 "part_info_1" uboot env & mtd info  
mtd3: 00200000 00020000 "part_info_2" uboot env & mtd info  
mtd4: 00100000 00020000 "mac_addr_1" mac address  
mtd5: 00100000 00020000 "mac_addr_2" mac address  
mtd6: 00100000 00020000 "key_block_1" crypto info  
mtd7: 00100000 00020000 "key_block_2" crypto info  
mtd8: 00200000 00020000 "fe_bin_1" Front end binairy (servo)  
mtd9: 00200000 00020000 "fe_bin_2" Front end binairy (servo)  
mtd10: 00100000 00020000 "fe_test_data" Factory test data  
mtd11: 00100000 00020000 "fe_parameters" FE Calibration  
mtd12: 00100000 00020000 "fe_power_curve" Laser curb  
mtd13: 00200000 00020000 "kernel_1" Kernel  
mtd14: 00200000 00020000 "kernel_2" kernel  
mtd15: 00300000 00020000 "initrd_1" Initramfs   
mtd16: 01900000 00020000 "rootfs_normal_1" 1st SquashFS (contains lot of bin files, lib and /res)  
mtd17: 01900000 00020000 "rootfs_enc_1" Netflix/DRM ?  
mtd18: 00100000 00020000 "rootfs_it_1" BD-IT  
mtd19: 00100000 00020000 "rootfs_enc_it_1" Encrypted BD IT  
mtd20: 00200000 00020000 "initrd_2" BCK Initramfs ?  
mtd21: 00c00000 00020000 "rootfs_normal_2" BCK SquashFS?  
mtd22: 00400000 00020000 "rootfs_enc_2" BCK  
mtd23: 00100000 00020000 "rootfs_it_2" BCK  
mtd24: 00100000 00020000 "rootfs_enc_it_2" BCK  
mtd25: 00100000 00020000 "upg_status" Upgrade status (read at boot)  
mtd26: 00200000 00020000 "fast_init_logo" Fast boot logo  
mtd27: 00100000 00020000 "fast_init_param" Fast boot parameters  
mtd28: 00100000 00020000 "log_raw" Logs  
mtd29: 01620000 00020000 "ubi0" UBIFS -> Persistent storage (contains logs, settings..)  
