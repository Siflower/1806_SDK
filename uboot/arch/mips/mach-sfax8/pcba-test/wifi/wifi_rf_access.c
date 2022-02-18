#ifndef __WIFI_RF_ACCESS_H__
#define __WIFI_RF_ACCESS_H__
#include <common.h>
#include <asm/io.h>
#include "wifi_addr.h"
#include "rwnx_config.h"
#include "reg_rc.h"
#include "rf_regbank_defines.h"
#include "rf_access.h"
#include "rf_def_data.h"
#include "reg_macbypass.h"
#include "wifi_ldfw.h"
#define LB_COMMANDIF_BASE_ADDR 0x1440
#define HB_COMMANDIF_BASE_ADDR 0x1400
#define MAX_RETRY_TIME 100

#define INIT_RX_GAIN_INDEX 63
#define MAX_TRX_CALIBRATE_CONF 0x3FF
#define SEC_TRX_CALIBRATE_CONF 0x20F
#define CHECK_VERSION(hb) \
    {   \
        __UNUSED int8_t resp_number; \
        __UNUSED uint16_t value; \
        uint16_t cmd_base = ((hb) ? HB_CMD_IF_BASE : LB_CMD_IF_BASE);   \
        resp_number = ml_apb_send_0_params_cmd(1, cmd_base, _CWA_GEN_GetVersion);  \
		printf("resp_number : %d\n", resp_number);	\
        value = ml_apb_read(cmd_base + 0x000B);  \
        printf("hw version : 0x%x\n", value);   \
        value = ml_apb_read(cmd_base + 0x000C);  \
        printf("sw version : 0x%x\n", value);   \
		value = ml_apb_read(cmd_base + 0x000D);  \
        printf("patch version :0x%x\n", value); \
	}


struct rf_cmd
{
    uint16_t cmd_id;
    int16_t resp_argc;
    char    cmd_name[64];

};

struct rf_cmd g_rf_cmd[] =
{
    {
        _CWA_GEN_GetRegister,
        1,
        "_CWA_GEN_GetRegister"
    },
    {
        _CWA_GEN_SetRegister,
        0,
        "_CWA_GEN_SetRegister"
    },
    {
        _CWA_TRX_SwitchMode,
        0,
        "_CWA_TRX_SwitchMode"
    },
    {
        _CWA_GEN_GetVersion,
        3,
        "_CWA_GEN_GetVersion"
    },
    {
        _CWA_APP_SetOpMode,
        0,
        "_CWA_APP_SetOpMode"
    },
    {
        _CWA_APP_GetOpMode,
        1,
        "_CWA_APP_GetOpMode"
    },
    {
        _CWA_APP_GetStatus,
        1,
        "_CWA_APP_GetStatus"
    },
    {
        _CWA_APP_SetCalConfig,
        0,
        "_CWA_APP_SetCalConfig"
    },
	{
        _CWA_APP_GetCalConfig,
        1,
        "_CWA_APP_GetCalConfig"
    },
	{
        _CWA_APP_DoCalibration,
        0,
        "_CWA_APP_DoCalibration"
    },
	{
        _CWA_APP_GetCalStatus,
        1,
        "_CWA_APP_GetCalStatus"
    },
	{
        _CWA_APP_SetHKConfig,
        0,
        "_CWA_APP_SetHKConfig"
    },
	{
        _CWA_APP_GetHKConfig,
        4,
        "_CWA_APP_GetHKConfig"
    },
	{
        _CWA_TRX_SetRFChannel,
        0,
        "_CWA_TRX_SetRFChannel"
    },
    {
        _CWA_TRX_GetRFChannel,
        1,
        "_CWA_TRX_GetRFChannel"
    },
    {
        _CWA_TRX_SetChannelBW,
        0,
        "_CWA_TRX_SetChannelBW"
    },
    {
        _CWA_TRX_GetChannelBW,
        1,
        "_CWA_TRX_GetChannelBW"
    },
    {
        _CWA_TRX_SetRxGain,
        0,
        "_CWA_TRX_SetRxGain"
    },
    {
        _CWA_TRX_GetRxGain,
        1,
        "_CWA_TRX_GetRxGain"
    },
	{
        _CWA_TRX_SetTxPower,
        0,
        "_CWA_TRX_SetTxPower"
    },
    {
        _CWA_TRX_GetTxPower,
        1,
        "_CWA_TRX_GetTxPower"
    },
	{
        _CWA_TRX_SetOpMode,
        1,
        "_CWA_TRX_SetOpMode"
    },
    {
        _CWA_TRX_GetOpMode,
        1,
        "_CWA_TRX_GetOpMode"
    },
    {
        _CWA_TRX_GetStatus,
        1,
        "_CWA_TRX_GetStatus"
    },
    {
        _CWA_TRX_SetCalConfig,
        0,
        "_CWA_TRX_SetCalConfig"
    },
    {
        _CWA_TRX_GetCalConfig,
        1,
        "_CWA_TRX_GetCalConfig"
    },
    {
        _CWA_TRX_DoCalibration,
        0,
        "_CWA_TRX_DoCalibration"
    },
    {
        _CWA_TRX_GetCalStatus,
        1,
        "_CWA_TRX_GetCalStatus"
    },
	{
		_CWA_TRX_SwitchChannel,
		4,
		"_CWA_TRX_SwitchChannel"
	},
	{
		_CWA_TRX_SetRfAppConfig,
		0,
		"_CWA_TRX_SetRfAppConfig"
	},
	{
		_CWA_TRX_GetRfAppConfig,
		4,
		"_CWA_TRX_GetRfAppConfig"
	},
	{
		_CWA_AUX_DoAdcConv,
        0,
        "_CWA_AUX_DoAdcConv"
    },
	{
        _CWA_AUX_GetAdcResult,
        1,
        "_CWA_AUX_GetAdcResult"
    },
	{
        _CWA_AUX_SetAdcOpMode,
        0,
        "_CWA_AUX_SetAdcOpMode"
    },
	{
        _CWA_AUX_GetAdcOpMode,
        1,
        "_CWA_AUX_GetAdcOpMode"
    },
	{
        _CWA_AUX_SetAdcInput,
        0,
        "_CWA_AUX_SetAdcInput"
    },
	{
        _CWA_AUX_GetAdcInput,
        1,
        "_CWA_AUX_GetAdcInput"
    },
	{
        _CWA_TRX_GetTxGain,
        1,
        "_CWA_TRX_GetTxGain"
    },
};


//#define ENABLE_TESTTONE_GEN
void rf_switch_lb(uint8_t mode, uint8_t gain)
{
	uint32_t tmp = 0;
    //`RCAETNENSIS_set_sw_rfdata1_ctrl({16'b0,4'h1,TXRXMode,Gain});
    tmp = (0x0000 << 16 ) | (0x1 << 12) | ((mode & 0x0F) << 8) | gain;
    rc_sw_rfdata1_ctrl_set(tmp);


    //`RCAETNENSIS_set_sw_rfcmd_ctrl({0xa000,{4'h0, 8'h3, 4'h1}});
    tmp = 0;
    tmp = (0xa000 << 16) | (0x3 << 4) | 0x1;
    rc_sw_rfcmd_ctrl_set(tmp);
    udelay(1);
}

void rf_switch_hb(uint8_t mode, uint8_t gain)
{
	uint32_t tmp = 0;
    //`RCAETNENSIS_set_sw_rfdata1_ctrl({16'b0,4'h1,TXRXMode,Gain});
    tmp = (0x0000 << 16 ) | (0x0 << 12) | ((mode & 0x0F) << 8) | gain;
    rc_sw_rfdata1_ctrl_set(tmp);


    //`RCAETNENSIS_set_sw_rfcmd_ctrl({0xa000,{4'h0, 8'h3, 4'h1}});
    tmp = 0;
    tmp = (0xa000 << 16) | (0x3 << 4) | 0x1;
    rc_sw_rfcmd_ctrl_set(tmp);
    udelay(1);
}

