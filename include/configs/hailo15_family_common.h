/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019-2024 Hailo Technologies Ltd. All rights reserved.  
 *
 * Configuration for Hailo15.
 */

#ifndef __HAILO15_FAMILY_COMMON_H
#define __HAILO15_FAMILY_COMMON_H

/* Physical Memory Map */
#define PHYS_SDRAM_1 (0x80000000) /* SDRAM Bank #1 */

/* additions for new relocation code */
#define CONFIG_SYS_SDRAM_BASE (PHYS_SDRAM_1)
#define CONFIG_SYS_INIT_SP_ADDR (0x83000000)

/* GICv3 */
#define GICD_BASE (0x60600000)
#define GICR_BASE (GICD_BASE + 0x80000)

#define CPU_RELEASE_ADDR (0x800FF000)

#define CONFIG_SYS_MAX_FLASH_BANKS (1)
#define COUNTER_FREQUENCY (25000000) // based on xtal clock - 25Mhz
#define CONFIG_SYS_BOOTM_LEN (0x8000000)

#define CONFIG_SPL_BSS_START_ADDR	0x82000000
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME	"u-boot-tfa.itb"
// Dummy value
#define CONFIG_SYS_UBOOT_BASE 0

#ifdef CONFIG_HAILO15_SWUPDATE

#ifndef SWUPDATE_MMC_INDEX
#define SWUPDATE_MMC_INDEX "0"
#endif /* SWUPDATE_MMC_INDEX */

#define SWUPDATE_EXTRA_ENV_SETTINGS \
    "swupdate_ram_addr=0xB0000000\0" \
    "load_swupdate_image_from_mmc=" UNNEEDED_MMCINFO_HACK " fatload mmc ${device_num}:${mmc_boot_partition} ${swupdate_ram_addr} swupdate-image-${machine_name}.ext4.gz && setenv swupdate_filesize ${filesize}\0" \
    "write_swupdate_image_to_mmc=" UNNEEDED_MMCINFO_HACK " fatwrite mmc ${device_num}:${mmc_boot_partition} ${swupdate_ram_addr} swupdate-image-${machine_name}.ext4.gz ${filesize}\0" \
    "download_swupdate_image_to_ram= tftpboot ${swupdate_ram_addr} swupdate-image-${machine_name}.ext4.gz && setenv swupdate_filesize ${filesize}\0" \
    "swupdate_server_udp_logging_port=12345\0" \
    "setup_swupdate_update_filename=setenv swupdate_update_filename hailo-update-image-${machine_name}.swu\0" \
    "swupdate_update_modes=init-partitions-single,init-scu-bl,copy-a\0" \
    "bootargs_swupdate=run setup_swupdate_update_filename && setenv bootargs ${bootargs} SWUPDATE_SERVER_IP=${serverip} SWUPDATE_SERVER_UDP_LOGGING_PORT=${swupdate_server_udp_logging_port} SWUPDATE_UPDATE_FILENAME=${swupdate_update_filename} SWUPDATE_UPDATE_MODES=${swupdate_update_modes}\0" \
    "swupdate_load_mmc=run set_mmc" SWUPDATE_MMC_INDEX "_device_num && run load_fitimage_from_mmc && run load_swupdate_image_from_mmc\0" \
    "swupdate_load_tftp=run download_fitimage_to_ram && run download_swupdate_image_to_ram\0" \
    "boot_swupdate=run bootargs_base bootargs_ram bootargs_swupdate && bootm ${far_ram_addr} ${swupdate_ram_addr}:${swupdate_filesize}\0" \
    "boot_swupdate_mmc=run swupdate_load_mmc && run boot_swupdate\0" \
    "update_swupdate_image=run download_swupdate_image_to_ram && run write_swupdate_image_to_mmc\0" \
    "boot_swupdate_ab_tftp=setenv swupdate_update_modes init-partitions-dual,init-scu-bl,copy-a,copy-b && run swupdate_load_tftp && run boot_swupdate\0"

#define UPDATE_PARTITIONS_COMMAND "update_partitions=run update_uboot && run update_fitimage && run update_swupdate_image && run update_rootfs\0"

#ifndef SWUPDATE_BOOTMENU_OPTION
#ifndef CONFIG_HAILO15_EMMC_8BIT
#define SWUPDATE_BOOTMENU_OPTION "bootmenu_8=SWUpdate=run boot_swupdate_mmc\0" \
                                 "bootmenu_9=SWUpdate AB board init=run boot_swupdate_ab_tftp\0"
#else
#define SWUPDATE_BOOTMENU_OPTION "bootmenu_5=SWUpdate=run boot_swupdate_mmc\0" \
                                 "bootmenu_6=SWUpdate AB board init=run boot_swupdate_ab_tftp\0"
