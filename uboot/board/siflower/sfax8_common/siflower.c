/*
 * Copyright (C) 2013 Gabor Juhos <juhosg@openwrt.org>
 * Copyright (C) 2013 Imagination Technologies
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <common.h>
#include <ide.h>
#include <netdev.h>
#include <rtc.h>

#include <asm/addrspace.h>
#include <asm/io.h>
#include <phy.h>
#include "superio.h"
#include <dm/pinctrl.h>
#include <mach/pad.h>
#include <dm/uclass-id.h>
#include <dm/uclass.h>

#include <mach/sfax8.h>

enum sys_con {
	SYSCON_UNKNOWN,
	SYSCON_A18_MPW0,
	SYSCON_A18_MPW1,
	SYSCON_A18_FULLMASK_P20,
	SYSCON_A18_FULLMASK_P10,
	SYSCON_A18_FULLMASK_P10M,
	SYSCON_A18_FULLMASK_86V,
	SYSCON_A18_FULLMASK_AC,
	SYSCON_A18_FULLMASK_X10,
	SYSCON_A18_FULLMASK_P10H,
	SYSCON_A18_FULLMASK_AC20,
	SYSCON_A18_FULLMASK_REP,
	SYSCON_A18_FULLMASK_AIR001,
	SYSCON_A28_MPW0_FPGA,
	SYSCON_A28_MPW0_P20B,
	SYSCON_A28_MPW0_AC28,
	SYSCON_A28_MPW0_A28PHY,
	SYSCON_A28_MPW0_REP,
};

//todo remove when gpio is ready
extern int sf_pad_set_value(u32 index);
extern int sf_pad_set_func_2(u32 index);
extern int sf_pad_set_func(u32 index);

static enum sys_con sfa18_sys_con(void)
{
#ifdef CONFIG_TARGET_SFA18_MPW0
	return SYSCON_A18_MPW0;
#endif

#ifdef CONFIG_TARGET_SFA18_MPW1
	return SYSCON_A18_MPW1;
#endif

#ifdef CONFIG_TARGET_SFA18_P20B
	return SYSCON_A18_FULLMASK_P20;
#endif

#ifdef CONFIG_TARGET_SFA18_P10M
	return SYSCON_A18_FULLMASK_P10M;
#endif

#ifdef CONFIG_TARGET_SFA18_P10B
	return SYSCON_A18_FULLMASK_P10;
#endif

#ifdef CONFIG_TARGET_SFA18_86V
	return SYSCON_A18_FULLMASK_86V;
#endif

#ifdef CONFIG_TARGET_SFA18_AC
	return SYSCON_A18_FULLMASK_AC;
#endif

#ifdef CONFIG_TARGET_SFA18_X10
	return SYSCON_A18_FULLMASK_X10;
#endif

#ifdef CONFIG_TARGET_SFA18_P10H
	return SYSCON_A18_FULLMASK_P10H;
#endif

#ifdef CONFIG_TARGET_SFA18_AC20
	return SYSCON_A18_FULLMASK_AC20;
#endif

#ifdef CONFIG_TARGET_SFA18_REP
	return SYSCON_A18_FULLMASK_REP;
#endif

#ifdef CONFIG_TARGET_SFA18_AIR001
	return SYSCON_A18_FULLMASK_AIR001;
#endif

#ifdef CONFIG_TARGET_SFA28_FPGA
	return SYSCON_A28_MPW0_FPGA;
#endif

#ifdef CONFIG_TARGET_SFA28_P20B
	return SYSCON_A28_MPW0_P20B;
#endif

#ifdef CONFIG_TARGET_SFA28_AC28
	return SYSCON_A28_MPW0_AC28;
#endif

#ifdef CONFIG_TARGET_SFA28_A28PHY
	return SYSCON_A28_MPW0_A28PHY;
#endif

#ifdef CONFIG_TARGET_SFA28_REP
	return SYSCON_A28_MPW0_REP;
#endif

	return SYSCON_UNKNOWN;
}

phys_size_t initdram(int board_type)
{
	return CONFIG_SYS_MEM_SIZE;
}

int checkboard(void)
{
	enum sys_con con;
	puts("Board: MIPS sfa18");

	con = sfa18_sys_con();

	switch (con) {

		case SYSCON_A18_MPW0:
			puts(" MPW0");
			break;

		case SYSCON_A18_MPW1:
			puts(" MPW1");
			break;

		case SYSCON_A18_FULLMASK_P20:
			puts(" FULLMASK P20B");
			break;

		case SYSCON_A18_FULLMASK_P10:
			puts(" FULLMASK P10B");
			break;

		case SYSCON_A18_FULLMASK_P10M:
			puts(" FULLMASK P10M");
			break;

		case SYSCON_A18_FULLMASK_86V:
			puts(" FULLMASK 86V");
			break;

		case SYSCON_A18_FULLMASK_AC:
			puts(" FULLMASK AC");
			break;

		case SYSCON_A18_FULLMASK_X10:
			puts(" FULLMASK X10");
			break;

		case SYSCON_A18_FULLMASK_P10H:
			puts(" FULLMASK P10H");
			break;

		case SYSCON_A18_FULLMASK_AC20:
			puts(" FULLMASK AC20");
			break;

		case SYSCON_A18_FULLMASK_REP:
			puts(" FULLMASK REP");
			break;

		case SYSCON_A18_FULLMASK_AIR001:
			puts(" FULLMASK AIR001");
			break;

		case SYSCON_A28_MPW0_FPGA:
			puts(" A28 MPW0 FPGA");
			break;

		case SYSCON_A28_MPW0_P20B:
			puts(" A28 MPW0 p20b");
			break;

		case SYSCON_A28_MPW0_AC28:
			puts(" A28 MPW0 ac28");
			break;

		case SYSCON_A28_MPW0_A28PHY:
			puts(" A28 MPW0 a28phy");
			break;

		case SYSCON_A28_MPW0_REP:
			puts(" A28 MPW0 rep");
			break;

		default:
			puts("");
	}

	putc('\n');
	return 0;
}

void _machine_restart(void)
{
	writew(0x01,(void*)SOFT_REBOOT);
	mdelay(1000);
}

int board_early_init_f(void)
{
	sfa18_superio_init();

	return 0;
}

int misc_init_r(void)
{
	//marked by neil for a short time.
	//rtc_reset();

	return 0;
}

#ifdef CONFIG_ETH_LED
void request_eth_led(void)
{
	int ret = 0;
	struct udevice *pinctrl;
	//request led
	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if(ret){
		printf("can not get pinctrl\n");
	}else{
		ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_ETH_LED);
		printf("request led pin ctrl %s\n",ret ? "fail" : "success");
	}
}
#endif

#ifdef CONFIG_SFA18_PCBA_TEST
extern void pcba_net_register(void);
int board_eth_init(bd_t *bis) {
	int rc = -1;
	printf("Registering sfa18 net\n");
#ifdef CONFIG_SFA18_ETH
	printf("Registering sfa18 eth\n");
	rc = sf_eth_register();
	if (rc)
		return rc;
#endif
#if defined CONFIG_SFA18_GMAC && defined CONFIG_SFAX8_WLAN_GMAC
	printf("Registering sfa18 gmac\n");
	rc = sf_gmac_register();
	if (rc)
		return rc;
#endif
	pcba_net_register();
	return rc;
}
#else /* CONFIG_SFA18_PCBA_TEST */
#ifdef CONFIG_SFA18_ETH
int board_eth_init(bd_t *bis)
{
	int rc = -1;
	printf("Registering sfa18 net\n");
#ifdef CONFIG_ETH_LED
	request_eth_led();
#endif
	printf("Registering sfa18 eth\n");
	rc = sf_eth_register();
	return rc;
}
#else /* CONFIG_SFA18_ETH */
int board_eth_init(bd_t *bis)
{
	int rc = -1;
#if defined CONFIG_SFA18_GMAC
	printf("Registering sfa18 gmac\n");
	rc = sf_gmac_register();
#endif
	return rc;
}
#endif /* CONFIG_SFA18_ETH */
#endif /* CONFIG_SFA18_PCBA_TEST */