//this API is working for the command _CWA_GEN_GetRegister
uint8_t rf_get_reg_value(uint16_t addr, uint16_t *cmd_response, uint16_t *cmd_rddata)
{
    uint32_t tmp = 0;

    //`RCAETNENSIS_set_sw_rfdata1_ctrl({0xa5a5,0x0801});
    tmp = (0xa5a5 << 16) | addr;
    rc_sw_rfdata1_ctrl_set(tmp);

    //`RCAETNENSIS_set_sw_rfcmd_ctrl({0xa000,0x8011});
    tmp = (0xa000 << 16) | 0x8011;
    rc_sw_rfcmd_ctrl_set(tmp);

    udelay(10);

    // Clear DATA1 value
    //`RCAETNENSIS_set_sw_rfdata1_ctrl({32'h0});
    tmp = 0;
    rc_sw_rfdata1_ctrl_set(tmp);
    // Response Request
    // `RCAETNENSIS_set_sw_rfcmd_ctrl({0xe000,0x0001});
    tmp = (0xe000 << 16) | 0x0001;
    rc_sw_rfcmd_ctrl_set(tmp);

    udelay(10);

    // read response header
    // `RCAETNENSIS_getf_HEADER_OR_ADDR(cmd_response);
    *cmd_response = rc_header_or_addr_getf();
    udelay(10);

    if (*cmd_response != 0x0011)
    {
#ifndef CFG_RWTL
        printf("error: Received Bad API Command Response : expected 0x0011, received : 0x%x\n", *cmd_response);
        printf("Simulation FAILURE\n");
#endif
		return 1;
    }
    // read response data
    *cmd_rddata = rc_data1_getf();
    return 0;
}

void rf_apb_write(uint16_t addr, uint16_t data)
{
    uint32_t tmp;
    tmp = (0x0000 << 16) | data;
    rc_sw_rfdata1_ctrl_set(tmp);

    tmp = (0x8000 << 16) | addr;
    rc_sw_rfcmd_ctrl_set(tmp);
    udelay(1);
}

void ml_apb_write(uint16_t addr, uint16_t data)
{
    //this is just a workaround for the continuous writing for the default register loading
    if(addr == 0xFFFF){
        mdelay(data);
        return;
    }

	REG_PL_WR16((REG_CATRF_BASE_ADDR + (addr << 1)), data);
	//because for RSBUS register, it need time to take effect.
    //RSBUS range : 0x1300 ~ 0x13EF
    if(addr < 0x1300 || addr > 0x13EF)
        return;
    udelay(30);
}

uint16_t ml_apb_read(uint16_t addr)
{
    return REG_PL_RD16((REG_CATRF_BASE_ADDR + (addr << 1)));
}

uint16_t ml_apb_RSBUS_read(uint16_t addr)
{
    uint16_t value;
    REG_PL_RD16((REG_CATRF_BASE_ADDR + (addr << 1)));
    while(1){
        udelay(1);
        value = REG_PL_RD16((REG_CATRF_BASE_ADDR + (0x13F0 << 1)));
        if(value & 0x4)
            break;
    }
	return REG_PL_RD16((REG_CATRF_BASE_ADDR + (0x13F1 << 1)));
}

static int16_t rf_find_cmd_resp_arg_num(uint16_t cmdid, int l)
{
    int32_t i;
    for(i = 0; i < sizeof(g_rf_cmd)/sizeof(g_rf_cmd[0]); i++)
    {
        if(cmdid == g_rf_cmd[i].cmd_id){
            return g_rf_cmd[i].resp_argc;
        }
    }
	printf("cmdid : 0x%x, l : %d\n", cmdid, l);
    ASSERT_ERR(0);
    return 0;
}

uint16_t rf_cmd_rsp_get(int16_t response_argc, int l)
{
    int rsp_to = 2000;
    uint16_t rsphdr = 0;

    do
    {
        int loops = 1 << 20;

        // Start the transaction
        rc_sw_rfcmd_ctrl_set(RC_START_DONE_BIT | RC_READNOTWRITE_BIT | RC_APICMD_BIT | response_argc);


        // Poll for the start_done bit going back to 0
        while (rc_start_done_getf() && --loops){
            udelay(10);
        }
        if (!loops)
        {
#ifndef CFG_RWTL
            printf("%s:%d:%d failed\n", __func__, __LINE__, l);
#endif
            return 0;
        }

        // Read the response header
        rsphdr = rc_header_or_addr_getf();
        if ((!(rsphdr & TS_BIT)) && ((rsphdr & RSPID_MSK) >> RSPID_OFT))
            break;

        // Wait a bit between each attempt
        udelay(50);
    } while (--rsp_to);

    // Ensure we got a response
    if (!rsp_to)
    {
#ifndef CFG_RWTL
        printf("%s:%d:%d failed\n", __func__, __LINE__, l);
#endif
        return 0;
    }
    return (rsphdr);
}


int8_t rf_cmd_send(uint16_t rrq, uint16_t cmdhdr, int l)
{
    int loops = 1 << 20;
    uint8_t rsp_argc = 0;


    // Start the transaction
    rc_sw_rfcmd_ctrl_set(RC_START_DONE_BIT | RC_APICMD_BIT |
                         (cmdhdr << RC_HEADER_OR_ADDR_LSB));

    // Poll for the start_done bit going back to 0
    while (rc_start_done_getf() && --loops);
    if (!loops)
    {
        dbg("%s:%d:%d - %04X failed\n", __func__, __LINE__, l, cmdhdr);
        return -1;
    }

    // If a response is needed, wait for it
    if (rrq)
    {
        uint16_t rsphdr;//rf_cmd_rsp_get(l);
        uint16_t rspid;
        uint16_t cmdid;
        int32_t response_argc = rf_find_cmd_resp_arg_num(cmdhdr, l);
		uint8_t flag = 1;

		if(!response_argc){
			response_argc = 1;
			flag = 0;
		}
		rsphdr = rf_cmd_rsp_get(response_argc, l);

		rspid = (rsphdr & RSPID_MSK) >> RSPID_OFT;
        cmdid = (cmdhdr & RSPID_MSK) >> RSPID_OFT;

        // Check if the response is valid
		if ((rsphdr & EF_BIT) || (rspid != cmdid) || (((rsphdr & RSP_ARGC_MSK) != response_argc) & flag))
		{
#ifndef CFG_RWTL
            printf("%s:%d:%d - %04X Rsp Error - EF %d, RSPID %04X, ERRCODE %04X\n", __func__, __LINE__, l,
                cmdid, (rsphdr & EF_BIT) != 0, rspid, rc_data1_getf());
#endif
            return -2;
        }

        // Get the number of response arguments
        rsp_argc = (rsphdr & RSP_ARGC_MSK) >> RSP_ARGC_OFT;
    }

    return (rsp_argc);
}

int8_t rf_send_0_param_cmd(uint16_t rrq, uint16_t cmdhdr, int l)
{
    // Send the command and wait for the response if needed
    return(rf_cmd_send(rrq, cmdhdr, l));
}


int8_t rf_send_1_param_cmd(uint16_t rrq, uint16_t cmdhdr, uint16_t arg, int l)
{
    // Set the parameter
    rc_sw_rfdata1_ctrl_set(arg);

    // Send the command and wait for the response if needed
    return(rf_cmd_send(rrq, cmdhdr, l));
}

//disable it because now no one used this function, fix the compile waring
int8_t rf_send_2_param_cmd(uint16_t rrq, uint16_t cmdhdr, uint16_t arg0, uint16_t arg1, int l)
{
    // Set the parameters
    rc_sw_rfdata1_ctrl_set((arg1 << RC_DATA2_LSB) | arg0);

    // Send the command and wait for the response if needed
    return(rf_cmd_send(rrq, cmdhdr, l));
}

int8_t rf_send_3_param_cmd(uint16_t rrq, uint16_t cmdhdr, uint16_t arg0, uint16_t arg1, uint16_t arg2, int l)
{
    // Set the parameters
    rc_sw_rfdata1_ctrl_set((arg1 << RC_DATA2_LSB) | arg0);
    rc_sw_rfdata2_ctrl_set(arg2);

    // Send the command and wait for the response if needed
    return(rf_cmd_send(rrq, cmdhdr, l));
}

int8_t rf_send_4_param_cmd(uint16_t rrq, uint16_t cmdhdr, uint16_t arg0, uint16_t arg1, uint16_t arg2, uint16_t arg3, int l)
{
    // Set the parameters
    rc_sw_rfdata1_ctrl_set((arg1 << RC_DATA2_LSB) | arg0);
    rc_sw_rfdata2_ctrl_set((arg3 << RC_DATA2_LSB) | arg2);

    // Send the command and wait for the response if needed
    return(rf_cmd_send(rrq, cmdhdr, l));
}

int8_t rf_send_5_param_cmd(uint16_t rrq, uint16_t cmdhdr, uint16_t arg0, uint16_t arg1, uint16_t arg2, uint16_t arg3, uint16_t arg4, int l)
{
    // Set the parameters
    rc_sw_rfdata1_ctrl_set((arg1 << RC_DATA2_LSB) | arg0);
    rc_sw_rfdata2_ctrl_set((arg3 << RC_DATA2_LSB) | arg2);
	rc_sw_rfdata3_ctrl_set(arg4);

    // Send the command and wait for the response if needed
    return(rf_cmd_send(rrq, cmdhdr, l));
}