#endif /* CONFIG_HAILO15_EMMC_8BIT */
#endif /* SWUPDATE_BOOTMENU_OPTION */

#else

#define SWUPDATE_EXTRA_ENV_SETTINGS ""
#define SWUPDATE_BOOTMENU_OPTION ""
#define UPDATE_PARTITIONS_COMMAND "update_partitions=run update_uboot && run update_fitimage && run update_rootfs\0"

#endif /* CONFIG_HAILO15_SWUPDATE */

#ifndef BOOTMENU
#ifndef CONFIG_HAILO15_EMMC_8BIT
#define BOOTMENU \
    /* Try all boot options by order */ \
    "bootmenu_0=Autodetect=" \
        "if test ${boot_image_mode} = 1; then run boot_swupdate_mmc; exit 1; fi; " \
        "if test \"${auto_uboot_update_enable}\" = \"yes\"; then run auto_uboot_update; exit 1; fi; " \
        "echo Trying Boot from SD; run boot_mmc0;" \
        "echo Trying Boot from eMMC; run boot_mmc1;" \
        "echo Trying Boot from NFS; run bootnfs;" \
        "echo ERROR: All boot options failed\0" \
    "bootmenu_1=Boot from SD Card=run boot_mmc0\0" \
    "bootmenu_2=Boot from eMMC=run boot_mmc1\0" \
    "bootmenu_3=Update SD (wic) from TFTP=run update_wic_mmc0 && bootmenu -1\0" \
    "bootmenu_4=Update eMMC (wic) from TFTP=run update_wic_mmc1 && bootmenu -1\0" \
    "bootmenu_5=Update SD (partitions) from TFTP=run update_partitions_mmc0 && bootmenu -1\0" \
    "bootmenu_6=Update eMMC (partitions) from TFTP=run update_partitions_mmc1 && bootmenu -1\0" \
    "bootmenu_7=Boot from NFS=run bootnfs\0" \
    "default_spl_boot_source=mmc12\0" \
    "spl_boot_source=mmc12\0"
#else
#define BOOTMENU \
    /* Try all boot options by order */ \
    "bootmenu_0=Autodetect=" \
        "if test ${boot_image_mode} = 1; then run boot_swupdate_mmc; exit 1; fi; " \
        "if test \"${auto_uboot_update_enable}\" = \"yes\"; then run auto_uboot_update; exit 1; fi; " \
        "echo Trying Boot from eMMC; run boot_mmc1;" \
        "echo Trying Boot from NFS; run bootnfs;" \
        "echo ERROR: All boot options failed\0" \
    "bootmenu_1=Boot from eMMC=run boot_mmc1\0" \
    "bootmenu_2=Update eMMC (wic) from TFTP=run update_wic_mmc1 && bootmenu -1\0" \
    "bootmenu_3=Update eMMC (partitions) from TFTP=run update_partitions_mmc1 && bootmenu -1\0" \
    "bootmenu_4=Boot from NFS=run bootnfs\0" \
    "default_spl_boot_source=mmc2\0" \
    "spl_boot_source=mmc2\0"
#endif

#endif

/* extra build is only relevant in full u-boot */
#ifndef CONFIG_SPL_BUILD

/* used this for tftp to mmc: https://stackoverflow.com/a/47594311 */
/* see MSW-714 for details */
#define UNNEEDED_MMCINFO_HACK "mmcinfo;"

