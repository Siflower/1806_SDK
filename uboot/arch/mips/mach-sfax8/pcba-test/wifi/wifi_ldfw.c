/*
 * =====================================================================================
 *
 *       Filename:  wifi_ldfw.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  08/07/2016 10:56:42 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  franklin , franklin.wang@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */

#include <common.h>
#include <asm/io.h>

#include "rwnx_config.h"
#include "wifi_ldfw.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"
#include "reg_riu.h"
#include "reg_mdm_cfg.h"
#include "reg_rc.h"

//#include "irq_handle.h"

#ifdef WITH_CATRF
#include "rf_access.h"
#endif
//TODO:once agc firmware changed, this head file shoud be re-generated again
#include "agc_ram_bin.h"

//TODO:once rf firmware changed, this head file shoud be re-generated again
#include "rf_pmem_bin.h"
//#include "aetnensis/rf_regbank_defines.h"
#define AGC_MEMORY_ADDR		(WIFI_BASE_ADDR + 0x00100000 + 0xA000)

void load_agc_ram_bin()
{
    int i = 0;

    //open AGC CLK
	mdm_agcclkforce_setf(1);

	//1.AGC memory clock must be switched to AHB clock domain, set register AGCMEMCLKCTRL=0
	/* AGCCNTL0[AGCFSMRESET]=1 */
	riu_agcfsmreset_setf(1);
	/* MEMCLKCTRL0[AGCMEMCLKCTRL]=0 */
	//mdm_agcmemclkforce_setf(0);


	//2.Download buffer in the address 0xA000
	for(i = 0; i < AGC_RAM_BIN_INT_NUM; i++){
		REG_PL_WR(AGC_MEMORY_ADDR + (4 * i), g_agc_ram_bin[i]);
	}

	//3.At the end of FSM microcode downloading,
	//AGC memory clock and control must be switched back to default AGC clock domain,
	//set the register AGCMEMCLKCTRL=1,
	//mdm_agcmemclkctrl_setf(1);
	//mdm_agcmemclkforce_setf(1);
	riu_agcfsmreset_setf(0);
}

#ifdef WITH_CATRF
char is_rf_boot_finished()
{
	uint16_t tmp = ml_apb_read(0x1420);
	if (tmp & 0x1) {
		return 1;
	} else {
		return 0;
	}
}
#endif
void load_rf_pmem_bin()
{
#ifdef WITH_CATRF
	uint16_t rd_data_reg;
	int i = 0;

	//enable ML_APB access to PMEM
	rd_data_reg = ml_apb_read(0x1081);//MISC_DEBUG
	ml_apb_write(0x1081,(0xFFF7 & rd_data_reg));

	//1.0x4001
	ml_apb_write(0x4001, 0x001D);//PMEM_CTRL

	//2.reset pointer 0x4000
	ml_apb_write(0x4000,0);//PMEM_POINTER

	//3.write to 0x4002
	// the rf pmem bin consist of (RF_PMEM_BIN_INT_NUM - RF_PMEM_BIN_ZERO_NUM =)534 none-zero values and
	// (RF_PMEM_BIN_ZERO_NUM)24042 zero values. So we have two steps to load respectively.
	for(i = 0; i<(RF_PMEM_BIN_INT_NUM - RF_PMEM_BIN_ZERO_NUM); i++){
		ml_apb_write(0x4002,g_rf_pmem_bin[i]);//PMEM_DATA
	}
#endif
}