static int8_t ml_apb_wait_cmd_resp(uint16_t rrq, uint16_t cmd_base, uint16_t cmd)
{
    uint16_t value;
    if(!rrq)
        return 0;
    while(1){
        value = ml_apb_read(cmd_base + 0x000A);
        if(!((value >> 14) & 0x1) && ((value >> 4) & 0x3FF))
            break;
        udelay(10);
    }
    if(value & 0x8000)
    {
        dbg("[cmd : 0x%x]error happen!\n", (cmd >> 4) & 0x3FF);
        return 0;
    }
    if(((value >> 4) & 0x3FF) != ((cmd >> 4) & 0x3FF))
    {
        dbg("[cmd : 0x%x]response command ID (0x%x) is not correct!\n",
                (cmd >> 4) & 0x3FF, (value >> 4) & 0x3FF);
        return 0;
    }
    return (value & 0xF);

}

int8_t ml_apb_send_0_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd)
{
    ml_apb_write(cmd_base, cmd);
    return ml_apb_wait_cmd_resp(rrq, cmd_base, cmd);
}

int8_t ml_apb_send_1_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args)
{
    ml_apb_write(cmd_base, cmd);
    ml_apb_write(cmd_base + 0x0001, args);
    return ml_apb_wait_cmd_resp(rrq, cmd_base, cmd);
}

int8_t ml_apb_send_2_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1)
{
    ml_apb_write(cmd_base, cmd);
    ml_apb_write(cmd_base + 0x0001, args0);
    ml_apb_write(cmd_base + 0x0002, args1);

    return ml_apb_wait_cmd_resp(rrq, cmd_base, cmd);
}

int8_t ml_apb_send_3_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1, uint16_t args2)
{
    ml_apb_write(cmd_base, cmd);
    ml_apb_write(cmd_base + 0x0001, args0);
    ml_apb_write(cmd_base + 0x0002, args1);
    ml_apb_write(cmd_base + 0x0003, args2);
    return ml_apb_wait_cmd_resp(rrq, cmd_base, cmd);
}

int8_t ml_apb_send_4_params_cmd(uint16_t rrq, uint16_t cmd_base, uint16_t cmd, uint16_t args0, uint16_t args1, uint16_t args2, uint16_t args3)
{
    ml_apb_write(cmd_base, cmd);
    ml_apb_write(cmd_base + 0x0001, args0);
    ml_apb_write(cmd_base + 0x0002, args1);
    ml_apb_write(cmd_base + 0x0003, args2);
    ml_apb_write(cmd_base + 0x0004, args3);
    return ml_apb_wait_cmd_resp(rrq, cmd_base, cmd);
}

uint8_t rf_single_check_reg(uint16_t addr, uint16_t expect)
{
    if(expect != ml_apb_read(addr))
    {
        return 1;
    }
    return 0;
}

uint8_t rf_aetnensis_init_lb()
{
    uint16_t cmd_rddata = 0;
    uint16_t cmd_response = 0;
    uint8_t ret = 0;
    //set the rc command API base address
    rc_sw_rfapicmd_addr_set(0x1440);

    /**how to load rf pmem(program memory) and dram0/1
     * 1,host should program the control register at address 0x4001 via ML_APB or I2C, in order to set the
     *   disired memory that will be filled:PRAM,DRAM1,DRAM2
     * 2,host program a pointer with the start address of the memory image.Usually the start address will be zero,
     *   in case the full memory will be written. It will always be written from low to high address range.The
     *   pointer is located at address 0x4000
     * 3,host program the control register if the access is 16b or 32b
     * 4,The host does one write to 0x4002.This address is the 'port' to the select memory.The first write will end up
     *   at the address which was specifiled wi the pointer that was programed at step2
     * 5,The host continue writing to address 0x4002, In case of I2C, there is an auto increment function and the address
     *   doest not need to be delivered for the 2nd and following writes. For ML_APB the same address 0x4002, need to be repeated.
     * 6,when the complete FW img is transfered , the host writes another bit in the control register 0x4001 which release the processor
     *   from its frozen state. The processor will then start to execute the FW from PMEM address 0x0000 onwards
     * */
#ifdef LDFW_RF
	load_rf_pmem_bin();
#else
    //TODO, make sure that IC team has load the firmware by themselves
    // Emulation of PMEM loading, necessary for defreezing the X2 DSP
    ml_apb_write(PMEM_CTRL, 0x0);
    udelay(20);
#endif
    // Select clock frequency: X2/XBUS: 160 MHz, RSBUS: 160 MHz, Watchdog: 80 MHz
    ml_apb_write(CLOCK_SELECTION, 0x55 );
    //Enable all clocks through LB APB @
    ml_apb_write(CLOCK_ENABLES, 0x1fff );

    // Deassert RSBUS software reset
    rf_apb_write(SWRESET, 0x0000);
    //Enable LOWBAND clocks through LB APB
    rf_apb_write(LB_CLOCK_ENABLE, 0x0003 );
    // Configuration of Data Switch Matrix:
    // IQ data from LOWBAND RX DBB routed to  LOWBAND RX PINS
    rf_apb_write(DSM_CTRL_REG, 0x0000);

    //GetVersion ID
    CHECK_VERSION(0);

    //LOWBAND: RX GAIN TABLE
    rf_apb_write(LB_RX_GAIN_TABLE_0, 0x0000 );
    rf_apb_write(LB_RX_GAIN_TABLE_31, 0x0EEE );
    rf_apb_write(LB_RX_GAIN_TABLE_63, 0xFFFF );

    // RX ANALOG BASEBAND GAIN TABLE
    rf_apb_write(LB_RX_ABB_GAIN_TABLE_0_1,      0x0000);
    rf_apb_write(LB_RX_ABB_GAIN_TABLE_6_7,      0x0110);
    rf_apb_write(LB_RX_ABB_GAIN_TABLE_14_15,      0x3C1F);


    // RX DC OFFSET CORRECTION GAIN TABLE
    rf_apb_write( LB_RX_ABB_CH0_DCOC_I_TABLE_0,    0x0000);
    rf_apb_write( LB_RX_ABB_CH0_DCOC_I_TABLE_2,    0x0202);
    rf_apb_write( LB_RX_ABB_CH0_DCOC_I_TABLE_7,    0x2121);

    // DBB FINE GAIN TABLE
    rf_apb_write(  LB_RX_DBB_FINE_GAIN_TABLE_0,    0x02D4);
    rf_apb_write(  LB_RX_DBB_FINE_GAIN_TABLE_8,    0x041D);
    rf_apb_write( LB_RX_DBB_FINE_GAIN_TABLE_15,    0x05A6);

    // DBB COARSE GAIN TABLE
    rf_apb_write(LB_RX_DBB_COARSE_GAIN_TABLE,      0x3210);

    // -- TX Gain -------------------------------------------------------
    // GAIN TABLE
    rf_apb_write(  LB_TX_GAIN_TABLE_0,             0x0000);
    rf_apb_write(  LB_TX_GAIN_TABLE_7,             0x0077);
    rf_apb_write( LB_TX_GAIN_TABLE_31,             0x01FF);

    // RF GAIN TABLE
    rf_apb_write(   LB_TX_RF_GAIN_TABLE_0_1,       0x0900);
    rf_apb_write( LB_TX_RF_GAIN_TABLE_18_19,       0x1818);
    rf_apb_write( LB_TX_RF_GAIN_TABLE_30_31,       0x3830);

    // DC Offset TABLE I
    rf_apb_write(  LB_TX_DBB_DCOC_I_TABLE_0,        0x0000);
    rf_apb_write( LB_TX_DBB_DCOC_I_TABLE_13,        0x0CB2);
    rf_apb_write( LB_TX_DBB_DCOC_I_TABLE_31,        0x0FFF);

    // DC Offset TABLE Q
    rf_apb_write(  LB_TX_DBB_DCOC_Q_TABLE_0,        0x0000);
    rf_apb_write( LB_TX_DBB_DCOC_Q_TABLE_10,        0x0A28);
    rf_apb_write( LB_TX_DBB_DCOC_Q_TABLE_31,        0x0FFF);

    // DBB TABLE
    rf_apb_write(  LB_TX_DBB_GAIN_TABLE_0,          0x02D4);
    rf_apb_write( LB_TX_DBB_GAIN_TABLE_12,          0x0401);
    rf_apb_write( LB_TX_DBB_GAIN_TABLE_15,          0x05A6);

    // Firmware emulation: Configuration of RXTX chain
    rf_apb_write(LB_BB_RSBUS_CTRL14,       LB_TX_BW_20M_TDEF_INT_ACT);
    rf_apb_write(LB_RX_BW,                 RX_BW_20M_STR_40M_TDEF);
    rf_apb_write(LB_TX_BW,                 LB_TX_BW_20M_TDEF_INT_ACT);
    rf_apb_write(LB_RX_ADC_ENABLE,         RX_ADC_ACTIVE_EN_STDBY_GEC);
    rf_apb_write(LB_TX_DAC_ENABLE,         TX_DAC_ACTIVE_EN_STDBY_GEC);
    rf_apb_write(LB_RX_CLOCK_ENABLE,       RX_CLK_ON_GEC);
    rf_apb_write(LB_TX_CLOCK_ENABLE,       TX_CLK_ON_GEC);
    rf_apb_write(LB_RX_DBB_IQC_CONFIG,     RX_IQC_ON);
    rf_apb_write(LB_TX_DBB_CONFIG,         TX_IQC_ON);
    rf_apb_write(LB_BB_RSBUS_CTRL1,        BB_VREG_BL_RX_BDEF);
    rf_apb_write(LB_BB_RSBUS_CTRL0,        BB_RX_ACTIVE_EN_GEC);
    rf_apb_write(LB_BB_RSBUS_CTRL8,        BB_TX_ACTIVE_EN_GEC);
    rf_apb_write(LB_BB_RSBUS_CTRL9,        BB_TX_BDEF);

    // 1 -  Fill-in RAM0 for RX switch (emulation of future FIRMWARE)
    ml_apb_write(0x2005, 0x0c1); // TRX_ON_RX0_STDBY_TXDAC
    // Reserve last addresses in memory RAM0
    ml_apb_write(0x2fff,  0x0c7); // TRX_ON_RX1_STDBY_TXDAC
    ml_apb_write(0x2ffe,  0x0cf); // TRX_ON_RX_STDBY_TXDAC
    ml_apb_write(0x2ffd , 0x0001              ); // lb_rx_iqdata_valid
    rf_single_check_reg(0x2005, 0x0c1);
    rf_single_check_reg(0x2fff, 0x0c7);
    rf_single_check_reg(0x2ffe, 0x0cf);
    rf_single_check_reg(0x2ffd, 0x001);

    // 2- Fill-in LOWBAND XDMA TABLE (emulation of future FIRMWARE)
    // 1st LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE,         DATA_RAM_0_TOP      );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  1,   LB_ANALOG_ENABLE    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  2,   0x8001            );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  3,   0x0020            );

    // 2nd LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  4,   DATA_RAM_0_TOP  - 1 );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  5,   LB_ANALOG_ENABLE    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  6,   0x8001            );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  7,   0x0040            );

    // 3rd LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  8,   DATA_RAM_0_TOP  - 2 );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  +  9,   LB_RX_IQDATA_VALID  );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 10,   0x0001            );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 11,   0x0100            );


    // API COMMAND to put RF in RX mode with RxgainIndex set to 31
    rf_switch_lb(0x3, 0x1e);

    // 1st LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 256, DATA_RAM_0_TOP - 3    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 257, LB_ANALOG_ENABLE      );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 258, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 259, 0x0000              );

    // 2nd LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 260, DATA_RAM_0_TOP - 4     );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 261, LB_ANALOG_ENABLE     );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 262, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 263, 0x0020              );

    // 3rd LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 264, DATA_RAM_0_TOP - 7    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 265, LB_TX_BIAS_SEL        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 266, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 267, 0x0020              );

    // 4th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 268, DATA_RAM_0_TOP - 10   );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 269, LB_TX_GAIN_PTR        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 270, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 271, 0x0000              );

    // 5th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 272, DATA_RAM_0_TOP - 5    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 273, LB_ANALOG_ENABLE      );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 274, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 275, 0x0020              );

    // 6th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 276, DATA_RAM_0_TOP - 6    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 277, LB_ANALOG_ENABLE      );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 278, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 279, 0x0040              );

    // 7th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 280, DATA_RAM_0_TOP - 8    );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 281, LB_TX_BIAS_SEL        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 282, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 283, 0x0040              );

    // 8th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 284, DATA_RAM_0_TOP - 9   );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 285, LB_TX_BIAS_SEL        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 286, 0x8001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 287, 0x0020              );

    // 9th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 288, DATA_RAM_0_TOP - 11   );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 289, LB_TX_GAIN_PTR        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 290, 0x8001             );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 291, 0x0020             );

    // 10th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 292, DATA_RAM_0_TOP - 12   );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 293, LB_TX_GAIN_PTR        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 294, 0x8001  );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 295, 0x0020              );

    // 11th LLI
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 296, DATA_RAM_0_BASE + 9   );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 297, LB_TX_GAIN_PTR        );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 298, 0x0001              );
    rf_apb_write(XDMA_TABLE_LB_SPACE_BASE  + 299, 0x0020              );

    // Check RX GAIN value with an API GET_REGISTER command
    ret = rf_get_reg_value(LB_RX_GAIN_PTR, &cmd_response, &cmd_rddata);
    if (ret == 0 &&
            cmd_rddata != 0x001e){
#ifndef CFG_RWTL
        printf("error: Bad RX GAIN register value expected :8'h1f , received:0x%x\n", cmd_rddata);
        printf("Simulation FAILURE\n");
#endif
        ret = 2;
    }

	return ret;
}

