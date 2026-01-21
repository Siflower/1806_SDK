/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_stat.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_STAT_H
#define __YT_STAT_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_types.h"

#if defined(SWITCH_SERIES_TIGER)

typedef enum yt_stat_mib_e {
    RX_BROADCAST = 0,       //--Count-- rx broadcast pkt    
    RX_PAUSE,               //--Count-- rx pause pkt
    RX_MULTICAST,           //--Count-- rx multicast pkt,exclude pause frame and OAM frame
    RX_FCS_ERR,             //--Count-- crc error pkt, pkt len >= 64
    RX_ALIGNMENT_ERR,       //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte)
    RX_UNDERSIZE,           //--Count-- pkt length less than 64B but crc right
    RX_FRAGMENT,            //--Count-- pkt length less than 64B but crc error
    RX_64B,                 //--Count-- pkt length 64B
    RX_65_127B,             //--Count-- pkt length between 65B and 127B 
    RX_128_255B,            //--Count-- pkt length between 128B and 255B
    RX_256_511B,            //--Count-- pkt length between 256B nad 511B
    RX_512_1023B,           //--Count-- pkt length between 512B nad 1023B
    RX_1024_1518B,          //--Count-- pkt length between 1024B nad 1518B
    RX_JUMBO,               //--Count-- jumbo frame,pkt length > 1518B
    RX_OKBYTE = 0xf,        //--Byte--  receive OK pkt length
    RX_NOT_OKBYTE = 0X11,   //--Byte--  receive not OK pkt length
    RX_OVERSIZE = 0X13,     //--Count-- pkt length is more than mac config
    RX_DISCARD,             //--Count-- drop in Ingress ppe exclude crc error and pause  
    TX_BROADCAST,           //--Count-- tx broadcast pkt
    TX_PAUSE,               //--Count-- tx pause pkt
    TX_MULTICAST,           //--Count-- tx multicast pkt,exclude pause frame and OAM frame
    TX_UNDERSIZE,           //--Count-- pkt length less than 64B
    TX_64B,                 //--Count-- pkt length 64B
    TX_65_127B,             //--Count-- pkt length between 65B and 127B 
    TX_128_255B,            //--Count-- pkt length between 128B and 255B
    TX_256_511B,            //--Count-- pkt length between 256B nad 511B
    TX_512_1023B,           //--Count-- pkt length between 512B nad 1023B
    TX_1024_1518B,          //--Count-- pkt length between 1024B nad 1518B
    TX_JUMBO,               //--Count-- jumbo frame,pkt length > 1518B
    TX_OKBYTE = 0X21,       //--Byte--  tx  OK pkt length
    TX_COLLISION = 0X23,    //--Count-- when half duplex and collision location is less than 64B(mac define),tx collision sum count
    TX_EXCESSIVE_COLLISION, //--Count-- when half duplex,the drop pkt which collision times is more than max retry times
    TX_MULTI_COLLISION,     //--Count-- when half duplex,multiple collision for one mac
    TX_SINGLE_COLLISION,    //--Count--  when half duplex,one collision for one mac
    TX_OK_PKT,              //--Count-- tx OK pkt
    TX_DEFER,               //--Count-- when half duplex,pkt is delayed because receive defer singal,not include frames involved in collisions
    TX_LATE_COLLISION,      //--Count-- when half duplex and collision location is more than 64B(mac define),tx collision count
    RX_OAM_COUNTER,         //--Count-- rx oam pkt counter
    TX_OAM_COUNTER,         //--Count-- tx oam pkt counter   
    RX_UNICAST,             //--Count-- rx unicast pkt counter
    TX_UNICAST,             //--Count-- tx unicast pkt counter  
    YT_STAT_MAX             //END 
}yt_stat_mib_t;

