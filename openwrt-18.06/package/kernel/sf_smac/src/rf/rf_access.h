
#include "rf_regbank_defines.h"

#ifndef _RF_ACCESS_H_
#define _RF_ACCESS_H_

/**
 * INCLUDE FILES
 * */

#define LOW_BAND 1
#define HIGH_BAND 2

#define NO_BAND_RUN             -2
#define TEMP_ERROR_FROM_LMAC    -1
#define TEMP_ABNORMAL           -50
#define TEMP_DEFAULT            25
#define HK_EXPIRE_INTERVAL      1000 //ms
#define HK_TEMP_DIFF            55

/**
 ****************************************************************************************
 * @brief set the base address for rf_access, this must be doned before any actions to access rf
 *
 * @param[in] void *base the base address for accessing
 *
 * */
void rf_set_access_base(void *base);

/**
 ****************************************************************************************
 * @brief initialize the RF's modules during bootup
 *
 * @param[in]
 *
 * @return 0 success, otherwise failed
 *
 * */
int rf_bootup(void);

/**
 ****************************************************************************************
 * @brief app calibrate, in fact, it just do the app mode transition(2->4)
 *
 * @param[in] bool reactive, if need deactive>active the rf
 *
 * @return 0 success, otherwise failed
 *
 * */
int rf_app_calibrate(bool reactive);


/**
 ****************************************************************************************
 * @brief app calibrate, in fact, it just do the app calibrate restore to 0xFF
 *
 * @param[in]
 *
 * @return 0 success, otherwise failed
 *
 * */
int rf_app_calibrate_restore(void);

/**
 ****************************************************************************************
 * @brief load the rf firmware, if run to 1, then trigger the firmware to run
 *      if not, just load the firmware
 * @param[in] uint16_t *data, the firmware source data
 *       [in] uint32_t size, the firmware size
 *       [in] uint8_t run, wether to trigger the firmware to run
 * */
void ml_fw_load(uint16_t *data, uint32_t size, uint8_t run);

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

/**
 ****************************************************************************************
 * @brief read rs bus registers through ml apb bus
 *
 * @param[in] addr rs register address
 *
 * @return return the register value
 * */
uint16_t ml_apb_rsbus_read(uint16_t addr);

/**
 ****************************************************************************************
 * @brief init the aetnensis by scripts, this do not contain a commmand firmware
 *
 * @param[in] band type
 *
 * @return 0 success, otherwise failed
 * */
int8_t init_aetnensis_by_script(uint8_t band, uint8_t trx);

/**
 ****************************************************************************************
 * @brief check the app status
 *
 * @param[in]
 * */
void aet_app_check_status(void);

/**
 ****************************************************************************************
 * @brief get the rf tempperture
 *
 * @param[in]
 *
 * @return tempure
 *
 * */
int rf_get_temperature(bool from_lmac);
int32_t  rf_trx_status_change(int txpower_idx,uint32_t txrxid,bool txrx);
int8_t rf_switch_fem_ctrl(uint8_t fem_ctl);
int rf_trx_stop(int trxid);
int rf_trx_set_rfappconfig(
               uint8_t conf_in_idel_hb1, uint8_t conf_in_idel_hb2,uint8_t conf_in_rx_hb1, uint8_t conf_in_rx_hb2,
               uint8_t conf_in_tx_hb1, uint8_t conf_in_tx_hb2, uint8_t conf_in_pa_hb1, uint8_t conf_in_pa_hb2,
               uint8_t conf_in_idel_lb1, uint8_t conf_in_idel_lb2,uint8_t conf_in_rx_lb1, uint8_t conf_in_rx_lb2,
               uint8_t conf_in_tx_lb1, uint8_t conf_in_tx_lb2, uint8_t conf_in_pa_lb1, uint8_t conf_in_pa_lb2
               );

#ifdef UPDATE_GAIN_TABLE
#define SF19A28_RF_GAIN_NUM 32
#define SF19A28_RF_DBB_GAIN_NUM 16
extern  uint16_t rf_lb_dbb_gain_table[SF19A28_RF_DBB_GAIN_NUM];
extern  uint16_t rf_lb_dbb_gain_table_def[SF19A28_RF_DBB_GAIN_NUM];
extern  uint16_t rf_hb_gain_table[SF19A28_RF_GAIN_NUM];
extern  uint16_t rf_lb_gain_table[SF19A28_RF_GAIN_NUM];
extern  uint16_t rf_lb_gain_table_def[SF19A28_RF_GAIN_NUM];
/**
 ****************************************************************************************
 * @brief set rf gain table change
 *
 * @param[in] table address ,gain address,gian num
 *
 * @return N/A
 *
 * */
void rf_update_gain_table(uint16_t table_base_addr, uint16_t *table_addr,int gain_num);

    /**
 ****************************************************************************************
 * @brief set rf gain table for low power
 *
 * @param[in]  new gain table
 *
 * @return N/A
 *
 * */
void  rf_update_hb_tx_gain_table(void);
/**
 ****************************************************************************************
 * @brief reset  rf gain table for low power
 *
 * @param[in]  new gain table
 *
 * @return N/A
 *
 * */
void  rf_reset_hb_tx_gain_table(void);
#endif

//when use two calibrate table you should use different default gain for rf
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
#ifdef CFG_DUAL_ANTENNA_CALIBRATE_CSD
#define RF_HB_BASE_GAIN 3
#else
#define RF_HB_BASE_GAIN 10
#endif
#define RF_LB_BASE_GAIN 16
#define RF_HB_BASE_GAIN_DIFF 3
#endif

#ifdef CONFIG_SFA28_FULLMASK
int32_t rf_get_trx_status(bool is_hb, uint16_t *trx_state, uint16_t *freq, uint16_t *chbw);
int16_t rf_set_property(uint8_t property_id, uint16_t value);
uint16_t rf_get_property(uint8_t property_id);

int8_t ml_apb_send_0_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd);
int8_t ml_apb_send_1_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args);
int8_t ml_apb_send_2_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1);
int8_t ml_apb_send_3_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1, uint16_t args2);
int8_t ml_apb_send_4_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1, uint16_t args2, uint16_t args3);
#endif
#endif
