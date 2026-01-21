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
#include "acl.h"
#include "vlan.h"
#include "svlan.h"
#include "rate.h"
#ifdef L2SW_LINUX_KERNEL
#include "linux/kernel.h"
#endif
#include "sal_string.h"
#include "l2sw_asicdrv_acl.h"
#include "l2sw_asicdrv_hsb.h"
#include "l2sw_asicdrv_vlan.h"
#include "l2sw_asicdrv_svlan.h"
#include "l2sw_asicdrv_cputag.h"
#include "l2sw_asicdrv_mib.h"

CONST_T l2sw_uint8 filter_templateField[L2SW_ACLTEMPLATENO][L2SW_ACLRULEFIELDNO] = {
    {ACL_DMAC0,             ACL_DMAC1,          ACL_DMAC2,          ACL_SMAC0,          ACL_SMAC1,          ACL_SMAC2,          ACL_ETHERTYPE,      ACL_FIELD_SELECT15},
    {ACL_IP4SIP0,           ACL_IP4SIP1,        ACL_IP4DIP0,        ACL_IP4DIP1,        ACL_FIELD_SELECT13, ACL_FIELD_SELECT14, ACL_FIELD_SELECT02, ACL_FIELD_SELECT15},
    {ACL_IP6SIP0WITHIPV4,   ACL_IP6SIP1WITHIPV4,ACL_FIELD_SELECT03, ACL_FIELD_SELECT04, ACL_FIELD_SELECT05, ACL_FIELD_SELECT06, ACL_FIELD_SELECT07, ACL_FIELD_SELECT08},
    {ACL_IP6DIP0WITHIPV4,   ACL_IP6DIP1WITHIPV4,ACL_FIELD_SELECT09, ACL_FIELD_SELECT10, ACL_FIELD_SELECT11, ACL_FIELD_SELECT12, ACL_FIELD_SELECT13, ACL_FIELD_SELECT14},
    {ACL_VIDRANGE,          ACL_IPRANGE,        ACL_PORTRANGE,      ACL_CTAG,           ACL_STAG,           ACL_FIELD_SELECT13, ACL_FIELD_SELECT14, ACL_FIELD_SELECT15}
};

CONST_T l2sw_uint8 filter_advanceCaretagField_template[L2SW_ACLTEMPLATENO][2] = {
        {L2SW_TRUE,      7},
        {L2SW_TRUE,      7},
        {L2SW_FALSE,     0},
        {L2SW_FALSE,     0},
        {L2SW_TRUE,      7},
};


l2sw_uint8 filter_advanceCaretagField[L2SW_MAX_NUM_OF_UNIT][L2SW_ACLTEMPLATENO][2] = {0};


CONST_T l2sw_uint8 filter_fieldTemplateIndex[FILTER_FIELD_END][L2SW_FILTER_FIELD_USED_MAX] = {
    {0x00, 0x01,0x02},
    {0x03, 0x04,0x05},
    {0x06},
    {0x43},
    {0x44},
    {0x10, 0x11},
    {0x12, 0x13},
    {0x24},
    {0x25},
    {0x35},
    {0x35},
    {0x20, 0x21,0x22,0x23},
    {0x30, 0x31,0x32,0x33},
    {0x26},
    {0x27},
    {0x14},
    {0x15},
    {0x16},
    {0x14},
    {0x15},
    {0x14},
    {0x14},
    {0x14},

    {0x40},
    {0x41},
    {0x42},

    {0x14},
    {0x15},
    {0x16},
    {0x22},
    {0x23},
    {0x24},
    {0x25},
    {0x26},
    {0x27},
    {0x32},
    {0x33},
    {0x34},
    {0x35},
    {0x36},
    {0x37},
    {0x47},

    {0xFF} /* Pattern Match */
};

CONST_T l2sw_uint8 filter_fieldSize[FILTER_FIELD_END] = {
    3, 3, 1, 1, 1,
    2, 2, 1, 1, 1, 1, 4, 4, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    8
};

CONST_T l2sw_uint16 field_selector[L2SW_FIELDSEL_FORMAT_NUMBER][2] =
{
    {FIELDSEL_FORMAT_DEFAULT, 0},    /* Field Selector 0 */
    {FIELDSEL_FORMAT_DEFAULT, 0},    /* Field Selector 1 */
    {FIELDSEL_FORMAT_IPPAYLOAD, 12}, /* Field Selector 2 */
    {FIELDSEL_FORMAT_IPV6, 10},      /* Field Selector 3 */
    {FIELDSEL_FORMAT_IPV6, 8},       /* Field Selector 4 */
    {FIELDSEL_FORMAT_IPV4, 0},       /* Field Selector 5 */
    {FIELDSEL_FORMAT_IPV4, 8},       /* Field Selector 6 */
    {FIELDSEL_FORMAT_IPV6, 0},       /* Field Selector 7 */
    {FIELDSEL_FORMAT_IPV6, 6},       /* Field Selector 8 */
    {FIELDSEL_FORMAT_IPV6, 26},      /* Field Selector 9 */
    {FIELDSEL_FORMAT_IPV6, 24},      /* Field Selector 10 */
    {FIELDSEL_FORMAT_DEFAULT, 0},    /* Field Selector 11 */
    {FIELDSEL_FORMAT_IPV4, 6},       /* Field Selector 12 */
    {FIELDSEL_FORMAT_IPPAYLOAD, 0},  /* Field Selector 13 */
    {FIELDSEL_FORMAT_IPPAYLOAD, 2},  /* Field Selector 14 */
    {FIELDSEL_FORMAT_DEFAULT, 0}     /* Field Selector 15 */
};

static l2sw_api_ret_t _l2sw_filter_igrAcl_cfg_delAll(l2sw_uint8 units);

