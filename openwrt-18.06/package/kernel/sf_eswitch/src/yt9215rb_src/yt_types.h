#ifndef _YT_TYPES_H
#define _YT_TYPES_H

/*
 * Include Files
 */
#if defined (__LINUX__)
#include <stdint.h>
#include <unistd.h>
#endif

#if !defined (__KERNEL__)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#endif

/*
 * Symbol Definition
 */

/*
 * Macro Definition
 */ 
#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


#define MAC_ADDR_LEN                    6
#define IP4_ADDR_LEN                    4
#define IP6_ADDR_LEN                    16

#define YT_MAX_PORT                     32 /* max user port num */
#define YT_MAX_UNIT                     2
#define YT_MAX_PORT_PER_UNIT            28
#define YT_MAX_SERDES_PER_UNIT          4
#define YT_MAX_PHY_PER_UNIT             4
#define YT_PORTS_WORD_SIZE              32
#define YT_PORTS_WORD_NUM               ((YT_MAX_PORT +  31)/YT_PORTS_WORD_SIZE)

/*
 * Data Type Declaration
 */
#if !defined (__LINUX__)
typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef short                   int16_t;
#ifndef _UINT32_T_DECLARED
typedef unsigned int            uint32_t;
#endif

#ifndef _INT32_T_DECLARED
typedef int                     int32_t;
#endif
#endif

typedef unsigned long long      uint64;

typedef _Bool                   yt_bool_t;
typedef uint16_t                yt_vlan_t;
typedef uint16_t                yt_tpid_t;
typedef uint16_t                yt_fid_t;
typedef uint32_t                yt_port_t;
typedef uint32_t                yt_macid_t;
typedef uint32_t                yt_ret_t;
typedef uint8_t                 yt_unit_t;
typedef uint8_t                 yt_tpidprofile_id_mask_t;
typedef uint8_t                 yt_stp_id_t;
typedef uint8_t                 yt_local_id_t;
typedef uint8_t                 yt_profile_id_t;
typedef uint8_t                 yt_meterid_t;
typedef uint8_t                 yt_pri_t;
typedef uint8_t                 yt_dscp_t;
typedef uint32_t                yt_cpu_code_t;
typedef uint32_t                yt_queue_pri_t;
typedef uint32_t                yt_queue_weight_t;
typedef uint16_t                yt_trans_tbl_id_t;
typedef uint32_t                yt_intr_status_t;
typedef uint32_t                uint32;
typedef unsigned char           uint8;

typedef struct yt_mac_addr_s
{
    uint8_t addr[MAC_ADDR_LEN];
} yt_mac_addr_t;

typedef struct yt_port_mask_s
{
    uint32_t portbits[YT_PORTS_WORD_NUM];
} yt_port_mask_t;

typedef enum yt_queue_type_e
{
    UNICAST_QUEUE,
    MULTICAST_QUEUE
}yt_queue_type_t;

typedef struct yt_qid_s
{
    uint8_t    port;
    yt_queue_type_t    qtype;
    uint8_t    qid;
}yt_qid_t;

typedef enum yt_rate_mode_e
{
    RATE_MODE_BYTE,
    RATE_MODE_PACKET,
}yt_rate_mode_t;

/* port medium type */
typedef enum yt_port_medium_e
{
    PORT_MEDI_COPPER	= 0,      /* port's PHY media is copper */
    PORT_MEDI_FIBER,       /* port's PHY media is fiber */
    PORT_MEDI_COMBO_FIBER	,   /* port's PHY media is combo and fiber first */
    PORT_MEDI_COMBO_COPPER,   /* port's PHY media is combo and copper first */
    PORT_MEDI_SERDES,      /* port media is serdes */
    PORT_MEDI_END
}yt_port_medium_t;

typedef enum yt_port_speed_e
{
    PORT_SPEED_10M = 0,
    PORT_SPEED_100M,
    PORT_SPEED_1000M,
    PORT_SPEED_2500M,
    PORT_SPEED_END
}yt_port_speed_t;

typedef enum yt_port_duplex_e
{
    PORT_DUPLEX_HALF = 0,
    PORT_DUPLEX_FULL
}yt_port_duplex_t;

typedef enum yt_port_speed_duplex_e
{
    PORT_SPEED_DUP_10HALF = 0,
    PORT_SPEED_DUP_10FULL,
    PORT_SPEED_DUP_100HALF,
    PORT_SPEED_DUP_100FULL,
    PORT_SPEED_DUP_1000FULL,
    PORT_SPEED_DUP_2500FULL,
    PORT_SPEED_DUP_END
}yt_port_speed_duplex_t;

typedef enum yt_port_link_status_e
{
    PORT_LINK_DOWN = 0,
    PORT_LINK_UP
}yt_port_link_status_t;

typedef enum yt_phy_type_e
{
    PHY_INTERNAL,
    PHY_EXTERNAL,
}yt_phy_type_t;

#endif //YT_TYPES_H
