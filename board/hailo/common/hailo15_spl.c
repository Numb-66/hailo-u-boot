// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019-2023 Hailo Technologies Ltd. All rights reserved. 
 */

#include <common.h>
#include <spl.h>
#include <env.h>
#include <hang.h>
#include <init.h>
#include <cpu_func.h>
#include "hailo15_board.h"
#ifdef CONFIG_SPL_ENV_IS_IN_MMC
#include "mmc.h"
#endif

#define BASE_SPI_FLASH_ADDRESS 0x70000000

DECLARE_GLOBAL_DATA_PTR;

void board_init_f(ulong dummy)
{
	/* we enable MMU + caching before doing anything else to improve performace */
	gd->relocaddr = PHYS_SDRAM_1 + CONFIG_HAILO15_SPL_DRAM_SIZE;
	arch_reserve_mmu();
	icache_enable();
	dcache_enable();

	/* this part is the same as board_init_f in common/spl/spl.c */

	if (CONFIG_IS_ENABLED(OF_CONTROL)) {
		int ret;

		ret = spl_early_init();
		if (ret) {
			debug("spl_early_init() failed: %d\n", ret);
			hang();
		}
	}

       preloader_console_init();

}

void spl_board_init(void)
{
#ifdef CONFIG_SPL_ENV_IS_IN_MMC
	mmc_initialize(NULL);
#endif
	if (hailo15_scmi_init()) {
		hang();
	}
	if (hailo15_scmi_check_version_match()) {
		hang();
	}
}

void spl_board_prepare_for_boot(void)
{
	dcache_disable();
	icache_disable();
}

void board_boot_order(u32 *spl_boot_list)
{
	const char *s;

	env_init();
	env_load();

	s = env_get("spl_boot_source");
	if (!s) {
		puts("failed to get 'spl_boot_source' from env, falling back to mmc12\n");
		s = "mmc12";
	}

	if (!strcmp(s, "mmc1")) {
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
#ifdef CONFIG_TARGET_HAILO15L_OREGANO
		spl_boot_list[1] = BOOT_DEVICE_MMC1;
#endif /* CONFIG_TARGET_HAILO15L_OREGANO */
	} else if (!strcmp(s, "mmc2")) {
		spl_boot_list[0] = BOOT_DEVICE_MMC2;
	} else if (!strcmp(s, "mmc12")) {
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
		spl_boot_list[1] = BOOT_DEVICE_MMC2;
	} else if (!strcmp(s, "mmc21")) {
		spl_boot_list[0] = BOOT_DEVICE_MMC2;
		spl_boot_list[1] = BOOT_DEVICE_MMC1;
	} else if (!strcmp(s, "ram_mmc2")) {
		spl_boot_list[0] = BOOT_DEVICE_RAM;
		spl_boot_list[1] = BOOT_DEVICE_MMC2;
	} else if (!strcmp(s, "uart")) {
		spl_boot_list[0] = BOOT_DEVICE_UART;
	} else if (!strcmp(s, "ram")) {
		spl_boot_list[0] = BOOT_DEVICE_RAM;
	} else if (!strcmp(s, "nor")) {
		spl_boot_list[0] = BOOT_DEVICE_NOR;
	} else {
		printf("spl_boot_source=%s unsupported, falling back to mmc12\n", s);
		s = "mmc12";
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
		spl_boot_list[1] = BOOT_DEVICE_MMC2;
	}

	printf("U-Boot SPL boot source %s\n", s);
}

int spl_mmc_fs_boot_partition(void)
{
	return hailo15_mmc_boot_partition();
}

unsigned long spl_nor_get_uboot_base(void)
{
	return BASE_SPI_FLASH_ADDRESS + CONFIG_SYS_UBOOT_OFFSET + hailo15_get_active_boot_image_offset();
}