#pragma pack (4)
typedef struct yt_stat_mib_port_cnt_s {
    uint32 RX_BROADCAST;            //--Count-- rx broadcast pkt    
    uint32 RX_PAUSE;                //--Count-- rx pause pkt
    uint32 RX_MULTICAST;            //--Count-- rx multicast pkt,exclude pause frame and OAM frame
    uint32 RX_FCS_ERR;              //--Count-- crc error pkt, pkt len >= 64
    uint32 RX_ALIGNMENT_ERR;        //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte)
    uint32 RX_UNDERSIZE;            //--Count-- pkt length less than 64B but crc right
    uint32 RX_FRAGMENT;             //--Count-- pkt length less than 64B but crc error
    uint32 RX_64B;                  //--Count-- pkt length 64B
    uint32 RX_65_127B;              //--Count-- pkt length between 65B and 127B 
    uint32 RX_128_255B;             //--Count-- pkt length between 128B and 255B
    uint32 RX_256_511B;             //--Count-- pkt length between 256B nad 511B
    uint32 RX_512_1023B;            //--Count-- pkt length between 512B nad 1023B
    uint32 RX_1024_1518B;           //--Count-- pkt length between 1024B nad 1518B
    uint32 RX_JUMBO;                //--Count-- jumbo frame,pkt len > 1518B
    uint64 RX_OKBYTE;               //--Byte--  receive OK pkt length
    uint64 RX_NOT_OKBYTE;           //--Byte--  receive not OK pkt length
    uint32 RX_OVERSIZE;             //--Count-- pkt length is more than mac config
    uint32 RX_DISCARD;              //--Count-- drop in Ingress ppe exclude crc error and pause    
    uint32 TX_BROADCAST;            //--Count-- tx broadcast pkt
    uint32 TX_PAUSE;                //--Count-- tx pause pkt
    uint32 TX_MULTICAST;            //--Count-- tx multicast pkt,exclude pause frame and OAM frame
    uint32 TX_UNDERSIZE;            //--Count-- pkt length less than 64B
    uint32 TX_64B;                  //--Count-- pkt length less than 64B
    uint32 TX_65_127B;              //--Count-- pkt length between 65B and 127B 
    uint32 TX_128_255B;             //--Count-- pkt length between 128B and 255B
    uint32 TX_256_511B;             //--Count-- pkt length between 256B nad 511B
    uint32 TX_512_1023B;            //--Count-- pkt length between 512B nad 1023B
    uint32 TX_1024_1518B;           //--Count-- pkt length between 1024B nad 1518B
    uint32 TX_JUMBO;                //--Count-- jumbo frame,pkt length > 1518B
    uint64 TX_OKBYTE;               //--Byte--  tx  OK pkt length
    uint32 TX_COLLISION;            //--Count-- when half duplex and collision location is less than 64B(mac define),tx collision sum count
    uint32 TX_EXCESSIVE_COLLISION;  //--Count-- when half duplex,the drop pkt which collision times is more than max retry times
    uint32 TX_MULTI_COLLISION;      //--Count-- when half duplex,multiple collision for one mac
    uint32 TX_SINGLE_COLLISION;     //--Count-- when half duplex,one collision for one mac
    uint32 TX_OK_PKT;               //--Count-- tx OK pkt
    uint32 TX_DEFER;                //--Count-- when half duplex,pkt is delayed because receive defer singal,not include frames involved in collisions
    uint32 TX_LATE_COLLISION;       //--Count-- when half duplex and collision location is more than 64B(mac define),tx collision count
    uint32 RX_OAM_COUNTER;          //--Count-- rx oam pkt counter
    uint32 TX_OAM_COUNTER;          //--Count-- tx oam pkt counter
    uint32 RX_UNICAST;              //--Count-- rx unicast pkt counter
    uint32 TX_UNICAST;              //--Count-- tx unicast pkt counter
}__attribute__((aligned(4)))yt_stat_mib_port_cnt_t;
#pragma pack ()

#endif

#if defined(SWITCH_SERIES_SHARK)

