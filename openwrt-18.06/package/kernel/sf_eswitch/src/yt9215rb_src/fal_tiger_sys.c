/*
 * Include Files
 */
#include "yt_error.h"
#include "fal_tiger_sys.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
#include "fal_tiger_acl.h"

/*
 * Function Declaration
 */

yt_ret_t fal_tiger_sys_mac_get(yt_unit_t unit, yt_mac_addr_t *psys_mac)
{
    global_mac_addr0_t global_mac_addr0;
    global_mac_addr1_t global_mac_addr1;
    uint32 macAddr0;
    uint32 macAddr1;
    uint32 macAddr2;
    uint32 macAddr3;
    uint32 macAddr4;
    uint32 macAddr5;
    
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK(NULL == psys_mac, CMM_ERR_NULL_POINT);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_MAC_ADDR0m, 0, sizeof(global_mac_addr0_t), &global_mac_addr0), ret);  
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_MAC_ADDR1m, 0, sizeof(global_mac_addr1_t), &global_mac_addr1), ret); 
    HAL_FIELD_GET(GLOBAL_MAC_ADDR1m, GLOBAL_MAC_ADDR1_MAC_ADDR_BYTE0f, &global_mac_addr1, &macAddr0);
    HAL_FIELD_GET(GLOBAL_MAC_ADDR1m, GLOBAL_MAC_ADDR1_MAC_ADDR_BYTE1f, &global_mac_addr1, &macAddr1);
    HAL_FIELD_GET(GLOBAL_MAC_ADDR1m, GLOBAL_MAC_ADDR1_MAC_ADDR_BYTE2f, &global_mac_addr1, &macAddr2);
    HAL_FIELD_GET(GLOBAL_MAC_ADDR1m, GLOBAL_MAC_ADDR1_MAC_ADDR_BYTE3f, &global_mac_addr1, &macAddr3);
    HAL_FIELD_GET(GLOBAL_MAC_ADDR0m, GLOBAL_MAC_ADDR0_MAC_ADDR_BYTE4f, &global_mac_addr0, &macAddr4);
    HAL_FIELD_GET(GLOBAL_MAC_ADDR0m, GLOBAL_MAC_ADDR0_MAC_ADDR_BYTE5f, &global_mac_addr0, &macAddr5);    

    psys_mac->addr[0] = macAddr0;
    psys_mac->addr[1] = macAddr1;
    psys_mac->addr[2] = macAddr2;
    psys_mac->addr[3] = macAddr3;
    psys_mac->addr[4] = macAddr4;
    psys_mac->addr[5] = macAddr5;
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_sys_chip_reset(yt_unit_t unit)
{
    global_ctrl0_t global_entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL0m, 0, sizeof(global_ctrl0_t), &global_entry), ret);
    HAL_FIELD_SET(GLOBAL_CTRL0m, GLOBAL_CTRL0_CHIP_SW_RSTf, &global_entry, YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL0m, 0, sizeof(global_ctrl0_t), &global_entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_sys_database_reset(yt_unit_t unit)
{
    yt_port_t port;

    for(port = 0; port < CAL_PORT_NUM_ON_UNIT(unit); port++)
    {
        CMM_PLIST_UPDATE(unit, port, TRUE);
    }

#ifdef ACL_INCLUDED
    fal_tiger_acl_reset(unit);
#endif

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_sys_chipInfo_get(yt_unit_t unit, yt_switch_chip_t *pChip)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t regChip;
    uint32_t regMode;

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_CHIP_ID_REG, &regChip), ret);
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, CHIP_CHIP_MODE_REG, &regMode), ret);

    if (0x9002 == (regChip>>16&0x0ffff))
    {
        switch(regMode&0x3)
        {
            case 1:
                *pChip = SWCHIP_YT9215SC;
                break;
            case 2:
                *pChip = SWCHIP_YT9215S;
                break;
            case 3:
                *pChip = SWCHIP_YT9215RB;
                break;
           default:
                *pChip = 0;
                break;
        }
        return CMM_ERR_OK;
    }

    *pChip = 0;

    return CMM_ERR_FAIL;
}
