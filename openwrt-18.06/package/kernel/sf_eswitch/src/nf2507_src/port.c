/**  @file
  *  @brief    brief  description
  *  @author
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#include "l2sw_switch.h"
#include "port.h"
#include "sal_string.h"
#include "l2sw_asicdrv_port.h"
#include "l2sw_asicdrv_misc.h"
#include "l2sw_asicdrv_portIsolation.h"
#include "smi.h"

extern user_mgmt_fun_t m_fun;

#if defined(USE_L2SW_SDK)

#else
#define FIBER_INIT_SIZE 1507
CONST_T l2sw_uint8 Fiber[FIBER_INIT_SIZE] = {
0x02,0x04,0x41,0xE4,0xF5,0xA8,0xD2,0xAF,
0x22,0x00,0x00,0x02,0x05,0x2D,0xE4,0x90,
0x06,0x2A,0xF0,0xFD,0x7C,0x01,0x7F,0x3F,
0x7E,0x1D,0x12,0x05,0xAF,0x7D,0x40,0x12,
0x02,0x5F,0xE4,0xFF,0xFE,0xFD,0x80,0x08,
0x12,0x05,0x9E,0x50,0x0C,0x12,0x05,0x8B,
0xFC,0x90,0x06,0x24,0x12,0x03,0x76,0x80,
0xEF,0xE4,0xF5,0xA8,0xD2,0xAF,0x7D,0x1F,
0xFC,0x7F,0x49,0x7E,0x13,0x12,0x05,0xAF,
0x12,0x05,0xD6,0x7D,0xD7,0x12,0x02,0x1E,
0x7D,0x80,0x12,0x01,0xCA,0x7D,0x94,0x7C,
0xF9,0x12,0x02,0x3B,0x7D,0x81,0x12,0x01,
0xCA,0x7D,0xA2,0x7C,0x31,0x12,0x02,0x3B,
0x7D,0x82,0x12,0x01,0xDF,0x7D,0x60,0x7C,
0x69,0x12,0x02,0x43,0x7D,0x83,0x12,0x01,
0xDF,0x7D,0x28,0x7C,0x97,0x12,0x02,0x43,
0x7D,0x84,0x12,0x01,0xF4,0x7D,0x85,0x7C,
0x9D,0x12,0x02,0x57,0x7D,0x23,0x12,0x01,
0xF4,0x7D,0x10,0x7C,0xD8,0x12,0x02,0x57,
0x7D,0x24,0x7C,0x04,0x12,0x02,0x28,0x7D,
0x00,0x12,0x02,0x1E,0x7D,0x2F,0x12,0x02,
0x09,0x7D,0x20,0x7C,0x0F,0x7F,0x02,0x7E,
0x66,0x12,0x05,0xAF,0x7D,0x01,0x12,0x02,
0x09,0x7D,0x04,0x7C,0x00,0x7F,0x01,0x7E,
0x66,0x12,0x05,0xAF,0x7D,0x80,0x7C,0x00,
0x7F,0x00,0x7E,0x66,0x12,0x05,0xAF,0x7F,
0x02,0x7E,0x66,0x12,0x02,0x4B,0x44,0x02,
0xFF,0x90,0x06,0x28,0xEE,0xF0,0xA3,0xEF,
0xF0,0x44,0x04,0xFF,0x90,0x06,0x28,0xEE,
0xF0,0xFC,0xA3,0xEF,0xF0,0xFD,0x7F,0x02,
0x7E,0x66,0x12,0x05,0xAF,0x7D,0x04,0x7C,
0x00,0x12,0x02,0x28,0x7D,0xB9,0x7C,0x15,
0x7F,0xEB,0x7E,0x13,0x12,0x05,0xAF,0x7D,
0x07,0x7C,0x00,0x7F,0xE7,0x7E,0x13,0x12,
0x05,0xAF,0x7D,0x40,0x7C,0x11,0x7F,0x00,
0x7E,0x62,0x12,0x05,0xAF,0x12,0x03,0x82,
0x7D,0x41,0x12,0x02,0x5F,0xE4,0xFF,0xFE,
0xFD,0x80,0x08,0x12,0x05,0x9E,0x50,0x0C,
0x12,0x05,0x8B,0xFC,0x90,0x06,0x24,0x12,
0x03,0x76,0x80,0xEF,0xC2,0x00,0xC2,0x01,
0xD2,0xA9,0xD2,0x8C,0x7F,0x01,0x7E,0x62,
0x12,0x02,0x4B,0x30,0xE2,0x05,0xE4,0xA3,
0xF0,0x80,0xF1,0x90,0x06,0x2A,0xE0,0x70,
0x12,0x12,0x01,0x89,0x90,0x06,0x2A,0x74,
0x01,0xF0,0xE4,0x90,0x06,0x2D,0xF0,0xA3,
0xF0,0x80,0xD9,0xC3,0x90,0x06,0x2E,0xE0,
0x94,0x64,0x90,0x06,0x2D,0xE0,0x94,0x00,
0x40,0xCA,0xE4,0xF0,0xA3,0xF0,0x12,0x01,
0x89,0x90,0x06,0x2A,0x74,0x01,0xF0,0x80,
0xBB,0x7D,0x04,0xFC,0x7F,0x02,0x7E,0x66,
0x12,0x05,0xAF,0x7D,0x00,0x7C,0x04,0x7F,
0x01,0x7E,0x66,0x12,0x05,0xAF,0x7D,0xC0,
0x7C,0x00,0x7F,0x00,0x7E,0x66,0x12,0x05,
0xAF,0xE4,0xFD,0xFC,0x7F,0x02,0x7E,0x66,
0x12,0x05,0xAF,0x7D,0x00,0x7C,0x04,0x7F,
0x01,0x7E,0x66,0x12,0x05,0xAF,0x7D,0xC0,
0x7C,0x00,0x7F,0x00,0x7E,0x66,0x12,0x05,
0xAF,0x22,0x7C,0x04,0x7F,0x01,0x7E,0x66,
0x12,0x05,0xAF,0x7D,0xC0,0x7C,0x00,0x7F,
0x00,0x7E,0x66,0x12,0x05,0xAF,0x22,0x7C,
0x04,0x7F,0x01,0x7E,0x66,0x12,0x05,0xAF,
0x7D,0xC0,0x7C,0x00,0x7F,0x00,0x7E,0x66,
0x12,0x05,0xAF,0x22,0x7C,0x04,0x7F,0x01,
0x7E,0x66,0x12,0x05,0xAF,0x7D,0xC0,0x7C,
0x00,0x7F,0x00,0x7E,0x66,0x12,0x05,0xAF,
0x22,0x7C,0x00,0x7F,0x01,0x7E,0x66,0x12,
0x05,0xAF,0x7D,0xC0,0x7C,0x00,0x7F,0x00,
0x7E,0x66,0x12,0x05,0xAF,0x22,0x7C,0x04,
0x7F,0x02,0x7E,0x66,0x12,0x05,0xAF,0x22,
0x7F,0x01,0x7E,0x66,0x12,0x05,0xAF,0x7D,
0xC0,0x7C,0x00,0x7F,0x00,0x7E,0x66,0x12,
0x05,0xAF,0x22,0x7F,0x02,0x7E,0x66,0x12,
0x05,0xAF,0x22,0x7F,0x02,0x7E,0x66,0x12,
0x05,0xAF,0x22,0x12,0x05,0x67,0x90,0x06,
0x28,0xEE,0xF0,0xA3,0xEF,0xF0,0x22,0x7F,
0x02,0x7E,0x66,0x12,0x05,0xAF,0x22,0x7C,
0x00,0x7F,0x36,0x7E,0x13,0x12,0x05,0xAF,
0x22,0xC5,0xF0,0xF8,0xA3,0xE0,0x28,0xF0,
0xC5,0xF0,0xF8,0xE5,0x82,0x15,0x82,0x70,
0x02,0x15,0x83,0xE0,0x38,0xF0,0x22,0x75,
0xF0,0x08,0x75,0x82,0x00,0xEF,0x2F,0xFF,
0xEE,0x33,0xFE,0xCD,0x33,0xCD,0xCC,0x33,
0xCC,0xC5,0x82,0x33,0xC5,0x82,0x9B,0xED,
0x9A,0xEC,0x99,0xE5,0x82,0x98,0x40,0x0C,
0xF5,0x82,0xEE,0x9B,0xFE,0xED,0x9A,0xFD,
0xEC,0x99,0xFC,0x0F,0xD5,0xF0,0xD6,0xE4,
0xCE,0xFB,0xE4,0xCD,0xFA,0xE4,0xCC,0xF9,
0xA8,0x82,0x22,0xB8,0x00,0xC1,0xB9,0x00,
0x59,0xBA,0x00,0x2D,0xEC,0x8B,0xF0,0x84,
0xCF,0xCE,0xCD,0xFC,0xE5,0xF0,0xCB,0xF9,
0x78,0x18,0xEF,0x2F,0xFF,0xEE,0x33,0xFE,
0xED,0x33,0xFD,0xEC,0x33,0xFC,0xEB,0x33,
0xFB,0x10,0xD7,0x03,0x99,0x40,0x04,0xEB,
0x99,0xFB,0x0F,0xD8,0xE5,0xE4,0xF9,0xFA,
0x22,0x78,0x18,0xEF,0x2F,0xFF,0xEE,0x33,
0xFE,0xED,0x33,0xFD,0xEC,0x33,0xFC,0xC9,
0x33,0xC9,0x10,0xD7,0x05,0x9B,0xE9,0x9A,
0x40,0x07,0xEC,0x9B,0xFC,0xE9,0x9A,0xF9,
0x0F,0xD8,0xE0,0xE4,0xC9,0xFA,0xE4,0xCC,
0xFB,0x22,0x75,0xF0,0x10,0xEF,0x2F,0xFF,
0xEE,0x33,0xFE,0xED,0x33,0xFD,0xCC,0x33,
0xCC,0xC8,0x33,0xC8,0x10,0xD7,0x07,0x9B,
0xEC,0x9A,0xE8,0x99,0x40,0x0A,0xED,0x9B,
0xFD,0xEC,0x9A,0xFC,0xE8,0x99,0xF8,0x0F,
0xD5,0xF0,0xDA,0xE4,0xCD,0xFB,0xE4,0xCC,
0xFA,0xE4,0xC8,0xF9,0x22,0xEB,0x9F,0xF5,
0xF0,0xEA,0x9E,0x42,0xF0,0xE9,0x9D,0x42,
0xF0,0xE8,0x9C,0x45,0xF0,0x22,0xE0,0xFC,
0xA3,0xE0,0xFD,0xA3,0xE0,0xFE,0xA3,0xE0,
0xFF,0x22,0xE0,0xF8,0xA3,0xE0,0xF9,0xA3,
0xE0,0xFA,0xA3,0xE0,0xFB,0x22,0xEC,0xF0,
0xA3,0xED,0xF0,0xA3,0xEE,0xF0,0xA3,0xEF,
0xF0,0x22,0x12,0x03,0xF8,0x12,0x04,0x1A,
0x44,0x40,0x12,0x04,0x0F,0x7D,0x03,0x7C,
0x00,0x12,0x04,0x23,0x12,0x05,0xAF,0x12,
0x03,0xF8,0x12,0x04,0x1A,0x54,0xBF,0x12,
0x04,0x0F,0x7D,0x03,0x7C,0x00,0x12,0x03,
0xD0,0x7F,0x02,0x7E,0x66,0x12,0x05,0x67,
0xEF,0x54,0xFD,0x54,0xFE,0x12,0x04,0x33,
0x12,0x03,0xD0,0x7F,0x02,0x7E,0x66,0x12,
0x05,0x67,0xEF,0x44,0x02,0x44,0x01,0x12,
0x04,0x33,0x12,0x04,0x23,0x02,0x05,0xAF,
0x7F,0x01,0x7E,0x66,0x12,0x05,0xAF,0x7D,
0xC0,0x7C,0x00,0x7F,0x00,0x7E,0x66,0x12,
0x05,0xAF,0xE4,0xFD,0xFC,0x7F,0x01,0x7E,
0x66,0x12,0x05,0xAF,0x7D,0x80,0x7C,0x00,
0x7F,0x00,0x7E,0x66,0x12,0x05,0xAF,0x22,
0x7D,0x03,0x7C,0x00,0x7F,0x01,0x7E,0x66,
0x12,0x05,0xAF,0x7D,0x80,0x7C,0x00,0x7F,
0x00,0x7E,0x66,0x12,0x05,0xAF,0x22,0xFD,
0xAC,0x06,0x7F,0x02,0x7E,0x66,0x12,0x05,
0xAF,0x22,0x7F,0x02,0x7E,0x66,0x12,0x05,
0x67,0xEF,0x22,0x7F,0x01,0x7E,0x66,0x12,
0x05,0xAF,0x7D,0xC0,0x7C,0x00,0x7F,0x00,
0x7E,0x66,0x22,0xFD,0xAC,0x06,0x7F,0x02,
0x7E,0x66,0x12,0x05,0xAF,0xE4,0xFD,0xFC,
0x22,0x78,0x7F,0xE4,0xF6,0xD8,0xFD,0x75,
0x81,0x3C,0x02,0x04,0x88,0x02,0x00,0x0E,
0xE4,0x93,0xA3,0xF8,0xE4,0x93,0xA3,0x40,
0x03,0xF6,0x80,0x01,0xF2,0x08,0xDF,0xF4,
0x80,0x29,0xE4,0x93,0xA3,0xF8,0x54,0x07,
0x24,0x0C,0xC8,0xC3,0x33,0xC4,0x54,0x0F,
0x44,0x20,0xC8,0x83,0x40,0x04,0xF4,0x56,
0x80,0x01,0x46,0xF6,0xDF,0xE4,0x80,0x0B,
0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
0x90,0x05,0xCB,0xE4,0x7E,0x01,0x93,0x60,
0xBC,0xA3,0xFF,0x54,0x3F,0x30,0xE5,0x09,
0x54,0x1F,0xFE,0xE4,0x93,0xA3,0x60,0x01,
0x0E,0xCF,0x54,0xC0,0x25,0xE0,0x60,0xA8,
0x40,0xB8,0xE4,0x93,0xA3,0xFA,0xE4,0x93,
0xA3,0xF8,0xE4,0x93,0xA3,0xC8,0xC5,0x82,
0xC8,0xCA,0xC5,0x83,0xCA,0xF0,0xA3,0xC8,
0xC5,0x82,0xC8,0xCA,0xC5,0x83,0xCA,0xDF,
0xE9,0xDE,0xE7,0x80,0xBE,0x75,0x0F,0x80,
0x75,0x0E,0x7E,0x75,0x0D,0xAA,0x75,0x0C,
0x83,0xE4,0xF5,0x10,0x75,0x0B,0xA0,0x75,
0x0A,0xAC,0x75,0x09,0xB9,0x75,0x08,0x03,
0x75,0x89,0x11,0x7B,0x60,0x7A,0x09,0xF9,
0xF8,0xAF,0x0B,0xAE,0x0A,0xAD,0x09,0xAC,
0x08,0x12,0x02,0xBB,0xAD,0x07,0xAC,0x06,
0xC3,0xE4,0x9D,0xFD,0xE4,0x9C,0xFC,0x78,
0x17,0xF6,0xAF,0x05,0xEF,0x08,0xF6,0x18,
0xE6,0xF5,0x8C,0x08,0xE6,0xF5,0x8A,0x74,
0x0D,0x2D,0xFD,0xE4,0x3C,0x18,0xF6,0xAF,
0x05,0xEF,0x08,0xF6,0x75,0x88,0x10,0x53,
0x8E,0xC7,0xD2,0xA9,0x22,0xC0,0xE0,0xC0,
0xF0,0xC0,0x83,0xC0,0x82,0xC0,0xD0,0x75,
0xD0,0x00,0xC0,0x00,0x78,0x17,0xE6,0xF5,
0x8C,0x78,0x18,0xE6,0xF5,0x8A,0x90,0x06,
0x2B,0xE4,0x75,0xF0,0x01,0x12,0x02,0x69,
0x90,0x06,0x2D,0xE4,0x75,0xF0,0x01,0x12,
0x02,0x69,0xD0,0x00,0xD0,0xD0,0xD0,0x82,
0xD0,0x83,0xD0,0xF0,0xD0,0xE0,0x32,0xC2,
0xAF,0xAD,0x07,0xAC,0x06,0x8C,0xA2,0x8D,
0xA3,0x75,0xA0,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAE,
0xA1,0xBE,0x00,0xF0,0xAE,0xA6,0xAF,0xA7,
0xD2,0xAF,0x22,0x90,0x06,0x24,0x12,0x03,
0x5E,0xEF,0x24,0x01,0xFF,0xE4,0x3E,0xFE,
0xE4,0x3D,0xFD,0xE4,0x3C,0x22,0xE4,0x7F,
0x20,0x7E,0x4E,0xFD,0xFC,0x90,0x06,0x24,
0x12,0x03,0x6A,0xC3,0x02,0x03,0x4D,0xC2,
0xAF,0xAB,0x07,0xAA,0x06,0x8A,0xA2,0x8B,
0xA3,0x8C,0xA4,0x8D,0xA5,0x75,0xA0,0x03,
0x00,0x00,0x00,0xAA,0xA1,0xBA,0x00,0xF8,
0xD2,0xAF,0x22,0x42,0x06,0x2D,0x00,0x00,
0x42,0x06,0x2B,0x00,0x00,0x00,0x12,0x05,
0xDF,0x12,0x04,0xCD,0x02,0x00,0x03,0xE4,
0xF5,0x8E,0x22};
#endif


static l2sw_api_ret_t _l2sw_port_phyAutoNegoAbility_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t       retVal;
    l2sw_uint32          phyData;
    l2sw_uint32          phyEnMsk0;
    l2sw_uint32          phyEnMsk4;
    l2sw_uint32          phyEnMsk9;
#ifndef USE_L2SW_SDK
	l2sw_port_media_t    media_type;
#endif
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if(NULL == pAbility)
        return L2SW_ERR_NULL_POINTER;

    if (pAbility->Half_10 >= L2SW_ENABLE_END || pAbility->Full_10 >= L2SW_ENABLE_END ||
       pAbility->Half_100 >= L2SW_ENABLE_END || pAbility->Full_100 >= L2SW_ENABLE_END ||
       pAbility->Full_1000 >= L2SW_ENABLE_END || pAbility->AutoNegotiation >= L2SW_ENABLE_END ||
       pAbility->AsyFC >= L2SW_ENABLE_END || pAbility->FC >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

#if defined(USE_L2SW_SDK)
		/*sf l2sw not support the below function*/
