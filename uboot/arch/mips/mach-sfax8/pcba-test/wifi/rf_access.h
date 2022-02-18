
#ifndef _RF_ACCESS_H_
#define _RF_ACCESS_H_

/**
 * INCLUDE FILES
 * */
#include "co_int.h"
//for __INLINE
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "rf_regbank_defines.h"

// Command header field definitions
/// Response Request offset
#define RRQ_OFT          15
/// Response Request bit
#define RRQ_BIT         (0x1 << RRQ_OFT)
/// Command ID offset
#define CMDID_OFT        4
/// Command ID mask
#define CMDID_MSK       (0x3FF << CMDID_OFT)
/// Command argument count offset
#define CMD_ARGC_OFT     0
/// Command argument count mask
#define CMD_ARGC_MSK    (0xF << ARGC_OFT)

// Response header field definitions
/// Error Flag offset
#define EF_OFT           15
/// Error Flag bit
#define EF_BIT          (0x1 << EF_OFT)
/// Task started status offset
#define TS_OFT           14
/// Task started status bit
#define TS_BIT          (0x1 << TS_OFT)
/// Response ID offset
#define RSPID_OFT        4
/// Response ID mask
#define RSPID_MSK       (0x3FF << RSPID_OFT)
/// Response argument count offset
#define RSP_ARGC_OFT     0
/// Response argument count mask
#define RSP_ARGC_MSK    (0xF << RSP_ARGC_OFT)

