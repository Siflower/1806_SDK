/* FILE NAME:   air_init.c
 * PURPOSE:
 *      Define the initialization function in AIR SDK.
 *
 * NOTES:
 *      None
 */

/* INCLUDE FILE DECLARATIONS
 */
#include "air.h"

/* NAMING CONSTANT DECLARATIONS
 */

/* MACRO FUNCTION DECLARATIONS
 */

/* DATA TYPE DECLARATIONS
 */

/* GLOBAL VARIABLE DECLARATIONS
 */
AIR_PRINTF _ext_printf;
AIR_UDELAY _ext_udelay;
AIR_MALLOC _ext_malloc;
AIR_FREE   _ext_free;

/* LOCAL SUBPROGRAM DECLARATIONS
 */

/* STATIC VARIABLE DECLARATIONS
 */

/* EXPORTED SUBPROGRAM BODIES
 */

/* LOCAL SUBPROGRAM BODIES
 */

/* FUNCTION NAME:   air_init
 * PURPOSE:
 *      This API is used to initialize the SDK.
 *
 * INPUT:
 *      unit            --  The device unit
 *      ptr_init_param  --  The sdk callback functions.
 *
 * OUTPUT:
 *      None
 *
 * RETURN:
 *      AIR_E_OK
 *      AIR_E_OTHERS
 *
 * NOTES:
 *      None
 */
AIR_ERROR_NO_T
air_init(
    const UI32_T unit,
    AIR_INIT_PARAM_T *ptr_init_param)
{
    AIR_ERROR_NO_T rc = AIR_E_OK;
    UI32_T u32dat = 0;
    UI8_T port = 0;
    AIR_LED_ON_EVT_T on_evt;
    AIR_LED_BLK_EVT_T blk_evt;

    /* check point */
    AIR_CHECK_PTR(ptr_init_param);

    _ext_dev_access.read_callback = ptr_init_param->dev_access.read_callback;
    _ext_dev_access.write_callback = ptr_init_param->dev_access.write_callback;
    _ext_dev_access.phy_read_callback = ptr_init_param->dev_access.phy_read_callback;
    _ext_dev_access.phy_write_callback = ptr_init_param->dev_access.phy_write_callback;
    _ext_dev_access.phy_cl45_read_callback = ptr_init_param->dev_access.phy_cl45_read_callback;
    _ext_dev_access.phy_cl45_write_callback = ptr_init_param->dev_access.phy_cl45_write_callback;
    _ext_printf = ptr_init_param->printf;
    _ext_udelay = ptr_init_param->udelay;
    _ext_malloc = ptr_init_param->malloc;
    _ext_free   = ptr_init_param->free;

    /*
     * Disable MAC power saving
     */
    aml_readReg(unit, CKGCR, &u32dat);
    u32dat &= ~(CKG_LNKDN_GLB_STOP | CKG_LNKDN_PORT_STOP);
    aml_writeReg(unit, CKGCR, u32dat);

    /*
     *EEE_enable
     */
#if 0
    for(port=0; port<AIR_MAX_NUM_OF_GIGA_PORTS; port++)
    {
        air_port_setPsMode(unit, port, AIR_PORT_PS_EEE);
    }
    /*
     * LED initial
     * Note: set one port can control all ports led
     */
    port = 0;
    /* Set LED to user define mode */
    air_led_setMode(unit, port, AIR_LED_MODE_USER_DEFINE);

    /* Enable LED0 and LED1 */
    air_led_setState(unit, port, 0, TRUE);
    air_led_setState(unit, port, 1, TRUE);

    /* Set LED independently controlled by LED_ON_CTL and LED_BLK_CTL reg */
    /* LED0 on ctrl: link 10/100/1000 and active high  */
    on_evt.link_1000m = TRUE;
    on_evt.link_100m = TRUE;
    on_evt.link_10m = TRUE;
    on_evt.link_dn = FALSE;
    on_evt.fdx = FALSE;
    on_evt.hdx = FALSE;
    on_evt.force = FALSE;
    /* LED0 blinking ctrl: 10/100/1000 tx/rx actionv */
    blk_evt.tx_act_1000m = TRUE;
    blk_evt.rx_act_1000m = TRUE;
    blk_evt.tx_act_100m = TRUE;
    blk_evt.rx_act_100m = TRUE;
    blk_evt.tx_act_10m = TRUE;
    blk_evt.rx_act_10m = TRUE;
    blk_evt.cls = FALSE;
    blk_evt.rx_crc = FALSE;
    blk_evt.rx_idle = FALSE;
    blk_evt.force = FALSE;
    air_led_setUsrDef(unit, port, 0, HIGH, on_evt, blk_evt);

    /* LED1 on ctrl: link 10/100/1000 and active high  */
    on_evt.link_1000m = FALSE;
    on_evt.link_100m = TRUE;
    on_evt.link_10m = TRUE;
    on_evt.link_dn = FALSE;
    on_evt.fdx = FALSE;
    on_evt.hdx = FALSE;
    on_evt.force = FALSE;
    /* LED1 blinking ctrl: 10/100/1000 tx/rx actionv */
    blk_evt.tx_act_1000m = FALSE;
    blk_evt.rx_act_1000m = FALSE;
    blk_evt.tx_act_100m = TRUE;
    blk_evt.rx_act_100m = TRUE;
    blk_evt.tx_act_10m = TRUE;
    blk_evt.rx_act_10m = TRUE;
    blk_evt.cls = FALSE;
    blk_evt.rx_crc = FALSE;
    blk_evt.rx_idle = FALSE;
    blk_evt.force = FALSE;
    air_led_setUsrDef(unit, port, 1, HIGH, on_evt, blk_evt);

    /*
     * Port5 and port6 mac control
     */
    for(port=AIR_SGMII_PORT_OFFSET_BEGIN; port<=AIR_SGMII_PORT_OFFSET_END; port++)
    {
        u32dat = (FORCE_LINK | FORCE_DPX | (MAC_SPD_1000 << FORCE_SPD_S) |
                  FORCE_TX_FC | FORCE_RX_FC | BACKPR_EN | BKOFF_EN |
                  MAC_TX_EN | MAC_RX_EN |
                  MAC_MODE | (IPG_96BIT_WITH_SHORT_IPG << IPG_CFG_S) |
                  FORCE_LINK_EN | FORCE_DPX_EN | FORCE_SPD_EN |
                  FORCE_TX_FC_EN | FORCE_RX_FC_EN);
        aml_writeReg(unit, PMCR(port), u32dat);
    }

    /*
     *Disable afifo reset
     */
    for(port=0; port<AIR_MAX_NUM_OF_PORTS; port++)
    {
        aml_writeReg(unit, DBG_CNT(port), DIS_CLR);
    }

    /*
     * Regards frames as IP multicast as follow
     * MAC DA:  01-00-5E-xx-xx-xx
     *          33-33-xx-xx-xx-xx
     * DIP:     224.x.x.x..239.x.x.x
     */
    for(port=0; port<AIR_MAX_NUM_OF_PORTS; port++)
    {
        u32dat = (DMAC_01005E | DMAC_3333XX | MCAST_DIP);
        aml_writeReg(unit, PIC(port), u32dat);
    }

    /*
     * Disable default router port
     */
    for(port=0; port<AIR_MAX_NUM_OF_PORTS; port++)
    {
        air_l2_setSnpStaticRp(unit, port, FALSE);
    }

    /*
     * Disable collision pool
     */
    aml_readReg(unit, CPGC, &u32dat);
    u32dat &= ~(COL_EN | COL_CLK_EN);
    aml_writeReg(unit, CPGC, u32dat);

    /*
     * Disable MAC power saving
     */
    aml_readReg(unit, CKGCR, &u32dat);
    u32dat &= ~CKG_LNKDN_GLB_STOP;
    aml_writeReg(unit, CKGCR, u32dat);

    /*
     * Enable trTCM
     */
    aml_readReg(unit, TRTCM, &u32dat);
    u32dat |= TRTCM_EN;
    aml_writeReg(unit, TRTCM, u32dat);

    /*
     * Enalbe SGMII an enable, method clause37
     */
    for(port=AIR_SGMII_PORT_OFFSET_BEGIN; port<=AIR_SGMII_PORT_OFFSET_END; port++)
    {
        air_port_setAnMode(unit, port, TRUE);
        air_port_setSgmiiMode(unit, port, AIR_PORT_SGMII_MODE_CLAUSE37);
    }

    /*
     * Set MTCC Limit to 0
     */
    aml_readReg(unit, GMACCR, &u32dat);
    u32dat &= ~(0xF << MTCC_LMT_S);
    aml_writeReg(unit, GMACCR, u32dat);
#endif
    return rc;
}