#else
    if (l2sw_switch_isComboPort(units, port) == L2SW_ERR_OK)
    {
        if ((retVal = _l2sw_port_phyComboPortMedia_get(units, port, &media_type)) != L2SW_ERR_OK)
            return retVal;

        if(media_type == PORT_MEDIA_FIBER)
        {
            return _l2sw_port_FiberModeAbility_set(units, port, pAbility);
        }
    }
#endif
    /*for PHY auto mode setup*/
    pAbility->AutoNegotiation = 1;

    phyEnMsk0 = 0;
    phyEnMsk4 = 0;
    phyEnMsk9 = 0;

    if (1 == pAbility->Half_10)
    {
        /*10BASE-TX half duplex capable in reg 4.5*/
        phyEnMsk4 = phyEnMsk4 | (1 << 5);

        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));
    }

    if (1 == pAbility->Full_10)
    {
        /*10BASE-TX full duplex capable in reg 4.6*/
        phyEnMsk4 = phyEnMsk4 | (1 << 6);
        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));

        /*Full duplex mode in reg 0.8*/
        phyEnMsk0 = phyEnMsk0 | (1 << 8);

    }

    if (1 == pAbility->Half_100)
    {
        /*100BASE-TX half duplex capable in reg 4.7*/
        phyEnMsk4 = phyEnMsk4 | (1 << 7);
        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 | (1 << 13);
    }


    if (1 == pAbility->Full_100)
    {
        /*100BASE-TX full duplex capable in reg 4.8*/
        phyEnMsk4 = phyEnMsk4 | (1 << 8);
        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 | (1 << 13);
        /*Full duplex mode in reg 0.8*/
        phyEnMsk0 = phyEnMsk0 | (1 << 8);
    }


    if (1 == pAbility->Full_1000)
    {
        /*1000 BASE-T FULL duplex capable setting in reg 9.9*/
        phyEnMsk9 = phyEnMsk9 | (1 << 9);

        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 | (1 << 6);
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));


        /*Auto-Negotiation setting in reg 0.12*/
        phyEnMsk0 = phyEnMsk0 | (1 << 12);

     }

    if (1 == pAbility->AutoNegotiation)
    {
        /*Auto-Negotiation setting in reg 0.12*/
        phyEnMsk0 = phyEnMsk0 | (1 << 12);
    }

    if (1 == pAbility->AsyFC)
    {
        /*Asymetric flow control in reg 4.11*/
        phyEnMsk4 = phyEnMsk4 | (1 << 11);
    }
    if (1 == pAbility->FC)
    {
        /*Flow control in reg 4.10*/
        phyEnMsk4 = phyEnMsk4 | (1 << 10);
    }

    /*1000 BASE-T control register setting*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_1000_BASET_CONTROL_REG, &phyData)) != L2SW_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x0200)) | phyEnMsk9;

    //set reg 9.8 0
    phyData = phyData & (~(1<<8));

    if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_1000_BASET_CONTROL_REG, phyData)) != L2SW_ERR_OK)
        return retVal;

    /*Auto-Negotiation control register setting*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_AN_ADVERTISEMENT_REG, &phyData)) != L2SW_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
    if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_AN_ADVERTISEMENT_REG, phyData)) != L2SW_ERR_OK)
        return retVal;

    /*Control register setting and restart auto*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, &phyData)) != L2SW_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x3140)) | phyEnMsk0;
    /*If have auto-negotiation capable, then restart auto negotiation*/
    if (1 == pAbility->AutoNegotiation)
    {
        phyData = phyData | (1 << 9);
    }

    if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, phyData)) != L2SW_ERR_OK)
        return retVal;

    if (1 == pAbility->AutoNegotiation)
    {
        phyData = phyData & ~(1 << 9);
    }

    if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, phyData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyAutoNegoAbility_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t       retVal;
    l2sw_uint32          phyData0;
    l2sw_uint32          phyData4;
    l2sw_uint32          phyData9;
