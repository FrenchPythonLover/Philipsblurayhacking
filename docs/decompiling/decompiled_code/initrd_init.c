/* 
 * File: /init
 * Location: MTD15
 * Decompiled By: @FrenchPythonLover
 * Code verified for logic errors by Gemini 3.6 Flash
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/reboot.h>

// Global pointers 
char **g_argv;
char **g_envp;

// Helper functions identified from addresses:
// FUN_00008938 -> strcmp
// FUN_00008620 -> system_reboot
// FUN_00008614 -> system_shutdown
// FUN_00008a88 -> exit / cleanup
// FUN_00008a78 -> execve
// Struct definition reconstructed from pointer offsets (puVar2 + -2 moves back 2 words = 8 bytes)
typedef struct {
    char *unused1;
    char *unused2;
    char *unused3;
    char *unused4;
    char *unused5;
    char *unused6;
    char *unused7;
    char *unused8;
    char *module_format; // puVar2[8]
    char *module_name;   // puVar2[9]
} boot_module_entry_t;
typedef struct {
    char *module_name; // DAT_00009004
    char *module_args; // DAT_00009008
} boot_module_t;
typedef struct {
    const char *source;
    const char *target;
    const char *fstype;
    unsigned long flags;
    const void *data;
} static_mount_entry_t;

// Pointer boundaries in data segment
extern boot_module_entry_t g_boot_modules_end;   // 0x00009004
extern boot_module_entry_t g_boot_modules_start; // 0x00008fdc
int unload_modules() {
    int status;
    char cmd_buf[128];
    boot_module_entry_t *entry = &g_boot_modules_end;

    // Iterate backwards through the module array
    do {
        // Format command: e.g. "rmmod %s" or "modprobe -r %s"
        snprintf(cmd_buf, sizeof(cmd_buf), "%s", entry->module_format); // or using entry->module_name

        // Execute module removal
        status = execute_cmd(cmd_buf); // FUN_00008888
        if (status < 0) {
            dbg_printf("Func:%s() fail : %s:%d:%d\r\n", 
                       "boot_module_remove", 
                       "src/system/init_prog/init/src/boot_module.c", 
                       85, // 0x55
                       status);
            return status;
        }

        entry--; // Move to previous module entry in the table
    } while (entry != &g_boot_modules_start);

    return 0;
}

extern boot_module_t g_boot_modules[5]; // 0x28 bytes total / 8 bytes per entry = 5 entries

int boot_module_insert(void)
{
    int status;
    int offset = 0;
    char path_buf[128];

    // 1. Loop through all 5 boot modules, insert them, and remove the .ko file
    // (Modyles located in /boot @ mtd15, awaiting decompilation but aint critical)
    do {
        boot_module_t *mod = (boot_module_t *)((char *)g_boot_modules + offset);

        // Build path: "/boot/<module_name>.ko"
        snprintf(path_buf, sizeof(path_buf), "%s/%s.ko", "/boot", mod->module_name);

        // Load kernel module (insmod)
        status = insmod(path_buf, mod->module_args); // FUN_00008820
        if (status < 0) {
            dbg_printf("Func:%s() fail : %s:%d:%d\r\n", 
                       "boot_module_insert", 
                       "src/system/init_prog/init/src/boot_module.c", 
                       55, // 0x37
                       status);
            return status;
        }

        // Remove the loaded module file to free up RAM space
        status = unlink(path_buf); // FUN_00008b24
        if (status != 0) {
            dbg_printf("[rcS init]rm %s fail:%s\n", "file", path_buf);
        }

        offset += sizeof(boot_module_t); // Advance 8 bytes
    } while (offset != 0x28); // 5 modules * 8 bytes = 40 (0x28)

    // 2. Cleanup temporary early-boot directories
    status = unlink("/boot/sbin"); // FUN_00008b24
    if (status != 0) {
        dbg_printf("[rcS init]rm %s fail:%s\n", "dir", "/boot/sbin");
    }

    status = rmdir("/boot"); // FUN_00008b14
    if (status != 0) {
        dbg_printf("[rcS init]rm %s fail:%s\n", "dir", "/boot");
    }

    return 0;
}

extern static_mount_entry_t g_static_mounts[]; // 0x91e0 to 0x9230

int init_mount_filesystems(void)
{
    int ret;
    char *token;
    char *mtd_num_str;
    char *mount_point;
    char *fstype_str;
    char *status_str;
    int mtd_num;
    
    char dev_path[30];
    char target_path[30];
    
    void *file_handle;
    size_t file_size;

    // 1. Mount static essential filesystems (/proc, /sys, /dev, etc.)
    static_mount_entry_t *entry = g_static_mounts;
    do {
        if (entry == (static_mount_entry_t *)0x9230) {
            
            // 2. Open /proc/part_info_tbl to scan flash partitions
            file_handle = file_open_and_map("/proc/part_info_tbl", &file_size); // FUN_0000862c
            if (file_handle == NULL) {
                dbg_printf("[rcS-mount] open /proc/part_info_tbl fail!\n");
                return -2;
            }

            // Tokenize lines by '\n'
            token = strtok(file_handle, "\n"); // FUN_00008998
            
            while (1) {
                // Read columns from partition line
                mtd_num_str = strtok(NULL, " ");
                if (mtd_num_str == NULL) {
                    file_close_unmap(file_handle, file_size); // FUN_00008810
                    return 0; // Success end of partition table
                }

                strtok(NULL, " ");
                fstype_str = strtok(NULL, " ");
                strtok(NULL, " ");
                strtok(NULL, " ");
                mount_point = strtok(NULL, " ");
                strtok(NULL, " ");
                status_str = strtok(NULL, "\n");

                // Check if valid partition and contains "squash"
                if (fstype_str && mount_point && status_str && mtd_num_str &&
                    strcmp(fstype_str, "squash") == 0 && 
                    strcmp(status_str, "enabled") == 0) 
                {
                    // Clean trailing newline/spaces if necessary
                    int len = strlen(mount_point);
                    mount_point[len - 2] = '\0';

                    // Convert MTD number
                    mtd_num = atoi(mtd_num_str); // FUN_000088ec

                    // Format device node and mount point
                    if (snprintf(dev_path, sizeof(dev_path), "/dev/mtdblock%d", mtd_num) < 0 ||
                        snprintf(target_path, sizeof(target_path), "%s", mount_point) < 0) {
                        break;
                    }

                    // Mount SquashFS partition Read-Only (0x8000 = MS_RDONLY)
                    ret = mount(dev_path, target_path, "squashfs", MS_RDONLY, ""); // FUN_00008ad0
                    if (ret < 0) {
                        dbg_printf("[rcS-mount] mount fail : %s -> %s \n", dev_path, target_path);
                        file_close_unmap(file_handle, file_size);
                        return -4;
                    }
                }
            }

            file_close_unmap(file_handle, file_size);
            return -3;
        }

        // Execute static mount
        ret = mount(entry->source, entry->target, entry->fstype, entry->flags, entry->data);
        entry++;

    } while (ret == 0);

    dbg_printf("[rcS-mount]- mount fail: %s \n", entry->source);
    return -1;
}

/* Note: GHIDRA Does not seem very competent with registers and is missing some a lot of the time (in decompilers), might switch to hexrays. */
int system_shutdown(void)
{
    // Command 0x4321fedc
    return reboot(LINUX_REBOOT_CMD_POWER_OFF); 
}

