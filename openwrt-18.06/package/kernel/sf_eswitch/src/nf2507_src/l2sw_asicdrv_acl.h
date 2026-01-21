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

#ifndef _L2SW_ASICDRV_ACL_H_
#define _L2SW_ASICDRV_ACL_H_

#include "l2sw_asicdrv.h"

#define l2sw_flag 1
#define L2SW_ACLRULENO                  96

#define L2SW_ACLRULEMAX                 (L2SW_ACLRULENO-1)
#define L2SW_ACLRULEFIELDNO             8
#define L2SW_ACLTEMPLATENO              5
#define L2SW_ACLTYPEMAX                 (L2SW_ACLTEMPLATENO-1)

#define L2SW_ACLRULETBLEN               9
#define L2SW_ACLACTTBLEN                4
#if (l2sw_flag==0)
#define L2SW_ACLRULETBADDR(type, rule)  ((type << 6) | rule)
#define L2SW_ACLRULETBADDR2(type, rule) ((type << 5) | (rule + 64))
#else
#define L2SW_ACLRULETBADDR_CAREBIT(rule)    (rule)
#define L2SW_ACLRULETBADDR_DATABIT(rule)    ((L2SW_ACLRULENO) + (rule))
#endif
#define ACL_ACT_CVLAN_ENABLE_MASK           0x1
#define ACL_ACT_SVLAN_ENABLE_MASK           0x2
#define ACL_ACT_PRIORITY_ENABLE_MASK        0x4
#define ACL_ACT_POLICING_ENABLE_MASK        0x8
#define ACL_ACT_FWD_ENABLE_MASK             0x10
#define ACL_ACT_INTGPIO_ENABLE_MASK         0x20

#define L2SW_ACLRULETAGBITS             5

#define L2SW_ACLRANGENO                 16

#define L2SW_ACLRANGEMAX                (L2SW_ACLRANGENO-1)

#define L2SW_ACL_PORTRANGEMAX           (0xFFFF)
#define L2SW_ACL_ACT_TABLE_LEN          (4)

enum ACLTCAMTYPES
{
    CAREBITS= 0,
    DATABITS
};

typedef enum aclFwdAct
{
    L2SW_ACL_FWD_MIRROR = 0,
    L2SW_ACL_FWD_REDIRECT,
    L2SW_ACL_FWD_MIRRORFUNTION,
    L2SW_ACL_FWD_TRAP,
} l2sw_aclFwd_t;

enum ACLFIELDTYPES
{
    ACL_UNUSED,
    ACL_DMAC0,
    ACL_DMAC1,
    ACL_DMAC2,
    ACL_SMAC0,
    ACL_SMAC1,
    ACL_SMAC2,
    ACL_ETHERTYPE,
    ACL_STAG,
    ACL_CTAG,
    ACL_IP4SIP0 = 0x10,
    ACL_IP4SIP1,
    ACL_IP4DIP0,
    ACL_IP4DIP1,
    ACL_IP6SIP0WITHIPV4 = 0x20,
    ACL_IP6SIP1WITHIPV4,
    ACL_IP6DIP0WITHIPV4 = 0x28,
    ACL_IP6DIP1WITHIPV4,
    ACL_VIDRANGE = 0x30,
    ACL_IPRANGE,
    ACL_PORTRANGE,
    ACL_FIELD_VALID,
    ACL_FIELD_SELECT00 = 0x40,
    ACL_FIELD_SELECT01,
    ACL_FIELD_SELECT02,
    ACL_FIELD_SELECT03,
    ACL_FIELD_SELECT04,
    ACL_FIELD_SELECT05,
    ACL_FIELD_SELECT06,
    ACL_FIELD_SELECT07,
    ACL_FIELD_SELECT08,
    ACL_FIELD_SELECT09,
    ACL_FIELD_SELECT10,
    ACL_FIELD_SELECT11,
    ACL_FIELD_SELECT12,
    ACL_FIELD_SELECT13,
    ACL_FIELD_SELECT14,
    ACL_FIELD_SELECT15,
    ACL_TCPSPORT = 0x80,
    ACL_TCPDPORT,
    ACL_TCPFLAG,
    ACL_UDPSPORT,
    ACL_UDPDPORT,
    ACL_ICMPCODETYPE,
    ACL_IGMPTYPE,
    ACL_SPORT,
    ACL_DPORT,
    ACL_IP4TOSPROTO,
    ACL_IP4FLAGOFF,
    ACL_TCNH,
    ACL_CPUTAG,
    ACL_L2PAYLOAD,
    ACL_IP6SIP0,
    ACL_IP6SIP1,
    ACL_IP6SIP2,
    ACL_IP6SIP3,
    ACL_IP6SIP4,
    ACL_IP6SIP5,
    ACL_IP6SIP6,
    ACL_IP6SIP7,
    ACL_IP6DIP0,
    ACL_IP6DIP1,
    ACL_IP6DIP2,
    ACL_IP6DIP3,
    ACL_IP6DIP4,
    ACL_IP6DIP5,
    ACL_IP6DIP6,
    ACL_IP6DIP7,
    ACL_TYPE_END
};