#ifndef USE_L2SW_SDK
	l2sw_port_media_t    media_type;
#endif
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if(NULL == pAbility)
        return L2SW_ERR_NULL_POINTER;

#if defined(USE_L2SW_SDK)
		/*sf l2sw not support the below function*/
#else
    if (l2sw_switch_isComboPort(units, port) == L2SW_ERR_OK)
    {
        if ((retVal = _l2sw_port_phyComboPortMedia_get(units, port, &media_type)) != L2SW_ERR_OK)
            return retVal;

        if(media_type == PORT_MEDIA_FIBER)
        {
            return _l2sw_port_FiberModeAbility_get(units, port, pAbility);
        }
    }
#endif
    /*Control register setting and restart auto*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, &phyData0)) != L2SW_ERR_OK)
        return retVal;

    /*Auto-Negotiation control register setting*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_AN_ADVERTISEMENT_REG, &phyData4)) != L2SW_ERR_OK)
        return retVal;

    /*1000 BASE-T control register setting*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_1000_BASET_CONTROL_REG, &phyData9)) != L2SW_ERR_OK)
        return retVal;

    if (phyData9 & (1 << 9))
        pAbility->Full_1000 = 1;
    else
        pAbility->Full_1000 = 0;

    if (phyData4 & (1 << 11))
        pAbility->AsyFC = 1;
    else
        pAbility->AsyFC = 0;

    if (phyData4 & (1 << 10))
        pAbility->FC = 1;
    else
        pAbility->FC = 0;


    if (phyData4 & (1 << 8))
        pAbility->Full_100 = 1;
    else
        pAbility->Full_100 = 0;

    if (phyData4 & (1 << 7))
        pAbility->Half_100 = 1;
    else
        pAbility->Half_100 = 0;

    if (phyData4 & (1 << 6))
        pAbility->Full_10 = 1;
    else
        pAbility->Full_10 = 0;

    if (phyData4 & (1 << 5))
        pAbility->Half_10 = 1;
    else
        pAbility->Half_10 = 0;


    if (phyData0 & (1 << 12))
        pAbility->AutoNegotiation = 1;
    else
        pAbility->AutoNegotiation = 0;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyForceModeAbility_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
     l2sw_api_ret_t      retVal;
     l2sw_uint32         phyData;
     l2sw_uint32         phyEnMsk0;
     l2sw_uint32         phyEnMsk4;
     l2sw_uint32         phyEnMsk9;
#ifndef USE_L2SW_SDK
	 l2sw_port_media_t   media_type;
#endif
     /* Check initialization state */
     L2SW_CHK_INIT_STATE(units);

     /* Check Port Valid */
     L2SW_CHK_PORT_IS_UTP(units, port);

     if(NULL == pAbility)
        return L2SW_ERR_NULL_POINTER;

     if (pAbility->Half_10 >= L2SW_ENABLE_END || pAbility->Full_10 >= L2SW_ENABLE_END ||
        pAbility->Half_100 >= L2SW_ENABLE_END || pAbility->Full_100 >= L2SW_ENABLE_END ||
        pAbility->Full_1000 >= L2SW_ENABLE_END || pAbility->AutoNegotiation >= L2SW_ENABLE_END ||
        pAbility->AsyFC >= L2SW_ENABLE_END || pAbility->FC >= L2SW_ENABLE_END)
         return L2SW_ERR_INPUT;
#if defined(USE_L2SW_SDK)
		 /*sf l2sw not support the below function*/
#else
     if (l2sw_switch_isComboPort(units, port) == L2SW_ERR_OK)
     {
         if ((retVal = _l2sw_port_phyComboPortMedia_get(units, port, &media_type)) != L2SW_ERR_OK)
             return retVal;

         if(media_type == PORT_MEDIA_FIBER)
         {
             return _l2sw_port_FiberModeAbility_set(units, port, pAbility);
         }
     }

     if (1 == pAbility->Full_1000)
         return L2SW_ERR_INPUT;
