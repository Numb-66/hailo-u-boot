// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019-2023 Hailo Technologies Ltd. All rights reserved.
 */

#include <common.h>
#include <asm/armv8/mmu.h>

/* original */
struct mm_region hailo15_mem_map[] = {
#ifdef CONFIG_SPL_BUILD
	/* In SPL, we don't want to rely on dram_init() parsing the devicetree
	   for getting the correct RAM size, since we want to enable falcon mode
	   to be as fast as possible. So we opt for a static DRAM configuration,
	   which does not have to be the same as actual DRAM size (it may be smaller). */
	{
		.phys = PHYS_SDRAM_1,
		.virt = PHYS_SDRAM_1,
		.size = CONFIG_HAILO15_SPL_DRAM_SIZE,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	},
#else /* U-Boot proper build */
	{
		/* Updated by dram_init() */
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	},
	{
		/* Updated by dram_init() */
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	},
#endif
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0x80000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	{
		/* List terminator */
		0
	}
};

struct mm_region *mem_map = hailo15_mem_map;
