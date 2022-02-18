/**
 ****************************************************************************************
 *
 * @file phy_aetnensis.c
 *
 * Copyright (C) Siflower 2015-2019
 *
 * When setting a channel, several procedures can be advantageously left-out depending on
 * whether the band, bw, frequency remain unchanged (calibrations, old modem clk mmc
 * toggles ..), e.g. for multi-channel. Although the changes are trivial they might get
 * in the way of properly measuring the RF behavior.
 ****************************************************************************************
 */
#include <common.h>
#include <asm/io.h>
#include <mach/pcba_test.h>

#include "wifi_addr.h"
#include "rwnx_config.h"
#include "phy.h"

#include "hal_machw.h"
#include "reg_mac_core.h"

#include "reg_mdm_stat.h"
#include "reg_mdm_cfg.h"
#include "reg_mdmdsss_cfg.h"

#include "reg_riu.h"
#include "rf_access.h"
#include "reg_rc.h"
#include "reg_rc_gaintable.h"
#include "reg_rc_nftable.h"
#include "reg_macbypass.h"

#include "wifi_ldfw.h"

#if NX_MDM_VER != 20
#error "Needs to be compiled with modem version 20"
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))
#endif

/// Structure containing the parameters of the AETNENSIS PHY configuration
struct phy_aetnensis_cfg_tag
{
	//LDPC is enable or not
	uint32_t ldpc;
	//BW conf
	uint32_t bw;
	//init rf
	uint32_t donot_init_rf;
};

/// PHY driver context.
struct phy_env_tag
{
	/// Karst configuration parameters
	struct phy_aetnensis_cfg_tag cfg;
	///Currently configured 20MHz primary frequency (in MHz)
	uint16_t chnl_prim20_freq;
	///Currently configured nMHz contiguous channel (of primary 80 if 80+80)
	uint16_t chnl_center1_freq;
	///Currently configured secondary 80MHz channel if 80+80, unused otherwise
	uint16_t chnl_center2_freq;
	///Current selected band
	uint8_t band;
	//20/40/80/160/80+80 channel type
	uint8_t chnl_type;
};

struct phy_env_tag phy_env[1];

#define REG_DFLT_SW_CTRL    ((1 << RC_PRESCALER_LSB) & (~RC_START_DONE_BIT))

//TODO
#define __RIU_RADARFIFO_ADDR 0x60C04000

uint8_t phy_vht_supported(void)//star c
{
#if NX_MDM_VER > 20
	return ((mdm_vht_getf() != 0) || (mdm_chbw_getf() > PHY_CHNL_BW_40));
#else
	return true;
#endif
}

/// Gain table - @todo Update the values
static const uint32_t gaintable[REG_RC_NFTABLE_COUNT] =
{
	0x00020100,   // RWNXAGCGT0
	0x01000201,   // RWNXAGCGT1
	0x02010002,   // RWNXAGCGT2
	0x00020100,   // RWNXAGCGT3
	0x00000201,   // RWNXAGCGT4
	0x00000000,   // RWNXAGCGT5
	0x00000000,   // RWNXAGCGT6
	0x00000000,   // RWNXAGCGT7
	0x00000000,   // RWNXAGCGT8
	0x00000000,   // RWNXAGCGT9
	0x00000000,   // RWNXAGCGT10
	0x00000000,   // RWNXAGCGT11
	0x00000000,   // RWNXAGCGT12
	0x00000000,   // RWNXAGCGT13
	0x00000000,   // RWNXAGCGT14
	0x00000000,   // RWNXAGCGT15
};

/// Noise factor table - @todo Update the values
static const uint32_t nftable[REG_RC_NFTABLE_COUNT] =
{
	0x1F1F1F1F, // NFT_RWNXAGCNFT0_RESET
	0x1F1F1F1F, // NFT_RWNXAGCNFT1_RESET
	0x1C1D1E1F, // NFT_RWNXAGCNFT2_RESET
	0x1A1B1B1C, // NFT_RWNXAGCNFT3_RESET
	0x1718191A, // NFT_RWNXAGCNFT4_RESET
	0x14151616, // NFT_RWNXAGCNFT5_RESET
	0x0F111213, // NFT_RWNXAGCNFT6_RESET
	0x0B0C0C0D, // NFT_RWNXAGCNFT7_RESET
	0x09090A0B, // NFT_RWNXAGCNFT8_RESET
	0x07070808, // NFT_RWNXAGCNFT9_RESET
	0x04050506, // NFT_RWNXAGCNFT10_RESET
	0x04040404, // NFT_RWNXAGCNFT11_RESET
	0x04040404, // NFT_RWNXAGCNFT12_RESET
	0x04040404, // NFT_RWNXAGCNFT13_RESET
	0x04040404, // NFT_RWNXAGCNFT14_RESET
	0x04040404, // NFT_RWNXAGCNFT15_RESET
};