#endif
     /*for PHY force mode setup*/
     pAbility->AutoNegotiation = 0;

     phyEnMsk0 = 0;
     phyEnMsk4 = 0;
     phyEnMsk9 = 0;

     if (1 == pAbility->Half_10)
     {
         /*10BASE-TX half duplex capable in reg 4.5*/
         phyEnMsk4 = phyEnMsk4 | (1 << 5);

         /*Speed selection [1:0] */
         /* 11=Reserved*/
         /* 10= 1000Mpbs*/
         /* 01= 100Mpbs*/
         /* 00= 10Mpbs*/
         phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
         phyEnMsk0 = phyEnMsk0 & (~(1 << 13));
     }

     if (1 == pAbility->Full_10)
     {
         /*10BASE-TX full duplex capable in reg 4.6*/
         phyEnMsk4 = phyEnMsk4 | (1 << 6);
         /*Speed selection [1:0] */
         /* 11=Reserved*/
         /* 10= 1000Mpbs*/
         /* 01= 100Mpbs*/
         /* 00= 10Mpbs*/
         phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
         phyEnMsk0 = phyEnMsk0 & (~(1 << 13));

         /*Full duplex mode in reg 0.8*/
         phyEnMsk0 = phyEnMsk0 | (1 << 8);

     }

     if (1 == pAbility->Half_100)
     {
         /*100BASE-TX half duplex capable in reg 4.7*/
         phyEnMsk4 = phyEnMsk4 | (1 << 7);
         /*Speed selection [1:0] */
         /* 11=Reserved*/
         /* 10= 1000Mpbs*/
         /* 01= 100Mpbs*/
         /* 00= 10Mpbs*/
         phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
         phyEnMsk0 = phyEnMsk0 | (1 << 13);
     }


     if (1 == pAbility->Full_100)
     {
         /*100BASE-TX full duplex capable in reg 4.8*/
         phyEnMsk4 = phyEnMsk4 | (1 << 8);
         /*Speed selection [1:0] */
         /* 11=Reserved*/
         /* 10= 1000Mpbs*/
         /* 01= 100Mpbs*/
         /* 00= 10Mpbs*/
         phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
         phyEnMsk0 = phyEnMsk0 | (1 << 13);
         /*Full duplex mode in reg 0.8*/
         phyEnMsk0 = phyEnMsk0 | (1 << 8);
     }

     if (1 == pAbility->AsyFC)
     {
         /*Asymetric flow control in reg 4.11*/
         phyEnMsk4 = phyEnMsk4 | (1 << 11);
     }
     if (1 == pAbility->FC)
     {
         /*Flow control in reg 4.10*/
         phyEnMsk4 = phyEnMsk4 | ((1 << 10));
     }

     /*1000 BASE-T control register setting*/
     if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_1000_BASET_CONTROL_REG, &phyData)) != L2SW_ERR_OK)
         return retVal;

     phyData = (phyData & (~0x0200)) | phyEnMsk9 ;

     if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_1000_BASET_CONTROL_REG, phyData)) != L2SW_ERR_OK)
         return retVal;

     /*Auto-Negotiation control register setting*/
     if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_AN_ADVERTISEMENT_REG, &phyData)) != L2SW_ERR_OK)
         return retVal;

     phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
     if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_AN_ADVERTISEMENT_REG, phyData)) != L2SW_ERR_OK)
         return retVal;

     /*Control register setting and power off/on*/
     phyData = phyEnMsk0 & (~(1 << 12));
     phyData |= (1 << 11);   /* power down PHY, bit 11 should be set to 1 */
     if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, phyData)) != L2SW_ERR_OK)
         return retVal;

     phyData = phyData & (~(1 << 11));   /* power on PHY, bit 11 should be set to 0*/
     if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, phyData)) != L2SW_ERR_OK)
         return retVal;

     return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyForceModeAbility_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t       retVal;
    l2sw_uint32          phyData0;
    l2sw_uint32          phyData4;
    l2sw_uint32          phyData9;
#ifndef USE_L2SW_SDK
	l2sw_port_media_t    media_type;
#endif
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
     L2SW_CHK_PORT_IS_UTP(units, port);

     if(NULL == pAbility)
        return L2SW_ERR_NULL_POINTER;

#if defined(USE_L2SW_SDK)
		 /*sf l2sw not support the below function*/
#else
     if (l2sw_switch_isComboPort(units, port) == L2SW_ERR_OK)
     {
         if ((retVal = _l2sw_port_phyComboPortMedia_get(units, port, &media_type)) != L2SW_ERR_OK)
             return retVal;

         if(media_type == PORT_MEDIA_FIBER)
         {
             return _l2sw_port_FiberModeAbility_get(units, port, pAbility);
         }
     }
#endif
    /*Control register setting and restart auto*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, &phyData0)) != L2SW_ERR_OK)
        return retVal;

    /*Auto-Negotiation control register setting*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_AN_ADVERTISEMENT_REG, &phyData4)) != L2SW_ERR_OK)
        return retVal;

    /*1000 BASE-T control register setting*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_1000_BASET_CONTROL_REG, &phyData9)) != L2SW_ERR_OK)
        return retVal;

    if (phyData9 & (1 << 9))
        pAbility->Full_1000 = 1;
    else
        pAbility->Full_1000 = 0;

    if (phyData4 & (1 << 11))
        pAbility->AsyFC = 1;
    else
        pAbility->AsyFC = 0;

    if (phyData4 & ((1 << 10)))
        pAbility->FC = 1;
    else
        pAbility->FC = 0;


    if (phyData4 & (1 << 8))
        pAbility->Full_100 = 1;
    else
        pAbility->Full_100 = 0;

    if (phyData4 & (1 << 7))
        pAbility->Half_100 = 1;
    else
        pAbility->Half_100 = 0;

    if (phyData4 & (1 << 6))
        pAbility->Full_10 = 1;
    else
        pAbility->Full_10 = 0;

    if (phyData4 & (1 << 5))
        pAbility->Half_10 = 1;
    else
        pAbility->Half_10 = 0;


    if (phyData0 & (1 << 12))
        pAbility->AutoNegotiation = 1;
    else
        pAbility->AutoNegotiation = 0;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_linkStatus_t *pLinkStatus, l2sw_port_speed_t *pSpeed, l2sw_port_duplex_t *pDuplex)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 phyData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if( (NULL == pLinkStatus) || (NULL == pSpeed) || (NULL == pDuplex) )
        return L2SW_ERR_NULL_POINTER;

    /*Get PHY resolved register*/
    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_RESOLVED_REG, &phyData)) != L2SW_ERR_OK)
        return retVal;

    /*check link status*/
    if (phyData & (1<<2))
    {
        *pLinkStatus = 1;

        /*check link speed*/
        *pSpeed = (phyData&0x0030) >> 4;

        /*check link duplex*/
        *pDuplex = (phyData&0x0008) >> 3;
    }
    else
    {
        *pLinkStatus = 0;
        *pSpeed = 0;
        *pDuplex = 0;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_macForceLink_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;
    l2sw_port_ability_t ability;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if(NULL == pPortability)
        return L2SW_ERR_NULL_POINTER;

    if (pPortability->forcemode >1|| pPortability->speed > 2 || pPortability->duplex > 1 ||
       pPortability->link > 1 || pPortability->nway > 1 || pPortability->txpause > 1 || pPortability->rxpause > 1)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_getAsicPortForceLink(units, l2sw_switch_port_L2P_get(units, port), &ability)) != L2SW_ERR_OK)
        return retVal;

    ability.forcemode = pPortability->forcemode;
    ability.speed     = pPortability->speed;
    ability.duplex    = pPortability->duplex;
    ability.link      = pPortability->link;
    ability.nway      = pPortability->nway;
    ability.txpause   = pPortability->txpause;
    ability.rxpause   = pPortability->rxpause;

    if ((retVal = l2sw_setAsicPortForceLink(units, l2sw_switch_port_L2P_get(units, port), &ability)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_macForceLink_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;
    l2sw_port_ability_t ability;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if(NULL == pPortability)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortForceLink(units, l2sw_switch_port_L2P_get(units, port), &ability)) != L2SW_ERR_OK)
        return retVal;

    pPortability->forcemode = ability.forcemode;
    pPortability->speed     = ability.speed;
    pPortability->duplex    = ability.duplex;
    pPortability->link      = ability.link;
    pPortability->nway      = ability.nway;
    pPortability->txpause   = ability.txpause;
    pPortability->rxpause   = ability.rxpause;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_macForceLinkExt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mode_ext_t mode, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;
    l2sw_port_ability_t ability;
    l2sw_uint32 ext_id;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

#if defined(USE_L2SW_SDK)
    L2SW_CHK_PORT_VALID(units, port);
#else
    /* Check Port Valid */
    L2SW_CHK_PORT_IS_EXT(units, port);