/* FUNCTION NAME:   air_hw_reset
 * PURPOSE:
 *      This API is used to reset hardware.
 *
 * INPUT:
 *      unit            --  The device unit
 *
 * OUTPUT:
 *      None
 *
 * RETURN:
 *      AIR_E_OK
 *      AIR_E_OTHERS
 *
 * NOTES:
 *      None
 */
AIR_ERROR_NO_T
air_hw_reset(
    const UI32_T unit)
{
    AIR_PRINT(">>>>> enct_hw_reset\n");
    /* Set an8855 reset pin to 0 */

    /* Delay 100ms */

    /* Set an8855 reset pin to 1 */

    /* Delay 600ms */

    return AIR_E_OK;
}

/* FUNCTION NAME:   air_set_gpio_pin_mux
 * PURPOSE:
 *      This API is used to set gpio pin mux.
 *
 * INPUT:
 *      unit            --  The device unit
 *
 * OUTPUT:
 *      None
 *
 * RETURN:
 *      AIR_E_OK
 *      AIR_E_OTHERS
 *
 * NOTES:
 *      None
 */
AIR_ERROR_NO_T
air_set_gpio_pin_mux(
    const UI32_T unit)
{
    AIR_PRINT(">>>>> enct_set_gpio_pin_mux\n");
    /* Set GPIO_MODE0 */
    /* Implementation for SLT HW */
    aml_writeReg(unit, GPIO_MODE0, 0x11111111);

    return AIR_E_OK;
}