typedef enum yt_stat_mib_e {
    RX_BROADCAST = 0,       //--Count-- rx broadcast pkt    
    RX_PAUSE,               //--Count-- rx pause pkt
    RX_UNSUPPORTFRAME,      //--Count-- rx pause pkt and opcode != 1
    RX_OAM_COUNTER,         //--Count-- rx oam pkt counter
    RX_MULTICAST,           //--Count-- rx multicast pkt,exclude pause frame and OAM frame
    RX_ALIGN_ERR_LESS_64B,  //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len < 64
    RX_ALIGN_ERR_64B_1518B, //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >= 64 and <=1518
    RX_ALIGN_ERR_JUMBO,     //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >1518
    RX_FCS_ERR_64B_1518B,   //--Count-- crc error pkt, pkt len >= 64 and <=1518
    RX_FCS_ERR_JUMBO,       //--Count-- crc error pkt, pkt len >1518
    RX_UNDERSIZE,           //--Count-- pkt length less than 64B but crc right
    RX_FRAGMENT,            //--Count-- pkt length less than 64B but crc error
    RX_64B,                 //--Count-- pkt length 64B
    RX_65_127B,             //--Count-- pkt length between 65B and 127B 
    RX_128_255B,            //--Count-- pkt length between 128B and 255B
    RX_256_511B,            //--Count-- pkt length between 256B nad 511B
    RX_512_1023B,           //--Count-- pkt length between 512B nad 1023B
    RX_1024_1518B,          //--Count-- pkt length between 1024B nad 1518B
    RX_JUMBO,               //--Count-- jumbo frame,pkt length > 1518B
    RX_OVERSIZE,            //--Count-- pkt length is more than mac config
    RX_OKBYTE,              //--Byte--  receive OK pkt length
    RX_NOT_OKBYTE = 0X16,   //--Byte--  receive not OK pkt length
    TX_BROADCAST = 0X18,    //--Count-- tx broadcast pkt
    TX_PAUSE,               //--Count-- tx pause pkt
    TX_OAM_COUNTER,         //--Count-- tx oam pkt counter  
    TX_MULTICAST,           //--Count-- tx multicast pkt,exclude pause frame and OAM frame
    RX_FC_DROP,             //--Count-- fc drop pkt
    TX_64B,                 //--Count-- pkt length 64B
    TX_65_127B,             //--Count-- pkt length between 65B and 127B 
    TX_128_255B,            //--Count-- pkt length between 128B and 255B
    TX_256_511B,            //--Count-- pkt length between 256B nad 511B
    TX_512_1023B,           //--Count-- pkt length between 512B nad 1023B
    TX_1024_1518B,          //--Count-- pkt length between 1024B nad 1518B
    TX_JUMBO,               //--Count-- jumbo frame,pkt length > 1518B
    TX_OVERSIZE,            //--Count-- pkt length is more than mac config
    TX_LATE_COLLISION,      //--Count-- when half duplex and collision location is more than 64B(mac define),tx collision count
    TX_EXCESSIVE_COLLISION, //--Count-- when half duplex,the drop pkt which collision times is more than max retry times
    TX_MULTI_COLLISION,     //--Count-- when half duplex,multiple collision for one mac
    TX_SINGLE_COLLISION,    //--Count-- when half duplex,one collision for one mac
    TX_OK_PKT,              //--Count-- tx OK pkt
    TX_DEFER,               //--Count-- when half duplex,pkt is delayed because receive defer singal,not include frames involved in collisions
    TX_COLLISION,           //--Count-- when half duplex and collision location is less than 64B(mac define),tx collision sum count
    TX_OKBYTE,              //--Byte--  tx  OK pkt length
    RX_DISCARD = 0X2e,      //--Count-- drop in Ingress ppe exclude crc error and pause  
    TX_QUEUE_DROP,          //--Count-- queue drop pkt
    RX_UNICAST,             //--Count-- rx unicast pkt counter
    TX_UNICAST,             //--Count-- tx unicast pkt counter  
    YT_STAT_MAX             //END 
}yt_stat_mib_t;

