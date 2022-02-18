#ifndef _RF_DEF_DATA_H_
#define _RF_DEF_DATA_H_

//AETNEN_Register_Definition_C0.xlsm v0.10
struct aet_reg_pair{
    uint16_t addr;
    uint16_t value;
};

const static struct aet_reg_pair g_rf_def_data[]={
{0x0001,0x001F},// hb_rx_gain_ptr (reset value = 0000)
{0x0003,0x000A},// hb_tx_bias_sel (reset value = 0000)
{0x0005,0x00F0},// hb_rx_rf_appl_ctrl (reset value = 0000)
{0x0204,0x0000},// hb_wbpll_tune_settings (reset value = 0606)
{0x0282,0x0666},// hb_rx_xcorr_ph_incr (reset value = 05B7)
{0x0285,0x0666},// hb_tx_tone_freq0 (reset value = 0000)
{0x0286,0x0CCC},// hb_tx_tone_freq1 (reset value = 0000)
{0x0289,0x2500},// hb_tx_tone_amplitude0 (reset value = 0000)
{0x0341,0x0600},// hb_rx_adc_enable (reset value = 0000)
{0x0342,0x3222},// hb_rx_bw (reset value = 0000)
{0x036C,0x0002},// hb_rx_dbb_iqc_config (reset value = 0000)
{0x0380,0x0100},// hb_tx_clock_enable (reset value = 0000)
{0x0381,0x0600},// hb_tx_dac_enable (reset value = 0000)
{0x0382,0x3202},// hb_tx_bw (reset value = 0000)
{0x0383,0x0008},// hb_tx_dbb_config (reset value = 0000)
{0x0500,0x04FC},// hb_rx_gain_table_0 (reset value = 1E81)
{0x0501,0x052C},// hb_rx_gain_table_1 (reset value = 1E82)
{0x0502,0x0534},// hb_rx_gain_table_2 (reset value = 0014)
{0x0503,0x053C},// hb_rx_gain_table_3 (reset value = 001C)
{0x0504,0x08A8},// hb_rx_gain_table_4 (reset value = 0024)
{0x0505,0x08B0},// hb_rx_gain_table_5 (reset value = 002C)
{0x0506,0x08B8},// hb_rx_gain_table_6 (reset value = 0034)
{0x0507,0x08E8},// hb_rx_gain_table_7 (reset value = 0064)
{0x0508,0x08F0},// hb_rx_gain_table_8 (reset value = 006C)
{0x0509,0x08F8},// hb_rx_gain_table_9 (reset value = 0074)
{0x050A,0x0CA4},// hb_rx_gain_table_10 (reset value = 00A4)
{0x050B,0x0CAC},// hb_rx_gain_table_11 (reset value = 00AC)
{0x050C,0x0CB4},// hb_rx_gain_table_12 (reset value = 00B4)
{0x050D,0x0CBC},// hb_rx_gain_table_13 (reset value = 0460)
{0x050E,0x0CEC},// hb_rx_gain_table_14 (reset value = 0468)
{0x050F,0x0CF4},// hb_rx_gain_table_15 (reset value = 0470)
{0x0510,0x0CFC},// hb_rx_gain_table_16 (reset value = 04A0)
{0x0511,0x10A8},// hb_rx_gain_table_17 (reset value = 04A8)
{0x0512,0x10B0},// hb_rx_gain_table_18 (reset value = 04B0)
{0x0513,0x10B8},// hb_rx_gain_table_19 (reset value = 085C)
{0x0514,0x10E8},// hb_rx_gain_table_20 (reset value = 0864)
{0x0515,0x10F0},// hb_rx_gain_table_21 (reset value = 086C)
{0x0516,0x10F8},// hb_rx_gain_table_22 (reset value = 089C)
{0x0517,0x14A4},// hb_rx_gain_table_23 (reset value = 08A4)
{0x0518,0x14AC},// hb_rx_gain_table_24 (reset value = 08AC)
{0x0519,0x14B4},// hb_rx_gain_table_25 (reset value = 0C58)
{0x051A,0x14BC},// hb_rx_gain_table_26 (reset value = 0C60)
{0x051B,0x14EC},// hb_rx_gain_table_27 (reset value = 0C68)
{0x051C,0x14F4},// hb_rx_gain_table_28 (reset value = 1014)
{0x051D,0x14FC},// hb_rx_gain_table_29 (reset value = 101C)
{0x051E,0x18A8},// hb_rx_gain_table_30 (reset value = 1024)
{0x051F,0x18B0},// hb_rx_gain_table_31 (reset value = 1054)
{0x0520,0x18B8},// hb_rx_gain_table_32 (reset value = 105C)
{0x0521,0x18E8},// hb_rx_gain_table_33 (reset value = 1064)
{0x0522,0x18F0},// hb_rx_gain_table_34 (reset value = 1094)
{0x0523,0x18F8},// hb_rx_gain_table_35 (reset value = 109C)
{0x0524,0x1928},// hb_rx_gain_table_36 (reset value = 10A4)
{0x0525,0x1CAC},// hb_rx_gain_table_37 (reset value = 1458)
{0x0526,0x1CB4},// hb_rx_gain_table_38 (reset value = 1460)
{0x0527,0x1CBC},// hb_rx_gain_table_39 (reset value = 1468)
{0x0528,0x1CEC},// hb_rx_gain_table_40 (reset value = 1818)
{0x0529,0x1CF4},// hb_rx_gain_table_41 (reset value = 1820)
{0x052A,0x1CFC},// hb_rx_gain_table_42 (reset value = 1828)
{0x052B,0x1D2C},// hb_rx_gain_table_43 (reset value = 1858)
{0x052C,0x1D34},// hb_rx_gain_table_44 (reset value = 1860)
{0x052D,0x1D3C},// hb_rx_gain_table_45 (reset value = 1868)
{0x052E,0x1D6C},// hb_rx_gain_table_46 (reset value = 1C20)
{0x052F,0x1D74},// hb_rx_gain_table_47 (reset value = 1C28)
{0x0530,0x1D7C},// hb_rx_gain_table_48 (reset value = 1C30)
{0x0531,0x1DAC},// hb_rx_gain_table_49 (reset value = 1C60)
{0x0532,0x1DB4},// hb_rx_gain_table_50 (reset value = 1C68)
{0x0533,0x1DBC},// hb_rx_gain_table_51 (reset value = 1C70)
{0x0534,0x1DEC},// hb_rx_gain_table_52 (reset value = 1CA0)
{0x0535,0x1DF4},// hb_rx_gain_table_53 (reset value = 1CA8)
{0x0536,0x1DFC},// hb_rx_gain_table_54 (reset value = 1CB0)
{0x0537,0x1E2C},// hb_rx_gain_table_55 (reset value = 1CE0)
{0x0538,0x1E34},// hb_rx_gain_table_56 (reset value = 1CE8)
{0x0539,0x1E3C},// hb_rx_gain_table_57 (reset value = 1CF0)
{0x053A,0x1E6C},// hb_rx_gain_table_58 (reset value = 1D20)
{0x053B,0x1E74},// hb_rx_gain_table_59 (reset value = 1D28)
{0x053C,0x1E7C},// hb_rx_gain_table_60 (reset value = 1D30)
{0x053D,0x1EAC},// hb_rx_gain_table_61 (reset value = 1D60)
{0x053E,0x1EB4},// hb_rx_gain_table_62 (reset value = 1D68)
{0x053F,0x1EBC},// hb_rx_gain_table_63 (reset value = 1D70)
{0x0540,0x0400},// hb_rx_rf_gain_table_0_1 (reset value = 0804)
{0x0541,0x0C08},// hb_rx_rf_gain_table_2_3 (reset value = 100C)
{0x0542,0x1410},// hb_rx_rf_gain_table_4_5 (reset value = 1814)
{0x0543,0x1615},// hb_rx_rf_gain_table_6_7 (reset value = 1918)
{0x0570,0x1000},// hb_rx_abb_gain_table_0_1 (reset value = 0000)
{0x0571,0x1101},// hb_rx_abb_gain_table_2_3 (reset value = 0000)
{0x0572,0x1303},// hb_rx_abb_gain_table_4_5 (reset value = 1000)
{0x0573,0x1707},// hb_rx_abb_gain_table_6_7 (reset value = 1808)
{0x0574,0x1F0F},// hb_rx_abb_gain_table_8_9 (reset value = 1C0C)
{0x0575,0x3F3F},// hb_rx_abb_gain_table_10_11 (reset value = 1D0D)
{0x0576,0x3F3F},// hb_rx_abb_gain_table_12_13 (reset value = 1F0F)
{0x0577,0x333F},// hb_rx_abb_gain_table_14_15 (reset value = 003F)
{0x0580,0x01B0},// hb_rx_dbb_fine_gain_table_0 (reset value = 050C)
{0x0581,0x01C9},// hb_rx_dbb_fine_gain_table_1 (reset value = 0559)
{0x0582,0x01E4},// hb_rx_dbb_fine_gain_table_2 (reset value = 05AA)
{0x0583,0x0201},// hb_rx_dbb_fine_gain_table_3 (reset value = 0600)
{0x0584,0x021F},// hb_rx_dbb_fine_gain_table_4 (reset value = 065B)
{0x0585,0x0240},// hb_rx_dbb_fine_gain_table_5 (reset value = 06BB)
{0x0586,0x0262},// hb_rx_dbb_fine_gain_table_6 (reset value = 0721)
{0x0587,0x0286},// hb_rx_dbb_fine_gain_table_7 (reset value = 078D)
{0x0588,0x02AC},// hb_rx_dbb_fine_gain_table_8 (reset value = 0800)
{0x0589,0x02D5},// hb_rx_dbb_fine_gain_table_9 (reset value = 0879)
{0x058A,0x0300},// hb_rx_dbb_fine_gain_table_10 (reset value = 08FA)
{0x058B,0x032D},// hb_rx_dbb_fine_gain_table_11 (reset value = 0982)
{0x058C,0x035D},// hb_rx_dbb_fine_gain_table_12 (reset value = 0A12)
{0x058D,0x0390},// hb_rx_dbb_fine_gain_table_13 (reset value = 0AAB)
{0x058E,0x03C6},// hb_rx_dbb_fine_gain_table_14 (reset value = 0B4D)
{0x058F,0x0400},// hb_rx_dbb_fine_gain_table_15 (reset value = 0BF8)
{0x05A0,0x000A},// hb_rx_adc_tmreg0 (reset value = 0008)
{0x05A1,0x0008},// hb_rx_adc_tmreg1 (reset value = 000C)
{0x05A4,0x0003},// hb_rx_adc_tmreg4 (reset value = 0000)
{0x05A5,0x0003},// hb_rx_adc_tmreg5 (reset value = 0000)
{0x05A6,0x0000},// hb_rx_adc_tmreg6 (reset value = 0080)
{0x05AA,0x00D8},// hb_rx_adc_tmreg10 (reset value = 0018)
{0x05AF,0x0008},// hb_rx_adc_tmreg15 (reset value = 0000)
{0x0600,0x0018},// hb_tx_gain_table_0 (reset value = 0000)
{0x0601,0x001E},// hb_tx_gain_table_1 (reset value = 0000)
{0x0602,0x002E},// hb_tx_gain_table_2 (reset value = 0000)
{0x0603,0x006E},// hb_tx_gain_table_3 (reset value = 0000)
{0x0604,0x007E},// hb_tx_gain_table_4 (reset value = 0000)
{0x0605,0x008E},// hb_tx_gain_table_5 (reset value = 0000)
{0x0606,0x00A9},// hb_tx_gain_table_6 (reset value = 0000)
{0x0607,0x00AF},// hb_tx_gain_table_7 (reset value = 0000)
{0x0608,0x00F7},// hb_tx_gain_table_8 (reset value = 0000)
{0x0609,0x00F9},// hb_tx_gain_table_9 (reset value = 0000)
{0x060A,0x00FB},// hb_tx_gain_table_10 (reset value = 0000)
{0x060B,0x00FD},// hb_tx_gain_table_11 (reset value = 0000)
{0x060C,0x0148},// hb_tx_gain_table_12 (reset value = 0000)
{0x060D,0x014A},// hb_tx_gain_table_13 (reset value = 0000)
{0x060E,0x014C},// hb_tx_gain_table_14 (reset value = 0000)
{0x060F,0x0158},// hb_tx_gain_table_15 (reset value = 0000)
{0x0610,0x015A},// hb_tx_gain_table_16 (reset value = 0000)
{0x0611,0x0167},// hb_tx_gain_table_17 (reset value = 0000)
{0x0612,0x0169},// hb_tx_gain_table_18 (reset value = 0000)
{0x0613,0x016B},// hb_tx_gain_table_19 (reset value = 0000)
{0x0614,0x016D},// hb_tx_gain_table_20 (reset value = 0000)
{0x0615,0x0184},// hb_tx_gain_table_21 (reset value = 0000)
{0x0616,0x0186},// hb_tx_gain_table_22 (reset value = 0000)
{0x0617,0x0188},// hb_tx_gain_table_23 (reset value = 0000)
{0x0618,0x018A},// hb_tx_gain_table_24 (reset value = 0000)
{0x0619,0x018C},// hb_tx_gain_table_25 (reset value = 0000)
{0x061A,0x018E},// hb_tx_gain_table_26 (reset value = 0000)
{0x061B,0x01EA},// hb_tx_gain_table_27 (reset value = 0000)
{0x061C,0x01EC},// hb_tx_gain_table_28 (reset value = 0000)
{0x061D,0x01EF},// hb_tx_gain_table_29 (reset value = 0000)
{0x061E,0x01FC},// hb_tx_gain_table_30 (reset value = 0000)
{0x061F,0x01FF},// hb_tx_gain_table_31 (reset value = 0000)
{0x0621,0x0201},// hb_tx_rf_gain_table_2_3 (reset value = 0000)
{0x0622,0x0403},// hb_tx_rf_gain_table_4_5 (reset value = 0000)
{0x0623,0x0908},// hb_tx_rf_gain_table_6_7 (reset value = 0000)
{0x0624,0x0B0A},// hb_tx_rf_gain_table_8_9 (reset value = 0000)
{0x0625,0x0D0C},// hb_tx_rf_gain_table_10_11 (reset value = 0000)
{0x0626,0x1211},// hb_tx_rf_gain_table_12_13 (reset value = 0000)
{0x0627,0x1413},// hb_tx_rf_gain_table_14_15 (reset value = 0000)
{0x0628,0x1615},// hb_tx_rf_gain_table_16_17 (reset value = 0000)
{0x0629,0x1A19},// hb_tx_rf_gain_table_18_19 (reset value = 0000)
{0x062A,0x1C1B},// hb_tx_rf_gain_table_20_21 (reset value = 0000)
{0x062B,0x1E1D},// hb_tx_rf_gain_table_22_23 (reset value = 0000)
{0x062C,0x211F},// hb_tx_rf_gain_table_24_25 (reset value = 0000)
{0x062D,0x2322},// hb_tx_rf_gain_table_26_27 (reset value = 0000)
{0x062E,0x2524},// hb_tx_rf_gain_table_28_29 (reset value = 0000)
{0x062F,0x2726},// hb_tx_rf_gain_table_30_31 (reset value = 0000)
{0x0670,0x01B0},// hb_tx_dbb_gain_table_0 (reset value = 0060)
{0x0671,0x01C9},// hb_tx_dbb_gain_table_1 (reset value = 0393)
{0x0672,0x01E4},// hb_tx_dbb_gain_table_2 (reset value = 03C9)
{0x0673,0x0201},// hb_tx_dbb_gain_table_3 (reset value = 0402)
{0x0674,0x021F},// hb_tx_dbb_gain_table_4 (reset value = 043F)
{0x0675,0x0240},// hb_tx_dbb_gain_table_5 (reset value = 0480)
{0x0676,0x0262},// hb_tx_dbb_gain_table_6 (reset value = 04C4)
{0x0677,0x0286},// hb_tx_dbb_gain_table_7 (reset value = 050C)
{0x0678,0x02AC},// hb_tx_dbb_gain_table_8 (reset value = 0559)
{0x0679,0x02D5},// hb_tx_dbb_gain_table_9 (reset value = 05AA)
{0x067A,0x0300},// hb_tx_dbb_gain_table_10 (reset value = 0600)
{0x067B,0x032D},// hb_tx_dbb_gain_table_11 (reset value = 065B)
{0x067C,0x035D},// hb_tx_dbb_gain_table_12 (reset value = 06BB)
{0x067D,0x0390},// hb_tx_dbb_gain_table_13 (reset value = 0721)
{0x067E,0x03C6},// hb_tx_dbb_gain_table_14 (reset value = 078D)
{0x067F,0x0400},// hb_tx_dbb_gain_table_15 (reset value = 0800)
{0x0801,0x001F},// lb_rx_gain_ptr (reset value = 0000)
{0x0803,0x000A},// lb_tx_bias_sel (reset value = 0000)
{0x0805,0x00F0},// lb_rx_rf_appl_ctrl (reset value = 0000)
{0x0A04,0x0000},// lb_wbpll_tune_settings (reset value = 0606)
{0x0A82,0x0666},// lb_rx_xcorr_ph_incr (reset value = 05B7)
{0x0A85,0x0666},// lb_tx_tone_freq0 (reset value = 0000)
{0x0A86,0x0CCC},// lb_tx_tone_freq1 (reset value = 0000)
{0x0A89,0x2500},// lb_tx_tone_amplitude0 (reset value = 0000)
{0x0B40,0x0100},// lb_rx_clock_enable (reset value = 0000)
{0x0B41,0x0600},// lb_rx_adc_enable (reset value = 0000)
{0x0B42,0x3111},// lb_rx_bw (reset value = 0000)
{0x0B6C,0x0002},// lb_rx_dbb_iqc_config (reset value = 0000)
{0x0B80,0x0100},// lb_tx_clock_enable (reset value = 0000)
{0x0B81,0x0600},// lb_tx_dac_enable (reset value = 0000)
{0x0B82,0x3103},// lb_tx_bw (reset value = 0000)
{0x0B83,0x0008},// lb_tx_dbb_config (reset value = 0000)
{0x0D00,0x04B8},// lb_rx_gain_table_0 (reset value = 1E81)
{0x0D01,0x04E8},// lb_rx_gain_table_1 (reset value = 1E82)
{0x0D02,0x04F0},// lb_rx_gain_table_2 (reset value = 0014)
{0x0D03,0x04F8},// lb_rx_gain_table_3 (reset value = 001C)
{0x0D04,0x0528},// lb_rx_gain_table_4 (reset value = 0024)
{0x0D05,0x0530},// lb_rx_gain_table_5 (reset value = 002C)
{0x0D06,0x0538},// lb_rx_gain_table_6 (reset value = 0034)
{0x0D07,0x08AC},// lb_rx_gain_table_7 (reset value = 0064)
{0x0D08,0x08B4},// lb_rx_gain_table_8 (reset value = 006C)
{0x0D09,0x08BC},// lb_rx_gain_table_9 (reset value = 0074)
{0x0D0A,0x08EC},// lb_rx_gain_table_10 (reset value = 00A4)
{0x0D0B,0x08F4},// lb_rx_gain_table_11 (reset value = 00AC)
{0x0D0C,0x0CA8},// lb_rx_gain_table_12 (reset value = 00B4)
{0x0D0D,0x0CB0},// lb_rx_gain_table_13 (reset value = 0460)
{0x0D0E,0x0CB8},// lb_rx_gain_table_14 (reset value = 0468)
{0x0D0F,0x0CE8},// lb_rx_gain_table_15 (reset value = 0470)
{0x0D10,0x0CF0},// lb_rx_gain_table_16 (reset value = 04A0)
{0x0D11,0x0CF8},// lb_rx_gain_table_17 (reset value = 04A8)
{0x0D12,0x10AC},// lb_rx_gain_table_18 (reset value = 04B0)
{0x0D13,0x10B4},// lb_rx_gain_table_19 (reset value = 085C)
{0x0D14,0x10BC},// lb_rx_gain_table_20 (reset value = 0864)
{0x0D15,0x10EC},// lb_rx_gain_table_21 (reset value = 086C)
{0x0D16,0x10F4},// lb_rx_gain_table_22 (reset value = 089C)
{0x0D17,0x10FC},// lb_rx_gain_table_23 (reset value = 08A4)
{0x0D18,0x14B0},// lb_rx_gain_table_24 (reset value = 08AC)
{0x0D19,0x14B8},// lb_rx_gain_table_25 (reset value = 0C58)
{0x0D1A,0x14E8},// lb_rx_gain_table_26 (reset value = 0C60)
{0x0D1B,0x14F0},// lb_rx_gain_table_27 (reset value = 0C68)
{0x0D1C,0x14F8},// lb_rx_gain_table_28 (reset value = 1014)
{0x0D1D,0x18A8},// lb_rx_gain_table_29 (reset value = 101C)
{0x0D1E,0x18B0},// lb_rx_gain_table_30 (reset value = 1024)
{0x0D1F,0x18B8},// lb_rx_gain_table_31 (reset value = 1054)
{0x0D20,0x18E8},// lb_rx_gain_table_32 (reset value = 105C)
{0x0D21,0x18F0},// lb_rx_gain_table_33 (reset value = 1064)
{0x0D22,0x18F8},// lb_rx_gain_table_34 (reset value = 1094)
{0x0D23,0x1928},// lb_rx_gain_table_35 (reset value = 109C)
{0x0D24,0x1930},// lb_rx_gain_table_36 (reset value = 10A4)
{0x0D25,0x1938},// lb_rx_gain_table_37 (reset value = 1458)
{0x0D26,0x1CB4},// lb_rx_gain_table_38 (reset value = 1460)
{0x0D27,0x1CBC},// lb_rx_gain_table_39 (reset value = 1468)
{0x0D28,0x1CEC},// lb_rx_gain_table_40 (reset value = 1818)
{0x0D29,0x1CF4},// lb_rx_gain_table_41 (reset value = 1820)
{0x0D2A,0x1CFC},// lb_rx_gain_table_42 (reset value = 1828)
{0x0D2B,0x1D2C},// lb_rx_gain_table_43 (reset value = 1858)
{0x0D2C,0x1D34},// lb_rx_gain_table_44 (reset value = 1860)
{0x0D2D,0x1D3C},// lb_rx_gain_table_45 (reset value = 1868)
{0x0D2E,0x1D6C},// lb_rx_gain_table_46 (reset value = 1C20)
{0x0D2F,0x1D74},// lb_rx_gain_table_47 (reset value = 1C28)
{0x0D30,0x1D7C},// lb_rx_gain_table_48 (reset value = 1C30)
{0x0D31,0x1DAC},// lb_rx_gain_table_49 (reset value = 1C60)
{0x0D32,0x1DB4},// lb_rx_gain_table_50 (reset value = 1C68)
{0x0D33,0x1DBC},// lb_rx_gain_table_51 (reset value = 1C70)
{0x0D34,0x1DEC},// lb_rx_gain_table_52 (reset value = 1CA0)
{0x0D35,0x1DF4},// lb_rx_gain_table_53 (reset value = 1CA8)
{0x0D36,0x1DFC},// lb_rx_gain_table_54 (reset value = 1CB0)
{0x0D37,0x1E2C},// lb_rx_gain_table_55 (reset value = 1CE0)
{0x0D38,0x1E34},// lb_rx_gain_table_56 (reset value = 1CE8)
{0x0D39,0x1E3C},// lb_rx_gain_table_57 (reset value = 1CF0)
{0x0D3A,0x1E6C},// lb_rx_gain_table_58 (reset value = 1D20)
{0x0D3B,0x1E74},// lb_rx_gain_table_59 (reset value = 1D28)
{0x0D3C,0x1E7C},// lb_rx_gain_table_60 (reset value = 1D30)
{0x0D3D,0x1EAC},// lb_rx_gain_table_61 (reset value = 1D60)
{0x0D3E,0x1EB4},// lb_rx_gain_table_62 (reset value = 1D68)
{0x0D3F,0x1EBC},// lb_rx_gain_table_63 (reset value = 1D70)
{0x0D40,0x0400},// lb_rx_rf_gain_table_0_1 (reset value = 0804)
{0x0D41,0x0C08},// lb_rx_rf_gain_table_2_3 (reset value = 100C)
{0x0D42,0x1410},// lb_rx_rf_gain_table_4_5 (reset value = 1814)
{0x0D43,0x1615},// lb_rx_rf_gain_table_6_7 (reset value = 1918)
{0x0D70,0x1000},// lb_rx_abb_gain_table_0_1 (reset value = 0000)
{0x0D71,0x1101},// lb_rx_abb_gain_table_2_3 (reset value = 0000)
{0x0D72,0x1303},// lb_rx_abb_gain_table_4_5 (reset value = 1000)
{0x0D73,0x1707},// lb_rx_abb_gain_table_6_7 (reset value = 1808)
{0x0D74,0x1F0F},// lb_rx_abb_gain_table_8_9 (reset value = 1C0C)
{0x0D75,0x3F3F},// lb_rx_abb_gain_table_10_11 (reset value = 1D0D)
{0x0D76,0x3F3F},// lb_rx_abb_gain_table_12_13 (reset value = 1F0F)
{0x0D77,0x333F},// lb_rx_abb_gain_table_14_15 (reset value = 003F)
{0x0D80,0x01B0},// lb_rx_dbb_fine_gain_table_0 (reset value = 050C)
{0x0D81,0x01C9},// lb_rx_dbb_fine_gain_table_1 (reset value = 0559)
{0x0D82,0x01E4},// lb_rx_dbb_fine_gain_table_2 (reset value = 05AA)
{0x0D83,0x0201},// lb_rx_dbb_fine_gain_table_3 (reset value = 0600)
{0x0D84,0x021F},// lb_rx_dbb_fine_gain_table_4 (reset value = 065B)
{0x0D85,0x0240},// lb_rx_dbb_fine_gain_table_5 (reset value = 06BB)
{0x0D86,0x0262},// lb_rx_dbb_fine_gain_table_6 (reset value = 0721)
{0x0D87,0x0286},// lb_rx_dbb_fine_gain_table_7 (reset value = 078D)
{0x0D88,0x02AC},// lb_rx_dbb_fine_gain_table_8 (reset value = 0800)
{0x0D89,0x02D5},// lb_rx_dbb_fine_gain_table_9 (reset value = 0879)
{0x0D8A,0x0300},// lb_rx_dbb_fine_gain_table_10 (reset value = 08FA)
{0x0D8B,0x032D},// lb_rx_dbb_fine_gain_table_11 (reset value = 0982)
{0x0D8C,0x035D},// lb_rx_dbb_fine_gain_table_12 (reset value = 0A12)
{0x0D8D,0x0390},// lb_rx_dbb_fine_gain_table_13 (reset value = 0AAB)
{0x0D8E,0x03C6},// lb_rx_dbb_fine_gain_table_14 (reset value = 0B4D)
{0x0D8F,0x0400},// lb_rx_dbb_fine_gain_table_15 (reset value = 0BF8)
{0x0DA0,0x000A},// lb_rx_adc_tmreg0 (reset value = 0008)
{0x0DA1,0x0008},// lb_rx_adc_tmreg1 (reset value = 000C)
{0x0DA4,0x0003},// lb_rx_adc_tmreg4 (reset value = 0000)
{0x0DA5,0x0003},// lb_rx_adc_tmreg5 (reset value = 0000)
{0x0DA6,0x0000},// lb_rx_adc_tmreg6 (reset value = 0080)
{0x0DAA,0x00D8},// lb_rx_adc_tmreg10 (reset value = 0018)
{0x0DAF,0x0008},// lb_rx_adc_tmreg15 (reset value = 0000)
{0x0E00,0x0017},// lb_tx_gain_table_0 (reset value = 0000)
{0x0E01,0x001D},// lb_tx_gain_table_1 (reset value = 0000)
{0x0E02,0x002D},// lb_tx_gain_table_2 (reset value = 0000)
{0x0E03,0x003D},// lb_tx_gain_table_3 (reset value = 0000)
{0x0E04,0x004D},// lb_tx_gain_table_4 (reset value = 0000)
{0x0E05,0x008D},// lb_tx_gain_table_5 (reset value = 0000)
{0x0E06,0x009D},// lb_tx_gain_table_6 (reset value = 0000)
{0x0E07,0x00AD},// lb_tx_gain_table_7 (reset value = 0000)
{0x0E08,0x00ED},// lb_tx_gain_table_8 (reset value = 0000)
{0x0E09,0x00F9},// lb_tx_gain_table_9 (reset value = 0000)
{0x0E0A,0x00FB},// lb_tx_gain_table_10 (reset value = 0000)
{0x0E0B,0x00FD},// lb_tx_gain_table_11 (reset value = 0000)
{0x0E0C,0x0109},// lb_tx_gain_table_12 (reset value = 0000)
{0x0E0D,0x010B},// lb_tx_gain_table_13 (reset value = 0000)
{0x0E0E,0x010D},// lb_tx_gain_table_14 (reset value = 0000)
{0x0E0F,0x0119},// lb_tx_gain_table_15 (reset value = 0000)
{0x0E10,0x011B},// lb_tx_gain_table_16 (reset value = 0000)
{0x0E11,0x0169},// lb_tx_gain_table_17 (reset value = 0000)
{0x0E12,0x016B},// lb_tx_gain_table_18 (reset value = 0000)
{0x0E13,0x016D},// lb_tx_gain_table_19 (reset value = 0000)
{0x0E14,0x0179},// lb_tx_gain_table_20 (reset value = 0000)
{0x0E15,0x017B},// lb_tx_gain_table_21 (reset value = 0000)
{0x0E16,0x01CF},// lb_tx_gain_table_22 (reset value = 0000)
{0x0E17,0x01DB},// lb_tx_gain_table_23 (reset value = 0000)
{0x0E18,0x01DD},// lb_tx_gain_table_24 (reset value = 0000)
{0x0E19,0x01DF},// lb_tx_gain_table_25 (reset value = 0000)
{0x0E1A,0x01EB},// lb_tx_gain_table_26 (reset value = 0000)
{0x0E1B,0x01ED},// lb_tx_gain_table_27 (reset value = 0000)
{0x0E1C,0x01F9},// lb_tx_gain_table_28 (reset value = 0000)
{0x0E1D,0x01FB},// lb_tx_gain_table_29 (reset value = 0000)
{0x0E1E,0x01FD},// lb_tx_gain_table_30 (reset value = 0000)
{0x0E1F,0x01FF},// lb_tx_gain_table_31 (reset value = 0000)
{0x0E21,0x0201},// lb_tx_rf_gain_table_2_3 (reset value = 0000)
{0x0E22,0x0403},// lb_tx_rf_gain_table_4_5 (reset value = 0000)
{0x0E23,0x0908},// lb_tx_rf_gain_table_6_7 (reset value = 0000)
{0x0E24,0x0B0A},// lb_tx_rf_gain_table_8_9 (reset value = 0000)
{0x0E25,0x0D0C},// lb_tx_rf_gain_table_10_11 (reset value = 0000)
{0x0E26,0x1211},// lb_tx_rf_gain_table_12_13 (reset value = 0000)
{0x0E27,0x1413},// lb_tx_rf_gain_table_14_15 (reset value = 0000)
{0x0E28,0x1615},// lb_tx_rf_gain_table_16_17 (reset value = 0000)
{0x0E29,0x1A19},// lb_tx_rf_gain_table_18_19 (reset value = 0000)
{0x0E2A,0x1C1B},// lb_tx_rf_gain_table_20_21 (reset value = 0000)
{0x0E2B,0x1E1D},// lb_tx_rf_gain_table_22_23 (reset value = 0000)
{0x0E2C,0x211F},// lb_tx_rf_gain_table_24_25 (reset value = 0000)
{0x0E2D,0x2322},// lb_tx_rf_gain_table_26_27 (reset value = 0000)
{0x0E2E,0x2524},// lb_tx_rf_gain_table_28_29 (reset value = 0000)
{0x0E2F,0x2726},// lb_tx_rf_gain_table_30_31 (reset value = 0000)
{0x0E70,0x01B0},// lb_tx_dbb_gain_table_0 (reset value = 0060)
{0x0E71,0x01C9},// lb_tx_dbb_gain_table_1 (reset value = 0393)
{0x0E72,0x01E4},// lb_tx_dbb_gain_table_2 (reset value = 03C9)
{0x0E73,0x0201},// lb_tx_dbb_gain_table_3 (reset value = 0402)
{0x0E74,0x021F},// lb_tx_dbb_gain_table_4 (reset value = 043F)
{0x0E75,0x0240},// lb_tx_dbb_gain_table_5 (reset value = 0480)
{0x0E76,0x0262},// lb_tx_dbb_gain_table_6 (reset value = 04C4)
{0x0E77,0x0286},// lb_tx_dbb_gain_table_7 (reset value = 050C)
{0x0E78,0x02AC},// lb_tx_dbb_gain_table_8 (reset value = 0559)
{0x0E79,0x02D5},// lb_tx_dbb_gain_table_9 (reset value = 05AA)
{0x0E7A,0x0300},// lb_tx_dbb_gain_table_10 (reset value = 0600)
{0x0E7B,0x032D},// lb_tx_dbb_gain_table_11 (reset value = 065B)
{0x0E7C,0x035D},// lb_tx_dbb_gain_table_12 (reset value = 06BB)
{0x0E7D,0x0390},// lb_tx_dbb_gain_table_13 (reset value = 0721)
{0x0E7E,0x03C6},// lb_tx_dbb_gain_table_14 (reset value = 078D)
{0x0E7F,0x0400},// lb_tx_dbb_gain_table_15 (reset value = 0800)
{0x1001,0x1DA3},// clock_enables (reset value = 09A0)
{0x1045,0x3FFF},// dma_end_addr (reset value = 0FFF)
{0x1070,0x0003},// aux_adc_ctrl1 (reset value = 0001)
{0x1071,0x1010},// aux_adc_ctrl2 (reset value = 0000)
{0x1073,0x0003},// aux_analog_ctrl (reset value = 0000)
{0x1080,0x0000},// swreset (reset value = 0002)
{0x1308,0x0016},// xo_aux_rsbus_ctrl8 (reset value = 0000)
{0x1311,0x0800},// clk_synth_rsbus_ctrl1 (reset value = 0000)
{0x1312,0x0A08},// clk_synth_rsbus_ctrl2 (reset value = 0000)
{0x1313,0x3170},// clk_synth_rsbus_ctrl3 (reset value = 0000)
{0x1320,0x2A00},// hb_synth_rsbus_ctrl0 (reset value = 0000)
{0x1323,0x0100},// hb_synth_rsbus_ctrl3 (reset value = 0000)
{0x1324,0x0818},// hb_synth_rsbus_ctrl4 (reset value = 0000)
{0x1325,0x0410},// hb_synth_rsbus_ctrl5 (reset value = 0000)
{0x132A,0x0016},// hb_synth_rsbus_ctrl10 (reset value = 0000)
{0x132D,0x0016},// hb_synth_rsbus_ctrl13 (reset value = 0000)
{0x1332,0x000D},// hb_rf_rsbus_ctrl2 (reset value = 0000)
{0x1333,0x0602},// hb_rf_rsbus_ctrl3 (reset value = 0000)
{0x1334,0xA000},// hb_rf_rsbus_ctrl4 (reset value = 0000)
{0x1335,0x0006},// hb_rf_rsbus_ctrl5 (reset value = 0000)
{0x1339,0xFCC0},// hb_rf_rsbus_ctrl9 (reset value = 0000)
{0x133A,0x2B0D},// hb_rf_rsbus_ctrl10 (reset value = 0000)
{0x133B,0x4A6A},// hb_rf_rsbus_ctrl11 (reset value = 0000)
{0x133C,0x02CF},// hb_rf_rsbus_ctrl12 (reset value = 0000)
{0x133D,0x6000},// hb_rf_rsbus_ctrl13 (reset value = 0000)
{0x1340,0x0300},// hb_bb_rsbus_ctrl0 (reset value = 0000)
{0x1341,0x0030},// hb_bb_rsbus_ctrl1 (reset value = 0000)
{0x1342,0xC0C0},// hb_bb_rsbus_ctrl2 (reset value = 0000)
{0x1349,0x0030},// hb_bb_rsbus_ctrl9 (reset value = 0000)
{0x134C,0x6500},// hb_bb_rsbus_ctrl12 (reset value = 0000)
{0x134E,0x0500},// hb_bb_rsbus_ctrl14 (reset value = 0000)
{0x1350,0x2A00},// lb_synth_rsbus_ctrl0 (reset value = 0000)
{0x1353,0x1100},// lb_synth_rsbus_ctrl3 (reset value = 0000)
{0x1355,0x0410},// lb_synth_rsbus_ctrl5 (reset value = 0000)
{0x135A,0x000A},// lb_synth_rsbus_ctrl10 (reset value = 0000)
{0x135D,0x000A},// lb_synth_rsbus_ctrl13 (reset value = 0000)
{0x1361,0x0100},// lb_rf_rsbus_ctrl1 (reset value = 0000)
{0x1362,0x000D},// lb_rf_rsbus_ctrl2 (reset value = 0000)
{0x1363,0x0602},// lb_rf_rsbus_ctrl3 (reset value = 0000)
{0x1364,0xA000},// lb_rf_rsbus_ctrl4 (reset value = 0000)
{0x1365,0x0006},// lb_rf_rsbus_ctrl5 (reset value = 0000)
{0x136A,0x2B0D},// lb_rf_rsbus_ctrl10 (reset value = 0000)
{0x136B,0xCA6A},// lb_rf_rsbus_ctrl11 (reset value = 0000)
{0x136C,0x014C},// lb_rf_rsbus_ctrl12 (reset value = 0000)
{0x136D,0x6000},// lb_rf_rsbus_ctrl13 (reset value = 0000)
{0x1370,0x0300},// lb_bb_rsbus_ctrl0 (reset value = 0000)
{0x1371,0x0030},// lb_bb_rsbus_ctrl1 (reset value = 0000)
{0x1372,0xC0C0},// lb_bb_rsbus_ctrl2 (reset value = 0000)
{0x1379,0x0030},// lb_bb_rsbus_ctrl9 (reset value = 0000)
{0x137C,0x6500},// lb_bb_rsbus_ctrl12 (reset value = 0000)
{0x137E,0x0300},// lb_bb_rsbus_ctrl14 (reset value = 0000)
};

