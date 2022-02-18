/*
 * Copyright (c) 2016, NVIDIA CORPORATION.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/arch/tegra.h>
#include <asm/arch-tegra/mmc.h>
#include <asm/arch-tegra/tegra_mmc.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = (1.5 * 1024 * 1024 * 1024);
	return 0;
}

int board_early_init_f(void)
{
	return 0;
}

int board_init(void)
{
	return 0;
}

int board_late_init(void)
{
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = gd->ram_size;
}

void pad_init_mmc(struct mmc_host *host)
{
}

int board_mmc_init(bd_t *bd)
{
	tegra_mmc_init();

	return 0;
}

int ft_system_setup(void *blob, bd_t *bd)
{
	return 0;
}