#pragma pack (4)
typedef struct yt_stat_mib_port_cnt_s {
    uint32 RX_BROADCAST;             //--Count-- rx broadcast pkt    
    uint32 RX_PAUSE;                 //--Count-- rx pause pkt
    uint32 RX_UNSUPPORTFRAME;        //--Count-- rx pause pkt and opcode != 1
    uint32 RX_OAM_COUNTER;           //--Count-- rx oam pkt counter
    uint32 RX_MULTICAST;             //--Count-- rx multicast pkt,exclude pause frame and OAM frame
    uint32 RX_ALIGN_ERR_LESS_64B;    //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len < 64
    uint32 RX_ALIGN_ERR_64B_1518B;   //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >= 64 and <=1518
    uint32 RX_ALIGN_ERR_JUMBO;       //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >1518
    uint32 RX_FCS_ERR_64B_1518B;     //--Count-- crc error pkt, pkt len >= 64 and <=1518
    uint32 RX_FCS_ERR_JUMBO;         //--Count-- crc error pkt, pkt len >1518
    uint32 RX_UNDERSIZE;             //--Count-- pkt length less than 64B but crc right
    uint32 RX_FRAGMENT;              //--Count-- pkt length less than 64B but crc error
    uint32 RX_64B;                   //--Count-- pkt length 64B
    uint32 RX_65_127B;               //--Count-- pkt length between 65B and 127B 
    uint32 RX_128_255B;              //--Count-- pkt length between 128B and 255B
    uint32 RX_256_511B;              //--Count-- pkt length between 256B nad 511B
    uint32 RX_512_1023B;             //--Count-- pkt length between 512B nad 1023B
    uint32 RX_1024_1518B;            //--Count-- pkt length between 1024B nad 1518B
    uint32 RX_JUMBO;                 //--Count-- jumbo frame,pkt len > 1518B
    uint32 RX_OVERSIZE;              //--Count-- pkt length is more than mac config
    uint64 RX_OKBYTE;                //--Byte--  receive OK pkt length
    uint64 RX_NOT_OKBYTE;            //--Byte--  receive not OK pkt length
    uint32 TX_BROADCAST;             //--Count-- tx broadcast pkt
    uint32 TX_PAUSE;                 //--Count-- tx pause pkt
    uint32 TX_OAM_COUNTER;           //--Count-- tx oam pkt counter
    uint32 TX_MULTICAST;             //--Count-- tx multicast pkt,exclude pause frame and OAM frame
    uint32 RX_FC_DROP;               //--Count-- fc drop pkt
    uint32 TX_64B;                   //--Count-- pkt length less than 64B
    uint32 TX_65_127B;               //--Count-- pkt length between 65B and 127B 
    uint32 TX_128_255B;              //--Count-- pkt length between 128B and 255B
    uint32 TX_256_511B;              //--Count-- pkt length between 256B nad 511B
    uint32 TX_512_1023B;             //--Count-- pkt length between 512B nad 1023B
    uint32 TX_1024_1518B;            //--Count-- pkt length between 1024B nad 1518B
    uint32 TX_JUMBO;                 //--Count-- jumbo frame,pkt length > 1518B
    uint32 TX_OVERSIZE;              //--Count-- pkt length is more than mac config
    uint32 TX_LATE_COLLISION;        //--Count-- when half duplex and collision location is more than 64B(mac define),tx collision count
    uint32 TX_EXCESSIVE_COLLISION;   //--Count-- when half duplex,the drop pkt which collision times is more than max retry times
    uint32 TX_MULTI_COLLISION;       //--Count-- when half duplex,multiple collision for one mac
    uint32 TX_SINGLE_COLLISION;      //--Count-- when half duplex,one collision for one mac
    uint32 TX_OK_PKT;                //--Count-- tx OK pkt
    uint32 TX_DEFER;                 //--Count-- when half duplex,pkt is delayed because receive defer singal,not include frames involved in collisions
    uint32 TX_COLLISION;             //--Count-- when half duplex and collision location is less than 64B(mac define),tx collision sum count
    uint64 TX_OKBYTE;                //--Byte--  tx  OK pkt length
    uint32 RX_DISCARD;               //--Count-- drop in Ingress ppe exclude crc error and pause    
    uint32 TX_QUEUE_DROP;            //--Count-- queue drop pkt
    uint32 RX_UNICAST;               //--Count-- rx unicast pkt counter
    uint32 TX_UNICAST;               //--Count-- tx unicast pkt counter
}__attribute__((aligned(4)))yt_stat_mib_port_cnt_t;
#pragma pack ()

#endif

#if defined(SWITCH_SERIES_WHALE)