#endif

    if(NULL == pPortability)
        return L2SW_ERR_NULL_POINTER;

    if (mode >=MODE_EXT_END)
        return L2SW_ERR_INPUT;

    if(mode == MODE_EXT_HSGMII)
    {
        if (pPortability->forcemode > 1 || pPortability->speed != PORT_SPEED_2500M || pPortability->duplex != PORT_FULL_DUPLEX ||
           pPortability->link >= PORT_LINKSTATUS_END || pPortability->nway > 1 || pPortability->txpause > 1 || pPortability->rxpause > 1)
            return L2SW_ERR_INPUT;

        if(l2sw_switch_isHsgPort(units, port) != L2SW_ERR_OK)
            return L2SW_ERR_PORT_ID;
    }
    else
    {
        if (pPortability->forcemode > 1 || pPortability->speed > PORT_SPEED_1000M || pPortability->duplex >= PORT_DUPLEX_END ||
           pPortability->link >= PORT_LINKSTATUS_END || pPortability->nway > 1 || pPortability->txpause > 1 || pPortability->rxpause > 1)
            return L2SW_ERR_INPUT;
    }

    ext_id = port - 15;

    if(mode == MODE_EXT_DISABLE)
    {
        PRINT("port mode is disable!\n");
        MEMSET(&ability, 0x00, sizeof(l2sw_port_ability_t));
        if ((retVal = l2sw_setAsicPortForceLinkExt(units, ext_id, &ability)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicPortExtMode(units, ext_id, mode)) != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        if ((retVal = l2sw_setAsicPortExtMode(units, ext_id, mode)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_getAsicPortForceLinkExt(units, ext_id, &ability)) != L2SW_ERR_OK)
            return retVal;

        ability.forcemode = pPortability->forcemode;
        ability.speed     = (mode == MODE_EXT_HSGMII) ? PORT_SPEED_1000M : pPortability->speed;
        ability.duplex    = pPortability->duplex;
        ability.link      = pPortability->link;
        ability.nway      = pPortability->nway;
        ability.txpause   = pPortability->txpause;
        ability.rxpause   = pPortability->rxpause;

        if ((retVal = l2sw_setAsicPortForceLinkExt(units, ext_id, &ability)) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_macForceLinkExt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mode_ext_t *pMode, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;
    l2sw_port_ability_t ability;
    l2sw_uint32 ext_id;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

#if defined(USE_L2SW_SDK)
    L2SW_CHK_PORT_VALID(units, port);
#else
    /* Check Port Valid */
    L2SW_CHK_PORT_IS_EXT(units, port);
#endif

    if(NULL == pMode)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pPortability)
        return L2SW_ERR_NULL_POINTER;

    ext_id = port - 15;

    if ((retVal = l2sw_getAsicPortExtMode(units, ext_id, (l2sw_uint32 *)pMode)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPortForceLinkExt(units, ext_id, &ability)) != L2SW_ERR_OK)
        return retVal;

    pPortability->forcemode = ability.forcemode;
    pPortability->speed     = (*pMode == MODE_EXT_HSGMII) ? PORT_SPEED_2500M : ability.speed;
    pPortability->duplex    = ability.duplex;
    pPortability->link      = ability.link;
    pPortability->nway      = ability.nway;
    pPortability->txpause   = ability.txpause;
    pPortability->rxpause   = ability.rxpause;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_port_macStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortstatus)
{
    l2sw_api_ret_t retVal;
    l2sw_port_status_t status;
    l2sw_uint32 hsgsel;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pPortstatus)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortStatus(units, l2sw_switch_port_L2P_get(units, port), &status)) != L2SW_ERR_OK)
        return retVal;


    pPortstatus->duplex    = status.duplex;
    pPortstatus->link      = status.link;
    pPortstatus->nway      = status.nway;
    pPortstatus->txpause   = status.txpause;
    pPortstatus->rxpause   = status.rxpause;

    if( (retVal = l2sw_getAsicRegBit(units, L2SW_REG_SDS_MISC, L2SW_CFG_MAC8_SEL_HSGMII_OFFSET, &hsgsel)) != L2SW_ERR_OK)
            return retVal;

    if( (l2sw_switch_isHsgPort(units, port) == L2SW_ERR_OK) && (hsgsel == 1) )
        pPortstatus->speed = PORT_SPEED_2500M;
    else
        pPortstatus->speed = status.speed;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_macLocalLoopbackEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t   retVal;
#if 0
	l2sw_uint32      data;
#endif
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortLoopback(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
        return retVal;

#if 0
    if(l2sw_switch_isUtpPort(units, port) == L2SW_ERR_OK)
    {
        if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, &data)) != L2SW_ERR_OK)
            return retVal;

        if(enable == L2SW_ENABLED)
            data |= (0x0001 << 14);
        else
            data &= ~(0x0001 << 14);

        if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, data)) != L2SW_ERR_OK)
            return retVal;
    }
#endif
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_macLocalLoopbackEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortLoopback(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyReg_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_reg_t reg, l2sw_port_phy_data_t regData)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), reg, regData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyReg_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_reg_t reg, l2sw_port_phy_data_t *pData)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), reg, pData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_backpressureEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortJamMode(units, !enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_backpressureEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortJamMode(units, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pEnable = !regData;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_adminEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32      data;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_getAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, &data)) != L2SW_ERR_OK)
        return retVal;

    if(L2SW_ENABLED == enable)
    {
        data = PHY0_DEFAULT_VALUE;
    }
    else if (L2SW_DISABLED == enable)
    {
        data |= PHY0_POWER_DOWN_BIT;
    }

    if ((retVal = l2sw_setAsicPHYReg(units, l2sw_switch_port_L2P_get(units, port), PHY_CONTROL_REG, data)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_adminEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32      data;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_UTP(units, port);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_port_phyReg_get(units, port, PHY_CONTROL_REG, &data)) != L2SW_ERR_OK)
        return retVal;

    if ( (data & 0x0800) == 0x0800)
    {
        *pEnable = L2SW_DISABLED;
    }
    else
    {
        *pEnable = L2SW_ENABLED;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_isolation_set(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    /* check port mask */
    L2SW_CHK_PORTMASK_VALID(units, pPortmask);

    if ((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPortIsolationPermittedPortmask(units, l2sw_switch_port_L2P_get(units, port), pmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_isolation_get(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortIsolationPermittedPortmask(units, l2sw_switch_port_L2P_get(units, port), &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_rgmiiDelayExt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t rx_reverse, l2sw_enable_t tx_reverse, l2sw_data_t txDelay, l2sw_data_t rxDelay)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regAddr, regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_EXT(units, port);

    if(port == EXT_PORT0)
        regAddr = L2SW_REG_EXT1_RGMXF;
    else if(port == EXT_PORT1)
        regAddr = L2SW_REG_EXT2_RGMXF;
    else
        return L2SW_ERR_INPUT;

    if ((txDelay > 0x1f) || (rxDelay > 0x1f))
        return L2SW_ERR_INPUT;

    regData = (txDelay << 6) | rxDelay | (rx_reverse << 5) | (tx_reverse << 11);

    if ((retVal = l2sw_setAsicReg(units, regAddr, regData)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_rgmiiDelayExt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *rx_reverse, l2sw_enable_t *tx_reverse, l2sw_data_t *pTxDelay, l2sw_data_t *pRxDelay)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 regAddr, regData;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_IS_EXT(units, port);

    if( (NULL == pTxDelay) || (NULL == pRxDelay) )
        return L2SW_ERR_NULL_POINTER;

    if(port == EXT_PORT0)
        regAddr = L2SW_REG_EXT1_RGMXF;
    else if(port == EXT_PORT1)
        regAddr = L2SW_REG_EXT2_RGMXF;
    else
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_getAsicReg(units, regAddr, &regData)) != L2SW_ERR_OK)
        return retVal;

    *pTxDelay = (regData & 0x7c0) >> 6;
    *pRxDelay = regData & 0x1f;
    *rx_reverse = (regData & 0x20) >> 5;
    *tx_reverse = (regData & 0x800) >> 11;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_phyEnableAll_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 data;
    l2sw_uint32 port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicPortEnableAll(units, enable)) != L2SW_ERR_OK)
        return retVal;

    L2SW_SCAN_ALL_LOG_PORT(units, port)
    {
        if(l2sw_switch_isUtpPort(units, port) == L2SW_ERR_OK)
        {
            if ((retVal = _l2sw_port_phyReg_get(units, port, PHY_CONTROL_REG, &data)) != L2SW_ERR_OK)
                return retVal;

            if (L2SW_ENABLED == enable)
            {
                data &= 0xF7FF;
                data |= 0x0200;
            }
            else
            {
                data |= 0x0800;
            }

            if ((retVal = _l2sw_port_phyReg_set(units, port, PHY_CONTROL_REG, data)) != L2SW_ERR_OK)
                return retVal;
        }
    }

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_port_phyEnableAll_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortEnableAll(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_efid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_data_t efid)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    /* efid must be 0~7 */
    if (efid > L2SW_EFID_MAX)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicPortIsolationEfid(units, l2sw_switch_port_L2P_get(units, port), efid))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_efid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_data_t *pEfid)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEfid)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortIsolationEfid(units, l2sw_switch_port_L2P_get(units, port), pEfid))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_mtu_set(l2sw_uint8 units, l2sw_port_mtu_t mtu)
{
    l2sw_uint32 retVal;

    L2SW_CHK_INIT_STATE(units);

    if (mtu >= PORT_MTU_END)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_MAX_LENGTH_LIMINT_IPG, L2SW_MAX_LENTH_CTRL_MASK, mtu)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_mtu_get(l2sw_uint8 units, l2sw_port_mtu_t *mtu)
{
    l2sw_uint32 retVal;

    L2SW_CHK_INIT_STATE(units);

    if(NULL == mtu)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicRegBits(units, L2SW_REG_MAX_LENGTH_LIMINT_IPG, L2SW_MAX_LENTH_CTRL_MASK, mtu)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_ipg_set(l2sw_uint8 units, l2sw_uint32 ipg)
{
    l2sw_uint32 retVal;

    L2SW_CHK_INIT_STATE(units);

    ipg /= 3;

    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_MAX_LENGTH_LIMINT_IPG, L2SW_LIMIT_IPG_CFG_MASK, ipg)) != L2SW_ERR_OK)
        return retVal;