uint8_t rf_aetnensis_init_hb()
{
    uint16_t cmd_rddata = 0;
    uint16_t cmd_response = 0;
    uint8_t ret = 0;
    //set the rc command API base address
    rc_sw_rfapicmd_addr_set(0x1400);

    /**how to load rf pmem(program memory) and dram0/1
     * 1,host should program the control register at address 0x4001 via ML_APB or I2C, in order to set the
     *   disired memory that will be filled:PRAM,DRAM1,DRAM2
     * 2,host program a pointer with the start address of the memory image.Usually the start address will be zero,
     *   in case the full memory will be written. It will always be written from low to high address range.The
     *   pointer is located at address 0x4000
     * 3,host program the control register if the access is 16b or 32b
     * 4,The host does one write to 0x4002.This address is the 'port' to the select memory.The first write will end up
     *   at the address which was specifiled wi the pointer that was programed at step2
     * 5,The host continue writing to address 0x4002, In case of I2C, there is an auto increment function and the address
     *   doest not need to be delivered for the 2nd and following writes. For ML_APB the same address 0x4002, need to be repeated.
     * 6,when the complete FW img is transfered , the host writes another bit in the control register 0x4001 which release the processor
     *   from its frozen state. The processor will then start to execute the FW from PMEM address 0x0000 onwards
     * */
#ifdef LDFW_RF
	load_rf_pmem_bin();
#else
    ml_apb_write(PMEM_CTRL, 0x0000);
    udelay(20);
#endif

    // Select clock frequency: X2/XBUS: 160 MHz, RSBUS: 160 MHz, Watchdog: 80 MHz
    ml_apb_write(CLOCK_SELECTION, 0x0055);
    //Enable all clocks through HB APB @
    ml_apb_write(CLOCK_ENABLES, 0x1fff);

    // Deassert RSBUS software reset
    ml_apb_write(SWRESET, 0x0000);
    //Enable HIGHBAND clocks through HB APB
    ml_apb_write(HB_CLOCK_ENABLE, 0x0003 );
    // Configuration of Data Switch Matrix:
    // IQ data from HIGHBADN RX DBB routed to HIGHBAND RX PINS
    ml_apb_write(DSM_CTRL_REG, 0x0000);

    //Get Version
    CHECK_VERSION(1);

    // Fill-in RAM0 for RX switch (emulation of future FIRMWARE)
    //HIGHBAND: RX GAIN TABLE
    ml_apb_write(HB_RX_GAIN_TABLE_0, 0x0000);
    ml_apb_write(HB_RX_GAIN_TABLE_15, 0x0666);
    ml_apb_write(HB_RX_GAIN_TABLE_63, 0xFFFF);
    // RX ANALOG BASEBAND GAIN TABLE
    ml_apb_write(HB_RX_ABB_GAIN_TABLE_0_1,      0x0000);
    ml_apb_write(HB_RX_ABB_GAIN_TABLE_8_9,      0x0311);
    ml_apb_write(HB_RX_ABB_GAIN_TABLE_14_15,      0x3C1F);

    // RX DC OFFSET CORRECTION GAIN TABLE
    ml_apb_write(HB_RX_ABB_CH0_DCOC_I_TABLE_0,    0x0000);
    ml_apb_write(HB_RX_ABB_CH0_DCOC_I_TABLE_6,    0x2020);
    ml_apb_write(HB_RX_ABB_CH0_DCOC_I_TABLE_7,    0x2121);

    // DBB FINE GAIN TABLE
    ml_apb_write(HB_RX_DBB_FINE_GAIN_TABLE_0,    0x02D4);
    ml_apb_write(HB_RX_DBB_FINE_GAIN_TABLE_8,    0x041D);
    ml_apb_write(HB_RX_DBB_FINE_GAIN_TABLE_15,    0x05A6);

    // DBB COARSE GAIN TABLE
    ml_apb_write(HB_RX_DBB_COARSE_GAIN_TABLE,      0x3210);

    // -- TX Gain -------------------------------------------------------
    // GAIN TABLE
    ml_apb_write(HB_TX_GAIN_TABLE_0,             0x0000);
    ml_apb_write(HB_TX_GAIN_TABLE_31,             0x01FF);

    // RF GAIN TABLE
    ml_apb_write(HB_TX_RF_GAIN_TABLE_0_1,       0x0900);
    ml_apb_write(HB_TX_RF_GAIN_TABLE_14_15,       0x1008);
    ml_apb_write(HB_TX_RF_GAIN_TABLE_30_31,       0x3830);

    // DC Offset TABLE I
    ml_apb_write(HB_TX_DBB_DCOC_I_TABLE_0,        0x0000);
    ml_apb_write(HB_TX_DBB_DCOC_I_TABLE_23,        0x0F3C);
    ml_apb_write(HB_TX_DBB_DCOC_I_TABLE_31,        0x0FFF);

    // DC Offset TABLE Q
    ml_apb_write(HB_TX_DBB_DCOC_Q_TABLE_0,        0x0000);
    ml_apb_write(HB_TX_DBB_DCOC_Q_TABLE_25,        0x0F3C);
    ml_apb_write(HB_TX_DBB_DCOC_Q_TABLE_31,        0x0FFF);

    // DBB TABLE
    ml_apb_write(HB_TX_DBB_GAIN_TABLE_0,          0x02D4);
    ml_apb_write(HB_TX_DBB_GAIN_TABLE_10,          0x045C);
    ml_apb_write(HB_TX_DBB_GAIN_TABLE_15,          0x05A6);

    // Firmware emulation: Configuration of RXTX chain
    ml_apb_write(HB_BB_RSBUS_CTRL14,       HB_TX_BW_20M_TDEF_INT_ACT);
    ml_apb_write(HB_RX_BW,                 RX_BW_20M_STR_40M_TDEF);
    ml_apb_write(HB_TX_BW,                 HB_TX_BW_20M_TDEF_INT_ACT);
    ml_apb_write(HB_RX_ADC_ENABLE,         RX_ADC_ACTIVE_EN_STDBY_GEC);
    ml_apb_write(HB_TX_DAC_ENABLE,         TX_DAC_ACTIVE_EN_STDBY_GEC);
    ml_apb_write(HB_RX_CLOCK_ENABLE,       RX_CLK_ON_GEC);
    ml_apb_write(HB_TX_CLOCK_ENABLE,       TX_CLK_ON_GEC);
    ml_apb_write(HB_RX_DBB_IQC_CONFIG,     RX_IQC_ON);
    ml_apb_write(HB_TX_DBB_CONFIG,         TX_IQC_ON);
    ml_apb_write(HB_BB_RSBUS_CTRL1,        BB_VREG_BL_RX_BDEF);
    ml_apb_write(HB_BB_RSBUS_CTRL0,        BB_RX_ACTIVE_EN_GEC);
    ml_apb_write(HB_BB_RSBUS_CTRL8,        BB_TX_ACTIVE_EN_GEC);
    ml_apb_write(HB_BB_RSBUS_CTRL9,        BB_TX_BDEF);

    // API COMMAND to put RF in RX mode with RxgainIndex set to 31
    rf_switch_hb(0x3, 0x1f);

    // 1st LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 256, DATA_RAM_0_TOP - 3    );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 257, HB_ANALOG_ENABLE      );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 258, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 259, 0x0000              );

    // 2nd LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 260, DATA_RAM_0_TOP - 4     );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 261, HB_ANALOG_ENABLE     );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 262, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 263, 0x0020              );

    // 3rd LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 264, DATA_RAM_0_TOP - 7    );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 265, HB_TX_BIAS_SEL        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 266, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 267, 0x0020              );

    // 4th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 268, DATA_RAM_0_TOP - 10   );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 269, HB_TX_GAIN_PTR        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 270, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 271, 0x0000              );

    // 5th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 272, DATA_RAM_0_TOP - 5    );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 273, HB_ANALOG_ENABLE      );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 274, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 275, 0x0020              );

    // 6th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 276, DATA_RAM_0_TOP - 6    );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 277, HB_ANALOG_ENABLE      );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 278, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 279, 0x0040              );

    // 7th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 280, DATA_RAM_0_TOP - 8    );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 281, HB_TX_BIAS_SEL        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 282, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 283, 0x0040              );

    // 8th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 284, DATA_RAM_0_TOP - 9   );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 285, HB_TX_BIAS_SEL        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 286, 0x8001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 287, 0x0020              );

    // 9th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 288, DATA_RAM_0_TOP - 11   );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 289, HB_TX_GAIN_PTR        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 290, 0x8001             );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 291, 0x0020             );

    // 10th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 292, DATA_RAM_0_TOP - 12   );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 293, HB_TX_GAIN_PTR        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 294, 0x8001  );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 295, 0x0020              );

    // 11th LLI
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 296, DATA_RAM_0_BASE + 9   );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 297, HB_TX_GAIN_PTR        );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 298, 0x0001              );
    ml_apb_write(XDMA_TABLE_HB_SPACE_BASE  + 299, 0x0020              );

    // Check RX GAIN value with an API GET_REGISTER command
    ret = rf_get_reg_value(HB_RX_GAIN_PTR, &cmd_response, &cmd_rddata);
    if (ret == 0 &&
            cmd_rddata != 0x001f){
#ifndef CFG_RWTL
        printf("error: Bad RX GAIN register value expected :8'h1f , received:0x%x\n", cmd_rddata);
        printf("Simulation FAILURE\n");
#endif
        ret = 2;
    }
    return ret;
}
#ifdef WITH_CATRF
//now the soc do not support single apb read access
int8_t rf_trx_check_status(uint8_t txrxid)
{
    uint16_t value;

    //2 Get status before do anthing
    rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetStatus, (txrxid << 12), __LINE__);
    value = rc_data1_getf();
    if((value & 0x10))
    {
#ifndef CFG_RWTL
		printf("***************TRX Status ERROR!*******************\n");
		printf("txrxid : %d, trx status : 0x%x\n", txrxid, value);
#endif
        return -1;
	}
    return 0;
}