static void phy_set_table(const uint32_t val[], uint32_t reg_addr, int reg_cnt)
{
	int i;

	// Copy the table into the registers
	for (i = 0; i < reg_cnt; i++)
	{
		REG_PL_WR(reg_addr, val[i]);
		reg_addr += 4;
	}
}


static void mdm_reset(void)
{
	mdm_swreset_set(0x1);
	mdelay(2);
	mdm_swreset_set(0x0);
	mdelay(2);
}

/*
 * Definitions of MODEM Configurations
 * MDMCONF0: CBW=20MHz, NSS=1, 11nac, BCC
 * MDMCONF1: CBW=20MHz, NSS=2, 11n, BCC
 * MDMCONF2: CBW=40MHz, NSS=1, 11n, BCC/LDPC
 * MDMCONF3: CBW=40MHz, NSS=1, 11nac, BCC/LDPC
 * MDMCONF4: CBW=80MHz, NSS=1, BCC
 * MDMCONF5: CBW=80MHz, NSS=2, BCC
 * MDMCONF6: CBW=80MHz, NSS=2, BCC/LDPC
 * MDMCONF7: CBW=80MHz, NSS=1, BCC/LDPC
 */
static int phy_mdm_conf(uint8_t chan_type)
{
	int ldpc = mdm_ldpcdec_getf();
	int mdmconf = 0;

	switch (chan_type)
	{
		case PHY_CHNL_BW_20:
			mdmconf = 0;
			break;
		case PHY_CHNL_BW_40:
			mdmconf = 2;
			break;
		case PHY_CHNL_BW_80:
			if (ldpc)
				mdmconf = 7;
			else
				mdmconf = 4;
			break;
		default:
			ASSERT_ERR(0);
			break;
	}

	return mdmconf;
}

static uint8_t rf_set_channel(uint8_t band, uint16_t freq1, uint8_t chantype, uint8_t txrxid)
{
	uint16_t bw_mode;
	uint16_t channel = phy_freq_to_channel(band, freq1);

	if (chantype == PHY_CHNL_BW_20)
		bw_mode = 20;
	else if (chantype == PHY_CHNL_BW_40)
		bw_mode = 40;
	else
		bw_mode = 80;

    if(rf_switch_chan_bw(band == PHY_BAND_2G4, channel, bw_mode)){
		return 1;
	}
	return 0;
}

/**
 ****************************************************************************************
 * @brief phy_set_primary_channel function.
 * This function is called to switch the primary channel
 * the RIU's digtal front-end module have the primary channel select function
 ****************************************************************************************
 * TODO: Primary is lower 40MHz
 *      Primary is lower 20MHz
 *      Primary is upper 20MHz
 *      Primary is upper 40MHz
 * Is it mapping ?
 */
void phy_set_primary_channel(uint8_t primary0, uint8_t primary1, uint8_t primary2, uint8_t primary3)
{
    if(BW_PARAM_CONFIG == 20)
    {
        //TODO:
        //20M the lmac source code write the ps select register to 0
        //but the simulation test case set the register to 2;
        //why??
        //RIUAETNENSIS_setf_PSSELECT(2'b10);
        riu_psselect_setf(2);
        wifi_sleep_us(1);
    }
    else if(BW_PARAM_CONFIG == 40)
    {
        if(primary1){
            riu_psselect_setf(1);
        }else{
            riu_psselect_setf(2);
        }
        wifi_sleep_us(1);
    }
    else if(BW_PARAM_CONFIG == 80)
    {
        if(primary0){
            riu_psselect_setf(0);
        }else if(primary1){
            riu_psselect_setf(1);
        }else if(primary2){
            riu_psselect_setf(2);
        }else{
            riu_psselect_setf(3);
        }
        wifi_sleep_us(1);
    }else{
#ifndef CFG_RWTL
        printf("BW_PARAM_CONFIG exceeds 80\n");
#endif
    }
}