typedef enum yt_stat_mib_e {
    RX_BROADCAST = 0,       //--Count-- rx broadcast pkt    
    RX_PAUSE,               //--Count-- rx pause pkt
    RX_UNICAST,             //--Count-- rx unicast pkt
    RX_OAM_COUNTER,         //--Count-- rx oam pkt counter
    RX_MULTICAST,           //--Count-- rx multicast pkt,exclude pause frame and OAM frame
    RX_ALIGN_ERR_LESS_64B,  //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len < 64
    RX_ALIGN_ERR_64B_1518B, //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >= 64 and <=1518
    RX_ALIGN_ERR_JUMBO,     //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >1518
    RX_FCS_ERR_64B_1518B,   //--Count-- crc error pkt, pkt len >= 64 and <=1518
    RX_FCS_ERR_JUMBO,       //--Count-- crc error pkt, pkt len >1518
    RX_UNDERSIZE,           //--Count-- pkt length less than 64B but crc right
    RX_FRAGMENT,            //--Count-- pkt length less than 64B but crc error
    RX_64B,                 //--Count-- pkt length 64B
    RX_65_127B,             //--Count-- pkt length between 65B and 127B 
    RX_128_255B,            //--Count-- pkt length between 128B and 255B
    RX_256_511B,            //--Count-- pkt length between 256B nad 511B
    RX_512_1023B,           //--Count-- pkt length between 512B nad 1023B
    RX_1024_1518B,          //--Count-- pkt length between 1024B nad 1518B
    RX_JUMBO,               //--Count-- jumbo frame,pkt length > 1518B
    RX_OVERSIZE,            //--Count-- pkt length is more than mac config
    TX_BROADCAST,           //--Count-- tx broadcast pkt
    TX_PAUSE,               //--Count-- tx pause pkt
    TX_OAM_COUNTER,         //--Count-- tx oam pkt counter  
    TX_MULTICAST,           //--Count-- tx multicast pkt,exclude pause frame and OAM frame
    RX_FC_DROP,             //--Count-- fc drop pkt
    TX_64B,                 //--Count-- pkt length 64B
    TX_65_127B,             //--Count-- pkt length between 65B and 127B 
    TX_128_255B,            //--Count-- pkt length between 128B and 255B
    TX_256_511B,            //--Count-- pkt length between 256B nad 511B
    TX_512_1023B,           //--Count-- pkt length between 512B nad 1023B
    TX_1024_1518B,          //--Count-- pkt length between 1024B nad 1518B
    TX_JUMBO,               //--Count-- jumbo frame,pkt length > 1518B
    TX_OVERSIZE,            //--Count-- pkt length is more than mac config
    TX_LATE_COLLISION,      //--Count-- when half duplex and collision location is more than 64B(mac define),tx collision count
    TX_EXCESSIVE_COLLISION, //--Count-- when half duplex,the drop pkt which collision times is more than max retry times
    TX_MULTI_COLLISION,     //--Count-- when half duplex,multiple collision for one mac
    TX_SINGLE_COLLISION,    //--Count-- when half duplex,one collision for one mac
    TX_UNICAST,             //--Count-- tx unicast pkt
    TX_DEFER,               //--Count-- when half duplex,pkt is delayed because receive defer singal,not include frames involved in collisions
    TX_COLLISION,           //--Count-- when half duplex and collision location is less than 64B(mac define),tx collision sum count
    RX_DISCARD,             //--Count-- drop in Ingress ppe exclude crc error and pause  
    TX_QUEUE_DROP,          //--Count-- queue drop pkt
    TX_MAC_FILTER,          //--Count-- tx mac filter
    RX_OKBYTE,              //--Byte--  receive OK pkt length
    RX_NOT_OKBYTE,          //--Byte--  receive not OK pkt length
    TX_OKBYTE,              //--Byte--  tx  OK pkt length
    YT_STAT_MAX             //END 
}yt_stat_mib_t;