static void _l2sw_acl_global_init_resource(void)
{
    l2sw_uint32 index = 0; 
    
    for(index = 0; index < L2SW_MAX_NUM_OF_UNIT; index++)
    {
        MEMCPY(filter_advanceCaretagField[index], filter_advanceCaretagField_template, sizeof(filter_advanceCaretagField_template));
	}
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_init(l2sw_uint8 units)
{
    l2sw_acltemplate_t       aclTemp;
    l2sw_uint32                 i, j;
    l2sw_api_ret_t          ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    _l2sw_acl_global_init_resource();

    if ((ret = _l2sw_filter_igrAcl_cfg_delAll(units)) != L2SW_ERR_OK)
        return ret;

    for(i = 0; i < L2SW_ACLTEMPLATENO; i++)
    {
        for(j = 0; j < L2SW_ACLRULEFIELDNO;j++)
            aclTemp.field[j] = filter_templateField[i][j];

        if ((ret = l2sw_setAsicAclTemplate(units, i, &aclTemp)) != L2SW_ERR_OK)
            return ret;
    }

    for(i = 0; i < L2SW_FIELDSEL_FORMAT_NUMBER; i++)
    {
        if ((ret = l2sw_setAsicFieldSelector(units, i, field_selector[i][0], field_selector[i][1])) != L2SW_ERR_OK)
            return ret;
    }

    L2SW_SCAN_ALL_PHY_PORTMASK(units, i)
    {
        if ((ret = l2sw_setAsicAcl(units, i, L2SW_TRUE)) != L2SW_ERR_OK)
            return ret;

        if ((ret = l2sw_setAsicAclUnmatchedPermit(units, i, L2SW_TRUE)) != L2SW_ERR_OK)
            return ret;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_field_add(l2sw_uint8 units, l2sw_filter_cfg_t* pFilter_cfg, l2sw_filter_field_t* pFilter_field)
{
    l2sw_uint32 i;
    l2sw_filter_field_t *tailPtr;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pFilter_cfg || NULL == pFilter_field)
        return L2SW_ERR_NULL_POINTER;

    if(pFilter_field->fieldType >= FILTER_FIELD_END)
        return L2SW_ERR_ENTRY_INDEX;


    if(0 == pFilter_field->fieldTemplateNo)
    {
        pFilter_field->fieldTemplateNo = filter_fieldSize[pFilter_field->fieldType];

        for(i = 0; i < pFilter_field->fieldTemplateNo; i++)
        {
            pFilter_field->fieldTemplateIdx[i] = filter_fieldTemplateIndex[pFilter_field->fieldType][i];
        }
    }

    if(NULL == pFilter_cfg->fieldHead)
    {
        pFilter_cfg->fieldHead = pFilter_field;
    }
    else
    {
        if (pFilter_cfg->fieldHead->next == NULL)
        {
            pFilter_cfg->fieldHead->next = pFilter_field;
        }
        else
        {
            tailPtr = pFilter_cfg->fieldHead->next;
            while( tailPtr->next != NULL)
            {
                tailPtr = tailPtr->next;
            }
            tailPtr->next = pFilter_field;
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_writeDataField(l2sw_uint8 units, l2sw_aclrule *aclRule, l2sw_filter_field_t *fieldPtr)
{
    l2sw_uint32 i, tempIdx,fieldIdx, ipValue, ipMask;
    l2sw_uint32 ip6addr[L2SW_IPV6_ADDR_WORD_LENGTH];
    l2sw_uint32 ip6mask[L2SW_IPV6_ADDR_WORD_LENGTH];

    for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
    {
        tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;

        aclRule[tempIdx].valid = L2SW_TRUE;
    }

    switch (fieldPtr->fieldType)
    {
    /* use DMAC structure as representative for mac structure */
    case FILTER_FIELD_DMAC:
    case FILTER_FIELD_SMAC:

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.mac.value.octet[5 - i*2] | (fieldPtr->filter_pattern_union.mac.value.octet[5 - (i*2 + 1)] << 8);
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.mac.mask.octet[5 - i*2] | (fieldPtr->filter_pattern_union.mac.mask.octet[5 - (i*2 + 1)] << 8);
        }
        break;
    case FILTER_FIELD_ETHERTYPE:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.etherType.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.etherType.mask;
        }
        break;
    case FILTER_FIELD_IPV4_SIP:
    case FILTER_FIELD_IPV4_DIP:

        ipValue = fieldPtr->filter_pattern_union.sip.value;
        ipMask = fieldPtr->filter_pattern_union.sip.mask;

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = (0xFFFF & (ipValue >> (i*16)));
            aclRule[tempIdx].care_bits.field[fieldIdx] = (0xFFFF & (ipMask >> (i*16)));
        }
        break;
    case FILTER_FIELD_IPV4_TOS:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.ipTos.value & 0xFF;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.ipTos.mask  & 0xFF;
        }
        break;
    case FILTER_FIELD_IPV4_PROTOCOL:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.protocol.value & 0xFF;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.protocol.mask  & 0xFF;
        }
        break;
    case FILTER_FIELD_IPV6_SIPV6:
    case FILTER_FIELD_IPV6_DIPV6:
        for(i = 0; i < L2SW_IPV6_ADDR_WORD_LENGTH; i++)
        {
            ip6addr[i] = fieldPtr->filter_pattern_union.sipv6.value.addr[i];
            ip6mask[i] = fieldPtr->filter_pattern_union.sipv6.mask.addr[i];
        }

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            if(i < 2)
            {
                aclRule[tempIdx].data_bits.field[fieldIdx] = ((ip6addr[0] & (0xFFFF << (i * 16))) >> (i * 16));
                aclRule[tempIdx].care_bits.field[fieldIdx] = ((ip6mask[0] & (0xFFFF << (i * 16))) >> (i * 16));
            }
            else
            {
                /*default acl template for ipv6 address supports MSB 32-bits and LSB 32-bits only*/
                aclRule[tempIdx].data_bits.field[fieldIdx] = ((ip6addr[3] & (0xFFFF << ((i&1) * 16))) >> ((i&1) * 16));
                aclRule[tempIdx].care_bits.field[fieldIdx] = ((ip6mask[3] & (0xFFFF << ((i&1) * 16))) >> ((i&1) * 16));
            }
        }

        break;
    case FILTER_FIELD_CTAG:
    case FILTER_FIELD_STAG:

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = (fieldPtr->filter_pattern_union.l2tag.pri.value << 13) | (fieldPtr->filter_pattern_union.l2tag.cfi.value << 12) | fieldPtr->filter_pattern_union.l2tag.vid.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] = (fieldPtr->filter_pattern_union.l2tag.pri.mask << 13) | (fieldPtr->filter_pattern_union.l2tag.cfi.mask << 12) | fieldPtr->filter_pattern_union.l2tag.vid.mask;
        }
        break;
    case FILTER_FIELD_IPV4_FLAG:

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] &= 0x1FFF;
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.ipFlag.xf.value << 15);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.ipFlag.df.value << 14);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.ipFlag.mf.value << 13);

            aclRule[tempIdx].care_bits.field[fieldIdx] &= 0x1FFF;
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.ipFlag.xf.mask << 15);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.ipFlag.df.mask << 14);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.ipFlag.mf.mask << 13);
        }

        break;
    case FILTER_FIELD_IPV4_OFFSET:

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] &= 0xE000;
            aclRule[tempIdx].data_bits.field[fieldIdx] |= fieldPtr->filter_pattern_union.inData.value;

            aclRule[tempIdx].care_bits.field[fieldIdx] &= 0xE000;
            aclRule[tempIdx].care_bits.field[fieldIdx] |= fieldPtr->filter_pattern_union.inData.mask;
        }

        break;

    case FILTER_FIELD_IPV6_TRAFFIC_CLASS:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;


            aclRule[tempIdx].data_bits.field[fieldIdx] = (fieldPtr->filter_pattern_union.inData.value << 4)&0x0FF0;
            aclRule[tempIdx].care_bits.field[fieldIdx] = (fieldPtr->filter_pattern_union.inData.mask << 4)&0x0FF0;
        }
        break;
    case FILTER_FIELD_IPV6_NEXT_HEADER:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.inData.value << 8;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.inData.mask << 8;
        }
        break;
    case FILTER_FIELD_TCP_SPORT:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.tcpSrcPort.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.tcpSrcPort.mask;
        }
        break;
    case FILTER_FIELD_TCP_DPORT:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.tcpDstPort.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.tcpDstPort.mask;
        }
        break;
    case FILTER_FIELD_TCP_FLAG:

        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.cwr.value << 7);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.ece.value << 6);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.urg.value << 5);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.ack.value << 4);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.psh.value << 3);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.rst.value << 2);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.syn.value << 1);
            aclRule[tempIdx].data_bits.field[fieldIdx] |= fieldPtr->filter_pattern_union.tcpFlag.fin.value;

            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.cwr.mask << 7);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.ece.mask << 6);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.urg.mask << 5);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.ack.mask << 4);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.psh.mask << 3);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.rst.mask << 2);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.tcpFlag.syn.mask << 1);
            aclRule[tempIdx].care_bits.field[fieldIdx] |= fieldPtr->filter_pattern_union.tcpFlag.fin.mask;
        }
        break;
    case FILTER_FIELD_UDP_SPORT:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.udpSrcPort.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.udpSrcPort.mask;
        }
        break;
    case FILTER_FIELD_UDP_DPORT:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.udpDstPort.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.udpDstPort.mask;
        }
        break;
    case FILTER_FIELD_ICMP_CODE:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] &= 0xFF00;
            aclRule[tempIdx].data_bits.field[fieldIdx] |= fieldPtr->filter_pattern_union.icmpCode.value;
            aclRule[tempIdx].care_bits.field[fieldIdx] &= 0xFF00;
            aclRule[tempIdx].care_bits.field[fieldIdx] |= fieldPtr->filter_pattern_union.icmpCode.mask;
        }
        break;
    case FILTER_FIELD_ICMP_TYPE:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] &= 0x00FF;
            aclRule[tempIdx].data_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.icmpType.value << 8);
            aclRule[tempIdx].care_bits.field[fieldIdx] &= 0x00FF;
            aclRule[tempIdx].care_bits.field[fieldIdx] |= (fieldPtr->filter_pattern_union.icmpType.mask << 8);
        }
        break;
    case FILTER_FIELD_IGMP_TYPE:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = (fieldPtr->filter_pattern_union.igmpType.value << 8);
            aclRule[tempIdx].care_bits.field[fieldIdx] = (fieldPtr->filter_pattern_union.igmpType.mask << 8);
        }
        break;
    case FILTER_FIELD_PATTERN_MATCH:
        for(i = 0; i < fieldPtr->fieldTemplateNo; i++)
        {
            tempIdx = (fieldPtr->fieldTemplateIdx[i] & 0xF0) >> 4;
            fieldIdx = fieldPtr->fieldTemplateIdx[i] & 0x0F;

            aclRule[tempIdx].data_bits.field[fieldIdx] = ((fieldPtr->filter_pattern_union.pattern.value[i/2] >> (16 * (i%2))) & 0x0000FFFF );
            aclRule[tempIdx].care_bits.field[fieldIdx] = ((fieldPtr->filter_pattern_union.pattern.mask[i/2] >> (16 * (i%2))) & 0x0000FFFF );
        }
        break;
    case FILTER_FIELD_VID_RANGE:
    case FILTER_FIELD_IP_RANGE:
    case FILTER_FIELD_PORT_RANGE:
    default:
        tempIdx = (fieldPtr->fieldTemplateIdx[0] & 0xF0) >> 4;
        fieldIdx = fieldPtr->fieldTemplateIdx[0] & 0x0F;

        aclRule[tempIdx].data_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.inData.value;
        aclRule[tempIdx].care_bits.field[fieldIdx] = fieldPtr->filter_pattern_union.inData.mask;
        break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_writeAction(l2sw_uint8 units, l2sw_filter_action_t *pAction, l2sw_acl_act_t *aclAct, l2sw_uint32 *pAclActCtrl)
{
	 l2sw_uint32 			 vidx, svidx, cpuPort,aclActCtrl;
	 l2sw_uint32 			 portmask;
	 l2sw_api_ret_t			 retVal; 
	 l2sw_uint32				actType;

	 /*action*/
    MEMSET(aclAct, 0, sizeof(l2sw_acl_act_t));
    aclActCtrl = 0;
    for(actType = 0; actType < FILTER_ENACT_END; actType ++)
    {
        if(pAction->actEnable[actType])
        {
            switch (actType)
            {
            case FILTER_ENACT_CVLAN_INGRESS:
                if(pAction->filterCvlanVid > L2SW_EVIDMAX)
                    return L2SW_ERR_INPUT;

                if((retVal = l2sw_vlan_checkAndCreateMbr(units, pAction->filterCvlanVid, &vidx)) != L2SW_ERR_OK)
                {
                    return retVal;
                }
				
                aclAct->cact = (l2sw_uint16)FILTER_ENACT_CVLAN_TYPE(actType);
                aclAct->cvidx_cact = vidx;

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_TAGONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_VLANONLY;
                }

                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
            case FILTER_ENACT_CVLAN_EGRESS:
                if(pAction->filterCvlanVid > L2SW_EVIDMAX)
                    return L2SW_ERR_INPUT;

                if((retVal = l2sw_vlan_checkAndCreateMbr(units, pAction->filterCvlanVid, &vidx)) != L2SW_ERR_OK)
                    return retVal;

                aclAct->cact = FILTER_ENACT_CVLAN_TYPE(actType);
                aclAct->cvidx_cact = vidx;

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_TAGONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_VLANONLY;
                }

                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
             case FILTER_ENACT_CVLAN_SVID:

                aclAct->cact = FILTER_ENACT_CVLAN_TYPE(actType);

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_TAGONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_VLANONLY;
                }

                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
             case FILTER_ENACT_POLICING_1:
                if(pAction->filterPolicingIdx[1] >= (L2SW_METER_NUM + L2SW_MAX_LOG_CNT_NUM))
                    return L2SW_ERR_INPUT;

                aclAct->cact = FILTER_ENACT_CVLAN_TYPE(actType);
                aclAct->cvidx_cact = pAction->filterPolicingIdx[1];

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_TAGONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_VLANONLY;
                }

                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;

            case FILTER_ENACT_SVLAN_INGRESS:
            case FILTER_ENACT_SVLAN_EGRESS:

                if((retVal = l2sw_svlan_checkAndCreateMbr(units, pAction->filterSvlanVid, &svidx)) != L2SW_ERR_OK)
                    return retVal;

                aclAct->sact = FILTER_ENACT_SVLAN_TYPE(actType);
                aclAct->svidx_sact = svidx;
                aclActCtrl |= FILTER_ENACT_SVLAN_MASK;
                break;
            case FILTER_ENACT_SVLAN_CVID:

                aclAct->sact = FILTER_ENACT_SVLAN_TYPE(actType);
                aclActCtrl |= FILTER_ENACT_SVLAN_MASK;
                break;
            case FILTER_ENACT_POLICING_2:
                if(pAction->filterPolicingIdx[2] >= (L2SW_METER_NUM + L2SW_MAX_LOG_CNT_NUM))
                    return L2SW_ERR_INPUT;

                aclAct->sact = FILTER_ENACT_SVLAN_TYPE(actType);
                aclAct->svidx_sact = pAction->filterPolicingIdx[2];
                aclActCtrl |= FILTER_ENACT_SVLAN_MASK;
                break;
            case FILTER_ENACT_POLICING_0:
                if(pAction->filterPolicingIdx[0] >= (L2SW_METER_NUM + L2SW_MAX_LOG_CNT_NUM))
                    return L2SW_ERR_INPUT;

                aclAct->aclmeteridx = pAction->filterPolicingIdx[0];
                aclActCtrl |= FILTER_ENACT_POLICING_MASK;
                break;
            case FILTER_ENACT_PRIORITY:
            case FILTER_ENACT_1P_REMARK:
                if(pAction->filterPriority > L2SW_PRIMAX)
                    return L2SW_ERR_INPUT;

                aclAct->priact = FILTER_ENACT_PRI_TYPE(actType);
                aclAct->pridx = pAction->filterPriority;
                aclActCtrl |= FILTER_ENACT_PRIORITY_MASK;
                break;
            case FILTER_ENACT_DSCP_REMARK:
                if(pAction->filterPriority > L2SW_DSCPMAX)
                    return L2SW_ERR_INPUT;

                aclAct->priact = FILTER_ENACT_PRI_TYPE(actType);
                aclAct->pridx = pAction->filterPriority;
                aclActCtrl |= FILTER_ENACT_PRIORITY_MASK;
                break;
            case FILTER_ENACT_POLICING_3:
                if(pAction->filterPriority >= (L2SW_METER_NUM + L2SW_MAX_LOG_CNT_NUM))
                    return L2SW_ERR_INPUT;

                aclAct->priact = FILTER_ENACT_PRI_TYPE(actType);
                aclAct->pridx = pAction->filterPolicingIdx[3];
                aclActCtrl |= FILTER_ENACT_PRIORITY_MASK;
                break;
            case FILTER_ENACT_DROP:

                aclAct->fwdact = FILTER_ENACT_FWD_TYPE(FILTER_ENACT_REDIRECT);
                aclAct->fwdact_ext = L2SW_FALSE;

                aclAct->fwdpmask = 0;
                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;
            case FILTER_ENACT_REDIRECT:
                L2SW_CHK_PORTMASK_VALID(units, &pAction->filterPortmask);

                aclAct->fwdact = FILTER_ENACT_FWD_TYPE(actType);
                aclAct->fwdact_ext = L2SW_FALSE;

                if(l2sw_switch_portmask_L2P_get(units, &pAction->filterPortmask, &portmask) != L2SW_ERR_OK)
                    return L2SW_ERR_PORT_MASK;
                aclAct->fwdpmask = portmask;

                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;

            case FILTER_ENACT_ADD_DSTPORT:
                L2SW_CHK_PORTMASK_VALID(units, &pAction->filterPortmask);

                aclAct->fwdact = FILTER_ENACT_FWD_TYPE(actType);
                aclAct->fwdact_ext = L2SW_FALSE;

                if(l2sw_switch_portmask_L2P_get(units, &pAction->filterPortmask, &portmask) != L2SW_ERR_OK)
                    return L2SW_ERR_PORT_MASK;
                aclAct->fwdpmask = portmask;

                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;
            case FILTER_ENACT_MIRROR:
                L2SW_CHK_PORTMASK_VALID(units, &pAction->filterPortmask);

                aclAct->fwdact = FILTER_ENACT_FWD_TYPE(actType);
                aclAct->cact_ext = L2SW_FALSE;

                if(l2sw_switch_portmask_L2P_get(units, &pAction->filterPortmask, &portmask) != L2SW_ERR_OK)
                    return L2SW_ERR_PORT_MASK;
                aclAct->fwdpmask = portmask;

                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;
            case FILTER_ENACT_TRAP_CPU:

                aclAct->fwdact = FILTER_ENACT_FWD_TYPE(actType);
                aclAct->fwdact_ext = L2SW_FALSE;

                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;
            case FILTER_ENACT_COPY_CPU:
                if((retVal = l2sw_getAsicCputagTrapPort(units, &cpuPort)) != L2SW_ERR_OK)
                    return retVal;

                aclAct->fwdact = FILTER_ENACT_FWD_TYPE(FILTER_ENACT_MIRROR);
                aclAct->fwdact_ext = L2SW_FALSE;

                aclAct->fwdpmask = 1 << cpuPort;
                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;
            case FILTER_ENACT_ISOLATION:
                L2SW_CHK_PORTMASK_VALID(units, &pAction->filterPortmask);

                aclAct->fwdact_ext = L2SW_TRUE;

                if(l2sw_switch_portmask_L2P_get(units, &pAction->filterPortmask, &portmask) != L2SW_ERR_OK)
                    return L2SW_ERR_PORT_MASK;
                aclAct->fwdpmask = portmask;

                aclActCtrl |= FILTER_ENACT_FWD_MASK;
                break;

            case FILTER_ENACT_INTERRUPT:

                aclAct->aclint = L2SW_TRUE;
                aclActCtrl |= FILTER_ENACT_INTGPIO_MASK;
                break;
            case FILTER_ENACT_GPO:

                aclAct->gpio_en = L2SW_TRUE;
                aclAct->gpio_pin = pAction->filterPin;
                aclActCtrl |= FILTER_ENACT_INTGPIO_MASK;
                break;
             case FILTER_ENACT_EGRESSCTAG_TAG:

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_VLANONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_TAGONLY;
                }
                aclAct->tag_fmt = FILTER_CTAGFMT_TAG;
                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
             case FILTER_ENACT_EGRESSCTAG_UNTAG:

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_VLANONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_TAGONLY;
                }
                aclAct->tag_fmt = FILTER_CTAGFMT_UNTAG;
                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
             case FILTER_ENACT_EGRESSCTAG_KEEP:

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_VLANONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_TAGONLY;
                }
                aclAct->tag_fmt = FILTER_CTAGFMT_KEEP;
                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
             case FILTER_ENACT_EGRESSCTAG_KEEPAND1PRMK:

                if(aclActCtrl &(FILTER_ENACT_CVLAN_MASK))
                {
                    if(aclAct->cact_ext == FILTER_ENACT_CACTEXT_VLANONLY)
                        aclAct->cact_ext = FILTER_ENACT_CACTEXT_BOTHVLANTAG;
                }
                else
                {
                    aclAct->cact_ext = FILTER_ENACT_CACTEXT_TAGONLY;
                }
                aclAct->tag_fmt = FILTER_CTAGFMT_KEEP1PRMK;
                aclActCtrl |= FILTER_ENACT_CVLAN_MASK;
                break;
			 case FILTER_ENACT_COUNTER:
				 if(pAction->filterCounterIdx >= (L2SW_MAX_LOG_CNT_NUM))
					 return L2SW_ERR_INPUT;
				 
				 aclAct->aclcounteridx = pAction->filterCounterIdx;
				 aclActCtrl |= FILTER_ENACT_COUNTER_MASK;
				 break;
			 case FILTER_ENACT_NOACTION:			 	
				 /* For an acl rule, only the first entry will be written with input action control, aclActCtrl of other rules will be zero 
			 	if set to noaction, means that this entry is not the first rule of the rule.*/
				 aclActCtrl = 0;
				 MEMSET(aclAct, 0, sizeof(l2sw_acl_act_t));
		  		 break;

            default:
                return L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT;
            }
        }
    }

    *pAclActCtrl = aclActCtrl;

	return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_cfg_add(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_filter_cfg_t* pFilter_cfg, l2sw_filter_action_t* pFilter_action, l2sw_filter_number_t *ruleNum)
{
    l2sw_api_ret_t               retVal;
    l2sw_uint32                  careTagData, careTagMask;
    l2sw_uint32                  i, ruleId;
    l2sw_uint32                  aclActCtrl;
    l2sw_filter_field_t*         fieldPtr;
    l2sw_aclrule            aclRule[L2SW_ACLTEMPLATENO];
    l2sw_aclrule            tempRule;
    l2sw_acl_act_t          aclAct;
    l2sw_uint32                  noRulesAdd;
    l2sw_uint32                  portmask;
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(filter_id > L2SW_ACLRULEMAX )
        return L2SW_ERR_ENTRY_INDEX;

    if((NULL == pFilter_cfg) || (NULL == pFilter_action) || (NULL == ruleNum))
        return L2SW_ERR_NULL_POINTER;

	/*this api not use for the action FILTER_ENACT_NOACTION*/
    if (L2SW_TRUE == pFilter_action->actEnable[FILTER_ENACT_NOACTION])
	{
		return L2SW_ERR_INPUT;
	}

    fieldPtr = pFilter_cfg->fieldHead;

    /* init RULE */
    for(i = 0; i < L2SW_ACLTEMPLATENO; i++)
    {
        MEMSET(&aclRule[i], 0, sizeof(l2sw_aclrule));

        aclRule[i].data_bits.type= i;
        aclRule[i].care_bits.type= 0x7;
    }

    while(NULL != fieldPtr)
    {
        _l2sw_filter_igrAcl_writeDataField(units, aclRule, fieldPtr);

        fieldPtr = fieldPtr->next;
    }

    /*set care tag mask in User Defined Field 15*/
    /*Follow care tag should not be used while ACL template and User defined fields are fully control by system designer*/
    /*those advanced packet type care tag is used in default template design structure only*/
    careTagData = 0;
    careTagMask = 0;

    for(i = CARE_TAG_TCP; i < CARE_TAG_END; i++)
    {
        if(pFilter_cfg->careTag.tagType[i].mask)
            careTagMask = careTagMask | (1 << (i-CARE_TAG_TCP));

        if(pFilter_cfg->careTag.tagType[i].value)
            careTagData = careTagData | (1 << (i-CARE_TAG_TCP));
    }

    if(careTagData || careTagMask)
    {
        i = 0;
        while(i < L2SW_ACLTEMPLATENO)
        {
            if(aclRule[i].valid == 1 && filter_advanceCaretagField[units][i][0] == L2SW_TRUE)
            {

                aclRule[i].data_bits.field[filter_advanceCaretagField[units][i][1]] = careTagData & 0xFFFF;
                aclRule[i].care_bits.field[filter_advanceCaretagField[units][i][1]] = careTagMask & 0xFFFF;
                break;
            }
            i++;
        }
        /*none of previous used template containing field 15*/
        if(i == L2SW_ACLTEMPLATENO)
        {
            i = 0;
            while(i < L2SW_ACLTEMPLATENO)
            {
                if(filter_advanceCaretagField[units][i][0] == L2SW_TRUE)
                {
                    aclRule[i].data_bits.field[filter_advanceCaretagField[units][i][1]] = careTagData & 0xFFFF;
                    aclRule[i].care_bits.field[filter_advanceCaretagField[units][i][1]] = careTagMask & 0xFFFF;
                    aclRule[i].valid = 1;
                    break;
                }
                i++;
            }
        }
    }

    /*Check rule number*/
    noRulesAdd = 0;
    for(i = 0; i < L2SW_ACLTEMPLATENO; i++)
    {
        if(1 == aclRule[i].valid)
        {
            noRulesAdd ++;
        }
    }

    *ruleNum = noRulesAdd;

    if((filter_id + noRulesAdd - 1) > L2SW_ACLRULEMAX)
    {
        return L2SW_ERR_ENTRY_INDEX;
    }

    /*set care tag mask in TAG Indicator*/
    careTagData = 0;
    careTagMask = 0;

    for(i = 0; i <= CARE_TAG_IPV6;i++)
    {
        if(0 == pFilter_cfg->careTag.tagType[i].mask )
        {
            careTagMask &= ~(1 << i);
        }
        else
        {
            careTagMask |= (1 << i);
            if(0 == pFilter_cfg->careTag.tagType[i].value )
                careTagData &= ~(1 << i);
            else
                careTagData |= (1 << i);
        }
    }

    for(i = 0; i < L2SW_ACLTEMPLATENO; i++)
    {
        aclRule[i].data_bits.tag_exist = (careTagData) & ACL_RULE_CARETAG_MASK;
        aclRule[i].care_bits.tag_exist = (careTagMask) & ACL_RULE_CARETAG_MASK;
    }

    L2SW_CHK_PORTMASK_VALID(units, &pFilter_cfg->activeport.value);
    L2SW_CHK_PORTMASK_VALID(units, &pFilter_cfg->activeport.mask);

    for(i = 0; i < L2SW_ACLTEMPLATENO; i++)
    {
        if(L2SW_TRUE == aclRule[i].valid)
        {
            if(l2sw_switch_portmask_L2P_get(units, &pFilter_cfg->activeport.value, &portmask) != L2SW_ERR_OK)
                return L2SW_ERR_PORT_MASK;

            aclRule[i].data_bits.active_portmsk = portmask;

            if(l2sw_switch_portmask_L2P_get(units, &pFilter_cfg->activeport.mask, &portmask) != L2SW_ERR_OK)
                return L2SW_ERR_PORT_MASK;

            aclRule[i].care_bits.active_portmsk = portmask;
        }
    }

    if(pFilter_cfg->invert >= FILTER_INVERT_END )
        return L2SW_ERR_INPUT;

    /*Last action gets high priority if actions are the same*/	
    MEMSET(&aclAct, 0, sizeof(l2sw_acl_act_t));
    aclActCtrl = 0;
	if((retVal = _l2sw_filter_igrAcl_writeAction(units, pFilter_action, &aclAct, &aclActCtrl)) != L2SW_ERR_OK)
		return retVal;	

    /*check if free ACL rules are enough*/
    for(i = filter_id; i < (filter_id + noRulesAdd); i++)
    {
        if((retVal = l2sw_getAsicAclRule(units, i, &tempRule)) != L2SW_ERR_OK )
            return retVal;
            
        if(tempRule.valid == L2SW_TRUE)
        {
            return L2SW_ERR_TBL_FULL;
        }
    }

    ruleId = 0;
    for(i = 0; i < L2SW_ACLTEMPLATENO; i++)
    {
        if(aclRule[i].valid == L2SW_TRUE)
        {
            /* write ACL action control */
            if((retVal = l2sw_setAsicAclActCtrl(units, filter_id + ruleId, aclActCtrl)) != L2SW_ERR_OK )
                return retVal;
            /* write ACL action */
            if((retVal = l2sw_setAsicAclAct(units, filter_id + ruleId, &aclAct)) != L2SW_ERR_OK )
                return retVal;

            /* write ACL not */
            if((retVal = l2sw_setAsicAclNot(units, filter_id + ruleId, pFilter_cfg->invert)) != L2SW_ERR_OK )
                return retVal;
            /* write ACL rule */
            if((retVal = l2sw_setAsicAclRule(units, filter_id + ruleId, &aclRule[i])) != L2SW_ERR_OK )
                return retVal;

            /* only the first rule will be written with input action control, aclActCtrl of other rules will be zero */
            aclActCtrl = 0;
            MEMSET(&aclAct, 0, sizeof(l2sw_acl_act_t));

            ruleId ++;
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_cfg_del(l2sw_uint8 units, l2sw_filter_id_t filter_id)
{
    l2sw_aclrule initRule;
    l2sw_acl_act_t  initAct;
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(filter_id > L2SW_ACLRULEMAX )
        return L2SW_ERR_FILTER_ENTRYIDX;

    MEMSET(&initRule, 0, sizeof(l2sw_aclrule));
    MEMSET(&initAct, 0, sizeof(l2sw_acl_act_t));

    if((ret = l2sw_setAsicAclRule(units, filter_id, &initRule)) != L2SW_ERR_OK)
        return ret;
    if((ret = l2sw_setAsicAclActCtrl(units, filter_id, FILTER_ENACT_INIT_MASK))!= L2SW_ERR_OK)
        return ret;
    if((ret = l2sw_setAsicAclAct(units, filter_id, &initAct)) != L2SW_ERR_OK)
        return ret;
    if((ret = l2sw_setAsicAclNot(units, filter_id, L2SW_DISABLED)) != L2SW_ERR_OK )
        return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_cfg_delAll(l2sw_uint8 units)
{
    l2sw_uint32            i;
    l2sw_api_ret_t     ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    for(i = 0; i < L2SW_ACLRULENO; i++)
    {
        if((ret = l2sw_setAsicAclActCtrl(units, i, FILTER_ENACT_INIT_MASK))!= L2SW_ERR_OK)
            return ret;
        if((ret = l2sw_setAsicAclNot(units, i, L2SW_DISABLED)) != L2SW_ERR_OK )
            return ret;
    }

    return l2sw_setAsicRegBit(units, L2SW_REG_ACL_RESET_CFG, L2SW_ACL_RESET_CFG_OFFSET, L2SW_TRUE);
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_cfg_get(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_filter_cfg_raw_t *pFilter_cfg, l2sw_filter_action_t *pAction)
{
    l2sw_api_ret_t               retVal;
    l2sw_uint32                  i, tmp;
    l2sw_aclrule            aclRule;
    l2sw_acl_act_t          aclAct;
    l2sw_uint32                  cpuPort;
    l2sw_acltemplate_t      type;
    l2sw_svlan_memconf_t    svlan_cfg;
    l2sw_vlanconfiguser     vlanMC;
    l2sw_uint32                  phyPmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pFilter_cfg || NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    if(filter_id > L2SW_ACLRULEMAX)
        return L2SW_ERR_ENTRY_INDEX;

    if ((retVal = l2sw_getAsicAclRule(units, filter_id, &aclRule)) != L2SW_ERR_OK)
        return retVal;

    /* Check valid */
    if(aclRule.valid == 0)
    {
        pFilter_cfg->valid = L2SW_DISABLED;
        return L2SW_ERR_OK;
    }

    phyPmask = aclRule.data_bits.active_portmsk;
    if(l2sw_switch_portmask_P2L_get(units, phyPmask,&(pFilter_cfg->activeport.value)) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    phyPmask = aclRule.care_bits.active_portmsk;
    if(l2sw_switch_portmask_P2L_get(units, phyPmask,&(pFilter_cfg->activeport.mask)) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    for(i = 0; i <= CARE_TAG_IPV6; i++)
    {
        if(aclRule.data_bits.tag_exist & (1 << i))
            pFilter_cfg->careTag.tagType[i].value = 1;
        else
            pFilter_cfg->careTag.tagType[i].value = 0;

        if (aclRule.care_bits.tag_exist & (1 << i))
            pFilter_cfg->careTag.tagType[i].mask = 1;
        else
            pFilter_cfg->careTag.tagType[i].mask = 0;
    }
#if 0
    if(filter_advanceCaretagField[aclRule.data_bits.type][0] == L2SW_TRUE)
    {
        /* Advanced Care tag setting */
        for(i = CARE_TAG_TCP; i < CARE_TAG_END; i++)
        {
            if(aclRule.data_bits.field[filter_advanceCaretagField[aclRule.data_bits.type][1]] & (0x0001 << (i-CARE_TAG_TCP)) )
                pFilter_cfg->careTag.tagType[i].value = 1;
            else
                pFilter_cfg->careTag.tagType[i].value = 0;

            if(aclRule.care_bits.field[filter_advanceCaretagField[aclRule.care_bits.type][1]] & (0x0001 << (i-CARE_TAG_TCP)) )
                pFilter_cfg->careTag.tagType[i].mask = 1;
            else
                pFilter_cfg->careTag.tagType[i].mask = 0;
        }
    }
#endif

    for(i = 0; i < L2SW_ACLRULEFIELDNO; i++)
    {
        pFilter_cfg->careFieldRaw[i] = aclRule.care_bits.field[i];
        pFilter_cfg->dataFieldRaw[i] = aclRule.data_bits.field[i];
    }

    if ((retVal = l2sw_getAsicAclNot(units, filter_id, &tmp))!= L2SW_ERR_OK)
        return retVal;

    pFilter_cfg->invert = tmp;

    pFilter_cfg->valid = aclRule.valid;

    MEMSET(pAction, 0, sizeof(l2sw_filter_action_t));

    if ((retVal = l2sw_getAsicAclActCtrl(units, filter_id, &tmp))!= L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicAclAct(units, filter_id, &aclAct)) != L2SW_ERR_OK)
        return retVal;

    if(tmp & FILTER_ENACT_FWD_MASK)
    {
        if(L2SW_TRUE == aclAct.fwdact_ext)
        {
            pAction->actEnable[FILTER_ENACT_ISOLATION] = L2SW_TRUE;

            phyPmask = aclAct.fwdpmask;
            if(l2sw_switch_portmask_P2L_get(units, phyPmask,&(pAction->filterPortmask)) != L2SW_ERR_OK)
                return L2SW_ERR_FAILED;
        }
        else if(aclAct.fwdact == L2SW_ACL_FWD_TRAP)
        {
            pAction->actEnable[FILTER_ENACT_TRAP_CPU] = L2SW_TRUE;
        }
        else if (aclAct.fwdact == L2SW_ACL_FWD_MIRRORFUNTION )
        {
            pAction->actEnable[FILTER_ENACT_MIRROR] = L2SW_TRUE;

            phyPmask = aclAct.fwdpmask;
            if(l2sw_switch_portmask_P2L_get(units, phyPmask,&(pAction->filterPortmask)) != L2SW_ERR_OK)
                return L2SW_ERR_FAILED;
        }
        else if (aclAct.fwdact == L2SW_ACL_FWD_REDIRECT)
        {
            if(aclAct.fwdpmask == 0 )
                pAction->actEnable[FILTER_ENACT_DROP] = L2SW_TRUE;
            else
            {
                pAction->actEnable[FILTER_ENACT_REDIRECT] = L2SW_TRUE;

                phyPmask = aclAct.fwdpmask;
                if(l2sw_switch_portmask_P2L_get(units, phyPmask,&(pAction->filterPortmask)) != L2SW_ERR_OK)
                    return L2SW_ERR_FAILED;
            }
        }
        else if (aclAct.fwdact == L2SW_ACL_FWD_MIRROR)
        {
            if((retVal = l2sw_getAsicCputagTrapPort(units, &cpuPort)) != L2SW_ERR_OK)
                return retVal;
            if (aclAct.fwdpmask == (1 << cpuPort))
            {
                pAction->actEnable[FILTER_ENACT_COPY_CPU] = L2SW_TRUE;
            }
            else
            {
                pAction->actEnable[FILTER_ENACT_ADD_DSTPORT] = L2SW_TRUE;

                phyPmask = aclAct.fwdpmask;
                if(l2sw_switch_portmask_P2L_get(units, phyPmask, &(pAction->filterPortmask)) != L2SW_ERR_OK)
                    return L2SW_ERR_FAILED;
            }
        }
        else
        {
            return L2SW_ERR_FAILED;
        }
    }

    if(tmp & FILTER_ENACT_POLICING_MASK)
    {
        pAction->actEnable[FILTER_ENACT_POLICING_0] = L2SW_TRUE;
        pAction->filterPolicingIdx[0] = aclAct.aclmeteridx;
    }

    if(tmp & FILTER_ENACT_PRIORITY_MASK)
    {
        if(aclAct.priact == FILTER_ENACT_PRI_TYPE(FILTER_ENACT_PRIORITY))
        {
            pAction->actEnable[FILTER_ENACT_PRIORITY] = L2SW_TRUE;
            pAction->filterPriority = aclAct.pridx;
        }
        else if(aclAct.priact == FILTER_ENACT_PRI_TYPE(FILTER_ENACT_1P_REMARK))
        {
            pAction->actEnable[FILTER_ENACT_1P_REMARK] = L2SW_TRUE;
            pAction->filterPriority = aclAct.pridx;
        }
        else if(aclAct.priact == FILTER_ENACT_PRI_TYPE(FILTER_ENACT_DSCP_REMARK))
        {
            pAction->actEnable[FILTER_ENACT_DSCP_REMARK] = L2SW_TRUE;
            pAction->filterPriority = aclAct.pridx;
        }
        else if(aclAct.priact == FILTER_ENACT_PRI_TYPE(FILTER_ENACT_POLICING_3))
        {
            pAction->actEnable[FILTER_ENACT_POLICING_3] = L2SW_TRUE;
            pAction->filterPolicingIdx[3]  = aclAct.pridx;
        }
    }

    if(tmp & FILTER_ENACT_SVLAN_MASK)
    {
        if(aclAct.sact == FILTER_ENACT_SVLAN_TYPE(FILTER_ENACT_SVLAN_INGRESS))
        {
            if((retVal = l2sw_getAsicSvlanMemberConfiguration(units, aclAct.svidx_sact, &svlan_cfg)) != L2SW_ERR_OK)
                return retVal;

            pAction->actEnable[FILTER_ENACT_SVLAN_INGRESS] = L2SW_TRUE;
            pAction->filterSvlanIdx = aclAct.svidx_sact;
            pAction->filterSvlanVid = svlan_cfg.vs_svid;
        }
        else if(aclAct.sact == FILTER_ENACT_SVLAN_TYPE(FILTER_ENACT_SVLAN_EGRESS))
        {
            if((retVal = l2sw_getAsicSvlanMemberConfiguration(units, aclAct.svidx_sact, &svlan_cfg)) != L2SW_ERR_OK)
                return retVal;

            pAction->actEnable[FILTER_ENACT_SVLAN_EGRESS] = L2SW_TRUE;
            pAction->filterSvlanIdx = aclAct.svidx_sact;
            pAction->filterSvlanVid = svlan_cfg.vs_svid;
        }
        else if(aclAct.sact == FILTER_ENACT_SVLAN_TYPE(FILTER_ENACT_SVLAN_CVID))
            pAction->actEnable[FILTER_ENACT_SVLAN_CVID] = L2SW_TRUE;
        else if(aclAct.sact == FILTER_ENACT_SVLAN_TYPE(FILTER_ENACT_POLICING_2))
        {
            pAction->actEnable[FILTER_ENACT_POLICING_2] = L2SW_TRUE;
            pAction->filterPolicingIdx[2]  = aclAct.svidx_sact;
        }
    }


    if(tmp & FILTER_ENACT_CVLAN_MASK)
    {
        if(FILTER_ENACT_CACTEXT_TAGONLY == aclAct.cact_ext ||
            FILTER_ENACT_CACTEXT_BOTHVLANTAG == aclAct.cact_ext )
        {
            if(FILTER_CTAGFMT_UNTAG == aclAct.tag_fmt)
            {
                pAction->actEnable[FILTER_ENACT_EGRESSCTAG_UNTAG] = L2SW_TRUE;
            }
            else if(FILTER_CTAGFMT_TAG == aclAct.tag_fmt)
            {
                pAction->actEnable[FILTER_ENACT_EGRESSCTAG_TAG] = L2SW_TRUE;
            }
            else if(FILTER_CTAGFMT_KEEP == aclAct.tag_fmt)
            {
                pAction->actEnable[FILTER_ENACT_EGRESSCTAG_KEEP] = L2SW_TRUE;
            }
            else if(FILTER_CTAGFMT_KEEP1PRMK== aclAct.tag_fmt)
            {
                pAction->actEnable[FILTER_ENACT_EGRESSCTAG_KEEPAND1PRMK] = L2SW_TRUE;
            }

        }

        if(FILTER_ENACT_CACTEXT_VLANONLY == aclAct.cact_ext ||
            FILTER_ENACT_CACTEXT_BOTHVLANTAG == aclAct.cact_ext )
        {
            if(aclAct.cact == FILTER_ENACT_CVLAN_TYPE(FILTER_ENACT_CVLAN_INGRESS))
            {
                if((retVal = l2sw_getAsicVlanMemberConfig(units, aclAct.cvidx_cact, &vlanMC)) != L2SW_ERR_OK)
                    return retVal;

                pAction->actEnable[FILTER_ENACT_CVLAN_INGRESS] = L2SW_TRUE;
                pAction->filterCvlanIdx  = aclAct.cvidx_cact;
                pAction->filterCvlanVid  = vlanMC.evid;
            }
            else if(aclAct.cact == FILTER_ENACT_CVLAN_TYPE(FILTER_ENACT_CVLAN_EGRESS))
            {
                if((retVal = l2sw_getAsicVlanMemberConfig(units, aclAct.cvidx_cact, &vlanMC)) != L2SW_ERR_OK)
                    return retVal;

                pAction->actEnable[FILTER_ENACT_CVLAN_EGRESS] = L2SW_TRUE;
                pAction->filterCvlanIdx  = aclAct.cvidx_cact;
                pAction->filterCvlanVid  = vlanMC.evid;
            }
            else if(aclAct.cact == FILTER_ENACT_CVLAN_TYPE(FILTER_ENACT_CVLAN_SVID))
            {
                pAction->actEnable[FILTER_ENACT_CVLAN_SVID] = L2SW_TRUE;
            }
            else if(aclAct.cact == FILTER_ENACT_CVLAN_TYPE(FILTER_ENACT_POLICING_1))
            {
                pAction->actEnable[FILTER_ENACT_POLICING_1] = L2SW_TRUE;
                pAction->filterPolicingIdx[1]  = aclAct.cvidx_cact;
            }
        }
    }

    if(tmp & FILTER_ENACT_INTGPIO_MASK)
    {
        if(L2SW_TRUE == aclAct.aclint)
        {
            pAction->actEnable[FILTER_ENACT_INTERRUPT] = L2SW_TRUE;
        }

        if(L2SW_TRUE == aclAct.gpio_en)
        {
            pAction->actEnable[FILTER_ENACT_GPO] = L2SW_TRUE;
            pAction->filterPin = aclAct.gpio_pin;
        }
    }

    /* Get field type of RAW data */
    if ((retVal = l2sw_getAsicAclTemplate(units, aclRule.data_bits.type, &type))!= L2SW_ERR_OK)
        return retVal;

    for(i = 0; i < L2SW_ACLRULEFIELDNO; i++)
    {
        pFilter_cfg->fieldRawType[i] = type.field[i];
    }/* end of for(i...) */

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_add
 * Description:
 *      Add an ACL configuration to ASIC
 * Input:
 *      filter_id       - Start index of ACL configuration.
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_action  - Action(s) of ACL configuration.
 * Output:
 *      ruleNum - number of rules written in acl table
 * Return:
 *      L2SW_ERR_OK                               - OK
 *      L2SW_ERR_FAILED                           - Failed
 *      L2SW_ERR_SMI                              - SMI access error
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT                            - Invalid input parameters.
 *      L2SW_ERR_ENTRY_INDEX                      - Invalid filter_id .
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT     - Action is not supported in this chip.
 *      L2SW_ERR_FILTER_INACL_RULE_NOT_SUPPORT    - Rule is not supported.
 * Note:
 *      This function store pFilter_cfg, pFilter_action into ASIC. The starting
 *      index(es) is filter_id.
 */
l2sw_api_ret_t _l2sw_filter_igrAcl_entry_add(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_uint32 templete_id, l2sw_filter_cfg_raw_t *pFilter_cfg, l2sw_filter_action_t *pAction)
{
    l2sw_uint32 				idxField, idx;
	l2sw_uint32          	careTagData, careTagMask;
	l2sw_uint32				portmask;	
	l2sw_uint32 				aclActCtrl;
	l2sw_api_ret_t   		retVal; 
	l2sw_acltemplate_t	aclType;
    l2sw_aclrule 		aclRule;
	l2sw_aclrule 		tempRule;
	l2sw_acl_act_t		aclAct;
//	l2sw_uint16 				flag = L2SW_FALSE;
	
	
	 /* Check initialization state */
	 L2SW_CHK_INIT_STATE(units);

	 if(templete_id >= L2SW_MAX_NUM_OF_FILTER_TYPE)
		 return L2SW_ERR_INPUT;
	 
	if(filter_id >= L2SW_ACLRULENO)
		return L2SW_ERR_INPUT;
	
    if(filter_id > L2SW_ACLRULEMAX)
        return L2SW_ERR_ENTRY_INDEX;

    if((NULL == pFilter_cfg) || (NULL == pAction))
        return L2SW_ERR_NULL_POINTER;

	#if 1
    if(pFilter_cfg->invert >= FILTER_INVERT_END )
        return L2SW_ERR_INPUT;
	#endif

    if((retVal = l2sw_getAsicAclTemplate(units, templete_id, &aclType)) != L2SW_ERR_OK)
		return retVal;

    if((retVal = l2sw_getAsicAclRule(units, filter_id, &tempRule)) != L2SW_ERR_OK )
		return retVal;	
    if(tempRule.valid == L2SW_TRUE)
    {
		return L2SW_ERR_FILTER_INACL_EXIST;
    }
	/* init RULE */
    MEMSET(&aclRule, 0, sizeof(l2sw_aclrule));	
	aclRule.data_bits.type= templete_id;
	aclRule.care_bits.type= 0x7;
	
	/*in portlist*/
    L2SW_CHK_PORTMASK_VALID(units, &pFilter_cfg->activeport.value);
    L2SW_CHK_PORTMASK_VALID(units, &pFilter_cfg->activeport.mask);

	if(l2sw_switch_portmask_L2P_get(units, &pFilter_cfg->activeport.value, &portmask) != L2SW_ERR_OK)
		return L2SW_ERR_PORT_MASK;	
	aclRule.data_bits.active_portmsk = portmask;	
	if(l2sw_switch_portmask_L2P_get(units, &pFilter_cfg->activeport.mask, &portmask) != L2SW_ERR_OK)
		return L2SW_ERR_PORT_MASK;	
	aclRule.care_bits.active_portmsk = portmask;

	/*care tag*/
	/*set care tag mask in TAG Indicator*/
    careTagData = 0;
    careTagMask = 0;

    for(idx = 0; idx<= CARE_TAG_IPV6;idx++)
    {
        if(0 == pFilter_cfg->careTag.tagType[idx].mask )
        {
            careTagMask &= ~(1 << idx);
        }
        else
        {
            careTagMask |= (1 << idx);
            if(0 == pFilter_cfg->careTag.tagType[idx].value )
                careTagData &= ~(1 << idx);
            else
                careTagData |= (1 << idx);
        }
    }
    aclRule.data_bits.tag_exist = (careTagData) & ACL_RULE_CARETAG_MASK;
    aclRule.care_bits.tag_exist = (careTagMask) & ACL_RULE_CARETAG_MASK;
	
	/*check the feild type of templete and entry is same or not*/
	 for(idxField = 0; idxField < L2SW_MAX_NUM_OF_FILTER_FIELD; idxField ++)
	 {
	 	 if (FILTER_FIELD_RAW_UNUSED == pFilter_cfg->fieldRawType[idxField])
	 	 {
			continue;
		 }
		 else 
		 {
			for (idx = 0; idx < L2SW_MAX_NUM_OF_FILTER_FIELD; idx ++)
			{
				if (aclType.field[idx] == pFilter_cfg->fieldRawType[idxField])
				{
					//flag = L2SW_TRUE;
					aclRule.data_bits.field[idx] = (pFilter_cfg->dataFieldRaw[idxField] & 0x0000FFFF);
					aclRule.care_bits.field[idx] = (pFilter_cfg->careFieldRaw[idxField] & 0x0000FFFF);
					
					break;
				}
			}
		 }
	 }
	 
	 /*action*/	 
	 if((retVal = _l2sw_filter_igrAcl_writeAction(units, pAction, &aclAct, &aclActCtrl)) != L2SW_ERR_OK)
		 return retVal;	 

	#if 0
	/*no feild/caretag/portlist set*/
	if (!flag) 
		return L2SW_ERR_INPUT
	#endif
	
	aclRule.valid = L2SW_TRUE;

	/* write ACL action control */
	if((retVal = l2sw_setAsicAclActCtrl(units, filter_id, aclActCtrl)) != L2SW_ERR_OK )
		return retVal;
	/* write ACL action */
	if((retVal = l2sw_setAsicAclAct(units, filter_id, &aclAct)) != L2SW_ERR_OK )
		return retVal;

	/* write ACL not */
	if((retVal = l2sw_setAsicAclNot(units, filter_id, pFilter_cfg->invert)) != L2SW_ERR_OK )
		return retVal;
	/* write ACL rule */
	if((retVal = l2sw_setAsicAclRule(units, filter_id, &aclRule)) != L2SW_ERR_OK )
		return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_unmatchAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_unmatch_action_t action)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(action >= FILTER_UNMATCH_END)
        return L2SW_ERR_INPUT;

    if((ret = l2sw_setAsicAclUnmatchedPermit(units, l2sw_switch_port_L2P_get(units, port), action)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_unmatchAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_unmatch_action_t* pAction)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if((ret = l2sw_getAsicAclUnmatchedPermit(units, l2sw_switch_port_L2P_get(units, port), pAction)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_state_set(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_state_t state)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(state >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if((ret = l2sw_setAsicAcl(units, l2sw_switch_port_L2P_get(units, port), state)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_state_get(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_state_t* pState)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pState)
        return L2SW_ERR_NULL_POINTER;

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if((ret = l2sw_getAsicAcl(units, l2sw_switch_port_L2P_get(units, port), pState)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_template_set(l2sw_uint8 units, l2sw_filter_template_t *aclTemplate)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 idxField;
    l2sw_acltemplate_t aclType;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(aclTemplate->index >= L2SW_MAX_NUM_OF_FILTER_TYPE)
        return L2SW_ERR_INPUT;

    for(idxField = 0; idxField < L2SW_MAX_NUM_OF_FILTER_FIELD; idxField++)
    {
        if(aclTemplate->fieldType[idxField] < FILTER_FIELD_RAW_DMAC_15_0 ||
            (aclTemplate->fieldType[idxField] > FILTER_FIELD_RAW_CTAG && aclTemplate->fieldType[idxField] < FILTER_FIELD_RAW_IPV4_SIP_15_0 ) ||
            (aclTemplate->fieldType[idxField] > FILTER_FIELD_RAW_IPV4_DIP_31_16 && aclTemplate->fieldType[idxField] < FILTER_FIELD_RAW_IPV6_SIP_15_0 ) ||
            (aclTemplate->fieldType[idxField] > FILTER_FIELD_RAW_IPV6_DIP_31_16 && aclTemplate->fieldType[idxField] < FILTER_FIELD_RAW_VIDRANGE ) ||
            (aclTemplate->fieldType[idxField] > FILTER_FIELD_RAW_FIELD_VALID && aclTemplate->fieldType[idxField] < FILTER_FIELD_RAW_FIELD_SELECT00 ) ||
            aclTemplate->fieldType[idxField] >= FILTER_FIELD_RAW_END)
        {
            return L2SW_ERR_INPUT;
        }
    }

    for(idxField = 0; idxField < L2SW_MAX_NUM_OF_FILTER_FIELD; idxField++)
    {
        aclType.field[idxField] = aclTemplate->fieldType[idxField];
    }

    if((retVal = l2sw_setAsicAclTemplate(units, aclTemplate->index, &aclType)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_template_get(l2sw_uint8 units, l2sw_filter_template_t *aclTemplate)
{
    l2sw_api_ret_t ret;
    l2sw_uint32 idxField;
    l2sw_acltemplate_t aclType;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == aclTemplate)
        return L2SW_ERR_NULL_POINTER;

    if(aclTemplate->index >= L2SW_MAX_NUM_OF_FILTER_TYPE)
        return L2SW_ERR_INPUT;

    if((ret = l2sw_getAsicAclTemplate(units, aclTemplate->index, &aclType)) != L2SW_ERR_OK)
       return ret;

    for(idxField = 0; idxField < L2SW_MAX_NUM_OF_FILTER_FIELD; idxField ++)
    {
        aclTemplate->fieldType[idxField] = aclType.field[idxField];
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_field_sel_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_field_sel_t format, l2sw_uint32 offset)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(index >= L2SW_FIELDSEL_FORMAT_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    if(format >= FORMAT_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if(offset > L2SW_FIELDSEL_MAX_OFFSET)
        return L2SW_ERR_OUT_OF_RANGE;

    if((ret = l2sw_setAsicFieldSelector(units, index, (l2sw_uint32)format, offset)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAcl_field_sel_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_field_sel_t *pFormat, l2sw_uint32 *pOffset)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pFormat || NULL == pOffset)
        return L2SW_ERR_NULL_POINTER;

    if(index >= L2SW_FIELDSEL_FORMAT_NUMBER)
        return L2SW_ERR_OUT_OF_RANGE;

    if((ret = l2sw_getAsicFieldSelector(units, index, pFormat, pOffset)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_iprange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_iprange_t type, ipaddr_t upperIp, ipaddr_t lowerIp)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(type >= IPRANGE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if(lowerIp > upperIp)
        return L2SW_ERR_INPUT;

    if((ret = l2sw_setAsicAclIpRange(units, index, type, upperIp, lowerIp)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_iprange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_iprange_t *pType, ipaddr_t *pUpperIp, ipaddr_t *pLowerIp)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if((NULL == pType) || (NULL == pUpperIp) || (NULL == pLowerIp))
        return L2SW_ERR_NULL_POINTER;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((ret = l2sw_getAsicAclIpRange(units, index, pType, pUpperIp, pLowerIp)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_vidrange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_vidrange_t type, l2sw_uint32 upperVid, l2sw_uint32 lowerVid)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(type >= VIDRANGE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if(lowerVid > upperVid)
        return L2SW_ERR_INPUT;

    if( (upperVid > L2SW_VIDMAX) || (lowerVid > L2SW_VIDMAX))
        return L2SW_ERR_OUT_OF_RANGE;

    if((ret = l2sw_setAsicAclVidRange(units, index, type, upperVid, lowerVid)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_vidrange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_vidrange_t *pType, l2sw_uint32 *pUpperVid, l2sw_uint32 *pLowerVid)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if((NULL == pType) || (NULL == pUpperVid) || (NULL == pLowerVid))
        return L2SW_ERR_NULL_POINTER;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((ret = l2sw_getAsicAclVidRange(units, index, pType, pUpperVid, pLowerVid)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_portrange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_portrange_t type, l2sw_uint32 upperPort, l2sw_uint32 lowerPort)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if(type >= PORTRANGE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if(lowerPort > upperPort)
        return L2SW_ERR_INPUT;

    if(upperPort > L2SW_ACL_PORTRANGEMAX)
        return L2SW_ERR_INPUT;

    if(lowerPort > L2SW_ACL_PORTRANGEMAX)
        return L2SW_ERR_INPUT;

    if((ret = l2sw_setAsicAclPortRange(units, index, type, upperPort, lowerPort)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_portrange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_portrange_t *pType, l2sw_uint32 *pUpperPort, l2sw_uint32 *pLowerPort)
{
    l2sw_api_ret_t ret;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if((NULL == pType) || (NULL == pUpperPort) || (NULL == pLowerPort))
        return L2SW_ERR_NULL_POINTER;

    if(index > L2SW_ACLRANGEMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((ret = l2sw_getAsicAclPortRange(units, index, pType, pUpperPort, pLowerPort)) != L2SW_ERR_OK)
       return ret;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_filter_igrAclPolarity_set(l2sw_uint8 units, l2sw_uint32 polarity)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(polarity > 1)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicAclGpioPolarity(units, polarity);
}

static l2sw_api_ret_t _l2sw_filter_igrAclPolarity_get(l2sw_uint8 units, l2sw_uint32* pPolarity)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPolarity)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicAclGpioPolarity(units, pPolarity);
}

static l2sw_api_ret_t _l2sw_acl_globalstate_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(enable > L2SW_ENABLE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicRegBit(units, L2SW_REG_ACL_ACCESS_MODE, L2SW_ACL_ACCESS_MODE_OFFSET, enable);
}

static l2sw_api_ret_t _l2sw_acl_globalstate_get(l2sw_uint8 units, l2sw_enable_t *enable)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == enable)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicRegBit(units, L2SW_REG_ACL_ACCESS_MODE, L2SW_ACL_ACCESS_MODE_OFFSET, enable);
}

static l2sw_api_ret_t _l2sw_acl_entry_matchnot_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_enable_t enable)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(enable > L2SW_ENABLE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    return l2sw_setAsicAclNot(units, index, enable);
}

static l2sw_api_ret_t _l2sw_acl_entry_matchnot_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_enable_t *enable)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == enable)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicAclNot(units, index, enable);
}

static l2sw_api_ret_t _l2sw_acl_entry_action_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 action)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    return l2sw_setAsicAclActCtrl(units, index, action);
}

static l2sw_api_ret_t _l2sw_acl_entry_action_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *action)
{
    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == action)
        return L2SW_ERR_NULL_POINTER;

    return l2sw_getAsicAclActCtrl(units, index, action);
}


/* Function Name:
 *      l2sw_filter_igrAcl_init
 * Description:
 *      ACL initialization function
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - Pointer pFilter_field or pFilter_cfg point to NULL.
 * Note:
 *      This function enable and intialize ACL function
 */
l2sw_api_ret_t l2sw_filter_igrAcl_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_init(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_field_add
 * Description:
 *      Add comparison rule to an ACL configuration
 * Input:
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_field   - The comparison rule that will be added.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      This function add a comparison rule (*pFilter_field) to an ACL configuration (*pFilter_cfg).
 *      Pointer pFilter_cfg points to an ACL configuration structure, this structure keeps multiple ACL
 *      comparison rules by means of linked list. Pointer pFilter_field will be added to linked
 *      list keeped by structure that pFilter_cfg points to.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_field_add(l2sw_uint8 units, l2sw_filter_cfg_t* pFilter_cfg, l2sw_filter_field_t* pFilter_field)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_field_add(units, pFilter_cfg, pFilter_field);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_add
 * Description:
 *      Add an ACL configuration to ASIC
 * Input:
 *      filter_id       - Start index of ACL configuration.
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_action  - Action(s) of ACL configuration.
 * Output:
 *      ruleNum - number of rules written in acl table
 * Return:
 *      L2SW_ERR_OK                               - OK
 *      L2SW_ERR_FAILED                           - Failed
 *      L2SW_ERR_SMI                              - SMI access error
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT                            - Invalid input parameters.
 *      L2SW_ERR_ENTRY_INDEX                      - Invalid filter_id .
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT     - Action is not supported in this chip.
 *      L2SW_ERR_FILTER_INACL_RULE_NOT_SUPPORT    - Rule is not supported.
 * Note:
 *      This function store pFilter_cfg, pFilter_action into ASIC. The starting
 *      index(es) is filter_id.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_cfg_add(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_filter_cfg_t* pFilter_cfg, l2sw_filter_action_t* pFilter_action, l2sw_filter_number_t *ruleNum)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_cfg_add(units, filter_id, pFilter_cfg, pFilter_action, ruleNum);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_del
 * Description:
 *      Delete an ACL configuration from ASIC
 * Input:
 *      filter_id   - Start index of ACL configuration.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_FILTER_ENTRYIDX  - Invalid filter_id.
 * Note:
 *      This function delete a group of ACL rules starting from filter_id.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_cfg_del(l2sw_uint8 units, l2sw_filter_id_t filter_id)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_cfg_del(units, filter_id);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_delAll
 * Description:
 *      Delete all ACL entries from ASIC
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      This function delete all ACL configuration from ASIC.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_cfg_delAll(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_cfg_delAll(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_get
 * Description:
 *      Get one ingress acl configuration from ASIC.
 * Input:
 *      filter_id       - Start index of ACL configuration.
 * Output:
 *      pFilter_cfg     - buffer pointer of ingress acl data
 *      pFilter_action  - buffer pointer of ingress acl action
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_NULL_POINTER     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_ENTRYIDX  - Invalid entry index.
 * Note:
 *      This function get configuration from ASIC.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_cfg_get(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_filter_cfg_raw_t *pFilter_cfg, l2sw_filter_action_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_cfg_get(units, filter_id,  pFilter_cfg,  pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_cfg_add
 * Description:
 *      Add an ACL configuration to ASIC
 * Input:
 *      filter_id       - Start index of ACL configuration.
 *      pFilter_cfg     - The ACL configuration that this function will add comparison rule
 *      pFilter_action  - Action(s) of ACL configuration.
 * Output:
 *      ruleNum - number of rules written in acl table
 * Return:
 *      L2SW_ERR_OK                               - OK
 *      L2SW_ERR_FAILED                           - Failed
 *      L2SW_ERR_SMI                              - SMI access error
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_field or pFilter_cfg point to NULL.
 *      L2SW_ERR_INPUT                            - Invalid input parameters.
 *      L2SW_ERR_ENTRY_INDEX                      - Invalid filter_id .
 *      L2SW_ERR_NULL_POINTER                     - Pointer pFilter_action or pFilter_cfg point to NULL.
 *      L2SW_ERR_FILTER_INACL_ACT_NOT_SUPPORT     - Action is not supported in this chip.
 *      L2SW_ERR_FILTER_INACL_RULE_NOT_SUPPORT    - Rule is not supported.
 * Note:
 *      This function store pFilter_cfg, pFilter_action into ASIC. The starting
 *      index(es) is filter_id.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_entry_add(l2sw_uint8 units, l2sw_filter_id_t filter_id, l2sw_uint32 templete_id, l2sw_filter_cfg_raw_t *pFilter_cfg, l2sw_filter_action_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_entry_add(units, filter_id, templete_id, pFilter_cfg, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_filter_igrAcl_unmatchAction_set
 * Description:
 *      Set action to packets when no ACL configuration match
 * Input:
 *      port    - Port id.
 *      action  - Action.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function sets action of packets when no ACL configruation matches.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_unmatchAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_unmatch_action_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_unmatchAction_set(units, port, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_unmatchAction_get
 * Description:
 *      Get action to packets when no ACL configuration match
 * Input:
 *      port    - Port id.
 * Output:
 *      pAction - Action.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function gets action of packets when no ACL configruation matches.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_unmatchAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_unmatch_action_t* pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_unmatchAction_get(units, port, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_state_set
 * Description:
 *      Set state of ingress ACL.
 * Input:
 *      port    - Port id.
 *      state   - Ingress ACL state.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function gets action of packets when no ACL configruation matches.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_state_set(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_state_t state)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_state_set(units, port, state);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_state_get
 * Description:
 *      Get state of ingress ACL.
 * Input:
 *      port    - Port id.
 * Output:
 *      pState  - Ingress ACL state.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port id.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This function gets action of packets when no ACL configruation matches.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_state_get(l2sw_uint8 units, l2sw_port_t port, l2sw_filter_state_t* pState)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_state_get(units, port, pState);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_template_set
 * Description:
 *      Set template of ingress ACL.
 * Input:
 *      template - Ingress ACL template
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT           - Invalid input parameters.
 * Note:
 *      This function set ACL template.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_template_set(l2sw_uint8 units, l2sw_filter_template_t *aclTemplate)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_template_set(units, aclTemplate);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_template_get
 * Description:
 *      Get template of ingress ACL.
 * Input:
 *      template - Ingress ACL template
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      This function gets template of ACL.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_template_get(l2sw_uint8 units, l2sw_filter_template_t *aclTemplate)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_template_get(units, aclTemplate);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_field_sel_set
 * Description:
 *      Set user defined field selectors in HSB
 * Input:
 *      index       - index of field selector 0-15
 *      format      - Format of field selector
 *      offset      - Retrieving data offset
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      System support 16 user defined field selctors.
 *      Each selector can be enabled or disable.
 *      User can defined retrieving 16-bits in many predefiend
 *      standard l2/l3/l4 payload.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_field_sel_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_field_sel_t format, l2sw_uint32 offset)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_field_sel_set(units, index, format, offset);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_igrAcl_field_sel_get
 * Description:
 *      Get user defined field selectors in HSB
 * Input:
 *      index       - index of field selector 0-15
 * Output:
 *      pFormat     - Format of field selector
 *      pOffset     - Retrieving data offset
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_filter_igrAcl_field_sel_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_field_sel_t *pFormat, l2sw_uint32 *pOffset)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAcl_field_sel_get(units, index, pFormat, pOffset);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_iprange_set
 * Description:
 *      Set IP Range check
 * Input:
 *      index       - index of IP Range 0-15
 *      type        - IP Range check type, 0:Delete a entry, 1: IPv4_SIP, 2: IPv4_DIP, 3:IPv6_SIP, 4:IPv6_DIP
 *      upperIp     - The upper bound of IP range
 *      lowerIp     - The lower Bound of IP range
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      upperIp must be larger or equal than lowerIp.
 */
l2sw_api_ret_t l2sw_filter_iprange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_iprange_t type, ipaddr_t upperIp, ipaddr_t lowerIp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_iprange_set(units, index, type, upperIp, lowerIp);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_iprange_get
 * Description:
 *      Set IP Range check
 * Input:
 *      index       - index of IP Range 0-15
 * Output:
 *      pType        - IP Range check type, 0:Delete a entry, 1: IPv4_SIP, 2: IPv4_DIP, 3:IPv6_SIP, 4:IPv6_DIP
 *      pUpperIp     - The upper bound of IP range
 *      pLowerIp     - The lower Bound of IP range
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_filter_iprange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_iprange_t *pType, ipaddr_t *pUpperIp, ipaddr_t *pLowerIp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_iprange_get(units, index, pType, pUpperIp, pLowerIp);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_filter_vidrange_set
 * Description:
 *      Set VID Range check
 * Input:
 *      index       - index of VID Range 0-15
 *      type        - IP Range check type, 0:Delete a entry, 1: CVID, 2: SVID
 *      upperVid    - The upper bound of VID range
 *      lowerVid    - The lower Bound of VID range
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      upperVid must be larger or equal than lowerVid.
 */
l2sw_api_ret_t l2sw_filter_vidrange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_vidrange_t type, l2sw_uint32 upperVid, l2sw_uint32 lowerVid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_vidrange_set(units, index, type, upperVid, lowerVid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_filter_vidrange_get
 * Description:
 *      Get VID Range check
 * Input:
 *      index       - index of VID Range 0-15
 * Output:
 *      pType        - IP Range check type, 0:Unused, 1: CVID, 2: SVID
 *      pUpperVid    - The upper bound of VID range
 *      pLowerVid    - The lower Bound of VID range
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_filter_vidrange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_vidrange_t *pType, l2sw_uint32 *pUpperVid, l2sw_uint32 *pLowerVid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_vidrange_get(units, index, pType, pUpperVid, pLowerVid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_filter_portrange_set
 * Description:
 *      Set Port Range check
 * Input:
 *      index       - index of Port Range 0-15
 *      type        - IP Range check type, 0:Delete a entry, 1: Source Port, 2: Destnation Port
 *      upperPort   - The upper bound of Port range
 *      lowerPort   - The lower Bound of Port range
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      upperPort must be larger or equal than lowerPort.
 */
l2sw_api_ret_t l2sw_filter_portrange_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_portrange_t type, l2sw_uint32 upperPort, l2sw_uint32 lowerPort)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_portrange_set(units, index, type, upperPort, lowerPort);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_filter_portrange_get
 * Description:
 *      Set Port Range check
 * Input:
 *      index       - index of Port Range 0-15
 * Output:
 *      pType       - IP Range check type, 0:Delete a entry, 1: Source Port, 2: Destnation Port
 *      pUpperPort  - The upper bound of Port range
 *      pLowerPort  - The lower Bound of Port range
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE    - The parameter is out of range
 *      L2SW_ERR_INPUT           - Input error
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_filter_portrange_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_portrange_t *pType, l2sw_uint32 *pUpperPort, l2sw_uint32 *pLowerPort)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_portrange_get(units, index, pType, pUpperPort, pLowerPort);
    L2SW_API_UNLOCK(units);

    return retVal;

}
/* Function Name:
 *      l2sw_filter_igrAclPolarity_set
 * Description:
 *      Set ACL Goip control palarity
 * Input:
 *      polarity - 1: High, 0: Low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_filter_igrAclPolarity_set(l2sw_uint8 units, l2sw_uint32 polarity)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAclPolarity_set(units, polarity);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_filter_igrAclPolarity_get
 * Description:
 *      Get ACL Goip control palarity
 * Input:
 *      pPolarity - 1: High, 0: Low
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_filter_igrAclPolarity_get(l2sw_uint8 units, l2sw_uint32* pPolarity)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_filter_igrAclPolarity_get(units, pPolarity);
    L2SW_API_UNLOCK(units);

    return retVal;

}

/* Function Name:
 *      l2sw_acl_globalstate_set
 * Description:
 *      Set ACL global state
 * Input:
 *      enable - 1: enable, 0: disable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_acl_globalstate_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_acl_globalstate_set(units, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_acl_globalstate_get
 * Description:
 *      Get ACL global state
 * Input:
 *      None
 * Output:
 *      enable - 1: enable, 0: disable
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_acl_globalstate_get(l2sw_uint8 units, l2sw_enable_t *enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_acl_globalstate_get(units, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_acl_entry_matchnot_set
 * Description:
 *      Set ACL entry matchnot
 * Input:
 *      enable - 1: enable, 0: disable
 *      index  - acl table index
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_acl_entry_matchnot_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_acl_entry_matchnot_set(units, index, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_acl_entry_matchnot_get
 * Description:
 *      Get ACL entry matchnot
 * Input:
 *      index  -acl table index
 * Output:
 *      enable - 1: enable, 0: disable
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_acl_entry_matchnot_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_enable_t *enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_acl_entry_matchnot_get(units, index, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_acl_entry_matchnot_set
 * Description:
 *      Set ACL entry action
 * Input:
 *      enable - 1: enable, 0: disable
 *      action  - acl table index action
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_acl_entry_action_set(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_acl_entry_action_set(units, index, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_acl_entry_matchnot_get
 * Description:
 *      Get ACL entry action
 * Input:
 *      index  -acl table index action
 * Output:
 *      enable - 1: enable, 0: disable
 * Return:
 *      L2SW_ERR_OK       - Success
 *      L2SW_ERR_SMI      - SMI access error
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_acl_entry_action_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_acl_entry_action_get(units, index, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

l2sw_api_ret_t l2sw_acl_defualt_template_get(l2sw_uint8 units, l2sw_uint32 index, l2sw_filter_template_t *acltemp)
{
    l2sw_uint32 i;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == acltemp)
        return L2SW_ERR_NULL_POINTER;

    for(i = 0; i < L2SW_ACLRULEFIELDNO; i++)
        acltemp->fieldType[i] = filter_templateField[index][i]; 
    
    return 0;
}