int system_reboot(void)
{
    // Command 0x01234567
    return reboot(LINUX_REBOOT_CMD_RESTART); 
}

void system_exit(int status)
{
    _exit(status);
}

int main(int argc, char **argv)
{
    int ret;

    // Save process arguments and ENV
    g_argv = argv;
    g_envp = argv + argc + 1;

    if (argc > 1) {
        // If program is ran manually
        dbg_printf("argv[0] : %s, argv[1]: %s, argc : %d\n", argv[0], argv[1], argc);

        // Run early system validation / lock check
        ret = unload_modules(); // FUN_0000819c
        if (ret < 0) {
            // Line 64 (0x40) failure
            dbg_printf("Func:%s() exit : %s:%d:%d\r\n", "_start", "src/system/init_prog/init/src/init.c", 64, ret);
            while (1); // Halt system permanently
        }

        if (!strcmp(argv[1], "-shutdown") == 0) {
            system_shutdown(); // FUN_00008614
        } 
        else {
            // test if command is "-reboot"
            if (!strcmp(argv[1], "-reboot") != 0) {
                system_reboot(); // FUN_00008620
            } 
            else {
                dbg_printf("[rcSinit] Need more arguments !!! \n");
            }
        }

    cleanup_exit:
        system_exit(0); // FUN_00008a88(0)
        return 0;
    }


    // If program is ran by kernel (as init)
    // Stage 1 Hardware modules load
    ret = boot_module_insert(); // FUN_00008094
    if (ret < 0) {
        // Line 79 (0x4f) failure
        dbg_printf("Func:%s() exit : %s:%d:%d\r\n", "_start", "src/system/init_prog/init/src/init.c", 79, ret);
        while (1); // Halt system
    }

    // Stage 2 File System Setup
    ret = init_mount_filesystems(); // FUN_000083ac
    if (ret < 0) {
        // Line 81 (0x51) failure
        dbg_printf("Func:%s() exit : %s:%d:%d\r\n", "_start", "src/system/init_prog/init/src/init.c", 81, ret);
        while (1); // Halt system
    }

    // Stage 3: Hand off process execution to standard SysVinit (/sbin/init)
    ret = execve("/sbin/init", g_argv, g_envp); // FUN_00008a78
    
    if (ret == -1) {
        dbg_printf("[rcS init]exec /sbin/init error !\n");
    } 
    else if (ret >= 0) {
        goto cleanup_exit;
    }

    // Line 83 (0x53) failure — execve failed to replace process
    dbg_printf("Func:%s() exit : %s:%d:%d\r\n", "_start", "src/system/init_prog/init/src/init.c", 83, ret);
    while (1); // Bricked / Panic halt
}