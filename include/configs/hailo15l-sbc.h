/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019-2024 Hailo Technologies Ltd. All rights reserved.  
 *
 * Configuration for SBC Hailo15L.
 */

#ifndef __HAILO15L_SBC_H
#define __HAILO15L_SBC_H

#define SPL_BOOT_SOURCE "mmc2"

#define BOOTMENU \
    "default_spl_boot_source=" SPL_BOOT_SOURCE "\0" \
    "spl_boot_source=" SPL_BOOT_SOURCE "\0" \
    /* Try all boot options by order */ \
    "bootmenu_0=Autodetect=" \
        "if test ${boot_image_mode} = 1; then run boot_swupdate_mmc; exit 1; fi; " \
        "if test \"${auto_uboot_update_enable}\" = \"yes\"; then run auto_uboot_update; exit 1; fi; " \
        "echo Trying Boot from eMMC; run boot_mmc1;" \
        "echo Trying Boot from SD; run boot_mmc0;" \
        "echo Trying Boot from NFS; run bootnfs;" \
        "echo ERROR: All boot options failed\0" \
    "bootmenu_1=Boot from SD Card=run boot_mmc0\0" \
    "bootmenu_2=Boot from eMMC=run boot_mmc1\0" \
    "bootmenu_3=Update SD (wic) from TFTP=run update_wic_mmc0 && bootmenu -1\0" \
    "bootmenu_4=Update eMMC (wic) from TFTP=run update_wic_mmc1 && bootmenu -1\0" \
    "bootmenu_5=Update SD (partitions) from TFTP=run update_partitions_mmc0 && bootmenu -1\0" \
    "bootmenu_6=Update eMMC (partitions) from TFTP=run update_partitions_mmc1 && bootmenu -1\0" \
    "bootmenu_7=Boot from NFS=run bootnfs\0"

#ifdef CONFIG_SPL_BUILD

    #define CONFIG_EXTRA_ENV_SETTINGS \
        BOOTMENU

#endif /* CONFIG_SPL_BUILD */

#include "hailo15l_common.h"

#endif /* __HAILO15L_SBC_H */