uint8_t phy_hw_set_channel(uint8_t band, uint16_t freq, uint16_t freq1, uint8_t chantype, uint8_t index)
{
    unsigned int pssel;
    int mdmconf;
#ifndef CFG_RWTL
    printf("%s: band=%d freq=%d freq1=%d chantype=%d sx=%d\n",__func__,band,freq,freq1,chantype,index);
#endif

    rc_rf_sel_2g4_setf(band == PHY_BAND_2G4);
    mdm_rxdsssen_setf(band == PHY_BAND_2G4);
    riu_ofdmonly_setf(band == PHY_BAND_5G);
    nxmac_abgn_mode_setf(band == PHY_BAND_2G4?MODE_802_11N_2_4:MODE_802_11AC_5);

    mdmconf = phy_mdm_conf(chantype);
    mdm_mdmconf_setf(mdmconf);
    mdm_reset();

    // Set LDPC table selection
    if (mdmconf == 7)
        mdm_ldpcdectablesel_setf(0);
    else
        mdm_ldpcdectablesel_setf(1);

    /**
     * PSSEL - use pssel of RW-WSDM-RIU-HW-REG.xls
     */
    if (chantype == PHY_CHNL_BW_40)
    {
        // pssel = 1 or 2
        pssel = freq < freq1 ? 1 : 2;
    }
    else if (chantype == PHY_CHNL_BW_80)
    {
        // pssel = 0, 1, 2 or 3
        int _offs = freq1 - freq;
        if (_offs > 0)
            pssel = _offs > 10 ? 0 : 1;
        else
            pssel = -_offs > 10 ? 3 : 2;
    }
    else
    {
        pssel = 0;
    }

    riu_psselect_setf(pssel);

    /* workaround periodic (6-12us) peaks caused by the DINI
     * switching power supply
     * only observed in 2.4G
     */
    if(band == PHY_BAND_5G)
    {
      riu_ofdmonly_setf(1);   // AGC detection OFDM only
      #if (NX_RADAR_DETECT)
      riu_radardeten_setf(1); // Enable radar detection
      mdm_radartimclkforce_setf(1); // Enable radar timer clock
      riu_irqmacradardeten_setf(1); // Enable radar detection interrupt
      #endif
      riu_rwnxagcevt3_set(0);     /* write 0 to disable DSSS detection correlator */
     }
    else
    {
      riu_ofdmonly_setf(0);   // AGC detection OFDM and DSSS
      #if (NX_RADAR_DETECT)
      riu_radardeten_setf(0); // Disable radar detection
      mdm_radartimclkforce_setf(0); // Disable radar timer clock
      riu_irqmacradardeten_setf(0); // disable radar detection interrupt
      #endif
      riu_rwnxagcevt3_set(RIU_RWNXAGCEVT3_RESET);/* write default to enable DSSS detection correlator */
    }

    //  3us for TX RAMPUP <=> TXRFRAMPUP = 360
    if (chantype == PHY_CHNL_BW_20)
    {
        mdm_txstartdelay_setf(180);
        mdm_txctrl1_pack(0, 0, 28, 19);
        // TBE for 60MHz
        mdm_tbe_count_adjust_20_setf(0);
        mdm_txctrl3_pack(720, 1080);
        mdm_dcestimctrl_pack(0, 0, 0, 15, 15);
        // For FPGA, divide value by 2 due to timing constraints
		mdm_waithtstf_setf(15);

        mdm_tddchtstfmargin_setf(6);

        // No ACI margin in BW=20MHz due to latency on HTSIG decoding
        riu_rwnxagcaci20marg0_set(0);
        riu_rwnxagcaci20marg1_set(0);
        riu_rwnxagcaci20marg2_set(0);
    }
    else
    {
        mdm_txstartdelay_setf(360);
        // TBE for 120MHz
        mdm_tbe_count_adjust_20_setf(2);
        mdm_txctrl3_pack(1440, 2160);

        if (chantype == PHY_CHNL_BW_40)
        {
            mdm_txctrl1_pack(0, 39, 82, 39);
            mdm_rxtdctrl0_pack(18, 64, 252, 13);
            mdm_dcestimctrl_pack(0, 0, 8, 30, 31);
            // For FPGA, divide value by 2 due to timing constraints
			mdm_waithtstf_setf(31);

            mdm_tddchtstfmargin_setf(6);
        }
        else
        {
            mdm_txctrl1_pack(22, 60, 105, 48);
            mdm_rxtdctrl0_pack(18, 64, 247, 23);
            mdm_dcestimctrl_pack(0, 0, 38, 43, 63);
            // For FPGA, divide value by 2 due to timing constraints
			mdm_waithtstf_setf(63);

            mdm_tddchtstfmargin_setf(14);
        }

		// Set back default ACI margin
        riu_rwnxagcaci20marg0_set(RIU_RWNXAGCACI20MARG0_RESET);
        riu_rwnxagcaci20marg1_set(RIU_RWNXAGCACI20MARG1_RESET);
        riu_rwnxagcaci20marg2_set(RIU_RWNXAGCACI20MARG2_RESET);
    }

	if(rf_set_channel(band, freq1, chantype, band == PHY_BAND_2G4)){
		return 1;
	}
	return 0;
}