#if defined(USE_L2SW_SDK)
#else
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_MAC_LIMIT_IPG_CFG, L2SW_LIMIT_IPG_CFG_MASK, ipg)) != L2SW_ERR_OK)
        return retVal;
#endif

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_ipg_get(l2sw_uint8 units, l2sw_uint32 *ipg, l2sw_uint32 *mac_ipg)
{
    l2sw_uint32 retVal;

    L2SW_CHK_INIT_STATE(units);

    if(NULL == ipg)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicRegBits(units, L2SW_REG_MAX_LENGTH_LIMINT_IPG, L2SW_LIMIT_IPG_CFG_MASK, ipg)) != L2SW_ERR_OK)
        return retVal;

    (*ipg) *= 3;
#if defined(USE_L2SW_SDK)
#else
    if((retVal = l2sw_getAsicRegBits(units, L2SW_REG_MAC_LIMIT_IPG_CFG, L2SW_LIMIT_IPG_CFG_MASK, mac_ipg)) != L2SW_ERR_OK)
        return retVal;
#endif

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_mode_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_media_t mode)
{
    l2sw_uint32 retVal;
    l2sw_uint32 mac_data, phy_data;

    L2SW_CHK_INIT_STATE(units);

    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_GEPHY_P_COMMON_CF+port, &mac_data) != 0))
        return retVal;

    if(mode == PORT_MEDIA_COPPER)
    {
        mac_data &= 0xfffe;
        phy_data = 0x728;
    }
    else
    {
        mac_data |= 0x1;
        phy_data = 0x768;
    }

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_GEPHY_P_COMMON_CF+port, mac_data) != 0))
        return retVal;

    if((retVal =  _l2sw_port_phyReg_set(units, port, 0x12, phy_data) != 0))
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_mode_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_media_t *mode)
{
    l2sw_uint32 retVal;
    l2sw_uint32 mac_data;

    L2SW_CHK_INIT_STATE(units);

    if(NULL == mode)
        return L2SW_ERR_NULL_POINTER;

    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_GEPHY_P_COMMON_CF+port, &mac_data) != 0))
        return retVal;

    mac_data &= 0x1;

    *mode = mac_data;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_gmac_phy_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint16 reg, l2sw_uint16 value)
{
    l2sw_uint32 retVal;
    int phy_id;
    l2sw_uint32 data;
    l2sw_uint32 before;

    L2SW_CHK_INIT_STATE(units);

    //set pin
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_PINMUX_CTRL, &before)) != 0)
        return retVal;
    data = before;
    data &= 0xfffe;
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_PINMUX_CTRL, data)) != 0)
        return retVal;

    //set mode clause 22
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, &data)) != 0)
        return retVal;
    data &= (~0x3);
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, data)) != 0)
        return retVal;

    //set clk freq 250khz
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, &data)) != 0)
        return data;
    data &= (~0xff);
    data |= 0xc8;
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, data)) != 0)
        return retVal;


    if(units == 0)
    {
        if(port == 6)
            phy_id = UNIT0_PORT6_PHY_ID;
        else
            phy_id = UNIT0_PORT7_PHY_ID;
    }
    else if(units == 1)
    {
        if(port == 6)
            phy_id = UNIT1_PORT6_PHY_ID;
        else
            phy_id = UNIT1_PORT7_PHY_ID;
    }
    else
    {
        PRINT("Wrong unit\n");
        return -1;
    }

    //set read bit and phy_id
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, &data)) != 0)
        return retVal;

    data &= 0xfff3;
    data |= (MDIO_WRITE_OPECODE<<2);

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, data)) != 0)
        return retVal;

    //set phy_id
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, &data)) != 0)
        return retVal;

    data &= 0xfe0f;
    data |= (phy_id<<4);

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, data)) != 0)
        return retVal;

    //set reg_addr
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_REGAD, reg)) != 0)
        return retVal;

    //write value
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_WDATA, value)) != 0)
        return retVal;

    //start
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, &data)) != 0)
        return retVal;

    data &= 0xfeff;

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, data)) != 0)
        return retVal;

    data |= 0x100;

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, data)) != 0)
        return retVal;

    //wait success
    while(1)
    {
        DELAY_MS(10);

        if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_STATUS, &data)) != 0)
            return retVal;

        if((data & 0x1) == 1)
            break;
    }

    //reset pin
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_PINMUX_CTRL, before)) != 0)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_gmac_phy_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint16 reg, l2sw_uint16 *value)
{
    l2sw_uint32 retVal;
    int phy_id;
    l2sw_uint32 data;
    l2sw_uint32 before;

    L2SW_CHK_INIT_STATE(units);

    if(NULL == value)
        return L2SW_ERR_NULL_POINTER;

    //set pin
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_PINMUX_CTRL, &before)) != 0)
        return retVal;
    data = before;
    data &= 0xfffe;
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_PINMUX_CTRL, data)) != 0)
        return retVal;

    //set mode clause 22
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, &data)) != 0)
        return retVal;
    data &= (~0x3);
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, data)) != 0)
        return retVal;

    //set clk freq 250khz
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, &data)) != 0)
        return retVal;
    data &= (~0xff);
    data |= 0xc8;
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, data)) != 0)
        return retVal;


    if(units == 0)
    {
        if(port == 6)
            phy_id = UNIT0_PORT6_PHY_ID;
        else
            phy_id = UNIT0_PORT7_PHY_ID;
    }
    else if(units == 1)
    {
        if(port == 6)
            phy_id = UNIT1_PORT6_PHY_ID;
        else
            phy_id = UNIT1_PORT7_PHY_ID;
    }
    else
    {
        PRINT("Wrong unit\n");
        return -1;
    }

    //set read bit and phy_id
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, &data)) != 0)
        return retVal;

    data &= 0xfff3;
    data |= (MDIO_READ_OPECODE<<2);

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, data)) != 0)
        return retVal;

    //set phy_id
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, &data)) != 0)
        return retVal;

    data &= 0xfe0f;
    data |= (phy_id<<4);

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS, data)) != 0)
        return retVal;

    //set reg_addr
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_REGAD, reg)) != 0)
        return retVal;

    //start
    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, &data)) != 0)
        return retVal;

    data &= 0xfeff;

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, data)) != 0)
        return retVal;

    data |= 0x100;

    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_START, data)) != 0)
        return retVal;

    //wait success
    while(1)
    {
        DELAY_MS(10);

        if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_STATUS, &data)) != 0)
            return retVal;

        if((data & 0x1) == 1)
            break;
    }

    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_MDIO_REG_ACCESS_RDATA, &data)) != 0)
        return retVal;

    *value = (data & 0xffff);

    //reset pin
    if((retVal =  l2sw_setAsicReg(units, L2SW_REG_MDIO_REG_PINMUX_CTRL, before)) != 0)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_master_slave_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_master_t m_s)
{
    l2sw_uint32 retVal;
    l2sw_uint32 value;

    L2SW_CHK_INIT_STATE(units);

    if((retVal =  _l2sw_port_phyReg_get(units, port, PHY_REG_1000_BASET_CONTROL, &value)) != 0)
        return retVal;

    if(m_s == PORT_MASTER)
        value |= (PHY9_ENABLE_M_S_CONFIG_BIT | PHY9_MASTER_BIT);
    else if(m_s == PORT_SLAVE)
    {
        value |= PHY9_ENABLE_M_S_CONFIG_BIT;
        value &= (~PHY9_MASTER_BIT);
    }
    else
    {
        value &= (~PHY9_ENABLE_M_S_CONFIG_BIT);
        value &= (~PHY9_MASTER_BIT);
    }

    if((retVal =  _l2sw_port_phyReg_set(units, port, PHY_REG_1000_BASET_CONTROL, value)) != 0)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_master_slave_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_enable_t *enable, l2sw_port_master_t *m_s)
{
    l2sw_uint32 retVal;
    l2sw_uint32 value;

    L2SW_CHK_INIT_STATE(units);

    if(NULL == m_s)
        return L2SW_ERR_NULL_POINTER;

    if((retVal =  _l2sw_port_phyReg_get(units, port, PHY_REG_1000_BASET_CONTROL, &value)) != 0)
        return retVal;

    if((value & PHY9_ENABLE_M_S_CONFIG_BIT) != 0)
        *enable = L2SW_ENABLED;
    else
        *enable = L2SW_DISABLED;

    if((retVal =  _l2sw_port_phyReg_get(units, port, PHY_REG_1000_BASET_STATUS, &value)) != 0)
        return retVal;

    if((value & PHY10_MASTER_BIT) != 0)
        *m_s = PORT_MASTER;
    else
        *m_s = PORT_SLAVE;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_port_power_down_set(l2sw_uint8 units, l2sw_uint8 pbmp)
{
    l2sw_uint32 retVal;
    l2sw_uint32 value_en = 0, value_common = 0;
    l2sw_uint8 port_num = 0;
    l2sw_uint8 port_flag[5] = {0};
    l2sw_uint8 index = 0;

    L2SW_CHK_INIT_STATE(units);

    for(index = 0; index < 5; index++)
    {
        if((pbmp & (1<<index)) != 0)
        {
            port_num++;
            port_flag[index] = 1;
        }
    }

    if((retVal =  l2sw_getAsicReg(units, L2SW_REG_GEPHY_COMMON_CFG, &value_common)) != 0)
        return retVal;

    switch (port_num)
    {
        case 0:
            value_common = 0x1f2;
            value_en = 0x0;

            break;

        case 5:
            PRINT("All port can not be down!\n");
            return L2SW_ERR_INPUT;

        default:
            for(index = 0; index < 5; index++)
            {
                if(port_flag[index] == 0)
                {
                    value_common &= (~0xe);
                    value_common |= (index<<1);
                }

                if(port_flag[index] == 1)
                {
                    value_en |= (1<<index);
                }
            }

            break;
    }

    if(port_num == 0)
    {
        if((retVal =  l2sw_setAsicReg(units, L2SW_REG_PORT_PHY_POWER_EN, value_en)) != 0)
            return retVal;

        if((retVal =  l2sw_setAsicReg(units, L2SW_REG_GEPHY_COMMON_CFG, value_common)) != 0)
            return retVal;
    }
    else
    {
        if((retVal =  l2sw_setAsicReg(units, L2SW_REG_GEPHY_COMMON_CFG, value_common)) != 0)
            return retVal;

        if((retVal =  l2sw_setAsicReg(units, L2SW_REG_PORT_PHY_POWER_EN, value_en)) != 0)
            return retVal;
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_port_phyAutoNegoAbility_set
 * Description:
 *      Set ethernet PHY auto-negotiation desired ability.
 * Input:
 *      port        - port id.
 *      pAbility    - Ability structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      If Full_1000 bit is set to 1, the AutoNegotiation will be automatic set to 1. While both AutoNegotiation and Full_1000 are set to 0, the PHY speed and duplex selection will
 *      be set as following 100F > 100H > 10F > 10H priority sequence.
 */
l2sw_api_ret_t l2sw_port_phyAutoNegoAbility_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyAutoNegoAbility_set(units, port, pAbility);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyAutoNegoAbility_get
 * Description:
 *      Get PHY ability through PHY registers.
 * Input:
 *      port - Port id.
 * Output:
 *      pAbility - Ability structure
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      Get the capablity of specified PHY.
 */
l2sw_api_ret_t l2sw_port_phyAutoNegoAbility_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyAutoNegoAbility_get(units, port, pAbility);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyForceModeAbility_set
 * Description:
 *      Set the port speed/duplex mode/pause/asy_pause in the PHY force mode.
 * Input:
 *      port        - port id.
 *      pAbility    - Ability structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      While both AutoNegotiation and Full_1000 are set to 0, the PHY speed and duplex selection will
 *      be set as following 100F > 100H > 10F > 10H priority sequence.
 *      This API can be used to configure combo port in fiber mode.
 *      The possible parameters in fiber mode are Full_1000 and Full 100.
 *      All the other fields in l2sw_port_phy_ability_t will be ignored in fiber port.
 */
l2sw_api_ret_t l2sw_port_phyForceModeAbility_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyForceModeAbility_set(units, port, pAbility);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyForceModeAbility_get
 * Description:
 *      Get PHY ability through PHY registers.
 * Input:
 *      port - Port id.
 * Output:
 *      pAbility - Ability structure
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      Get the capablity of specified PHY.
 */
l2sw_api_ret_t l2sw_port_phyForceModeAbility_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_ability_t *pAbility)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyForceModeAbility_get(units, port, pAbility);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyStatus_get
 * Description:
 *      Get ethernet PHY linking status
 * Input:
 *      port - Port id.
 * Output:
 *      linkStatus  - PHY link status
 *      speed       - PHY link speed
 *      duplex      - PHY duplex mode
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      API will return auto negotiation status of phy.
 */
l2sw_api_ret_t l2sw_port_phyStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_linkStatus_t *pLinkStatus, l2sw_port_speed_t *pSpeed, l2sw_port_duplex_t *pDuplex)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyStatus_get(units, port, pLinkStatus, pSpeed, pDuplex);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macForceLink_set
 * Description:
 *      Set port force linking configuration.
 * Input:
 *      port            - port id.
 *      pPortability    - port ability configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can set Port/MAC force mode properties.
 */
l2sw_api_ret_t l2sw_port_macForceLink_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macForceLink_set(units, port, pPortability);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macForceLink_get
 * Description:
 *      Get port force linking configuration.
 * Input:
 *      port - Port id.
 * Output:
 *      pPortability - port ability configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get Port/MAC force mode properties.
 */
l2sw_api_ret_t l2sw_port_macForceLink_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macForceLink_get(units, port, pPortability);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macForceLinkExt_set
 * Description:
 *      Set external interface force linking configuration.
 * Input:
 *      port            - external port ID
 *      mode            - external interface mode
 *      pPortability    - port ability configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface force mode properties.
 *      The external interface can be set to:
 *      - MODE_EXT_DISABLE,
 *      - MODE_EXT_RGMII,
 *      - MODE_EXT_MII_MAC,
 *      - MODE_EXT_MII_PHY,
 *      - MODE_EXT_TMII_MAC,
 *      - MODE_EXT_TMII_PHY,
 *      - MODE_EXT_GMII,
 *      - MODE_EXT_RMII_MAC,
 *      - MODE_EXT_RMII_PHY,
 *      - MODE_EXT_SGMII,
 *      - MODE_EXT_HSGMII,
 *      - MODE_EXT_1000X_100FX,
 *      - MODE_EXT_1000X,
 *      - MODE_EXT_100FX,
 */
l2sw_api_ret_t l2sw_port_macForceLinkExt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mode_ext_t mode, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macForceLinkExt_set(units, port, mode, pPortability);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macForceLinkExt_get
 * Description:
 *      Set external interface force linking configuration.
 * Input:
 *      port            - external port ID
 * Output:
 *      pMode           - external interface mode
 *      pPortability    - port ability configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get external interface force mode properties.
 */
l2sw_api_ret_t l2sw_port_macForceLinkExt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mode_ext_t *pMode, l2sw_port_mac_ability_t *pPortability)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macForceLinkExt_get(units, port, pMode, pPortability);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macStatus_get
 * Description:
 *      Get port link status.
 * Input:
 *      port - Port id.
 * Output:
 *      pPortstatus - port ability configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get Port/PHY properties.
 */