const static struct aet_reg_pair g_rf_xdma_def_data[]={
//Switch2ONRX
{0x1600,0x17AB},      //LLI01_HB
{0x1601,0x0005},
{0x1602,0x8002},
{0x1603,0x0018},
{0x1604,0x17FD},     //LLI02_HB
{0x1605,0x0000},
{0x1606,0x8002},
{0x1607,0x0038},
{0x1608,0x17FC},      //LLI03_HB
{0x1609,0x0000},
{0x160A,0x8002},
{0x160B,0x00F8},
{0x160C,0x17FB},      //LLI04_HB
{0x160D,0x0004},
{0x160E,0x0002},
{0x160F,0x0000},

//Switch2ONTX
{0x1618,0x17EE},     //LLI11_HB
{0x1619,0x0004},
{0x161A,0x8002},
{0x161B,0x0000},
{0x161C,0x17AD},     //LLI11_HB
{0x161D,0x0005},
{0x161E,0x8002},
{0x161F,0x0000},
{0x1620,0x17ED},      //LLI12_HB
{0x1621,0x0000},
{0x1622,0x8002},
{0x1623,0x0063},
{0x1624,0x17EC},      //LLI13_HB
{0x1625,0x0000},
{0x1626,0x8002},
{0x1627,0x0063},
{0x1628,0x17EB},      //LLI14_HB
{0x1629,0x0003},
{0x162A,0x8002},
{0x162B,0x0000},
{0x162C,0x17AC},      //LLI14_HB
{0x162D,0x0005},
{0x162E,0x8002},
{0x162F,0x0000},
{0x1630,0x17EA},      //LLI15_HB
{0x1631,0x0000},
{0x1632,0x8002},
{0x1633,0x001F},
{0x1634,0x17E9},      //LLI16_HB
{0x1635,0x0000},
{0x1636,0x8002},
{0x1637,0x000F},
{0x1638,0x17E8},      //LLI17_HB
{0x1639,0x0003},
{0x163A,0x8002},
{0x163B,0x000F},
{0x163C,0x17E7},      //LLI18_HB
{0x163D,0x0003},
{0x163E,0x8002},
{0x163F,0x000F},
{0x1640,0x17E6},     //LLI19_HB
{0x1641,0x0132},      //This value is only default, register shall be written to by FW
{0x1642,0x0002},
{0x1643,0x0000},

{0x1660,0x17DE},      //LLI21_HB
{0x1661,0x0002},
{0x1662,0x8002},
{0x1663,0x000F},
{0x1664,0x17DD},      //LLI22_HB
{0x1665,0x0002},
{0x1666,0x0002},
{0x1667,0x0000},

{0x1680,0x17CE},      //LLI31_HB
{0x1681,0x0002},
{0x1682,0x8002},
{0x1683,0x000F},
{0x1684,0x17CD},      //LLI32_HB
{0x1685,0x0002},
{0x1686,0x8002},
{0x1687,0x000F},
{0x1688,0x17CC},      //LLI33_HB
{0x1689,0x0002},
{0x168A,0x0002},
{0x168B,0x0000},

{0x1700,0x17BE},      //LLI41_HB
{0x1701,0x0002},
{0x1702,0x8002},
{0x1703,0x000F},
{0x1704,0x17BD},      //LLI42_HB
{0x1705,0x0002},
{0x1706,0x8002},
{0x1707,0x000F},
{0x1708,0x17BC},      //LLI43_HB
{0x1709,0x0002},
{0x170A,0x0002},
{0x170B,0x0000},

//XDMA LLI data LB-XDMA

//Switch2ONRX
{0x1800,0x19AB},      //LLI01_HB
{0x1801,0x0805},
{0x1802,0x8002},
{0x1803,0x0018},
{0x1804,0x19FD},      //LLI02_LB
{0x1805,0x0800},
{0x1806,0x8002},
{0x1807,0x0038},
{0x1808,0x19FC},      //LLI03_LB
{0x1809,0x0800},
{0x180A,0x8002},
{0x180B,0x00F8},
{0x180C,0x19FB},      //LLI04_LB
{0x180D,0x0804},
{0x180E,0x0002},
{0x180F,0x0000},

//Switch2ONTX
{0x1818,0x19EE},      //LLI11_HB
{0x1819,0x0804},
{0x181A,0x8002},
{0x181B,0x0000},
{0x181C,0x19AD},      //LLI11_HB
{0x181D,0x0805},
{0x181E,0x8002},
{0x181F,0x0000},
{0x1820,0x19ED},      //LLI12_HB
{0x1821,0x0800},
{0x1822,0x8002},
{0x1823,0x0063},
{0x1824,0x19EC},      //LLI13_HB
{0x1825,0x0800},
{0x1826,0x8002},
{0x1827,0x0063},
{0x1828,0x19EB},      //LLI14_HB
{0x1829,0x0803},
{0x182A,0x8002},
{0x182B,0x0000},
{0x182C,0x19AC},      //LLI14_HB
{0x182D,0x0805},
{0x182E,0x8002},
{0x182F,0x0000},
{0x1830,0x19EA},      //LLI15_LB
{0x1831,0x0800},
{0x1832,0x8002},
{0x1833,0x000F},
{0x1834,0x19E9},      //LLI16_LB
{0x1835,0x0800},
{0x1836,0x8002},
{0x1837,0x000F},
{0x1838,0x19E8},      //LLI17_LB
{0x1839,0x0803},
{0x183A,0x8002},
{0x183B,0x000F},
{0x183C,0x19E7},      //LLI18_LB
{0x183D,0x0803},
{0x183E,0x8002},
{0x183F,0x000F},
{0x1840,0x19E6},      //LLI19_LB
{0x1841,0x0932},      //This value is only default, register shall be written to by FW
{0x1842,0x0002},
{0x1843,0x0000},

{0x1860,0x19DE},      //LLI21_LB
{0x1861,0x0802},
{0x1862,0x8002},
{0x1863,0x000F},
{0x1864,0x19DD},      //LLI22_LB
{0x1865,0x0802},
{0x1866,0x0002},
{0x1867,0x0000},

{0x1880,0x19CE},      //LLI31_LB
{0x1881,0x0802},
{0x1882,0x8002},
{0x1883,0x000F},
{0x1884,0x19CD},      //LLI32_LB
{0x1885,0x0802},
{0x1886,0x8002},
{0x1887,0x001F},
{0x1888,0x19CC},      //LLI33_LB
{0x1889,0x0802},
{0x188A,0x0002},
{0x188B,0x0000},

{0x1900,0x19BE},      //LLI41_LB
{0x1901,0x0802},
{0x1902,0x8002},
{0x1903,0x000F},
{0x1904,0x19BD},      //LLI42_LB
{0x1905,0x0802},
{0x1906,0x8002},
{0x1907,0x001F},
{0x1908,0x19BC},      //LLI43_LB
{0x1909,0x0802},
{0x190A,0x0002},
{0x190B,0x0000},


//XDMA REG DATA HB-XDMA

//CH0
{0x0100,0x17FE},
{0x0108,0x0000},
{0x0110,0x0002},
{0x0118,0x0000},
{0x0120,0x1600},
{0x0128,0x0020},
{0x0138,0x0001},

//CH1
{0x0101,0x17EE},
{0x0109,0x0004},
{0x0111,0x0002},
{0x0119,0x0000},
{0x0121,0x1618},
{0x0129,0x0020},
{0x0139,0x0001},

//CH2
{0x0102,0x17DE},
{0x010A,0x0002},
{0x0112,0x0002},
{0x011A,0x0000},
{0x0122,0x1660},
{0x012A,0x0020},
{0x013A,0x0001},

//CH3
{0x0103,0x17CE},
{0x010B,0x0002},
{0x0113,0x0002},
{0x011B,0x0000},
{0x0123,0x1680},
{0x012B,0x0020},
{0x013B,0x0001},

//CH4
{0x0104,0x17BE},
{0x010C,0x0002},
{0x0114,0x0002},
{0x011C,0x0000},
{0x0124,0x1700},
{0x012C,0x0020},
{0x013C,0x0001},

//XDMA REG DATA LB-XDMA

//CH0
{0x0900,0x19FE},
{0x0908,0x0800},
{0x0910,0x0002},
{0x0918,0x0000},
{0x0920,0x1800},
{0x0928,0x0020},
{0x0938,0x0001},

//CH1
{0x0901,0x19EE},
{0x0909,0x0804},
{0x0911,0x0002},
{0x0919,0x0000},
{0x0921,0x1818},
{0x0929,0x0020},
{0x0939,0x0001},

//CH2
{0x0902,0x19DE},
{0x090A,0x0802},
{0x0912,0x0002},
{0x091A,0x0000},
{0x0922,0x1860},
{0x092A,0x0020},
{0x093A,0x0001},

//CH3
{0x0903,0x19CE},
{0x090B,0x0802},
{0x0913,0x0002},
{0x091B,0x0000},
{0x0923,0x1880},
{0x092B,0x0020},
{0x093B,0x0001},

//CH4
{0x0904,0x19BE},
{0x090C,0x0802},
{0x0914,0x0002},
{0x091C,0x0000},
{0x0924,0x1900},
{0x092C,0x0020},
{0x093C,0x0001},

//XDMA RAM0 DATA HB-XDMA

//HB Switch2ONRX
{0x17FE,0x00C1},      //HB_ANALOG_ENABLE   = TRX_AE_ON_RX0_STDBY_TXDAC
{0x17FD,0x00C5},      //HB_ANALOG_ENABLE   = TRX_AE_ON_RX1_STDBY_TXDAC
{0x17FC,0x00CF},      //HB_ANALOG_ENABLE   = TRX_AE_ON_RX_STDBY_TXDAC
{0x17FB,0x0001},      //HB_RX_IQDATA_VALID = TRX_RX_IQDATA_VALID_TRUE

//HB Switch2ONTX bias ramping
{0x17EE,0x0000},      //HB_RX_IQDATA_VALID = TRX_RX_IQDATA_VALID_FALSE
{0x17ED,0x05C0},      //HB_ANALOG_ENABLE   = TRX_AE_ON_TX0_STDBY_RXADC
{0x17EC,0x0FC0},      //HB_ANALOG_ENABLE   = TRX_AE_ON_TX1_STDBY_RXADC
{0x17EB,0x0000},      //HB_TX_GAIN_PTR     = 0x0000
{0x17EA,0x1FC0},      //HB_ANALOG_ENABLE   = TRX_AE_ON_TX2_STDBY_RXADC
{0x17E9,0x3FC0},     //HB_ANALOG_ENABLE   = TRX_AE_ON_TX_STDBY_RXADC
{0x17E8,0x0005},     //HB_TX_BIAS_SEL     = 0x0005
{0x17E7,0x000A},     //HB_TX_BIAS_SEL     = 0x0005
{0x17E6,0x0041},     //Start selected TX power ramp XDMA channel

//HB Switch2ONTX power ramping 1
{0x17DE,0x0004},     //HB_TX_GAIN_PTR     = 0x0004
{0x17DD,0x0008},     //HB_TX_GAIN_PTR     = value at this location. This value is only default, register shall be written to by FW

//HB Switch2ONTX power ramping 2
{0x17CE,0x0004},     //HB_TX_GAIN_PTR     = 0x0004
{0x17CD,0x0008},     //HB_TX_GAIN_PTR     = 0x0008
{0x17CC,0x0010},     //HB_TX_GAIN_PTR     = value at this location. This value is only default, register shall be written to by FW

//HB Switch2ONTX power ramping 3
{0x17BE,0x0004},     //HB_TX_GAIN_PTR     = 0x0004
{0x17BD,0x000A},     //HB_TX_GAIN_PTR     = 0x000A
{0x17BC,0x001D},     //HB_TX_GAIN_PTR     = value at this location. This value is only default, register shall be written to by FW

// HB external LNA/PA/switch control
{0x17AE,0x00F4},  // Idle - PA off, LNA off, Swithes in TX
{0x17AD,0x00F4},  // TX - PA off, LNA off, Swithes in TX
{0x17AC,0x00F5},  // PA - PA on, LNA off, Swithes in TX
{0x17AB,0x00FA},  // RX - PA off, LNA on, Swithes in RX

//XDMA RAM1 DATA LB-XDMA

//LB Switch2ONRX
{0x19FE,0x00C1},     //LB_ANALOG_ENABLE   = TRX_AE_ON_RX0_STDBY_TXDAC
{0x19FD,0x00C5},     //LB_ANALOG_ENABLE   = TRX_AE_ON_RX1_STDBY_TXDAC
{0x19FC,0x00CF},     //LB_ANALOG_ENABLE   = TRX_AE_ON_RX_STDBY_TXDAC
{0x19FB,0x0001},     //LB_RX_IQDATA_VALID = TRX_RX_IQDATA_VALID_TRUE

//LB Switch2ONTX bias ramping
{0x19EE,0x0000},     //LB_RX_IQDATA_VALID = TRX_RX_IQDATA_VALID_FALSE
{0x19ED,0x05C0},     //LB_ANALOG_ENABLE   = TRX_AE_ON_TX0_STDBY_RXADC
{0x19EC,0x0FC0},     //LB_ANALOG_ENABLE   = TRX_AE_ON_TX1_STDBY_RXADC
{0x19EB,0x0000},     //LB_TX_GAIN_PTR     = 0x0000
{0x19EA,0x1FC0},     //LB_ANALOG_ENABLE   = TRX_AE_ON_TX2_STDBY_RXADC
{0x19E9,0x3FC8},     //LB_ANALOG_ENABLE   = TRX_AE_ON_TX_STDBY_RXADC
{0x19E8,0x0005},     //LB_TX_BIAS_SEL     = 0x0005
{0x19E7,0x000A},     //LB_TX_BIAS_SEL     = 0x000A
{0x19E6,0x0041},     //Start selected TX power ramp XDMA channel

//LB Switch2ONTX power ramping 1
{0x19DE,0x0004},     //LB_TX_GAIN_PTR     = 0x0004
{0x19DD,0x0008},     //LB_TX_GAIN_PTR     = value at this location. This value is only default, register shall be written to by FW

//LB Switch2ONTX power ramping 2
{0x19CE,0x0004},     //LB_TX_GAIN_PTR     = 0x0004
{0x19CD,0x0008},     //LB_TX_GAIN_PTR     = 0x0008
{0x19CC,0x0010},     //LB_TX_GAIN_PTR     = value at this location. This value is only default, register shall be written to by FW

//LB Switch2ONTX power ramping 3
{0x19BE,0x0004},     //LB_TX_GAIN_PTR     = 0x0004
{0x19BD,0x000A},     //LB_TX_GAIN_PTR     = 0x000A
{0x19BC,0x001D},     //LB_TX_GAIN_PTR     = value at this location. This value is only default, register shall be written to by FW

// LB external LNA/PA/switch control
{0x19AE,0x00F4},  // Idle - PA off, LNA off, Swithes in TX
{0x19AD,0x00F4},  // TX - PA off, LNA off, Swithes in TX
{0x19AC,0x00F5},  // PA - PA on, LNA off, Swithes in TX
{0x19AB,0x00FA},  // RX - PA off, LNA on, Swithes in RX
};
#endif // _RFDEF_DATA_H_