//now the soc do not support single apb read access
void rf_trx_dump(uint8_t txrxid)
{
#ifndef CFG_RWTL
    uint16_t value;
    uint8_t trx_mode = 0;
    //1 Get current TRX OpMode
    rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetOpMode, (txrxid << 12), __LINE__);
    value = rc_data1_getf();
	printf("current %s trx operation mode : 0x%x\n", txrxid == 1 ? "lb": "hb", (value >> 8) & 0xF);
    printf("current %s rx gain_pwr: 0x%x\n", txrxid == 1 ? "lb": "hb", value & 0xFF);

	trx_mode = (value >> 8) & 0xF;

    //2 Get status before do anthing
    rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetStatus, (txrxid << 12), __LINE__);
    value = rc_data1_getf();
	printf("~~~~~~~~~~~~~~~~~~~TRX Status ~~~~~~~~~~~~~~~~~~~~\n");
	printf("current %s trx status is 0x%x\n", txrxid == 1 ? "lb": "hb", value);
    printf("Detector flags for RF synth (SX)(bits 4:0) : 0x%x\n", value & 0x1F);
    printf("Detector flags for RX-WBPLL (bits 10:8) : 0x%x\n", (value >> 8) & 0x7);
    printf("Detector flags for TX-WBPLL (bits 13:11) : 0x%x\n", (value >> 11) & 0x7);
    printf("Base band peak detector flag (bit 15): 0x%x\n", value >> 15);
	if(((trx_mode == 5) && (value & 0x400)) ||
        (value & 0x10) ||
        ((trx_mode == 6) && (value & 0x2000)))
    {
		printf("***************TRX Status ERROR!*******************\n");
	}

    //3 Get the BW & channel
    //get the channel & BW to check if it is the same with user set
    rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetChannelBW, (txrxid << 12), __LINE__);
	value = rc_data1_getf();
	printf("_CWA_TRX_GetChannelBW, return %d\n", value & 0xFF);

    rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetRFChannel, (txrxid << 12), __LINE__);
	value = rc_data1_getf();
	printf("_CWA_TRX_GetRFChannel, return %d\n", value & 0xFF);

	//4 Get Current Tx/Rx gain index
    rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetRxGain, (txrxid << 12), __LINE__);
    value = rc_data1_getf();
	printf("_CWA_TRX_GetRxGain : 0x%x\n", value & 0xFF);

	rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetTxGain, (txrxid << 12), __LINE__);
    value = rc_data1_getf();
	printf("_CWA_TRX_GetTxGain : 0x%x\n", value & 0x1F);

