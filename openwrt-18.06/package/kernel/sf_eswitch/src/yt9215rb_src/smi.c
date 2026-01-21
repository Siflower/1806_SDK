/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_error.h"
#include "ctrlif.h"
#include "smi.h"

#define SWITCH_SMI_CONTROLLER_ON_UNIT(unit)   (SWITCH_ACCESOR_CONTROLLER_ON_UNIT(unit).smi_controller)

/* SMI format */
#define REG_ADDR_BIT1_ADDR      0
#define REG_ADDR_BIT1_DATA      1
#define REG_ADDR_BIT0_WRITE     0
#define REG_ADDR_BIT0_READ      1

uint32_t smi_switch_write(uint8_t unit, uint32_t reg_addr, uint32_t reg_value)
{   
    uint32_t ret = CMM_ERR_OK;
    uint8_t phyAddr;
    uint8_t regAddr;
    uint16_t regVal;

    CMM_PARAM_CHK(NULL == SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write, CMM_ERR_NOT_INIT);
    
    phyAddr           = SWITCH_SMI_CONTROLLER_ON_UNIT(unit).phyAddr;
    regAddr           = ((SWITCH_SMI_CONTROLLER_ON_UNIT(unit).switchId<<2)|(REG_ADDR_BIT1_ADDR<<1)|(REG_ADDR_BIT0_WRITE));
    /* Set reg_addr[31:16] */
    regVal = (reg_addr >> 16)&0xffff;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write(phyAddr, regAddr, regVal), ret);

    /* Set reg_addr[15:0] */
    regVal = reg_addr&0xffff;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write(phyAddr, regAddr, regVal), ret);

    /* Write Data [31:16] out */
    regAddr           = ((SWITCH_SMI_CONTROLLER_ON_UNIT(unit).switchId<<2)|(REG_ADDR_BIT1_DATA<<1)|(REG_ADDR_BIT0_WRITE));
    regVal = (reg_value >> 16)&0xffff;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write(phyAddr, regAddr, regVal), ret);

    /* Write Data [15:0] out */
    regVal = reg_value&0xffff;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write(phyAddr, regAddr, regVal), ret);
        
    return CMM_ERR_OK;
}

uint32_t smi_switch_read(uint8_t unit, uint32_t reg_addr, uint32_t *reg_value)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t rData;
    uint8_t phyAddr;
    uint8_t regAddr;
    uint16_t regVal;

    CMM_PARAM_CHK(NULL == SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write, CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(NULL == SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_read, CMM_ERR_NOT_INIT);
    
    phyAddr           = SWITCH_SMI_CONTROLLER_ON_UNIT(unit).phyAddr;
    regAddr           = ((SWITCH_SMI_CONTROLLER_ON_UNIT(unit).switchId<<2)|(REG_ADDR_BIT1_ADDR<<1)|(REG_ADDR_BIT0_READ));
    /* Set reg_addr[31:16] */
    regVal = (reg_addr >> 16)&0xffff;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write(phyAddr, regAddr, regVal), ret);

    /* Set reg_addr[15:0] */
    regVal = reg_addr&0xffff;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_write(phyAddr, regAddr, regVal), ret);
    
    regAddr           = ((SWITCH_SMI_CONTROLLER_ON_UNIT(unit).switchId<<2)|(REG_ADDR_BIT1_DATA<<1)|((REG_ADDR_BIT0_READ)));
    /* Read Data [31:16] */
    regVal = 0x0;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_read(phyAddr, regAddr, &regVal), ret);
    rData = (uint32_t)(regVal<<16);

    /* Read Data [15:0] */
    regVal = 0x0;
    CMM_ERR_CHK(SWITCH_SMI_CONTROLLER_ON_UNIT(unit).smi_read(phyAddr, regAddr, &regVal), ret);
    rData |= regVal;
    
    *reg_value = rData;

    return CMM_ERR_OK;
}