#pragma pack (4)
typedef struct yt_stat_mib_port_cnt_s {
    uint64 RX_BROADCAST;             //--Count-- rx broadcast pkt    
    uint64 RX_PAUSE;                 //--Count-- rx pause pkt
    uint64 RX_UNICAST;               //--Count-- rx unicast pkt
    uint64 RX_OAM_COUNTER;           //--Count-- rx oam pkt counter
    uint64 RX_MULTICAST;             //--Count-- rx multicast pkt,exclude pause frame and OAM frame
    uint64 RX_ALIGN_ERR_LESS_64B;    //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len < 64
    uint64 RX_ALIGN_ERR_64B_1518B;   //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >= 64 and <=1518
    uint64 RX_ALIGN_ERR_JUMBO;       //--Count-- pkt which have a bad FCS with a non-integral number of octets(half byte), pkt len >1518
    uint64 RX_FCS_ERR_64B_1518B;     //--Count-- crc error pkt, pkt len >= 64 and <=1518
    uint64 RX_FCS_ERR_JUMBO;         //--Count-- crc error pkt, pkt len >1518
    uint64 RX_UNDERSIZE;             //--Count-- pkt length less than 64B but crc right
    uint64 RX_FRAGMENT;              //--Count-- pkt length less than 64B but crc error
    uint64 RX_64B;                   //--Count-- pkt length 64B
    uint64 RX_65_127B;               //--Count-- pkt length between 65B and 127B 
    uint64 RX_128_255B;              //--Count-- pkt length between 128B and 255B
    uint64 RX_256_511B;              //--Count-- pkt length between 256B nad 511B
    uint64 RX_512_1023B;             //--Count-- pkt length between 512B nad 1023B
    uint64 RX_1024_1518B;            //--Count-- pkt length between 1024B nad 1518B
    uint64 RX_JUMBO;                 //--Count-- jumbo frame,pkt len > 1518B
    uint64 RX_OVERSIZE;              //--Count-- pkt length is more than mac config
    uint64 TX_BROADCAST;             //--Count-- tx broadcast pkt
    uint64 TX_PAUSE;                 //--Count-- tx pause pkt
    uint64 TX_OAM_COUNTER;           //--Count-- tx oam pkt counter
    uint64 TX_MULTICAST;             //--Count-- tx multicast pkt,exclude pause frame and OAM frame
    uint64 RX_FC_DROP;               //--Count-- fc drop pkt
    uint64 TX_64B;                   //--Count-- pkt length less than 64B
    uint64 TX_65_127B;               //--Count-- pkt length between 65B and 127B 
    uint64 TX_128_255B;              //--Count-- pkt length between 128B and 255B
    uint64 TX_256_511B;              //--Count-- pkt length between 256B nad 511B
    uint64 TX_512_1023B;             //--Count-- pkt length between 512B nad 1023B
    uint64 TX_1024_1518B;            //--Count-- pkt length between 1024B nad 1518B
    uint64 TX_JUMBO;                 //--Count-- jumbo frame,pkt length > 1518B
    uint64 TX_OVERSIZE;              //--Count-- pkt length is more than mac config
    uint64 TX_LATE_COLLISION;        //--Count-- when half duplex and collision location is more than 64B(mac define),tx collision count
    uint64 TX_EXCESSIVE_COLLISION;   //--Count-- when half duplex,the drop pkt which collision times is more than max retry times
    uint64 TX_MULTI_COLLISION;       //--Count-- when half duplex,multiple collision for one mac
    uint64 TX_SINGLE_COLLISION;      //--Count-- when half duplex,one collision for one mac
    uint64 TX_UNICAST;               //--Count-- tx unicast pkt
    uint64 TX_DEFER;                 //--Count-- when half duplex,pkt is delayed because receive defer singal,not include frames involved in collisions
    uint64 TX_COLLISION;             //--Count-- when half duplex and collision location is less than 64B(mac define),tx collision sum count
    uint64 RX_DISCARD;               //--Count-- drop in Ingress ppe exclude crc error and pause    
    uint64 TX_QUEUE_DROP;            //--Count-- queue drop pkt
    uint64 TX_MAC_FILTER;            //--Count-- tx mac filter
    uint64 RX_OKBYTE;                //--Byte--  receive OK pkt length
    uint64 RX_NOT_OKBYTE;            //--Byte--  receive not OK pkt length
    uint64 TX_OKBYTE;                //--Byte--  tx  OK pkt length
}__attribute__((aligned(4)))yt_stat_mib_port_cnt_t;
#pragma pack ()

#endif


typedef enum yt_stat_type_e {
    YT_STATE_TYPE_FLOW = 0,
#if defined(SWITCH_SERIES_TIGER)
    YT_STATE_TYPE_CPU_CODE,
    YT_STATE_TYPE_DROP_CODE,
    YT_STATE_TYPE_PORT,
#endif
    YT_STATE_TYPE_MAX,
}yt_stat_type_t;