struct acl_rule_smi_st{
    l2sw_uint16 rule_info;
    l2sw_uint16 field[L2SW_ACLRULEFIELDNO];
};

struct acl_rule_smi_ext_st{
    l2sw_uint16 rule_info;
};

typedef struct ACLRULESMI{
    struct acl_rule_smi_st  care_bits;
    l2sw_uint16      valid:1;
    struct acl_rule_smi_st  data_bits;

    struct acl_rule_smi_ext_st care_bits_ext;
    struct acl_rule_smi_ext_st data_bits_ext;
}l2sw_aclrulesmi;

/* Shell层读写表地址赋值无法用位段，这里暂且注释 */
struct acl_rule_st{
    l2sw_uint16 active_portmsk;  //:11;
    l2sw_uint16 type;  //:3;
    l2sw_uint16 tag_exist;  //:5;
    l2sw_uint16 field[L2SW_ACLRULEFIELDNO];
};

/* Shell层读写表地址赋值无法用位段，这里暂且注释 */
typedef struct ACLRULE{
    struct acl_rule_st  data_bits;
    l2sw_uint16      valid;  //:1;
    struct acl_rule_st  care_bits;
}l2sw_aclrule;


typedef struct l2sw_acltemplate_s{
    l2sw_uint8 field[8];
}l2sw_acltemplate_t;


/* Shell层读写表地址赋值无法用位段，这里暂且注释 */
typedef struct acl_act_s{
    l2sw_uint16 cvidx_cact;  //:7;
    l2sw_uint16 cact;  //:2;
    l2sw_uint16 svidx_sact;  //:7;
    l2sw_uint16 sact;  //:2;

    l2sw_uint16 aclmeteridx;  //:7;
    l2sw_uint16 fwdpmask;  //:11;
    l2sw_uint16 fwdact;  //:2;

    l2sw_uint16 pridx;  //:7;
    l2sw_uint16 priact;  //:2;
    l2sw_uint16 gpio_pin;  //:4;
    l2sw_uint16 gpio_en;  //:1;
    l2sw_uint16 aclint;  //:1;

    l2sw_uint16 cact_ext;  //:2;
    l2sw_uint16 fwdact_ext;  //:1;
    l2sw_uint16 tag_fmt;  //:2;

    l2sw_uint16 aclcounteridx;  //:5;	
}l2sw_acl_act_t;

typedef struct acl_rule_union_s
{
    l2sw_aclrule aclRule;
    l2sw_acl_act_t aclAct;
    l2sw_uint32 aclActCtrl;
    l2sw_uint32 aclNot;
}l2sw_acl_rule_union_t;


extern ret_t l2sw_setAsicAcl(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicAcl(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicAclUnmatchedPermit(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicAclUnmatchedPermit(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicAclRule(l2sw_uint8 units, l2sw_uint32 index, l2sw_aclrule *pAclRule);
extern ret_t l2sw_getAsicAclRule(l2sw_uint8 units, l2sw_uint32 index, l2sw_aclrule *pAclRule);
extern ret_t l2sw_setAsicAclNot(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 not);
extern ret_t l2sw_getAsicAclNot(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pNot);
extern ret_t l2sw_setAsicAclTemplate(l2sw_uint8 units, l2sw_uint32 index, l2sw_acltemplate_t* pAclType);
extern ret_t l2sw_getAsicAclTemplate(l2sw_uint8 units, l2sw_uint32 index, l2sw_acltemplate_t *pAclType);
extern ret_t l2sw_setAsicAclAct(l2sw_uint8 units, l2sw_uint32 index, l2sw_acl_act_t* pAclAct);
extern ret_t l2sw_getAsicAclAct(l2sw_uint8 units, l2sw_uint32 index, l2sw_acl_act_t *pAclAct);
extern ret_t l2sw_setAsicAclActCtrl(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 aclActCtrl);
extern ret_t l2sw_getAsicAclActCtrl(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *aclActCtrl);
extern ret_t l2sw_setAsicAclPortRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type, l2sw_uint32 upperPort, l2sw_uint32 lowerPort);
extern ret_t l2sw_getAsicAclPortRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType, l2sw_uint32* pUpperPort, l2sw_uint32* pLowerPort);
extern ret_t l2sw_setAsicAclVidRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type, l2sw_uint32 upperVid, l2sw_uint32 lowerVid);
extern ret_t l2sw_getAsicAclVidRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType, l2sw_uint32* pUpperVid, l2sw_uint32* pLowerVid);
extern ret_t l2sw_setAsicAclIpRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type, ipaddr_t upperIp, ipaddr_t lowerIp);
extern ret_t l2sw_getAsicAclIpRange(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType, ipaddr_t* pUpperIp, ipaddr_t* pLowerIp);
extern ret_t l2sw_setAsicAclGpioPolarity(l2sw_uint8 units, l2sw_uint32 polarity);
extern ret_t l2sw_getAsicAclGpioPolarity(l2sw_uint8 units, l2sw_uint32* pPolarity);

#endif /*_L2SW_ASICDRV_ACL_H_*/