l2sw_api_ret_t l2sw_port_macStatus_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_mac_ability_t *pPortstatus)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macStatus_get(units, port, pPortstatus);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macLocalLoopbackEnable_set
 * Description:
 *      Set Port Local Loopback. (Redirect TX to RX.)
 * Input:
 *      port    - Port id.
 *      enable  - Loopback state, 0:disable, 1:enable
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can enable/disable Local loopback in MAC.
 *      For UTP port, This API will also enable the digital
 *      loopback bit in PHY register for sync of speed between
 *      PHY and MAC. For EXT port, users need to force the
 *      link state by themself.
 */
l2sw_api_ret_t l2sw_port_macLocalLoopbackEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macLocalLoopbackEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_macLocalLoopbackEnable_get
 * Description:
 *      Get Port Local Loopback. (Redirect TX to RX.)
 * Input:
 *      port    - Port id.
 * Output:
 *      pEnable  - Loopback state, 0:disable, 1:enable
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_port_macLocalLoopbackEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_macLocalLoopbackEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyReg_set
 * Description:
 *      Set PHY register data of the specific port.
 * Input:
 *      port    - port id.
 *      reg     - Register id
 *      regData - Register data
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      This API can set PHY register data of the specific port.
 */
l2sw_api_ret_t l2sw_port_phyReg_set(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_reg_t reg, l2sw_port_phy_data_t regData)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyReg_set(units, port, reg, regData);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyReg_get
 * Description:
 *      Get PHY register data of the specific port.
 * Input:
 *      port    - Port id.
 *      reg     - Register id
 * Output:
 *      pData   - Register data
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_PHY_REG_ID       - Invalid PHY address
 *      L2SW_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      This API can get PHY register data of the specific port.
 */
