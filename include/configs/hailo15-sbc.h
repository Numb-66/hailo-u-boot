/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019-2023 Hailo Technologies Ltd. All rights reserved.  
 *
 * Configuration for Hailo15.
 */

#ifndef __HAILO15_SBC_H
#define __HAILO15_SBC_H

#define SWUPDATE_MMC_INDEX "1"
#ifndef SPL_BOOT_SOURCE
#define SPL_BOOT_SOURCE "nor"
#endif
#define BOOTMENU \
    /* Try all boot options by order */ \
    "bootmenu_0=Autodetect=" \
        "if test ${boot_image_mode} = 1; then run boot_swupdate_mmc; exit 1; fi; " \
        "if test \"${auto_uboot_update_enable}\" = \"yes\"; then run auto_uboot_update; exit 1; fi; " \
        "echo Trying Boot from SD; run boot_mmc1;" \
        "echo Trying Boot from NFS; run bootnfs;" \
        "echo ERROR: All boot options failed\0" \
    "bootmenu_1=Boot from SD Card=run boot_mmc1\0" \
    "bootmenu_2=Boot from NFS=run bootnfs\0" \
    "default_spl_boot_source=nor\0" \
    "spl_boot_source=nor\0"

#ifdef CONFIG_HAILO15_SWUPDATE
#define SWUPDATE_BOOTMENU_OPTION    "bootmenu_3=SD Card Board Init=run boot_swupdate_sdio1_only_a\0" \
                                    "bootmenu_4=SD Card AB Board Init=run boot_swupdate_sdio1_ab\0" 
#endif /* CONFIG_HAILO15_SWUPDATE */

#include "hailo15_common.h"

/*! @note: lpddr4 inline ecc located at the top 1/8 of the referred CS.
 *         In regards of using LPDDR4 setup of:
 *           - 2 ranks (Also refered as CS)
 *           - 2 channels per rank
 *           - Each channel is 16 bits wide => each rank is 32 bits bide
 *           - Rank size: 2G bytes
 *         If __not__ using ECC, then memory access are located in a single region:
 *           - 0x80000000 -  0x17fffffff: Bank #0 (4G = 0x100000000)
 *         If using ECC, then memory region is spilted to 2 ranges:
 *           - 0x080000000 - 0x0efffffff: Bank #0     (1.75G = 0x70000000)
 *           - 0x0f0000000 - 0x0ffffffff: Bank #0 ECC (0.25G = 0x10000000)
 *           - 0x100000000 - 0x16fffffff: Bank #1     (1.75G = 0x70000000)
 *           - 0x170000000 - 0x17fffffff: Bank #1 ECC (0.25G = 0x10000000)
 */

#endif /* __HAILO15_SBC_H */