static int phy_hw_init(const struct phy_aetnensis_cfg_tag *cfg)
{
#ifndef CFG_RWTL
    load_agc_ram_bin();//star c
#endif

    /***************************************************************************
     * MODEM INIT
     **************************************************************************/
    // Check if the MDM version is what we expect
    //ASSERT_ERR(mdm_version_getf() == 0);

    mdm_mdmconf_set(phy_mdm_conf(BW_20MHZ));
    mdm_reset();

    mdm_rxmode_set(MDM_RXSTBCEN_BIT | MDM_RXGFEN_BIT  |
            MDM_RXMMEN_BIT   | MDM_RXDSSSEN_BIT);
    mdm_rxnssmax_setf(mdm_nss_getf() - 1);
    mdm_rxndpnstsmax_setf(mdm_nsts_getf() - 1);
    mdm_rxldpcen_setf(mdm_ldpcdec_getf());
    mdm_rxvhten_setf(phy_vht_supported());
    mdm_rxmumimoen_setf(mdm_mumimorx_getf());
    // mdm_rxmumimoapeplenen_setf(mdm_mumimorx_getf());star c

    // Set DSSS precomp
    mdm_precomp_setf(45);

    // Allow GF/SGI/STBC (bit14 reset) - TEMPORARY!!!!
    mdm_rxframeviolationmask_setf(0xFFFFBFFF);

    mdm_txmode_set(MDM_TXSTBCEN_BIT | MDM_TXGFEN_BIT  |
            MDM_TXMMEN_BIT   | MDM_TXDSSSEN_BIT);
    mdm_txnssmax_setf(1);
    mdm_ntxmax_setf(1);
    mdm_txcbwmax_setf(mdm_chbw_getf());
    mdm_txldpcen_setf(mdm_ldpcenc_getf());
    mdm_txvhten_setf(phy_vht_supported());
    mdm_txmumimoen_setf(mdm_mumimotx_getf());

    // AGC reset mode - don't turn off RF if rxreq de-asserted for few cycles after a RXERR
    mdm_rxtdctrl1_set(mdm_rxtdctrl1_get() | 0x01);

    // Enable automatic smoothing filter selection from SNR, then disable force
    mdm_cfgsmoothforce_setf(0);

    // Limit NDBPSMAX to 1x1 80 MCS9 LGI(390.0Mb/s) / SGI (433.3Mb/s)
    mdm_rxctrl1_set(0x06180618);

    // Adjust FFT short GI 80MHz window
    mdm_td_adjust_80_short_gi_setf(3);

    //PHYDSSSCONFIG_set_mdmbcntl2(32'h002d1205);
    mdm_mdmbcntl2_set(0x002d1205);

    // Enable RC clock
    mdm_rcclkforce_setf(1);

    /***************************************************************************
     * RIU INIT
     **************************************************************************/
    // limit RIU to 1 active antenna
    riu_activeant_setf(1);
    /* limit AGC with a single antenna (path0) */
    riu_combpathsel_setf(1);

    // Tx Digital gain
    riu_rwnxfectrl0_set(0x001a1a1a);

    /// @todo Check if we need to do that
    /*riu_rwnxagcevt2_set(0);*/ /* write 0 to disable disappearence detection */
    riu_crossupthrqdbm_setf(0x200); /* write 0x200 to disable AGC cross-up */
    riu_crossdnthrqdbm_setf(0x200); /* write 0x200 to disable AGC cross-down */

    riu_rwnxagcccatimeout_set(8000000); // 100ms
    riu_irqmacccatimeouten_setf(1);

    /***************************************************************************
     * RC INIT
     **************************************************************************/
    /// RC delays - @todo update values
    rc_hw_rx2tx_delay_set(0x00F000F0);
    rc_hw_rxidle2rx_delay_set(RC_HW_RXIDLE2RX_DELAY_RESET);
    rc_extpa_seq_delay_set(0x00060001);
    rc_hw_rx_agcgainref_set(0x00450007);


    if(wifi_band == WIFI_24G) {
        rc_static_cfg_set(1);
        //`RCAETNENSIS_setf_APICMDADDR LB (16'h1440);
        rc_sw_rfapicmd_addr_set(0x1440);
        rc_hw_rxgain_pack(0x0801, 63);
    } else {
	rc_static_cfg_set(0);
	rc_sw_rfapicmd_addr_set(0x1400);
        rc_hw_rxgain_pack(0x0001, 63);
    }

    rc_trxid_setf(wifi_band == WIFI_24G);
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
    rc_txmode_setf(6);
    rc_rxmode_setf(5);

    // Set gain table
    phy_set_table(gaintable, RCG_RWNXAGCGT0_ADDR, REG_RC_NFTABLE_COUNT);
    // Set noise factor table
    phy_set_table(nftable, RCN_RWNXAGCNFT0_ADDR, REG_RC_NFTABLE_COUNT);


    /***************************************************************************
	 * AETNENSIS INIT
	 **************************************************************************/
    if(rf_trx_prepare(wifi_band == WIFI_24G))
    {
#ifndef CFG_RWTL
        printf("rf_trx_prepare failed!\n");
#endif
        return -1;
    }
    /***************************************************************************
     * DEFAULT CHANNEL
     **************************************************************************/
    if(wifi_band == WIFI_24G) {
        phy_hw_set_channel(PHY_BAND_2G4, 2462, 2452, PHY_CHNL_BW_40, 0);
    } else {
        phy_hw_set_channel(PHY_BAND_5G, 5510, 5500, PHY_CHNL_BW_40, 0);
    }


    /***************************************************************************
     * MAC BYPASS INIT
     **************************************************************************/
    macbyp_trigger_set(0x00000012);
    macbyp_ctrl_set(0x00000100);

    return 0;
}