#endif
}

int8_t rf_trx_do_calibration(uint8_t txrxid, uint16_t calibration)
{
    int8_t resp_args;
    uint16_t value;
    uint32_t timeout = 0;
    while(timeout++ < MAX_RETRY_TIME){
        resp_args = rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_DoCalibration, txrxid << 12, __LINE__);
        if(resp_args != 0){
            dbg("_CWA_TRX_DoCalibration failed, return %d!\n", resp_args);
            udelay(100);
            continue;
        }
        udelay(100);

        resp_args = rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetCalStatus, txrxid << 12, __LINE__);
        if(resp_args != 1){
            dbg("_CWA_TRX_GetCalStatus failed, return %d\n", resp_args);
            udelay(100);
            continue;
        }
        value = rc_data1_getf();
        if((value & 0xFFF) != calibration){
            dbg("_CWA_TRX_GetCalStatus failed, status : 0x%x!\n", value);
            udelay(100);
            continue;
        }

        if(rf_trx_check_status(txrxid)){
            dbg("check trx status failed! line : %d\n", __LINE__);
            udelay(100);
            continue;
        }
        break;
    }
    return 0;
}

void rf_trx_stop(uint8_t txrxid)
{

#ifndef CFG_RWTL
    printf("%s\n", __func__);

    printf("switching trx mode to offline....\n");
#endif
	//1.send CWA_TRX_SetOpMOde(0,4,0,0,0)
	rf_send_2_param_cmd(RRQ_BIT, _CWA_TRX_SetOpMode, (txrxid << 12 | 4 << 8 | 0 << 0), 0, __LINE__);
    rc_data1_getf();
#ifndef CFG_RWTL
#endif
    udelay(100);
	//2.send CWA_TRX_SetOpMOde(0,0,0,0,0)
	rf_send_2_param_cmd(RRQ_BIT, _CWA_TRX_SetOpMode, (txrxid << 12 | 0 << 8 | 0 << 0), 0, __LINE__);
    rc_data1_getf();
#ifndef CFG_RWTL
#endif
    udelay(100);
}

int8_t rf_trx_calibrate(uint8_t txrxid, uint16_t calibration, uint16_t chan, uint16_t bw)
{
    int8_t resp_args;
    uint16_t value;
    uint8_t trx_mode;
    int8_t ret = 0;
    uint32_t timeout = 0;
    //1,save current trx mode, because the calibrations should be done in the onsxtrxidle mode
REGETOPMODE:
    resp_args = rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_GetOpMode, (txrxid << 12), __LINE__);
    ASSERT_ERR(resp_args == 1);
    value = rc_data1_getf();
    trx_mode = (value >> 8) & 0xF;
    ASSERT_ERR(trx_mode == 5);
    if(trx_mode >= 7){
        udelay(100);
		printf("rf is on mode 7\n");
        goto REGETOPMODE;
    }

	while(timeout++ < MAX_RETRY_TIME){
		resp_args = rf_send_5_param_cmd(RRQ_BIT, _CWA_TRX_SwitchChannel,
			                        (txrxid << 12 | 4 << 8 | INIT_RX_GAIN_INDEX << 0), (chan << 8 | bw << 0),
									calibration,(5 << 8 | INIT_RX_GAIN_INDEX << 0), (0 << 8 | 0 << 0), __LINE__);
		if(resp_args != 4){
			printf("_CWA_TRX_SwitchChannel resp_args is not 4, which is %d\n",resp_args);
			ret = -1;
			udelay(100);
			continue;
		}
		value = rc_data1_getf();
		if(((value >> 8) & 0xF) != 4){
			printf("Response value(rw1) of _CWA_TRX_SwitchChannel error %04x\n",value);
			ret = -2;
			udelay(100);
			continue;
		}
		value = rc_data2_getf();
		if((value & 0xFFF) != calibration){
			printf("Response value(rw2) of _CWA_TRX_SwitchChannel error %04x\n",value);
			ret = -3;
			udelay(100);
			continue;
		}
		value = rc_data3_getf();
		if(((value >> 8) & 0xF) != 5){
			printf("Response value(rw3) of _CWA_TRX_SwitchChannel error %04x\n",value);
			ret = -4;
			udelay(100);
			continue;
		}
		value = rc_data4_getf();
		if((value & 0x10) || (value & 0x400)){
			printf("Response value(rw4) of _CWA_TRX_SwitchChannel error %04x\n",value);
			printf("***************TRX Status ERROR!*******************\n");
			printf("txrxid : %d, trx status : 0x%x\n", txrxid, value);
			ret = -5;
			udelay(100);
			continue;
		}
		ret = 0;
		break;
	}

    return ret;
}

/*
   off,                    = 0,
   onid,                   = 1,
   onidsx,                 = 2,
   onsx,                   = 3,
   onsxtrxid,              = 4,
   onrx,                   = 5,
   ontx,                   = 6,
   ontrxcal,               = 7,
*/

int8_t rf_trx_prepare(uint8_t txrxid)
{
    int8_t resp_args;
    uint16_t value;
    uint8_t init_chan = txrxid ? 9 : 100;
	uint8_t init_bw = 40;
	if(!txrxid)
		init_bw = 80;

    printf("%s\n", __func__);

    if(txrxid)
        rc_apicmdaddr_setf(0x1440);
    else
        rc_apicmdaddr_setf(0x1400);

	//set calibration configuration 0x3FF
    dbg("_CWA_TRX_SetCalConfig, 0x20f\n");
	resp_args = rf_send_1_param_cmd(RRQ_BIT, _CWA_TRX_SetCalConfig, (txrxid << 12 | SEC_TRX_CALIBRATE_CONF), __LINE__);
    ASSERT_ERR(resp_args == 0);
	udelay(100);

	//1, send CWA_TRX_SetOpMOde(0, 1, 0, 0, 0);
    printf("switching the trx mode to ondile....\n");
    resp_args = rf_send_2_param_cmd(RRQ_BIT, _CWA_TRX_SetOpMode, (txrxid << 12 | 1 << 8 | 0 << 0), 0, __LINE__);
    ASSERT_ERR(resp_args == 1);
    value = rc_data1_getf();
    ASSERT_ERR(((value >> 12) & 0xf) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xf) == 1);
#ifndef CFG_RWTL
    printf("_CWA_TRX_SetOpMode(%d, 1, 0, 0, 0) have %d response\n", txrxid, resp_args);
    printf("response data[0] : 0x%x\n", value);
#endif
    udelay(100);

	////now change the trx mode to onsxtrxid
    //2 send CWA_TRX_SetOpMOde(0,4,0,9,40)
    printf("switching trx mode to onsxtrxid....\n");
    resp_args = rf_send_2_param_cmd(RRQ_BIT, _CWA_TRX_SetOpMode, (txrxid << 12 | 4 << 8 | 0 << 0), (init_chan << 8 | init_bw << 0), __LINE__);
    ASSERT_ERR(resp_args == 1);
    value = rc_data1_getf();
    ASSERT_ERR(((value >> 12) & 0xF) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xF) == 4);
#ifndef CFG_RWTL
    printf("_CWA_TRX_SetOpMode(%d, 4, 0, %d, %d) have %d response\n", txrxid, init_chan, init_bw, resp_args);
    printf("response data[0] : 0x%x\n", value);
#endif
    //wait 100us
    udelay(100);

    //3 Do calibration
    if(rf_trx_do_calibration(txrxid, SEC_TRX_CALIBRATE_CONF)){
#ifndef CFG_RWTL
        printf("rf_trx_do_calibration failed!\n");
#endif
        return -2;
    }

    // now change the trx mode to onrx
    //4 send CWA_TRX_SetOpMode(0,5,63,0,0)
    printf("switching trx to onrx....\n");
    resp_args = rf_send_2_param_cmd(RRQ_BIT, _CWA_TRX_SetOpMode, (txrxid << 12 | 5 << 8 | INIT_RX_GAIN_INDEX << 0), (0 << 8 | 0 << 0), __LINE__);
    ASSERT_ERR(resp_args == 1);
    value = rc_data1_getf();
    ASSERT_ERR(((value >> 12) & 0xF) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xF) == 5);
#ifndef CFG_RWTL
    printf("_CWA_TRX_SetOpMode(%d, 5, %d, 0, 0) have %d response\n", txrxid, INIT_RX_GAIN_INDEX, resp_args);
    printf("response data[0] : 0x%x\n", value);