/// RF command IDs
/* AET ASP API
*
* These command IDs include length fields
*
* */
enum {
    _CWA_GEN_GetRegister                   = 0x8011,  // CWA_GEN_GetRegister (address) -> (value)
    _CWA_GEN_SetRegister                   = 0x8022,  // CWA_GEN_SetRegister (address, value) -> ()
    _CWA_TRX_SwitchMode                    = 0x0031,  // CWA_TRX_SwitchMode (trxid, mode, gain_pwr) -> ()
    _CWA_GEN_GetVersion                    = 0x8040,  // CWA_GEN_GetVersion () -> (hw_version, target_ver, major_ver, minor_ver)
    _CWA_APP_SetOpMode                     = 0x8141,  // CWA_APP_SetOpMode (mode) -> ()
    _CWA_APP_GetOpMode                     = 0x8150,  // CWA_APP_GetOpMode () -> (mode)
    _CWA_APP_GetStatus                     = 0x8160,  // CWA_APP_GetStatus () -> (appmode, clkpllmode, unlocked, vtune_high, vtune_low, vco_toohigh, vco_toolow)
    _CWA_APP_SetCalConfig				   = 0x8171,  // CWA_APP_SetCalConfig (cal_config) -> ()
	_CWA_APP_GetCalConfig				   = 0x8180,  // CWA_APP_GetCalConfig () -> (cal_config)
	_CWA_APP_DoCalibration				   = 0x8190,  // CWA_APP_DoCalibration () -> ()
	_CWA_APP_GetCalStatus				   = 0x81a0,  // CWA_APP_GetCalStatus () -> (cal_status)
	_CWA_APP_SetHKConfig				   = 0x8254,  // CWA_APP_SetHKConfig (hk_mode, hk_clock, hk_task_config,hk_update_time,hk_tempdiff_threshold) -> ()
	_CWA_APP_GetHKConfig				   = 0x8260,  // CWA_APP_GetHKConfig () -> (hk_mode, hk_clock,hk_task_config,hk_update_time,hk_tempdiff_threshold)
	_CWA_TRX_SetRFChannel                  = 0x8053,  // CWA_TRX_SetRFChannel (trxid, rfch, ignore_cal_values, sx_vco_cap, sx_vco_cur, rx_wbpll_band, tx_wbpll_band) -> ()
    _CWA_TRX_GetRFChannel                  = 0x8061,  // CWA_TRX_GetRFChannel (trxid) -> (rfch)
    _CWA_TRX_SetChannelBW                  = 0x8071,  // CWA_TRX_SetChannelBW (trxid, chbw) -> ()
    _CWA_TRX_GetChannelBW                  = 0x8081,  // CWA_TRX_GetChannelBW (trxid) -> (chbw)
    _CWA_TRX_SetRxGain                     = 0x8091,  // CWA_TRX_SetRxGain (trxid, rxgain) -> ()
    _CWA_TRX_GetRxGain                     = 0x80a1,  // CWA_TRX_GetRxGain (trxid) -> (rxgain)
    _CWA_TRX_SetTxPower					   = 0x80b1,  // CWA_TRX_SetTxPower (trxid,txpwr) -> ()
	_CWA_TRX_GetTxPower					   = 0x80c1,  // CWA_TRX_GetTxPower (trxid) -> (txpwr)
	_CWA_TRX_SetOpMode                     = 0x80d2,  // CWA_TRX_SetOpMode (trxid, mode, gain_pwr, rfch, chbw) -> (trxid, mode, gain_pwr)
    _CWA_TRX_GetOpMode                     = 0x80e1,  // CWA_TRX_GetOpMode (trxid) -> (mode, gain_pwr)
    _CWA_TRX_GetStatus                     = 0x80f1,  // CWA_TRX_GetStatus (trxid) -> (trx_status)
    _CWA_TRX_SetCalConfig                  = 0x8101,  // CWA_TRX_SetCalConfig (trxid, cal_config) -> ()
    _CWA_TRX_GetCalConfig                  = 0x8111,  // CWA_TRX_GetCalConfig (trxid) -> (cal_config)
    _CWA_TRX_DoCalibration                 = 0x8121,  // CWA_TRX_DoCalibration (trxid) -> (tbd)
    _CWA_TRX_GetCalStatus                  = 0x8131,  // CWA_TRX_GetCalStatus (trxid) -> (tbd)
    _CWA_TRX_SwitchChannel                 = 0x82f5,   // CWA_TRX_SwitchChannel (trxid,mode,gain_pwr,rfch,chbw,cal_config,mode2,gain_pwr2,rfch2,chbw2) -> (mode,gain_pwr,cal_config,mode2,gain_pwr2,trx_status)
	_CWA_AUX_DoAdcConv					   = 0x81f1,  // CWA_AUX_DoAdcConv (conv_sel) -> ()
	_CWA_AUX_GetAdcResult				   = 0x8200,  // CWA_AUX_GetAdcResult () -> (adc_result, adc_old)
	_CWA_AUX_SetAdcOpMode				   = 0x8271,  // CWA_AUX_SetAdcOpMode (mode) -> ()
	_CWA_AUX_GetAdcOpMode				   = 0x8280,  // CWA_AUX_GetAdcOpMode () -> (mode)
	_CWA_AUX_SetAdcInput				   = 0x8291,  // CWA_AUX_SetAdcInput (pospin,negpin) -> ()
	_CWA_AUX_GetAdcInput				   = 0x82A0,  // CWA_AUX_GetAdcInput () -> (pospin,negpin)
	_CWA_TRX_SetRfAppConfig                = 0x82D4,  // CWA_TRX_SetRfAppConfig (trxid, idle, rx, tx, pa) -> ()
	_CWA_TRX_GetRfAppConfig                = 0x82E1,  // CWA_TRX_GetRfAppConfig (trxid) -> (trxid, idle, rx, tx, pa)
	_CWA_GEN_SetRSB                        = 0x8431,  // CWA_GEN_SetRSB (OnOff) -> ()
    _CWA_CLK_SetXO                         = 0x8461,  // CWA_CLK_SetXO (mode) -> ()
    _CWA_CLK_SetClkPll                     = 0x84c1,  // CWA_CLK_SetClkPll (mode) -> ()
    _CWA_CLK_CalClkPll                     = 0x8480,  // CWA_CLK_CalClkPll () -> (mode, unlocked, vtune_high, vtune_low, vco_toohigh, vco_toolow, vco_cur, vco_cap)
    _CWA_CLK_GetClkPllStatus               = 0x8490,  // CWA_CLK_GetClkPllStatus () -> (mode, unlocked, vtune_high, vtune_low, vco_toohigh, vco_toolow, vco_cur, vco_cap)
    _CWA_TRX_SetTxGain                     = 0x8411,  // CWA_TRX_SetTxGain (trxid, txgain) -> ()
    _CWA_TRX_GetTxGain                     = 0x8421,  // CWA_TRX_GetTxGain (trxid) -> (txgain)
    _CWA_CLK_SetAspBaseClk                 = 0x8441,  // CWA_CLK_SetAspBaseClk (baseclksel) -> ()
    _CWA_TRX_CalWbPll                      = 0x8471,  // CWA_TRX_CalWbPll (trxid, path, mode) -> (max_ok_cal_value, min_ok_cal_value, selected_cal_value)
    _CWA_TRX_GetWbPllStatus                = 0x84e1,  // CWA_TRX_GetWbPllStatus (trxid, path) -> (max_ok_cal_value, min_ok_cal_value, selected_cal_value)
    _CWA_TRX_CalSX                         = 0x84b1,  // CWA_TRX_CalSX (trxid) -> (cal_current, cal_cap)
    _CWA_TRX_CalRxDc                       = 0x84f1,  // CWA_TRX_CalRxDc (trxid) -> ()
};