uint8_t wifi_phy_init(const struct phy_cfg_tag *config)
{
	uint8_t ret = 0;
	const struct phy_aetnensis_cfg_tag *cfg =
			(const struct phy_aetnensis_cfg_tag *)&config->parameters;

	if ((ret = phy_hw_init(cfg)) != 0) {
		printf("phy_hw_init failed!\n");
		return ret;
	}
	memcpy(&phy_env->cfg, cfg, sizeof(struct phy_aetnensis_cfg_tag));
	phy_env->band = PHY_BAND_5G;
	phy_env->chnl_type = PHY_CHNL_BW_20;
	phy_env->chnl_prim20_freq = phy_env->chnl_center1_freq =
			phy_env->chnl_center2_freq = PHY_UNUSED;
	return 0;
}

void phy_mdm_isr(void)
{
#ifndef CFG_RWTL
	printf("%s: TODO\n", __func__);
#endif
}

void phy_rc_isr(void)
{
    uint32_t irq_status = riu_rwnxmacintstatmasked_get();

    riu_rwnxmacintack_clear(irq_status);

#if NX_RADAR_DETECT
    if (irq_status & RIU_IRQMACRADARDETMASKED_BIT)
    {
        PROF_RADAR_IRQ_SET();
        rd_event_ind(PHY_PRIM);
        PROF_RADAR_IRQ_CLR();
    }
#endif

    if (irq_status & RIU_IRQMACCCATIMEOUTMASKED_BIT)
        mdm_reset();

    ASSERT_REC(!(irq_status & RIU_IRQMACCCATIMEOUTMASKED_BIT));
}

void phy_get_version(uint32_t *version_1, uint32_t *version_2)
{
    //*version_1 = mdm_hdmconfig_get();//star change
    *version_1 = mdm_mdmconf_get();
    // TODO Add version reading for other PHY elements than modem.
    *version_2 = 0;
}