#define CONFIG_EXTRA_ENV_SETTINGS \
    "bootargs_base=setenv bootargs console=ttyS1,${baudrate}n8 earlycon loglevel=8 rootwait debug rw && run bootargs_board\0" \
    "bootargs_board=setenv bootargs ${bootargs} ${bootargs_board_options}\0" \
    "bootargs_ram=setenv bootargs ${bootargs} root=/dev/ram0 ramdisk_size=${ramdisk_size}\0" \
    "bootargs_mmc=setenv bootargs ${bootargs} root=/dev/mmcblk${device_num}p${mmc_rootfs_partition}\0" \
    "bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs rootfstype=nfs ip=${ipaddr} nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
    "ramdisk_size=0x8000000\0" \
    "far_ram_addr=0x85000000\0" \
    "get_rootfs_partition_start_offset=part start mmc ${device_num} ${mmc_rootfs_partition} rootfs_partition_start_offset\0" \
    "sd_block_size=200\0" /* in hex, taken from running mmcinfo */\
    "serverip=10.0.0.2\0" \
    "ipaddr=10.0.0.1\0" \
    "initrd_high=0xffffffffffffffff\0" /* disables relocation of initrd/initramfs - this saves a significant amount of boot time */ \
    "core_image_name=" CONFIG_CORE_IMAGE_NAME "\0" \
    "machine_name=" CONFIG_SYS_BOARD "\0" \
    "set_mmc0_device_num= setenv device_num 0 && mmc dev ${device_num}\0" \
    "set_mmc1_device_num= setenv device_num 1 && mmc dev ${device_num}\0" \
    "load_fitimage_from_mmc=" UNNEEDED_MMCINFO_HACK " fatload mmc ${device_num}:${mmc_boot_partition} ${far_ram_addr} fitImage\0" \
    "write_fitimage_to_mmc=" UNNEEDED_MMCINFO_HACK " fatwrite mmc ${device_num}:${mmc_boot_partition} ${far_ram_addr} fitImage ${filesize}\0" \
    "write_uboot_to_mmc=" UNNEEDED_MMCINFO_HACK " fatwrite mmc ${device_num}:${mmc_boot_partition} ${far_ram_addr} " CONFIG_SPL_FS_LOAD_PAYLOAD_NAME " ${filesize}\0" \
    "write_uboot_to_mmc0_mmc1=run set_mmc0_device_num && run write_uboot_to_mmc; run set_mmc1_device_num && run write_uboot_to_mmc\0" \
    /* "mmc write" writes in blocks, so we first calculate the number of blocks we read into wic_sdblock_count. */\
    /* we assume this is called after 'tftpboot' - so filesize is populated */\
    "write_wic_to_mmc=setexpr wic_sdblock_count ${filesize} / ${sd_block_size} && setexpr wic_sdblock_count ${wic_sdblock_count} + 1; " UNNEEDED_MMCINFO_HACK " mmc write ${far_ram_addr} 0 ${wic_sdblock_count}\0" \
    "write_rootfs_to_mmc=setexpr rootfs_sdblock_count ${filesize} / ${sd_block_size} && setexpr rootfs_sdblock_count ${rootfs_sdblock_count} + 1; " UNNEEDED_MMCINFO_HACK " run get_rootfs_partition_start_offset && mmc write ${far_ram_addr} ${rootfs_partition_start_offset} ${rootfs_sdblock_count}\0" \
    /* tftpboot sets filesize to the size it loaded */\
    "download_wic_to_ram=tftpboot ${far_ram_addr} ${core_image_name}-${machine_name}.wic\0" \
    "download_rootfs_to_ram=tftpboot ${far_ram_addr} ${core_image_name}-${machine_name}.ext4\0" \
    "download_fitimage_to_ram=tftpboot ${far_ram_addr} fitImage\0" \
    "dtb_overlays= \0" /* added space otherwise it gets removed */ \
    "download_uboot_to_ram=tftpboot ${far_ram_addr} " CONFIG_SPL_FS_LOAD_PAYLOAD_NAME "\0" \
    "boot_mmc=run bootargs_base bootargs_mmc && run load_fitimage_from_mmc && bootm ${far_ram_addr}#conf-hailo_${machine_name}.dtb${dtb_overlays}\0" \
    "boot_mmc0=run set_mmc0_device_num && run boot_mmc\0"\
    "boot_mmc1=run set_mmc1_device_num && run boot_mmc\0"\
    "update_wic=run download_wic_to_ram && run write_wic_to_mmc\0" \
    "update_wic_mmc0=run set_mmc0_device_num && run update_wic\0" \
    "update_wic_mmc1=run set_mmc1_device_num && run update_wic\0" \
    "update_rootfs=run download_rootfs_to_ram && run write_rootfs_to_mmc\0" \
    "update_fitimage=run download_fitimage_to_ram && run write_fitimage_to_mmc\0" \
    "update_uboot=run download_uboot_to_ram && run write_uboot_to_mmc\0" \
    "update_uboot_mmc0_mmc1=run download_uboot_to_ram && run write_uboot_to_mmc0_mmc1\0" \
    "update_partitions_mmc0=run set_mmc0_device_num && run update_partitions\0"\
    "update_partitions_mmc1=run set_mmc1_device_num && run update_partitions\0"\
    "nfsroot=/mnt/hailo15_nfs/\0"\
    "bootnfs=run download_fitimage_to_ram && run bootargs_base bootargs_nfs && bootm ${far_ram_addr}\0" \
    "auto_uboot_update=run update_uboot_mmc0_mmc1; env set auto_uboot_update_enable no && env set spl_boot_source ${default_spl_boot_source} && env set bootdelay ${default_bootdelay} && saveenv\0" \
    "auto_uboot_update_enable=no\0" \
    "default_bootdelay=2\0" \
    BOOTMENU \
    UPDATE_PARTITIONS_COMMAND \
    SWUPDATE_EXTRA_ENV_SETTINGS \
    SWUPDATE_BOOTMENU_OPTION

#endif /* !CONFIG_SPL_BUILD */

#endif /* __HAILO15_FAMILY_COMMON_H */