typedef enum yt_stat_mode_e {
    YT_STATE_MODE_BYTE = 0,
    YT_STATE_MODE_PACKET,
}yt_stat_mode_t;


/**
 * @internal      yt_stat_mib_init
 * @endinternal
 *
 * @brief         initial mib function and lock
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_init(yt_unit_t unit);


/**
 * @internal      yt_stat_mib_enable_set
 * @endinternal
 *
 * @brief         enable mib function
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_enable_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      yt_stat_mib_enable_get
 * @endinternal
 *
 * @brief         get mib enable value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_enable_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      yt_stat_mib_clear
 * @endinternal
 *
 * @brief         clear one port mib data
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_clear(yt_unit_t unit, yt_port_t port);


/**
 * @internal      yt_stat_mib_clear_all
 * @endinternal
 *
 * @brief         clear all ports mib data
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_clear_all(yt_unit_t unit);


/**
 * @internal      yt_stat_mib_port_get
 * @endinternal
 *
 * @brief         get one port all mib info
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pCnt                -mib info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_port_get(yt_unit_t unit, yt_port_t port, yt_stat_mib_port_cnt_t *pCnt);

/**
 * +++internal      yt_stat_mib_port_singleType_cnt_get
 * +++endinternal
 *
 * +++brief         get one port single type mib cnt info
 * +++note          APPLICABLE DEVICES  -Whale
 * +++param[in]     unit                -unit id
 * +++param[in]     port                -port num
 * +++param[out]    pCnt                -mib info
 * +++retval        CMM_ERR_OK          -on success
 * +++retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_mib_port_singleType_cnt_get(yt_unit_t unit, yt_port_t port, yt_stat_mib_t mibType, uint64 *pCnt);

/**
 * @internal      yt_stat_flow_enable_set
 * @endinternal
 *
 * @brief         enable flow function with flow id
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow  id,range(Tiger:0~63, Shark:0~127, Whale:0~127)
 * @param[in]     enable              -enable value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_flow_enable_set(yt_unit_t unit, uint32_t flow_id, yt_enable_t enable);

/**
 * @internal      yt_stat_flow_enable_get
 * @endinternal
 *
 * @brief         get flow function with flow id enable value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow  id,range(Tiger:0~63, Shark:0~127, Whale:0~127)
 * @param[in]     pEnable             -enable value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_flow_enable_get(yt_unit_t unit, uint32_t flow_id, yt_enable_t *pEnable);


/**
 * @internal      yt_stat_flow_mode_set
 * @endinternal
 *
 * @brief         set flow id type and mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id,range(Tiger:0~63, Shark:0~127, Whale:0~127)
 * @param[in]     type                -type value for port base or flow base
 * @param[in]     mode                -mode for byte or pkt
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_flow_mode_set(yt_unit_t unit, uint32_t flow_id, yt_stat_type_t type, yt_stat_mode_t mode);

/**
 * @internal      yt_stat_flow_mode_get
 * @endinternal
 *
 * @brief         get flow id type and mode
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id,range(Tiger:0~63, Shark:0~127, Whale:0~127)
 * @param[in]     pType               -type value for port base or flow base
 * @param[in]     pMode               -mode for byte or pkt
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_flow_mode_get(yt_unit_t unit, uint32_t flow_id, yt_stat_type_t *pType, yt_stat_mode_t *pMode);


/**
 * @internal      yt_stat_flow_count_set
 * @endinternal
 *
 * @brief         set flow count for clear count
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id,range(Tiger:0~63, Shark:0~127, Whale:0~127)
 * @param[in]     cnt                 -counter value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_flow_count_set(yt_unit_t unit, uint32_t flow_id, uint64 cnt);


/**
 * @internal      yt_stat_flow_count_get
 * @endinternal
 *
 * @brief            get flow counter value
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     flow_id             -flow id,range(Tiger:0~63, Shark:0~127, Whale:0~127)
 * @param[out]    pCnt                -count value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_stat_flow_count_get(yt_unit_t unit, uint32_t flow_id, uint64 *pCnt);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