void phy_set_channel(uint8_t band, uint8_t type, uint16_t prim20_freq,
        uint16_t center1_freq, uint16_t center2_freq, uint8_t index)
{
    if (index > 0)
    {
#ifndef CFG_RWTL
        printf("%s: radio %d does not exist\n", __func__, index);
#endif
        return;
    }

    if (phy_env->band == band && phy_env->chnl_type == type &&
            phy_env->chnl_prim20_freq == prim20_freq &&
            phy_env->chnl_center1_freq == center1_freq &&
            phy_env->chnl_center2_freq == center2_freq)
    {
#ifndef CFG_RWTL
        printf("%s: Setting same channel, do nothing\n", __func__);
#endif
        return;
    }

#ifndef CFG_RWTL
    printf("%s: c:%d c1:%d bw:%d\n", __func__,
            prim20_freq, center1_freq, (1 << type) * 20);
#endif

    phy_hw_set_channel(band, prim20_freq, center1_freq, type, index);

    phy_env->band              = band;
    phy_env->chnl_type         = type;
    phy_env->chnl_prim20_freq  = prim20_freq;
    phy_env->chnl_center1_freq = center1_freq;
    phy_env->chnl_center2_freq = center2_freq;
}

void phy_get_channel(struct phy_channel_info *info, uint8_t index)
{
    if (index > 0)
    {
#ifndef CFG_RWTL
        printf("%s: radio %d does not exist\n", __func__, index);
#endif
    }
    // Map the band, channel type, primary channel index and center 1 index on info1
    info->info1 = phy_env->band | (phy_env->chnl_type << 8) | (phy_env->chnl_prim20_freq << 16);

    // Map center 2 index on info2
    info->info2 =  phy_env->chnl_center1_freq | (phy_env->chnl_center2_freq << 16);
}

void phy_stop(void)
{
    if (nxmac_current_state_getf() != HW_IDLE)
#ifndef CFG_RWTL
        printf("%s MAC state != IDLE\n", __func__);
#endif

    //this will set the aet_rf_state to off
    rf_trx_stop(wifi_band == WIFI_24G);
}

#if NX_RADAR_DETECT
bool phy_has_radar_pulse(int rd_idx)
{

    ASSERT_ERR(rd_idx == PHY_PRIM);

    return (riu_radfifoempty_getf() == 0);
}

bool phy_get_radar_pulse(int rd_idx, struct phy_radar_pulse *pulse)
{
    ASSERT_ERR(rd_idx == PHY_PRIM);

    // Check if FIFO is empty
    if (riu_radfifoempty_getf())
        return (false);

    pulse->pulse = REG_PL_RD(__RIU_RADARFIFO_ADDR);

    return (true);
}
#endif

void mpif_clk_init(void)
{
}
#if 0
bool phy_vht_supported(void)
{
#if NX_MDM_VER > 20
    return ((mdm_vht_getf() != 0) || (mdm_chbw_getf() > PHY_CHNL_BW_40));
#else
    return true;
#endif
}
#endif
bool phy_bfmee_supported(void)
{
    return (mdm_bfmee_getf() != 0);
}

bool phy_bfmer_supported(void)
{
    return (mdm_bfmer_getf() != 0);
}

bool phy_mu_mimo_rx_supported(void)
{
    return (mdm_mumimorx_getf() != 0);
}

bool phy_mu_mimo_tx_supported(void)
{
    return (mdm_mumimotx_getf() != 0);
}

#if RW_MUMIMO_RX_EN
void phy_set_group_id_info(uint32_t membership_addr, uint32_t userpos_addr)
{
    int i;

    // Set membership status
    for(i=0; i<MDM_MUMIMO_GROUPID_TAB_COUNT; i++)
    {
        mdm_mumimo_groupid_tab_set(i, co_read32p(membership_addr + 4 * i));
    }

    // Set user position
    for(i=0; i<MDM_MUMIMO_USERPOSITION_TAB_COUNT; i++)
    {
        mdm_mumimo_userposition_tab_set(i, co_read32p(userpos_addr + 4 * i));
    }
}
#endif

uint8_t phy_get_nss(void)
{
    return (mdm_nss_getf() - 1);
}

uint8_t phy_get_ntx(void)
{
    return (mdm_ntx_getf() - 1);
}

#if RW_BFMER_EN
uint8_t phy_get_bfr_mem_size(void)
{
    return (mdm_bfmer_mem_size_getf());
}
#endif

void phy_get_rf_gain_idx(int8_t *power, uint8_t *idx)
{
    /* TO BE COMPLETED. For now set idx to power for debug */
    *idx = (uint8_t)*power;
}

void phy_get_rf_gain_capab(int8_t *max, int8_t *min)
{
    /* TO BE UPDATED with correct value */
    *max = 25; // dBm
    *min = 0;  // dBm
}

#if NX_DEBUG_DUMP
void phy_get_diag_state(struct printf_debug_info_tag *printf_info)
{
}
#endif
