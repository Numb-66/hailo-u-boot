/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019-2024 Hailo Technologies Ltd. All rights reserved.
 *
 * Configuration for Hailo10.
 */

#ifndef __HAILO10_M2_DEVEL_H
#define __HAILO10_M2_DEVEL_H

#define BOOTMENU_COMMON "bootargs_board_options=\"swiotlb=noforce\"\0"

#define BOOTMENU \
    BOOTMENU_COMMON \
    "default_spl_boot_source=ram_mmc2\0" \
    "spl_boot_source=ram_mmc2\0" \
    /* Try all boot options by order */ \
    "bootmenu_0=Autodetect=" \
        "if test \"${auto_uboot_update_enable}\" = \"yes\"; then run auto_uboot_update; exit 1; fi; " \
        "echo Trying Boot from RAM; run bootargs_base bootargs_ram && bootm ${fitimage_ram_addr} ${fs_ram_addr}:0x10000000;" \
        "echo Trying Boot from eMMC; run boot_mmc1;" \
        "echo Trying Boot from NFS; run bootnfs;" \
        "echo ERROR: All boot options failed\0" \
    "bootmenu_1=Boot from RAM=run bootargs_base bootargs_ram && bootm ${fitimage_ram_addr} ${fs_ram_addr}:0x10000000\0" \
    "bootmenu_2=Boot from eMMC=run boot_mmc1\0" \
    "bootmenu_3=Update eMMC (wic) from TFTP=run update_wic_mmc1 && bootmenu -1\0" \
    "bootmenu_4=Update eMMC (partitions) from TFTP=run update_partitions_mmc1 && bootmenu -1\0" \
    "bootmenu_5=Boot from NFS=run bootnfs\0"

#if defined(CONFIG_HAILO15_SWUPDATE)

#define SWUPDATE_MMC_INDEX "1"
#define SWUPDATE_BOOTMENU_OPTION "bootmenu_6=SWUpdate=run boot_swupdate_mmc\0" \
                                "bootmenu_7=SWUpdate AB board init=run boot_swupdate_ab_tftp\0"
#endif /* CONFIG_HAILO15_SWUPDATE */

#ifdef CONFIG_SPL_BUILD

    #define CONFIG_EXTRA_ENV_SETTINGS \
        BOOTMENU

#endif /* CONFIG_SPL_BUILD */

#define BOOTARGS_BASE "loglevel=3 quiet rootwait"

#include "hailo15_common.h"

/*! @note: lpddr4 inline ecc located at the top 1/8 of the referred CS.
 *         In regards of using LPDDR4 setup of:
 *           - 2 ranks (Also refered as CS)
 *           - 2 channels per rank
 *           - Each channel is 16 bits wide => each rank is 32 bits bide
 *           - Rank size: 4G bytes
 *         If __not__ using ECC, then memory access are located in a single region:
 *           - 0x80000000 -  0x27fffffff: Bank #0 (8G = 0x200000000)
 *         If using ECC, then memory region is spilted to 2 ranges:
 *           - 0x080000000 - 0x15FFFFFFF: Bank #0     (3.5G = 0xE0000000)
 *           - 0x160000000 - 0x17fffffff: Bank #0 ECC (0.5G = 0x20000000)
 *           - 0x180000000 - 0x25fffffff: Bank #1     (3.5G = 0x70000000)
 *           - 0x260000000 - 0x27fffffff: Bank #1 ECC (0.5G = 0xE0000000)
 */

#endif /* __HAILO10_M2_DEVEL_H */