#endif
    //wait 100us
    udelay(100);

	if(rf_trx_check_status(txrxid)){
#ifndef CFG_RWTL
        printf("check trx status failed! line : %d\n", __LINE__);
#endif
        return -3;
    }

#ifdef ONTX
	// now change the trx mode to onTx
    //5 send CWA_TRX_SetOpMode(0,6,31,0,0)
    printf("switching trx to ontx....\n");
    resp_args = rf_send_2_param_cmd(RRQ_BIT, _CWA_TRX_SetOpMode, (txrxid << 12 | 6 << 8 |  31 << 0), (0 << 8 | 0 << 0), __LINE__);
    ASSERT_ERR(resp_args == 1);
    value = rc_data1_getf();
    ASSERT_ERR(((value >> 12) & 0xF) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xF) == 6);
#ifndef CFG_RWTL
    printf("_CWA_TRX_SetOpMode(%d, 6, 31, 0, 0) have %d response\n", txrxid, resp_args);
    printf("response data[0] : 0x%x\n", value);
#endif
    //wait 100us
    udelay(100);
#endif
    return 0;
}

int8_t rf_trx_init(uint8_t txrxid)
{
    int8_t resp_args;
    uint16_t value;
    uint8_t init_chan = txrxid ? 7 : 100;
    uint8_t init_bw = txrxid ? 40 : 80;
	uint16_t calibration = MAX_TRX_CALIBRATE_CONF;
    uint32_t cmd_base = txrxid ? LB_COMMANDIF_BASE_ADDR : HB_COMMANDIF_BASE_ADDR;
    uint32_t timeout = 0;
    uint32_t trx_mode;

    dbg("%s, %d\n", __func__, txrxid);
    dbg("_CWA_TRX_SetCalConfig , 0x%x\n", calibration);
    //0, set all global configuration, this would not easily change later
    resp_args = ml_apb_send_1_params_cmd(1, cmd_base, _CWA_TRX_SetCalConfig, (txrxid << 12 | calibration));
    if(resp_args != 0){
        printf("_CWA_TRX_SetCalConfig, failed!, resp_args : %d\n", resp_args);
        return -1;
    }

    //2, send CWA_TRX_SetOpMOde(0, 1, 0, 0, 0);
    dbg("_CWA_TRX_SetOpMode, 1\n");
    resp_args = ml_apb_send_2_params_cmd(1, cmd_base, _CWA_TRX_SetOpMode, (txrxid << 12 | 1 << 8 | 0 << 0), 0);
    if(resp_args != 1){
        printf("_CWA_TRX_SetOpMode, failed!, resp_args : %d\n", resp_args);
        return -2;
    }
    value = ml_apb_read(cmd_base + 0x000B);
    ASSERT_ERR(((value >> 12) & 0xF) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xF) == 1);
    //wait 100us
    udelay(100);

    //3, Now change the trx mode to onsxtrxid with channel & bandwidth
    dbg("_CWA_TRX_SetOpMode 4, chan : %d, bw : %d\n", init_chan, init_bw);
    resp_args = ml_apb_send_2_params_cmd(1, cmd_base, _CWA_TRX_SetOpMode, (txrxid << 12 | 4 << 8 | 0 << 0), (init_chan << 8 | init_bw << 0));
    if(resp_args != 1){
        printf("_CWA_TRX_SetOpMode, failed!, resp_args : %d\n", resp_args);
        return -3;
    }
    value = ml_apb_read(cmd_base + 0x000B);
    ASSERT_ERR(((value >> 12) & 0xF) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xF) == 4);
    //wait 100us
    udelay(100);

    //4, do calibration
    dbg("rf_trx_do_calibration\n");
    resp_args = ml_apb_send_1_params_cmd(1, cmd_base, _CWA_TRX_GetCalConfig, txrxid << 12);
    if(resp_args != 1){
        dbg("_CWA_TRX_GetCalConfig failed, resp_args : %d\n", resp_args);
        return -4;
    }

    value = ml_apb_read(cmd_base + 0x000B);
    if((value & 0xFFF) != calibration){
        dbg("GetCalConfig error: 0x%x\n", value);
        return -5;
    }

    while(timeout++ < MAX_RETRY_TIME){
        resp_args = ml_apb_send_1_params_cmd(1, cmd_base, _CWA_TRX_DoCalibration, txrxid << 12);
        if(resp_args != 0){
            dbg("_CWA_TRX_DoCalibration failed, resp_args : %d\n", resp_args);
            udelay(100);
            continue;
        }
        udelay(100);

        resp_args = ml_apb_send_1_params_cmd(1, cmd_base, _CWA_TRX_GetCalStatus, txrxid << 12);
        if(resp_args != 1){
            dbg("_CWA_TRX_GetCalStatus failed, resp_args : %d\n", resp_args);
            udelay(100);
            continue;
        }
        value = ml_apb_read(cmd_base + 0x000B);
        if((calibration & 0xFFF) != (value & 0xFFF)){
            dbg("_CWA_TRX_GetCalStatus return 0x%x, different with orignal one :0x%x\n", value, calibration);
            udelay(100);
            continue;
        }

        resp_args = ml_apb_send_1_params_cmd(1, cmd_base, _CWA_TRX_GetOpMode, (txrxid << 12));
        if(resp_args != 1){
            dbg("_CWA_TRX_GetOpMode failed, resp_args : %d\n", resp_args);
            udelay(100);
            continue;
        }
        value = ml_apb_read(cmd_base + 0x000B);
        trx_mode = (value >> 8) & 0xF;
        dbg("trx_mode : %d\n", (int)trx_mode);

        resp_args = ml_apb_send_1_params_cmd(1, cmd_base, _CWA_TRX_GetStatus, (txrxid << 12));
        if(resp_args != 1){
            dbg("_CWA_TRX_GetStatus failed, resp_args : %d\n", resp_args);
            udelay(100);
            continue;
        }
        value = ml_apb_read(cmd_base + 0x000B);
        if((value & 0x10) ||
                ((trx_mode == 5) && (value & 0x400)) ||
                ((trx_mode == 6) && (value & 0x2000)))
        {
            dbg("TRX Status ERROR even after calibration!\n");
            udelay(100);
            continue;
        }

        break;
    }

    //5, Send CWA_TRX_SetOpMode(0,0, 0, 0, 0)
    dbg("_CWA_TRX_SetOpMode, 0\n");
    resp_args = ml_apb_send_2_params_cmd(1, cmd_base, _CWA_TRX_SetOpMode, (txrxid << 12), 0);
    if(resp_args != 1){
        printf("_CWA_TRX_SetOpMode(0) failed!, resp_args : %d\n", resp_args);
        return -5;
    }
    value = ml_apb_read(cmd_base + 0x000B);
    ASSERT_ERR(((value >> 12) & 0xF) == txrxid);
    ASSERT_ERR(((value >> 8) & 0xF) == 0);

    return 0;
}

int rf_app_housekeeping_prepare(uint8_t hk_mode, uint8_t hk_clock, uint16_t hk_task_config,
		                                uint16_t hk_update_time, int16_t hk_tempdiff_threshold)
{

	ml_apb_send_4_params_cmd(0, HB_COMMANDIF_BASE_ADDR, _CWA_APP_SetHKConfig, ((hk_clock << 1)| hk_mode), hk_task_config , hk_update_time, hk_tempdiff_threshold);
	mdelay(1);
	ml_apb_send_0_params_cmd(1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_GetHKConfig);
	ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000B);
	ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000C);
	ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000D);
	ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000E);

	return 0;
}


static int rf_app_calibrate(uint8_t calibration)
{
	uint16_t value;
	int8_t resp_args;

	//calibrate AUX-ADC & CLK PLL calibration
	ml_apb_send_1_params_cmd(1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_SetCalConfig, calibration);
	udelay(100);

	//get calibration set
	resp_args = ml_apb_send_0_params_cmd(1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_GetCalConfig);
	ASSERT_ERR(!(resp_args != 1));
	value = ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000B);
	ASSERT_ERR((value & 0xF)== calibration);
	if((value & 0xF)!= calibration)
		printf("different calibration config\n");
	dbg("CLK PLL calibration bit %s;AUX ADC calibration bit %s\n",
            (value & 0x1) ? "on" : "off", (value & 0x2) ? "on" : "off");

	// do calibration
	ml_apb_send_0_params_cmd(1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_DoCalibration);
	udelay(100);
	//check calibration status
	resp_args = ml_apb_send_0_params_cmd(1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_GetCalStatus);
	if (resp_args != 1) {
		dbg("_CWA_APP_GetCalStatus failed, return args : %d\n",
				resp_args);
	}
	value = ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000B);

	if (value != calibration) {
#ifndef CFG_RWTL
		printf("app calibration failed!, status : 0x%x\n", value);
		printf("CLK PLL calibration bit is %d;AUX ADC calibration bit is %d, app calibration failed!\n",(value & 0x1),(value & 0x2));
#endif
		return -1;
	}
	return 0;
}