enum {
    _TRXID_HB = 0,
    _TRXID_LB = 1
};

enum {
    _CWA_TRX_MODE_off                    = 0,
    _CWA_TRX_MODE_onid                   = 1,
    _CWA_TRX_MODE_onidsx                 = 2,
    _CWA_TRX_MODE_onsx                   = 3,
    _CWA_TRX_MODE_onsxtrxid              = 4,
    _CWA_TRX_MODE_onrx                   = 5,
    _CWA_TRX_MODE_ontx                   = 6,
    _CWA_TRX_MODE_ontrxcal               = 7,
};

/**
 ****************************************************************************************
 * @brief write the addr's through ml apb bus using the data
 *
 * @param[in] addr rf register address
 * @param[in] the write data
 * */
void ml_apb_write(uint16_t addr, uint16_t data);
/**
 ****************************************************************************************
 * @brief read the addr's through ml apb bus
 *
 * @param[in] addr rf register address
 *
 * @return return the register value
 * */
uint16_t ml_apb_read(uint16_t addr);

uint16_t ml_apb_RSBUS_read(uint16_t addr);

/**
 ****************************************************************************************
 * @brief write the addr's through lb/hb apb bus using the data
 *
 * @param[in] addr rf register address
 * @param[in] the write data
 * */
void rf_apb_write(uint16_t addr, uint16_t data);
/**
 ****************************************************************************************
 * @brief read the addr's through lb/hb apb bus
 *
 * @param[in] addr rf register address
 *
 * @return return the register value
 * */
uint16_t rf_apb_read(uint16_t addr);

/**
 ****************************************************************************************
 * @brief initialize the rf's logic and some registers which are reference with low band bb
 *
 * @param
 *
 * @return
 * */
uint8_t rf_aetnensis_init_lb(void);
/**
 ****************************************************************************************
 * @brief initialize the rf's logic and some registers which are reference with high band bb
 *
 * @param
 *
 * @return
 * */
uint8_t rf_aetnensis_init_hb(void);
uint8_t init_aetnensis_for_r20170127(uint8_t trx_id, uint8_t init_trx);
int rf_app_prepare(void);
int8_t rf_switch_chan_bw(uint8_t txrxid, uint16_t chan, uint16_t bw);
int8_t rf_trx_prepare(uint8_t txrxid);
int8_t rf_trx_init(uint8_t txrxid);
void rf_trx_stop(uint8_t txrxid);
int8_t rf_send_0_param_cmd(uint16_t rrq, uint16_t cmdid, int l);
int8_t rf_send_1_param_cmd(uint16_t rrq, uint16_t cmdid, uint16_t arg, int l);
int8_t rf_send_2_param_cmd(uint16_t rrq, uint16_t cmdid, uint16_t arg0, uint16_t arg1, int l);
int8_t rf_send_3_param_cmd(uint16_t rrq, uint16_t cmdid, uint16_t arg0, uint16_t arg1, uint16_t arg2, int l);


/*send command through command interface*/
int8_t ml_apb_send_0_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd);
int8_t ml_apb_send_1_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args);
int8_t ml_apb_send_2_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1);
int8_t ml_apb_send_3_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1, uint16_t args2);
int8_t ml_apb_send_4_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1, uint16_t args2, uint16_t args3);
#endif
