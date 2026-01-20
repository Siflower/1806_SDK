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

#ifndef _L2SW_TYPES_H_
#define _L2SW_TYPES_H_

//#define L2SW_LINUX_KERNEL

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/slab.h>

#define USE_L2SW_SDK 1 /*if use l2sw, here must define.*/

//#define SPI_OPERATION
//#define I2C_OPERATION
//#define MDC_MDIO_OPERATION

#if defined(I2C_OPERATION)
#define L2SW_SDK_SHELL_SPI_IIC "IIC"
#elif defined(SPI_OPERATION)
#define L2SW_SDK_SHELL_SPI_IIC "SPI"
#else
#define L2SW_SDK_SHELL_SPI_IIC "MDIO"
#endif

typedef unsigned long long      l2sw_uint64;
typedef long long               l2sw_int64;
typedef unsigned int            l2sw_uint32;
typedef int                     l2sw_int32;
typedef unsigned short          l2sw_uint16;
typedef short                   l2sw_int16;
typedef unsigned char           l2sw_uint8;
typedef char                    l2sw_int8;

#define CONST_T     const

#define L2SW_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST    1

#define L2SW_MAX_NUM_OF_PORT                         8
#define L2SW_PORT_ID_MAX                             (L2SW_MAX_NUM_OF_PORT-1)
#define L2SW_PHY_ID_MAX                              (L2SW_MAX_NUM_OF_PORT-4)
#define L2SW_MAX_PORT_MASK                           0xFF

#define L2SW_WHOLE_SYSTEM                            0xFF

typedef struct l2sw_portmask_s
{
    l2sw_uint32  bits[L2SW_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST];
} l2sw_portmask_t;

typedef enum l2sw_enable_e
{
    L2SW_DISABLED = 0,
    L2SW_ENABLED,
    L2SW_ENABLE_END
} l2sw_enable_t;

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN      6
#endif

/* ethernet address type */
typedef struct  l2sw_mac_s
{
    l2sw_uint8 octet[ETHER_ADDR_LEN];
} l2sw_mac_t;

typedef l2sw_uint32  l2sw_pri_t;      /* priority vlaue */
typedef l2sw_uint32  l2sw_qid_t;      /* queue id type */
typedef l2sw_uint32  l2sw_data_t;
typedef l2sw_uint32  l2sw_dscp_t;     /* dscp vlaue */
typedef l2sw_uint32  l2sw_fid_t;      /* filter id type */
typedef l2sw_uint32  l2sw_vlan_t;     /* vlan id type */
typedef l2sw_uint32  l2sw_mac_cnt_t;  /* MAC count type  */
typedef l2sw_uint32  l2sw_meter_id_t; /* meter id type  */
typedef l2sw_uint32  l2sw_rate_t;     /* rate type  */

typedef enum l2sw_port_e
{
    UTP_PORT0 = 0,
    UTP_PORT1,
    UTP_PORT2,
    UTP_PORT3,
    UTP_PORT4,
    UTP_PORT5,
    UTP_PORT6,
    UTP_PORT7,
    INTERNAL_PORT = 15,
    EXT_PORT0 = 16,
    EXT_PORT1,
    EXT_PORT2,

    UNDEFINE_PORT = 30,
    L2SW_PORT_MAX = 31,
    PORT_WHOLE_SYSTEM = 0xff /*add for avoid PC-LINT warning*/
} l2sw_port_t;

typedef l2sw_uint32                  ipaddr_t;
typedef l2sw_uint32                  memaddr;

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN      6
#endif

typedef struct ether_addr_s {
    l2sw_uint8 octet[ETHER_ADDR_LEN];
} ether_addr_t;

#define rtlglue_printf printk
#define rtlglue_delay mdelay
#define PRINT           rtlglue_printf
#define DELAY_MS        rtlglue_delay

/* type abstraction */
#ifdef EMBEDDED_SUPPORT

typedef l2sw_int16                   l2sw_api_ret_t;
typedef l2sw_int16                   ret_t;
typedef l2sw_uint32                  l2sw_u_long;

#else

typedef l2sw_int32                   l2sw_api_ret_t;
typedef l2sw_int32                   ret_t;
typedef l2sw_uint64                  l2sw_u_long_t;

#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef L2SW_TRUE
#define L2SW_TRUE 1
#endif

#ifndef L2SW_FALSE
#define L2SW_FALSE 0
#endif

#define CONST           const
#endif /* _L2SW_TYPES_H_ */
