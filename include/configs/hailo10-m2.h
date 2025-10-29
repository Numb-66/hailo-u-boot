/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019-2024 Hailo Technologies Ltd. All rights reserved.
 *
 * Configuration for Hailo10.
 */

#ifndef __HAILO10_M2_H
#define __HAILO10_M2_H

#define BOOTMENU_COMMON "bootargs_board_options=\"swiotlb=noforce\"\0"

#define BOOTMENU \
    BOOTMENU_COMMON \
    "default_spl_boot_source=ram\0" \
    "spl_boot_source=ram\0"

#ifdef CONFIG_SPL_BUILD

    #define CONFIG_EXTRA_ENV_SETTINGS \
        BOOTMENU

#endif /* CONFIG_SPL_BUILD */

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

#endif /* __HAILO10_M2_H */