l2sw_api_ret_t l2sw_port_phyReg_get(l2sw_uint8 units, l2sw_port_t port, l2sw_port_phy_reg_t reg, l2sw_port_phy_data_t *pData)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyReg_get(units, port, reg, pData);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_backpressureEnable_set
 * Description:
 *      Set the half duplex backpressure enable status of the specific port.
 * Input:
 *      port    - port id.
 *      enable  - Back pressure status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set the half duplex backpressure enable status of the specific port.
 *      The half duplex backpressure enable status of the port is as following:
 *      - DISABLE(Defer)
 *      - ENABLE (Backpressure)
 */
l2sw_api_ret_t l2sw_port_backpressureEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_backpressureEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_backpressureEnable_get
 * Description:
 *      Get the half duplex backpressure enable status of the specific port.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get the half duplex backpressure enable status of the specific port.
 *      The half duplex backpressure enable status of the port is as following:
 *      - DISABLE(Defer)
 *      - ENABLE (Backpressure)
 */
l2sw_api_ret_t l2sw_port_backpressureEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_backpressureEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_adminEnable_set
 * Description:
 *      Set port admin configuration of the specific port.
 * Input:
 *      port    - port id.
 *      enable  - Back pressure status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set port admin configuration of the specific port.
 *      The port admin configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
l2sw_api_ret_t l2sw_port_adminEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_adminEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_adminEnable_get
 * Description:
 *      Get port admin configurationof the specific port.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Back pressure status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get port admin configuration of the specific port.
 *      The port admin configuration of the port is as following:
 *      - DISABLE
 *      - ENABLE
 */
l2sw_api_ret_t l2sw_port_adminEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_adminEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_isolation_set
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port         - port id.
 *      pPortmask    - Permit port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      This API set the port mask that a port can trasmit packet to of each port
 *      A port can only transmit packet to ports included in permitted portmask
 */
l2sw_api_ret_t l2sw_port_isolation_set(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_isolation_set(units, port, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_isolation_get
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port - Port id.
 * Output:
 *      pPortmask - Permit port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API get the port mask that a port can trasmit packet to of each port
 *      A port can only transmit packet to ports included in permitted portmask
 */
l2sw_api_ret_t l2sw_port_isolation_get(l2sw_uint8 units, l2sw_port_t port, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_isolation_get(units, port, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_rgmiiDelayExt_set
 * Description:
 *      Set RGMII interface delay value for TX and RX.
 * Input:
 *      txDelay - TX delay value, 1 for delay 2ns and 0 for no-delay
 *      rxDelay - RX delay value, 0~7 for delay setup.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface 2 RGMII delay.
 *      In TX delay, there are 2 selection: no-delay and 2ns delay.
 *      In RX dekay, there are 8 steps for delay tunning. 0 for no-delay, and 7 for maximum delay.
 *      Note. This API should be called before l2sw_port_macForceLinkExt_set(units).
 */
l2sw_api_ret_t l2sw_port_rgmiiDelayExt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t rx_reverse, l2sw_enable_t tx_reverse, l2sw_data_t txDelay, l2sw_data_t rxDelay)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_rgmiiDelayExt_set(units, port, rx_reverse, tx_reverse, txDelay, rxDelay);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_rgmiiDelayExt_get
 * Description:
 *      Get RGMII interface delay value for TX and RX.
 * Input:
 *      None
 * Output:
 *      pTxDelay - TX delay value
 *      pRxDelay - RX delay value
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface 2 RGMII delay.
 *      In TX delay, there are 2 selection: no-delay and 2ns delay.
 *      In RX dekay, there are 8 steps for delay tunning. 0 for n0-delay, and 7 for maximum delay.
 */
l2sw_api_ret_t l2sw_port_rgmiiDelayExt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *rx_reverse, l2sw_enable_t *tx_reverse, l2sw_data_t *pTxDelay, l2sw_data_t *pRxDelay)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_rgmiiDelayExt_get(units, port, rx_reverse, tx_reverse, pTxDelay, pRxDelay);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyEnableAll_set
 * Description:
 *      Set all PHY enable status.
 * Input:
 *      enable - PHY Enable State.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can set all PHY status.
 *      The configuration of all PHY is as following:
 *      - DISABLE
 *      - ENABLE
 */
l2sw_api_ret_t l2sw_port_phyEnableAll_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyEnableAll_set(units, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_phyEnableAll_get
 * Description:
 *      Get all PHY enable status.
 * Input:
 *      None
 * Output:
 *      pEnable - PHY Enable State.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      This API can set all PHY status.
 *      The configuration of all PHY is as following:
 *      - DISABLE
 *      - ENABLE
 */
l2sw_api_ret_t l2sw_port_phyEnableAll_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_phyEnableAll_get(units, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_efid_set
 * Description:
 *      Set port-based enhanced filtering database
 * Input:
 *      port - Port id.
 *      efid - Specified enhanced filtering database.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_L2_FID - Invalid fid.
 *      L2SW_ERR_INPUT - Invalid input parameter.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can set port-based enhanced filtering database.
 */
l2sw_api_ret_t l2sw_port_efid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_data_t efid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_efid_set(units, port, efid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_efid_get
 * Description:
 *      Get port-based enhanced filtering database
 * Input:
 *      port - Port id.
 * Output:
 *      pEfid - Specified enhanced filtering database.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT - Invalid input parameters.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can get port-based enhanced filtering database status.
 */
l2sw_api_ret_t l2sw_port_efid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_data_t *pEfid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_efid_get(units, port, pEfid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_mtu_set
 * Description:
 *      Set Max packet length
 * Input:
 *      mtu        - Max packet length (0:1522, 1:1536, 2:1552, 3:10k)
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set Max packet length
 */
l2sw_api_ret_t l2sw_port_mtu_set(l2sw_uint8 units, l2sw_port_mtu_t mtu)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_mtu_set(units, mtu);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_mtu_get
 * Description:
 *      Get Max packet length
 * Input:
 *      mtu        - Max packet length (0:1522, 1:1536, 2:1552, 3:10k)
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set Max packet length
 */
l2sw_api_ret_t l2sw_port_mtu_get(l2sw_uint8 units, l2sw_port_mtu_t *mtu)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_mtu_get(units, mtu);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_ipg_set
 * Description:
 *      Set Interframe Gap length
 * Input:
 *      ipg        - Interframe Gap <4-32>
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set Interframe Gap length
 */
l2sw_api_ret_t l2sw_port_ipg_set(l2sw_uint8 units, l2sw_uint32 ipg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_ipg_set(units, ipg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_ipg_get
 * Description:
 *      Get Interframe Gap length
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get Interframe Gap length
 */
l2sw_api_ret_t l2sw_port_ipg_get(l2sw_uint8 units, l2sw_uint32 *ipg, l2sw_uint32 *mac_ipg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_ipg_get(units, ipg, mac_ipg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_mode_set
 * Description:
 *      Set utp port mode
 * Input:
 *      port - port id
 *      mode - copper,fiber
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set utp port mode
 */
l2sw_api_ret_t l2sw_port_mode_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_media_t mode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_mode_set(units, port, mode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_mode_get
 * Description:
 *      Get utp port mode
 * Input:
 *      port - port id
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get utp port mode
 */
l2sw_api_ret_t l2sw_port_mode_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_media_t *mode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_mode_get(units, port, mode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_gmacport_phy_set
 * Description:
 *      Set gmacport's phy value
 * Input:
 *      port - port id
 *      reg - phy reg
 *      value
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set gmacport's phy value
 */
l2sw_api_ret_t l2sw_port_gmac_phy_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint16 reg, l2sw_uint16 value)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_gmac_phy_set(units, port, reg, value);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_gmacport_phy_get
 * Description:
 *      Get gmacport's phy value
 * Input:
 *      port - port id
 *      reg - phy reg
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get gmacport's phy value
 */
l2sw_api_ret_t l2sw_port_gmac_phy_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint16 reg, l2sw_uint16 *value)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_gmac_phy_get(units, port, reg, value);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_master_slave_set
 * Description:
 *      Set utpport's master slave mode
 * Input:
 *      port - port id
 *      m_s - master or slave or auto
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set utpport's phy slave or master
 */
l2sw_api_ret_t l2sw_port_master_slave_set(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_master_t m_s)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_master_slave_set(units, port, m_s);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_master_slave_get
 * Description:
 *      Get utpport's master slave mode
 * Input:
 *      port - port id
 * Output:
 *      enable - manual config enable
 *      m_s - master or slave
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can get utpport's phy slave or master
 */
l2sw_api_ret_t l2sw_port_master_slave_get(l2sw_uint8 units, l2sw_uint32 port, l2sw_enable_t *enable, l2sw_port_master_t *m_s)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_master_slave_get(units, port, enable, m_s);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_port_powerdown
 * Description:
 *      Set power down port
 * Input:
 *      pbmp - port bitmap
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can set utpport's power down
 */
l2sw_api_ret_t l2sw_port_power_down_set(l2sw_uint8 units, l2sw_uint8 pbmp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_port_power_down_set(units, pbmp);
    L2SW_API_UNLOCK(units);

    return retVal;
}