#ifdef CONFIG_SFAX8_WIFI_EXPA
int8_t rf_expa_init(uint8_t txrxid)
{
	int8_t resp_args;
	uint32_t cmd_base = txrxid ? LB_COMMANDIF_BASE_ADDR : HB_COMMANDIF_BASE_ADDR;
	uint16_t conf_in_idel,conf_in_rx,conf_in_tx,conf_in_pa;
	//LB_EXTERNAL_PA_CONF_IN_IDLE=0xF0
	//LB_EXTERNAL_PA_CONF_IN_RX=0xF6
	//LB_EXTERNAL_PA_CONF_IN_TX=0xF4
	//LB_EXTERNAL_PA_CONF_IN_PA=0xF1
	//HB_EXTERNAL_PA_CONF_IN_IDLE=0xF0
	//HB_EXTERNAL_PA_CONF_IN_RX=0xF3
	//HB_EXTERNAL_PA_CONF_IN_TX=0xF1
	//HB_EXTERNAL_PA_CONF_IN_PA=0xF4
	if(txrxid){//LB
		conf_in_idel = 0xF0;
		conf_in_rx = 0xF6;
		conf_in_tx = 0xF4;
		conf_in_pa = 0xF1;
	}
	else{//HB
		conf_in_idel = 0xF0;
#ifdef CONFIG_TARGET_SFA18_P20B
		conf_in_rx = 0xFC;
		conf_in_tx = 0xF8;
		conf_in_pa = 0xF2;
#else
		conf_in_rx = 0xF3;
		conf_in_tx = 0xF1;
		conf_in_pa = 0xF4;
#endif
	}
	//set config for external PA
	resp_args = ml_apb_send_4_params_cmd(1, cmd_base, _CWA_TRX_SetRfAppConfig, (txrxid << 12 | conf_in_idel), conf_in_rx, conf_in_tx, conf_in_pa);
	if(resp_args != 0){
		dbg("_CWA_TRX_SetRfAppConfig,, failed!, resp_args : %d\n", resp_args);
		return -1;
	}
	return 0;
}
#endif

int rf_app_prepare(void)
{
	uint16_t value;
	int i;
	uint32_t re_prepare_time = 0;

	//*2, load default register values*/
	for (i = 0; i < sizeof(g_rf_def_data) / sizeof(struct aet_reg_pair);
			i++) {
		ml_apb_write(g_rf_def_data[i].addr, g_rf_def_data[i].value);
		if (g_rf_def_data[i].addr == 0x1073)
			udelay(50);
	}
	//*3, load XDMA register values
	for (i = 0; i < sizeof(g_rf_xdma_def_data) /
					sizeof(struct aet_reg_pair);
			i++) {
		ml_apb_write(g_rf_xdma_def_data[i].addr,
				g_rf_xdma_def_data[i].value);
	}
#ifdef CONFIG_SFAX8_UPDATE_GAIN_TABLE
    //*4, change rf hb tx gain table from 7~2
        ml_apb_write(HB_TX_GAIN_TABLE_1, 0x3E);
        ml_apb_write(HB_TX_GAIN_TABLE_2, 0xB3);
        ml_apb_write(HB_TX_GAIN_TABLE_3, 0xB5);
        ml_apb_write(HB_TX_GAIN_TABLE_4, 0xB7);
        ml_apb_write(HB_TX_GAIN_TABLE_5, 0xB9);
        ml_apb_write(HB_TX_GAIN_TABLE_6, 0xBB);
        ml_apb_write(HB_TX_GAIN_TABLE_7, 0xBD);
        printf("Updating tx gain table OK\n");
#endif
	// 5, when the complete FW img is transfered , the host writes another
	// bit in the
	// control register 0x4001 which release the processor from its frozen
	// state.
	// The processor will then start to execute the FW from PMEM address
	// 0x0000 onwards
	ml_apb_write(0x4001, 0x0); // PMEM_CTRL

	// 5.Wait until IRQ : BOOT_READY is given
	while (1) {
		mdelay(1);
		if (is_rf_boot_finished())
			break;
	}

// 6.set the XO CALIBRATION VALUE, This may be omitted but will result in
// frequency offsets
// TODO

RE_PREPARE:
	// use the LB COMMAND IF REGISTER ADDRESS to write the command
	/*set the app operation mode to 2(stdby_bootclk)*/
	ml_apb_send_1_params_cmd(
			1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_SetOpMode, 2);
	udelay(100);

	if (rf_app_calibrate(3))
		printf("app calibrate fail!\n");

	/*7, set the app operation mode to 4(idle_active)*/
	ml_apb_send_1_params_cmd(
			1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_SetOpMode, 4);
	udelay(100);
	/*8, Get application status & check the result*/
	/*
	   appmode t_AppMode dw1[15:12]
	   clkpllmode t_ClkPllMode dw1[9:8]
	   unlocked t_Unlocked dw1[4]
	   vtune_high t_Flag dw1[3]
	   vtune_low t_Flag dw1[2]
	   vco_toohigh t_Flag dw1[1]
	   vco_toolow t_Flag dw1[0]
	   */
	ml_apb_send_0_params_cmd(1, HB_COMMANDIF_BASE_ADDR, _CWA_APP_GetStatus);
	value = ml_apb_read(HB_COMMANDIF_BASE_ADDR + 0x000B);
	if ((value & 0xFFFC) != 0x4100) {
#ifndef CFG_RWTL
		printf("~~~~~~~~~~~~~the clkpll is unlocked,0x%x failed!\n",
				value);
#endif
		// just a hack for error process
		// so when bb module register, it will cause error
		re_prepare_time++;
		goto RE_PREPARE;
	} else {

		dbg("the app status is 0x%x, reprepare_time is %u\n", value,
				(unsigned int)re_prepare_time);
	}

	// rf_app_housekeeping_prepare(1,0,0xF,32768,100);
#ifdef CONFIG_SFAX8_WIFI_EXPA
	//init LB EXPA
	if(rf_expa_init(1)){
		dbg("LB rf_expa_init failed");
		return -1;
	}
	//init HB EXPA
	if(rf_expa_init(0)){
		dbg("HB rf_expa_init failed");
		return -2;
	}
#endif
	return 0;
}

//#define SWITCH_CHAN_USE_METHOD1
//1,set chanel
//2,set bw
//3,do calibration
//  3.1 switch mode to 4 without chan & bw
//  3.2 do calibration
//  3.3 switch trx mode back


int8_t rf_switch_chan_bw(uint8_t txrxid, uint16_t chan, uint16_t bw)
{
    int8_t ret = 0;

	// Set the BW
    ret = rf_trx_calibrate(txrxid, SEC_TRX_CALIBRATE_CONF, chan, bw);
	return ret;
}

/*
func:
this function is just for a pre-test for the firmware
we use this function to test all the functions
Step:
1,load default register value
2,load pmem
3,trigger the pmem & check the status
4,read the default registers, want to see if the program has changed these registers
4,use the API command to do some tests
4.1 CWA_GEN_GetRegister
4.2 CWA_GEN_SetRegister
4.3 CWA_GEN_GetVersion
4.4 CWA_APP_GetOpMode
4.5 CWA_APP_GetStatus
4.6 CWA_APP_GetCalConfig
4.7 CWE_APP_GetCalStatus
4.8 CWA_APP_GetAuxAdcConfig
4.9 CWA_APP_GetAdcSrc
4.10 CWE_APP_GetAdcResult
4.11 CWA_APP_GetHKConfig
4.12 CWA_TRX_GetRFChannel
4.13 CWA_TRX_GetChannelBW
4.14 CWA_TRX_GetRxGain
4.15 CWA_TRX_GetOpMode
4.16 CWA_TRX_GetStatus
4.17 CWA_TRX_GetCalConfig
4.18 CWA_TRX_GetCalStatus
4.19 CWA_TRX_GetTxPower
4.20 CWA_TRX_GetTxPower
params:

return:

*/
uint8_t init_aetnensis_for_r20170127(uint8_t txrxid, uint8_t init_trx)
{
	CHECK_VERSION(txrxid);

	if (!init_trx)
		return 0;
	// step3: init trx mode
	if (rf_trx_init(txrxid)) {
		dbg("rf_trx_prepare failed!\n");
		return 2;
	}
	return 0;
}
#endif
#endif
