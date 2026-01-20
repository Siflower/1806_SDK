/*
 * Include Files
 */
#include "yt_error.h"
#include "yt_util.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_acl.h"
#include "hal_mem.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_mem.h"

/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

/*
 * Data Declaration
 */
data_head_t head;
data_head_t database_header[YT_MAX_UNIT][ACL_LINE_NUM];

uint8_t flags[YT_IGRACL_TEMPLATE_MAX];

uint32_t acl_id[YT_MAX_UNIT][YT_ACL_KEY_BIT_SIZE(YT_ACL_IDX_MAX)];

acl_action_t    *pgAclAction = NULL;
yt_igrAcl_data_tlv_t    *pgAclDataList = NULL;
uint8_t gKeyDataNum = 0;
yt_port_mask_t gSrcPortMask;/*mac id based*/
yt_bool_t gSrcPortSetFlag = FALSE;

mac_da0_rule_t  mac_da0_rule0;
mac_da0_rule_mask_t mac_da0_mask;
mac_sa0_rule_t  mac_sa0_rule0;
mac_sa0_rule_mask_t mac_sa0_rule_mask;
mac_da1_sa1_rule_t mac_da1_sa1_rule;
mac_da1_sa1_rule_mask_t  mac_da1_sa1_rule_mask;
vlan_rule_t vlan_rule;
vlan_rule_mask_t vlan_rule_mask;
ipv4_da_rule_t ipv4_da_rule;
ipv4_da_rule_mask_t ipv4_da_rule_mask;
ipv4_sa_rule_t ipv4_sa_rule;
ipv4_sa_rule_mask_t ipv4_sa_rule_mask;
l4_port_rule_t l4_port_rule;
l4_port_rule_mask_t l4_port_rule_mask;
misc_rule_t misc_rule;
misc_rule_mask_t misc_rule_mask;
vid_pri_dei_rule_t vid_pri_dei_rule;
vid_pri_dei_rule_mask_t vid_pri_dei_rule_mask;
ipv6_da0_rule_t ipv6_da0_rule;
ipv6_da0_rule_mask_t ipv6_da0_rule_mask;
ipv6_da1_rule_t ipv6_da1_rule;
ipv6_da1_rule_mask_t ipv6_da1_rule_mask;
ipv6_da2_rule_t ipv6_da2_rule;
ipv6_da2_rule_mask_t ipv6_da2_rule_mask;
ipv6_da3_rule_t ipv6_da3_rule;
ipv6_da3_rule_mask_t ipv6_da3_rule_mask;

ipv6_sa0_rule_t ipv6_sa0_rule;
ipv6_sa0_rule_mask_t ipv6_sa0_rule_mask;
ipv6_sa1_rule_t ipv6_sa1_rule;
ipv6_sa1_rule_mask_t ipv6_sa1_rule_mask;
ipv6_sa2_rule_t ipv6_sa2_rule;
ipv6_sa2_rule_mask_t ipv6_sa2_rule_mask;
ipv6_sa3_rule_t ipv6_sa3_rule;
ipv6_sa3_rule_mask_t ipv6_sa3_rule_mask;
udf_rule_t udf_rule[8];
udf_rule_mask_t udf_rule_mask[8];
ether_type_value_rule_t ether_type_value_rule;
ether_type_value_rule_mask_t ether_type_value_rule_mask;

static uint32_t fal_tiger_acl_id_entry_add(uint32_t unit, uint32_t id);
static uint32_t fal_tiger_acl_id_entry_del(uint32_t unit, uint32_t id);
static uint32_t _fal_tiger_acl_action_set(yt_unit_t unit,acl_action_t action,uint8_t group, uint8_t offset);
static uint32_t  fal_tiger_acl_table_del_entry(yt_unit_t unit, uint32_t id);
static uint32_t fal_tiger_acl_list_destory(void);

static uint32_t fal_tiger_acl_global_enable(yt_unit_t unit, yt_enable_t enable)
{
    global_ctrl1_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_ACL_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_acl_init(yt_unit_t unit)
{
    uint8_t binIdx;
    yt_port_t port;

    ACL_LOCK_INIT();

    osal_memset(flags, sizeof(flags), 0, sizeof(flags));
    osal_memset(acl_id[unit], sizeof(acl_id[unit]), 0, sizeof(acl_id[unit]));

    for(binIdx=0; binIdx<ACL_LINE_NUM; binIdx++)
    {
        database_header[unit][binIdx].head = NULL;
        database_header[unit][binIdx].size = 0;
    }

    CMM_CLEAR_MEMBER_PORT(gSrcPortMask);
    gSrcPortSetFlag = FALSE;

    fal_tiger_acl_global_enable(unit, YT_ENABLE);

    for(port = 0; port < CAL_PORT_NUM_ON_UNIT(unit); port++)
    {
        fal_tiger_acl_unmatch_permit_en_set(unit, port, YT_ENABLE);
    }
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_acl_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    acl_port_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t enablemask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, ACL_PORT_CTRLm, 0,sizeof(acl_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(ACL_PORT_CTRLm, ACL_PORT_CTRL_ENABLEf, &entry, &enablemask);
    if(enable == YT_ENABLE)
    {
        SET_BIT(enablemask, macid);
    }
    else if(enable == YT_DISABLE)
    {
        CLEAR_BIT(enablemask, macid);
    }
    else
    {
        return CMM_ERR_INPUT;
    }
    HAL_FIELD_SET(ACL_PORT_CTRLm, ACL_PORT_CTRL_ENABLEf, &entry, enablemask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_PORT_CTRLm, 0, sizeof(acl_port_ctrl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_acl_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    acl_port_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t enablemask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, ACL_PORT_CTRLm, 0, sizeof(acl_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(ACL_PORT_CTRLm, ACL_PORT_CTRL_ENABLEf, &entry, &enablemask);
    *pEnable = IS_BIT_SET(enablemask, macid) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_acl_unmatch_permit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    acl_unmatch_permit_enable_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t enablemask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, ACL_UNMATCH_PERMIT_ENABLE_CTRLm, 0,sizeof(acl_unmatch_permit_enable_ctrl_t), &entry), ret);
    HAL_FIELD_GET(ACL_UNMATCH_PERMIT_ENABLE_CTRLm, ACL_UNMATCH_PERMIT_ENABLE_CTRL_ENABLEf, &entry, &enablemask);
    if(enable == YT_ENABLE)
    {
        SET_BIT(enablemask, macid);
    }
    else if(enable == YT_DISABLE)
    {
        CLEAR_BIT(enablemask, macid);
    }
    else
    {
        return CMM_ERR_INPUT;
    }
    HAL_FIELD_SET(ACL_UNMATCH_PERMIT_ENABLE_CTRLm, ACL_UNMATCH_PERMIT_ENABLE_CTRL_ENABLEf, &entry, enablemask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_UNMATCH_PERMIT_ENABLE_CTRLm, 0, sizeof(acl_unmatch_permit_enable_ctrl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_acl_unmatch_permit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    acl_unmatch_permit_enable_ctrl_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t enablemask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, ACL_UNMATCH_PERMIT_ENABLE_CTRLm, 0,sizeof(acl_unmatch_permit_enable_ctrl_t), &entry), ret);
    HAL_FIELD_GET(ACL_UNMATCH_PERMIT_ENABLE_CTRLm, ACL_UNMATCH_PERMIT_ENABLE_CTRL_ENABLEf, &entry, &enablemask);
    *pEnable = IS_BIT_SET(enablemask, macid) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_id_entry_add(uint32_t unit, uint32_t id)
{
    uint8_t index = 0;
    uint8_t bitId = 0;

    if (id >= YT_ACL_IDX_MAX)
    {
        osal_printf("ACL table is full.\r\n");
        return CMM_ERR_TABLE_FULL;
    }

    index = id/YT_ACL_BITWID;
    bitId = id%YT_ACL_BITWID;
    acl_id[unit][index] |= (1<<bitId);
    osal_printf("ACL entry %d is added.\r\n", id);
    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_id_entry_del(uint32_t unit, uint32_t id)
{
    uint8_t index = 0;
    uint8_t bitId = 0;

    if (id >= YT_ACL_IDX_MAX)
    {
        osal_printf("ACL entry %d is not found.\r\n", id);
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    index = id/YT_ACL_BITWID;
    bitId = id%YT_ACL_BITWID;
    if (!(acl_id[unit][index] & (1<<bitId)))
    {
        osal_printf("ACL entry %d is not found.\r\n", id);
        return CMM_ERR_ENTRY_NOT_FOUND;
    }
    else
    {
        acl_id[unit][index] &= ~(1<<bitId);
        osal_printf("ACL entry %d is removed.\r\n", id);
    }

    return CMM_ERR_OK;
}

static data_store_t  *fal_tiger_node_make(uint8_t type, uint8_t num)
{
    data_store_t      *node = NULL;

    mac_da0_rule_t tmpMacDa0Rule;
    mac_sa0_rule_t  tmpMacSa0Rule;
    mac_da1_sa1_rule_t tmpDa1Sa1Rule;
    vlan_rule_t tmpVlanRule;
    ipv4_da_rule_t tmpIpv4DaRule;
    ipv4_sa_rule_t tmpIpv4SaRule;
    l4_port_rule_t tmpl4PortRule;
    misc_rule_t tmpMiscRule;
    vid_pri_dei_rule_t tmpVidPriDeiRule;
    ipv6_da0_rule_t tmpIpv6Da0Rule;
    ipv6_da1_rule_t tmpIpv6Da1Rule;
    ipv6_da2_rule_t tmpIpv6Da2Rule;
    ipv6_da3_rule_t tmpIpv6Da3Rule;
    ipv6_sa0_rule_t tmpIpv6Sa0Rule;
    ipv6_sa1_rule_t tmpIpv6Sa1Rule;
    ipv6_sa2_rule_t tmpIpv6Sa2Rule;
    ipv6_sa3_rule_t tmpIpv6Sa3Rule;
    udf_rule_t tmpudf_rule;
    ether_type_value_rule_t tmpEther_type_value_rule;

    node = (data_store_t *)osal_malloc(sizeof(data_store_t), YT_MEM_MALLOC_MODULE_ACL);
    if(node == NULL)
    {
        return NULL;
    }
    osal_memset(node, sizeof(data_store_t), 0, sizeof(data_store_t));
    node->next = NULL;

    switch(type)
    {
        case YT_IGRACL_TEMPLATE_MAC_DA:
            if(num == 1)
            {
                osal_memcpy(&tmpMacDa0Rule, sizeof(mac_da0_rule_t), &mac_da0_rule0, sizeof(mac_da0_rule_t));
                HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_PRIOf, &tmpMacDa0Rule, 0);
                HAL_FIELD_GET(MAC_DA0_RULEm, MAC_DA0_RULE_RULE_TYPEf, &mac_da0_rule0, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(mac_da0_rule_t), &tmpMacDa0Rule, sizeof(mac_da0_rule_t));
                osal_memcpy(node->data_mask , sizeof(mac_da0_mask), &mac_da0_mask, sizeof(mac_da0_mask));
            }
            else
            {
                osal_memcpy(&tmpDa1Sa1Rule, sizeof(mac_da1_sa1_rule), &mac_da1_sa1_rule, sizeof(mac_da1_sa1_rule));
                HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_PRIOf, &tmpDa1Sa1Rule, 0);
                HAL_FIELD_GET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_RULE_TYPEf, &mac_da1_sa1_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpDa1Sa1Rule), &tmpDa1Sa1Rule, sizeof(tmpDa1Sa1Rule));
                osal_memcpy(node->data_mask, sizeof(mac_da1_sa1_rule_mask), &mac_da1_sa1_rule_mask, sizeof(mac_da1_sa1_rule_mask));
            }
            break;
            
        case YT_IGRACL_TEMPLATE_MAC_SA:
            if(num == 1)
            {
                osal_memcpy(&tmpMacSa0Rule, sizeof(mac_sa0_rule0), &mac_sa0_rule0, sizeof(mac_sa0_rule0));
                HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_PRIOf, &tmpMacSa0Rule, 0);
                HAL_FIELD_GET(MAC_SA0_RULEm, MAC_SA0_RULE_RULE_TYPEf, &mac_sa0_rule0, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpMacSa0Rule), &tmpMacSa0Rule, sizeof(tmpMacSa0Rule));
                osal_memcpy(node->data_mask , sizeof(mac_sa0_rule_mask), &mac_sa0_rule_mask, sizeof(mac_sa0_rule_mask));
            }
            else
            {
                osal_memcpy(&tmpDa1Sa1Rule, sizeof(mac_da1_sa1_rule), &mac_da1_sa1_rule, sizeof(mac_da1_sa1_rule));
                HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_PRIOf, &tmpDa1Sa1Rule, 0);
                HAL_FIELD_GET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_RULE_TYPEf, &mac_da1_sa1_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpDa1Sa1Rule), &tmpDa1Sa1Rule, sizeof(tmpDa1Sa1Rule));
                osal_memcpy(node->data_mask, sizeof(mac_da1_sa1_rule_mask), &mac_da1_sa1_rule_mask, sizeof(mac_da1_sa1_rule_mask));
            }
            break;
            
        case YT_IGRACL_TEMPLATE_L2_TYPE:
            osal_memcpy(&tmpDa1Sa1Rule, sizeof(mac_da1_sa1_rule_t), &mac_da1_sa1_rule, sizeof(mac_da1_sa1_rule_t));
            HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_PRIOf, &tmpDa1Sa1Rule, 0);
            HAL_FIELD_GET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_RULE_TYPEf, &mac_da1_sa1_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(mac_da1_sa1_rule_t), &tmpDa1Sa1Rule, sizeof(mac_da1_sa1_rule_t));
            osal_memcpy(node->data_mask, sizeof(mac_da1_sa1_rule_mask_t), &mac_da1_sa1_rule_mask, sizeof(mac_da1_sa1_rule_mask_t));
            break;   
            
        case YT_IGRACL_TEMPLATE_L3_TYPE:
            osal_memcpy(&tmpMacDa0Rule, sizeof(mac_da0_rule0), &mac_da0_rule0, sizeof(mac_da0_rule0));
            HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_PRIOf, &tmpMacDa0Rule, 0);
            HAL_FIELD_GET(MAC_DA0_RULEm, MAC_DA0_RULE_RULE_TYPEf, &mac_da0_rule0, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpMacDa0Rule), &tmpMacDa0Rule, sizeof(tmpMacDa0Rule));
            osal_memcpy(node->data_mask , sizeof(mac_da0_mask), &mac_da0_mask, sizeof(mac_da0_mask));
            break;
            
        case YT_IGRACL_TEMPLATE_CDEI:
        case YT_IGRACL_TEMPLATE_CPRI:
        case YT_IGRACL_TEMPLATE_CTAG_FMT:
        case YT_IGRACL_TEMPLATE_SDEI:
        case YT_IGRACL_TEMPLATE_SPRI:
        case YT_IGRACL_TEMPLATE_STAG_FMT:
        case YT_IGRACL_TEMPLATE_SVID:
        case YT_IGRACL_TEMPLATE_CVID:
            osal_memcpy(&tmpVlanRule, sizeof(vlan_rule), &vlan_rule, sizeof(vlan_rule));
            HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_PRIOf, &tmpVlanRule, 0);
            HAL_FIELD_GET(VLAN_RULEm, VLAN_RULE_RULE_TYPEf, &vlan_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpVlanRule), &tmpVlanRule, sizeof(tmpVlanRule));
            osal_memcpy(node->data_mask , sizeof(vlan_rule_mask), &vlan_rule_mask, sizeof(vlan_rule_mask));
            break;
        case YT_IGRACL_TEMPLATE_IPV4_DA:
            osal_memcpy(&tmpIpv4DaRule, sizeof(ipv4_da_rule), &ipv4_da_rule, sizeof(ipv4_da_rule));
            HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_PRIOf, &tmpIpv4DaRule, 0);
            HAL_FIELD_GET(IPV4_DA_RULEm, IPV4_DA_RULE_RULE_TYPEf, &ipv4_da_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpIpv4DaRule), &tmpIpv4DaRule, sizeof(tmpIpv4DaRule));
            osal_memcpy(node->data_mask , sizeof(ipv4_da_rule_mask), &ipv4_da_rule_mask, sizeof(ipv4_da_rule_mask));
            break;
        case YT_IGRACL_TEMPLATE_IPV4_SA:
            osal_memcpy(&tmpIpv4SaRule, sizeof(ipv4_sa_rule), &ipv4_sa_rule, sizeof(ipv4_sa_rule));
            HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_PRIOf, &tmpIpv4SaRule, 0);
            HAL_FIELD_GET(IPV4_SA_RULEm, IPV4_SA_RULE_RULE_TYPEf, &ipv4_sa_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpIpv4SaRule), &tmpIpv4SaRule, sizeof(tmpIpv4SaRule));
            osal_memcpy(node->data_mask , sizeof(ipv4_sa_rule_mask), &ipv4_sa_rule_mask, sizeof(ipv4_sa_rule_mask));
            break;
        case YT_IGRACL_TEMPLATE_L4_DPORT:
        case YT_IGRACL_TEMPLATE_L4_SPORT:
            osal_memcpy(&tmpl4PortRule, sizeof(l4_port_rule), &l4_port_rule, sizeof(l4_port_rule));
            HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_PRIOf, &tmpl4PortRule, 0);
            HAL_FIELD_GET(L4_PORT_RULEm, L4_PORT_RULE_RULE_TYPEf, &l4_port_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpl4PortRule), &tmpl4PortRule, sizeof(tmpl4PortRule));
            osal_memcpy(node->data_mask , sizeof(l4_port_rule_mask), &l4_port_rule_mask, sizeof(l4_port_rule_mask));
            break;
        case YT_IGRACL_TEMPLATE_IPV6_DA:
            if(num == 1)
            {
                osal_memcpy(&tmpIpv6Da0Rule, sizeof(ipv6_da0_rule), &ipv6_da0_rule, sizeof(ipv6_da0_rule));
                HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_PRIOf, &tmpIpv6Da0Rule, 0);
                HAL_FIELD_GET(IPV6_DA0_RULEm, IPV6_DA0_RULE_RULE_TYPEf, &ipv6_da0_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Da0Rule), &tmpIpv6Da0Rule, sizeof(tmpIpv6Da0Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_da0_rule_mask), &ipv6_da0_rule_mask, sizeof(ipv6_da0_rule_mask));
            }else if(num == 2)
            {
                osal_memcpy(&tmpIpv6Da1Rule, sizeof(ipv6_da1_rule), &ipv6_da1_rule, sizeof(ipv6_da1_rule));
                HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_PRIOf, &tmpIpv6Da1Rule, 0);
                HAL_FIELD_GET(IPV6_DA1_RULEm, IPV6_DA1_RULE_RULE_TYPEf, &ipv6_da1_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Da1Rule), &tmpIpv6Da1Rule, sizeof(tmpIpv6Da1Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_da1_rule_mask), &ipv6_da1_rule_mask, sizeof(ipv6_da1_rule_mask));
            }else if(num == 3)
            {
                osal_memcpy(&tmpIpv6Da2Rule, sizeof(ipv6_da2_rule), &ipv6_da2_rule, sizeof(ipv6_da2_rule));
                HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_PRIOf, &tmpIpv6Da2Rule, 0);
                HAL_FIELD_GET(IPV6_DA2_RULEm, IPV6_DA2_RULE_RULE_TYPEf, &ipv6_da2_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Da2Rule), &tmpIpv6Da2Rule, sizeof(tmpIpv6Da2Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_da2_rule_mask), &ipv6_da2_rule_mask, sizeof(ipv6_da2_rule_mask));
            }else if(num == 4)
            {
                osal_memcpy(&tmpIpv6Da3Rule, sizeof(ipv6_da3_rule), &ipv6_da3_rule, sizeof(ipv6_da3_rule));
                HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_PRIOf, &tmpIpv6Da3Rule, 0);
                HAL_FIELD_GET(IPV6_DA3_RULEm, IPV6_DA3_RULE_RULE_TYPEf, &ipv6_da3_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Da3Rule), &tmpIpv6Da3Rule, sizeof(tmpIpv6Da3Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_da3_rule_mask), &ipv6_da3_rule_mask, sizeof(ipv6_da3_rule_mask));
            }
            break;
        case YT_IGRACL_TEMPLATE_IPV6_SA:
            if(num == 1)
            {
                osal_memcpy(&tmpIpv6Sa0Rule, sizeof(ipv6_sa0_rule), &ipv6_sa0_rule, sizeof(ipv6_sa0_rule));
                HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_PRIOf, &tmpIpv6Sa0Rule, 0);
                HAL_FIELD_GET(IPV6_SA0_RULEm, IPV6_SA0_RULE_RULE_TYPEf, &ipv6_sa0_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Sa0Rule), &tmpIpv6Sa0Rule, sizeof(tmpIpv6Sa0Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_sa0_rule_mask), &ipv6_sa0_rule_mask, sizeof(ipv6_sa0_rule_mask));
            }else if(num == 2)
            {
                osal_memcpy(&tmpIpv6Sa1Rule, sizeof(ipv6_sa1_rule), &ipv6_sa1_rule, sizeof(ipv6_sa1_rule));
                HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_PRIOf, &tmpIpv6Sa1Rule, 0);
                HAL_FIELD_GET(IPV6_SA1_RULEm, IPV6_SA1_RULE_RULE_TYPEf, &ipv6_sa1_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Sa1Rule), &tmpIpv6Sa1Rule, sizeof(tmpIpv6Sa1Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_sa1_rule_mask), &ipv6_sa1_rule_mask, sizeof(ipv6_sa1_rule_mask));
            }else if(num == 3)
            {
                osal_memcpy(&tmpIpv6Sa2Rule, sizeof(ipv6_sa2_rule), &ipv6_sa2_rule, sizeof(ipv6_sa2_rule));
                HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_PRIOf, &tmpIpv6Sa2Rule, 0);
                HAL_FIELD_GET(IPV6_SA2_RULEm, IPV6_SA2_RULE_RULE_TYPEf, &ipv6_sa2_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Sa2Rule), &tmpIpv6Sa2Rule, sizeof(tmpIpv6Sa2Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_sa2_rule_mask), &ipv6_sa2_rule_mask, sizeof(ipv6_sa2_rule_mask));
            }else if(num == 4)
            {
                osal_memcpy(&tmpIpv6Sa3Rule, sizeof(ipv6_sa3_rule), &ipv6_sa3_rule, sizeof(ipv6_sa3_rule));
                HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_PRIOf, &tmpIpv6Sa3Rule, 0);
                HAL_FIELD_GET(IPV6_SA3_RULEm, IPV6_SA3_RULE_RULE_TYPEf, &ipv6_sa3_rule, &node->rule_type);
                osal_memcpy(node->data_entry, sizeof(tmpIpv6Sa3Rule), &tmpIpv6Sa3Rule, sizeof(tmpIpv6Sa3Rule));
                osal_memcpy(node->data_mask , sizeof(ipv6_sa3_rule_mask), &ipv6_sa3_rule_mask, sizeof(ipv6_sa3_rule_mask));
            }
            break;    
        case YT_IGRACL_TEMPLATE_PPPOE_FLAG:
        case YT_IGRACL_TEMPLATE_TCP_FLAGS:
        case YT_IGRACL_TEMPLATE_IP_PROTOCOL:
        case YT_IGRACL_TEMPLATE_TOS:
        case YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT:
        case YT_IGRACL_TEMPLATE_IP_OPTION:
        case YT_IGRACL_TEMPLATE_IP_FRAGMENT:
        case YT_IGRACL_TEMPLATE_L4_TYPE:
            osal_memcpy(&tmpMiscRule, sizeof(misc_rule), &misc_rule, sizeof(misc_rule));
            HAL_FIELD_SET(MISC_RULEm, MISC_RULE_PRIOf, &tmpMiscRule, 0);
            HAL_FIELD_GET(MISC_RULEm, MISC_RULE_RULE_TYPEf, &misc_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpMiscRule), &tmpMiscRule, sizeof(tmpMiscRule));
            osal_memcpy(node->data_mask , sizeof(misc_rule_mask), &misc_rule_mask, sizeof(misc_rule_mask));
            break;
        case YT_IGRACL_TEMPLATE_IS_IGMP:
            osal_memcpy(&tmpudf_rule, sizeof(udf_rule[0]), &udf_rule[0], sizeof(udf_rule[0]));
            HAL_FIELD_SET(UDF_RULEm, UDF_RULE_PRIOf, &tmpudf_rule, 0);
            HAL_FIELD_GET(UDF_RULEm, UDF_RULE_RULE_TYPEf, &udf_rule[0], &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpudf_rule), &tmpudf_rule, sizeof(tmpudf_rule));
            osal_memcpy(node->data_mask , sizeof(udf_rule_mask[0]), &udf_rule_mask[0], sizeof(udf_rule_mask[0]));
            break;
        case YT_IGRACL_TEMPLATE_UDF_0:
        case YT_IGRACL_TEMPLATE_UDF_1:
        case YT_IGRACL_TEMPLATE_UDF_2:
        case YT_IGRACL_TEMPLATE_UDF_3:
        case YT_IGRACL_TEMPLATE_UDF_4:
        case YT_IGRACL_TEMPLATE_UDF_5:
        case YT_IGRACL_TEMPLATE_UDF_6:
        case YT_IGRACL_TEMPLATE_UDF_7: 
            osal_memcpy(&tmpudf_rule, sizeof(udf_rule[type-YT_IGRACL_TEMPLATE_UDF_0]), &udf_rule[type-YT_IGRACL_TEMPLATE_UDF_0], sizeof(udf_rule[type-YT_IGRACL_TEMPLATE_UDF_0]));
            HAL_FIELD_SET(UDF_RULEm, UDF_RULE_PRIOf, &tmpudf_rule, 0);
            HAL_FIELD_GET(UDF_RULEm, UDF_RULE_RULE_TYPEf, &udf_rule[type-YT_IGRACL_TEMPLATE_UDF_0], &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpudf_rule), &tmpudf_rule, sizeof(tmpudf_rule));
            osal_memcpy(node->data_mask , sizeof(udf_rule_mask[type-YT_IGRACL_TEMPLATE_UDF_0]), &udf_rule_mask[type-YT_IGRACL_TEMPLATE_UDF_0], sizeof(udf_rule_mask[type-YT_IGRACL_TEMPLATE_UDF_0]));
            break;
        case YT_IGRACL_TEMPLATE_INNER_CVID:
        case YT_IGRACL_TEMPLATE_INNER_SVID:
        case YT_IGRACL_TEMPLATE_INNER_SPRI:
        case YT_IGRACL_TEMPLATE_INNER_CPRI:
        case YT_IGRACL_TEMPLATE_INNER_SDEI:
        case YT_IGRACL_TEMPLATE_INNER_CDEI:
            osal_memcpy(&tmpVidPriDeiRule, sizeof(vid_pri_dei_rule), &vid_pri_dei_rule, sizeof(vid_pri_dei_rule));
            HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_PRIOf, &tmpVidPriDeiRule, 0);
            HAL_FIELD_GET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_RULE_TYPEf, &vid_pri_dei_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpVidPriDeiRule), &tmpVidPriDeiRule, sizeof(tmpVidPriDeiRule));
            osal_memcpy(node->data_mask , sizeof(vid_pri_dei_rule_mask), &vid_pri_dei_rule_mask, sizeof(vid_pri_dei_rule_mask));
            break;
        case YT_IGRACL_TEMPLATE_ETHER_TYPE:
            osal_memcpy(&tmpEther_type_value_rule, sizeof(ether_type_value_rule), &ether_type_value_rule, sizeof(ether_type_value_rule));
            HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_PRIOf, &tmpEther_type_value_rule, 0);
            HAL_FIELD_GET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_RULE_TYPEf, &ether_type_value_rule, &node->rule_type);
            osal_memcpy(node->data_entry, sizeof(tmpEther_type_value_rule), &tmpEther_type_value_rule, sizeof(tmpEther_type_value_rule));
            osal_memcpy(node->data_mask , sizeof(ether_type_value_rule_mask), &ether_type_value_rule_mask, sizeof(ether_type_value_rule_mask));
            break;
        default:
            break;
    }
    return node;
}

static uint32_t fal_tiger_acl_list_insert(uint8_t type, uint8_t flag)
{
    data_store_t *node = NULL, *nodeNext;
    uint8_t tmpFlag = flag;
    uint8_t num =   1;
    uint8_t entryIdx =   0;

    nodeNext = head.head;
    if(head.head != NULL)
    {
        while(nodeNext->next)
        {
            nodeNext = nodeNext->next;
        }
    }

    while(tmpFlag)
    { 
        if(tmpFlag &1){
            node = fal_tiger_node_make(type, num);
            node->entryIdx = head.size;
            if (head.head == NULL)
            {
                head.head = node;
                nodeNext = head.head;
            }
            else
            {
                nodeNext->next  =   node;
                nodeNext = nodeNext->next;
            }
            head.size++;
            entryIdx++;
        }
        num++;
        tmpFlag >>= 1;
    }

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_list_destory(void)
{
    data_store_t *node = NULL;
    data_store_t *nodeNext = NULL;

    node = head.head;
    while(node)
    {
        nodeNext = node->next;
        osal_free(node, YT_MEM_MALLOC_MODULE_ACL);
        node = nodeNext;
    }
    head.head = NULL;

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_data_list_create(void)
{
    uint8_t clean_l3_type = 0;
    uint8_t clean_l2_type = 0;
    uint8_t clean_mac_sa1 = 0;
    uint8_t clean_ip_fragment = 0;
    uint8_t clean_l4_type = 0;
    uint8_t clean_ip_option = 0;
    uint8_t clean_ip_1st_fragment = 0;
    uint8_t clean_is_igmp = 0;
    uint8_t index = 0;

    //do not change the sequence
    switch(flags[YT_IGRACL_TEMPLATE_MAC_DA] )
    {
    
        case 0x1:
             if((flags[YT_IGRACL_TEMPLATE_L3_TYPE]  & (1<<0)) != 0)
            {
                clean_l3_type    =   1;
            }
             break;
        case 0x2:
            if((flags[YT_IGRACL_TEMPLATE_L2_TYPE]  & (1<<0)) != 0)
            {
                clean_l2_type    =   1;
            }
            if((flags[YT_IGRACL_TEMPLATE_MAC_SA]  & (1<<1)) != 0)
            {
                clean_mac_sa1    =   1;
            }
            break;
        case 0x3:
            if((flags[YT_IGRACL_TEMPLATE_L3_TYPE]  & (1<<0)) != 0)
            {
                clean_l3_type    =   1;
            }
            if((flags[YT_IGRACL_TEMPLATE_L2_TYPE]  & (1<<0)) != 0)
            {
                clean_l2_type    =   1;
            }
             if((flags[YT_IGRACL_TEMPLATE_MAC_SA]  & (1<<1)) != 0)
            {
                clean_mac_sa1    =   1;
            }
            break;
    }
    if(flags[YT_IGRACL_TEMPLATE_MAC_DA])
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_MAC_DA, flags[YT_IGRACL_TEMPLATE_MAC_DA] );
    
    if(clean_mac_sa1)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_MAC_SA],  1);
    }
    switch(flags[YT_IGRACL_TEMPLATE_MAC_SA] )
    {
        case 0x1:
            if((flags[YT_IGRACL_TEMPLATE_L3_TYPE]  & (1<<0)) != 0)
            {
                clean_l3_type    =   1;
            }
            break;
        case 0x2:
            if((flags[YT_IGRACL_TEMPLATE_L2_TYPE]  & (1<<0)) != 0)
            {
                clean_l2_type    =   1;
            }
            break;
        case 0x3:
            if((flags[YT_IGRACL_TEMPLATE_L3_TYPE]  & (1<<0)) != 0)
            {
                clean_l3_type    =   1;
            }
            if((flags[YT_IGRACL_TEMPLATE_L2_TYPE]  & (1<<0)) != 0)
            {
                clean_l2_type    =   1;
            }
            break;
    }

    if(flags[YT_IGRACL_TEMPLATE_MAC_SA])
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_MAC_SA, flags[YT_IGRACL_TEMPLATE_MAC_SA] );
    
    if(clean_l2_type)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_L2_TYPE],  0);
    }
    if(flags[YT_IGRACL_TEMPLATE_L2_TYPE])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_L2_TYPE, 0x1 );
    }

    if(flags[YT_IGRACL_TEMPLATE_CDEI] ||flags[YT_IGRACL_TEMPLATE_CPRI] || flags[YT_IGRACL_TEMPLATE_CTAG_FMT] 
        ||flags[YT_IGRACL_TEMPLATE_SDEI] ||flags[YT_IGRACL_TEMPLATE_SPRI] || flags[YT_IGRACL_TEMPLATE_STAG_FMT]
        || flags[YT_IGRACL_TEMPLATE_CVID] ||  flags[YT_IGRACL_TEMPLATE_SVID])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_CDEI, 0x1);
    }

    switch(flags[YT_IGRACL_TEMPLATE_IPV4_DA] )
    {
    
        case 0x1:
            if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
            {
                clean_ip_fragment = 1;
            }
             if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
            {
                clean_l4_type = 1;
            }
             fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IPV4_DA, 0x1);
             break;
 
    }

     switch(flags[YT_IGRACL_TEMPLATE_IPV4_SA] )
    {
    
        case 0x1:
            if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
            {
                clean_ip_fragment = 1;
            }
             if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
            {
                clean_l4_type = 1;
            }
             fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IPV4_SA, 0x1);
             break;
    }

     if(flags[YT_IGRACL_TEMPLATE_L4_DPORT] || flags[YT_IGRACL_TEMPLATE_L4_SPORT])
    {
            if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
            {
                clean_ip_fragment = 1;
            }
             if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
            {
                clean_l4_type = 1;
            }
             fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_L4_DPORT, 0x1);
    }

    if(flags[YT_IGRACL_TEMPLATE_IPV6_DA])
    {
        if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
        {
            clean_l4_type = 1;
        }

        if(flags[YT_IGRACL_TEMPLATE_IP_OPTION])
        {
            clean_ip_option = 1;
        }

        if(flags[YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT])
        {
            clean_ip_1st_fragment = 1;
        }

        if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
        {
            clean_ip_fragment = 1;
        }

        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IPV6_DA, flags[YT_IGRACL_TEMPLATE_IPV6_DA]);
    }

    if(flags[YT_IGRACL_TEMPLATE_IPV6_SA])
    {
        if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
        {
            clean_l4_type = 1;
        }
        if(flags[YT_IGRACL_TEMPLATE_IP_OPTION])
        {
            clean_ip_option = 1;
        }

        if(flags[YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT])
        {
            clean_ip_1st_fragment = 1;
        }

        if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
        {
            clean_ip_fragment = 1;
        }

        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IPV6_SA, flags[YT_IGRACL_TEMPLATE_IPV6_SA]);
    }

    if(flags[YT_IGRACL_TEMPLATE_TOS] || flags[YT_IGRACL_TEMPLATE_IP_PROTOCOL] ||flags[YT_IGRACL_TEMPLATE_TCP_FLAGS]
        ||flags[YT_IGRACL_TEMPLATE_PPPOE_FLAG])
    {
        if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
        {
            clean_l4_type = 1;
        }
        if((flags[YT_IGRACL_TEMPLATE_L3_TYPE]  & (1<<0)) != 0)
        {
            clean_l3_type  =  1;
        }
         if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
        {
            clean_ip_fragment = 1;
        }
         if(flags[YT_IGRACL_TEMPLATE_IP_OPTION])
        {
            clean_ip_option = 1;
        }
         if(flags[YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT])
        {
            clean_ip_1st_fragment = 1;
        }
         fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_TOS, 0x1);
    }

    if(flags[YT_IGRACL_TEMPLATE_ETHER_TYPE])
    {
        if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
        {
            clean_l4_type = 1;
        }
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_ETHER_TYPE, 0x1);
    }
    
    if(clean_ip_option)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_IP_OPTION],  0);
    }
    if(flags[YT_IGRACL_TEMPLATE_IP_OPTION])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IP_OPTION, 0x1 );
    }

    if(clean_ip_1st_fragment)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT],  0);
    }
     if(flags[YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT, 0x1 );
    }

    if(clean_ip_fragment)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT],  0);
    }
    if(flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IP_FRAGMENT, 0x1 );
    }

    if(clean_l4_type)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_L4_TYPE],  0);
    }
    if(flags[YT_IGRACL_TEMPLATE_L4_TYPE])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_L4_TYPE, 0x1 );
    }

    if(clean_l3_type)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_L3_TYPE],  0);
    }
     if(flags[YT_IGRACL_TEMPLATE_L3_TYPE])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_L3_TYPE, 0x1 );
    }

    if(flags[YT_IGRACL_TEMPLATE_INNER_CVID] || flags[YT_IGRACL_TEMPLATE_INNER_SVID]|| flags[YT_IGRACL_TEMPLATE_INNER_SPRI]
        || flags[YT_IGRACL_TEMPLATE_INNER_CPRI]|| flags[YT_IGRACL_TEMPLATE_INNER_SDEI]|| flags[YT_IGRACL_TEMPLATE_INNER_CDEI])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_INNER_CVID, 0x1 );
    }

    for(index=0; index<8; index++)
    {
        if(flags[YT_IGRACL_TEMPLATE_UDF_0+index])
        {
            if(flags[YT_IGRACL_TEMPLATE_IS_IGMP])
            {
                clean_is_igmp = 1;
            }
            fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_UDF_0+index, 0x1 );
        }
    }
    

     if(clean_is_igmp)
    {
        CLEAR_BIT(flags[YT_IGRACL_TEMPLATE_IS_IGMP],  0);
    }
    if(flags[YT_IGRACL_TEMPLATE_IS_IGMP])
    {
        fal_tiger_acl_list_insert(YT_IGRACL_TEMPLATE_IS_IGMP, 0x1 );
    }
     
    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_data_prepare(igrAcl_data_t *pData, uint8_t num_element)
{
    igrAcl_data_t   *pCurData;

    uint8_t index = 0;
    uint8_t index2 = 0;

    pCurData = pData;

    for(index=0; index<num_element; index++)
    {

        switch(pCurData->type)
        {
            case YT_IGRACL_TEMPLATE_MAC_DA:
                HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_MAC_DA0f, &mac_da0_rule0, ((pCurData->data[0] << 24) | (pCurData->data[1] << 16)
                    | (pCurData->data[2] << 8) |pCurData->data[3]));
                HAL_FIELD_SET(MAC_DA0_RULE_MASKm, MAC_DA0_RULE_MASK_MAC_DA0f, &mac_da0_mask, ((pCurData->mask[0] << 24) | (pCurData->mask[1] << 16)
                    | (pCurData->mask[2] << 8) |pCurData->mask[3]));
                HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_MAC_DA1f, &mac_da1_sa1_rule, ((pCurData->data[4] << 8) |pCurData->data[5]));
                HAL_FIELD_SET(MAC_DA1_SA1_RULE_MASKm, MAC_DA1_SA1_RULE_MASK_MAC_DA1f, &mac_da1_sa1_rule_mask, ((pCurData->mask[4] << 8) |pCurData->mask[5]));
               break;
                
            case YT_IGRACL_TEMPLATE_MAC_SA:
                HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_MAC_SA0f, &mac_sa0_rule0, ((pCurData->data[0] << 24) | (pCurData->data[1] << 16)
                    | (pCurData->data[2] << 8) |pCurData->data[3]));
                HAL_FIELD_SET(MAC_SA0_RULE_MASKm, MAC_SA0_RULE_MASK_MAC_SA0f, &mac_sa0_rule_mask, ((pCurData->mask[0] << 24) | (pCurData->mask[1] << 16)
                    | (pCurData->mask[2] << 8) |pCurData->mask[3]));
                HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_MAC_SA1f, &mac_da1_sa1_rule, ((pCurData->data[4] << 8) |pCurData->data[5]));
                HAL_FIELD_SET(MAC_DA1_SA1_RULE_MASKm, MAC_DA1_SA1_RULE_MASK_MAC_SA1f, &mac_da1_sa1_rule_mask, ((pCurData->mask[4] << 8) |pCurData->mask[5]));
                break;
                
            case YT_IGRACL_TEMPLATE_L2_TYPE:
                HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_L2_TYPEf, &mac_da1_sa1_rule, pCurData->data[0]);
                HAL_FIELD_SET(MAC_DA1_SA1_RULE_MASKm, MAC_DA1_SA1_RULE_MASK_L2_TYPEf, &mac_da1_sa1_rule_mask, pCurData->mask[0]);
                break;
                
            case YT_IGRACL_TEMPLATE_L3_TYPE:
                HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_L3_TYPEf, &mac_da0_rule0, pCurData->data[0]);
                HAL_FIELD_SET(MAC_DA0_RULE_MASKm, MAC_DA0_RULE_MASK_L3_TYPEf, &mac_da0_mask, pCurData->mask[0]);
                HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_L3_TYPEf, &mac_sa0_rule0, pCurData->data[0]);
                HAL_FIELD_SET(MAC_SA0_RULE_MASKm, MAC_SA0_RULE_MASK_L3_TYPEf, &mac_sa0_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_L3_TYPEf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_L3_TYPEf, &misc_rule_mask, pCurData->mask[0]);
                break;
                
            case YT_IGRACL_TEMPLATE_CDEI:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_CDEIf, &vlan_rule, pCurData->data[0]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_CDEIf, &vlan_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_CPRI:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_CPRIf, &vlan_rule, pCurData->data[0]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_CPRIf, &vlan_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_CTAG_FMT:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_CTAG_FMTf, &vlan_rule, pCurData->data[0]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_CTAG_FMTf, &vlan_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_SDEI:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_SDEIf, &vlan_rule, pCurData->data[0]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_SDEIf, &vlan_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_SPRI:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_SPRIf, &vlan_rule, pCurData->data[0]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_SPRIf, &vlan_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_STAG_FMT:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_STAG_FMTf, &vlan_rule, pCurData->data[0]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_STAG_FMTf, &vlan_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_SVID:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_SVIDf, &vlan_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_RANGE1_ENf, &vlan_rule, pCurData->data[2]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_SVIDf, &vlan_rule_mask, ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_CVID:
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_CVIDf, &vlan_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_RANGE0_ENf, &vlan_rule, pCurData->data[2]);
                HAL_FIELD_SET(VLAN_RULE_MASKm, VLAN_RULE_MASK_CVIDf, &vlan_rule_mask, ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_IPV4_DA:
                HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_IPV4_DAf, &ipv4_da_rule, ((pCurData->data[3] << 24) | (pCurData->data[2] << 16)
                        | (pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_RANGE0_ENf, &ipv4_da_rule, pCurData->data[4]);
                HAL_FIELD_SET(IPV4_DA_RULE_MASKm, IPV4_DA_RULE_MASK_IPV4_DAf, &ipv4_da_rule_mask, ((pCurData->mask[3] << 24) | (pCurData->mask[2] << 16)
                        | (pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_IPV4_SA:
                HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_IPV4_SAf, &ipv4_sa_rule, ((pCurData->data[3] << 24) | (pCurData->data[2] << 16)
                        | (pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_RANGE0_ENf, &ipv4_sa_rule, pCurData->data[4]);
                HAL_FIELD_SET(IPV4_SA_RULE_MASKm, IPV4_SA_RULE_MASK_IPV4_SAf, &ipv4_sa_rule_mask, ((pCurData->mask[3] << 24) | (pCurData->mask[2] << 16)
                        | (pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_L4_DPORT:
                HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_L4_DPORTf, &l4_port_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_RANGE1_ENf, &l4_port_rule, pCurData->data[2]);
                HAL_FIELD_SET(L4_PORT_RULE_MASKm, L4_PORT_RULE_MASK_L4_DPORTf, &l4_port_rule_mask, ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_L4_SPORT:
                HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_L4_SPORTf, &l4_port_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_RANGE0_ENf, &l4_port_rule, pCurData->data[2]);
                HAL_FIELD_SET(L4_PORT_RULE_MASKm, L4_PORT_RULE_MASK_L4_SPORTf, &l4_port_rule_mask, ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_PPPOE_FLAG:
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_PPPOE_FLAGf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_PPPOE_FLAGf, &misc_rule_mask, pCurData->mask[0]);
                break;
            
            case YT_IGRACL_TEMPLATE_TCP_FLAGS:
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_TCP_FLAGSf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_TCP_FLAGSf, &misc_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_IP_PROTOCOL:
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_IP_PROTOCOLf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_IP_PROTOCOLf, &misc_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_TOS:
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_TOSf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_TOSf, &misc_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_L4_TYPE:
                HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_L4_TYPEf, &ipv4_da_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV4_DA_RULE_MASKm, IPV4_DA_RULE_MASK_L4_TYPEf, &ipv4_da_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_L4_TYPEf, &ipv4_sa_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV4_SA_RULE_MASKm, IPV4_SA_RULE_MASK_L4_TYPEf, &ipv4_sa_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_L4_TYPEf, &l4_port_rule, pCurData->data[0]);
                HAL_FIELD_SET(L4_PORT_RULE_MASKm, L4_PORT_RULE_MASK_L4_TYPEf, &l4_port_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_L4_TYPEf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_L4_TYPEf, &misc_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_L4_TYPEf, &ipv6_da0_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA0_RULE_MASKm, IPV6_DA0_RULE_MASK_L4_TYPEf, &ipv6_da0_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_L4_TYPEf, &ipv6_da1_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA1_RULE_MASKm, IPV6_DA1_RULE_MASK_L4_TYPEf, &ipv6_da1_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_L4_TYPEf, &ipv6_da2_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA2_RULE_MASKm, IPV6_DA2_RULE_MASK_L4_TYPEf, &ipv6_da2_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_L4_TYPEf, &ipv6_da3_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA3_RULE_MASKm, IPV6_DA3_RULE_MASK_L4_TYPEf, &ipv6_da3_rule_mask, pCurData->mask[0]);
                
                HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_L4_TYPEf, &ipv6_sa0_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA0_RULE_MASKm, IPV6_SA0_RULE_MASK_L4_TYPEf, &ipv6_sa0_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_L4_TYPEf, &ipv6_sa1_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA1_RULE_MASKm, IPV6_SA1_RULE_MASK_L4_TYPEf, &ipv6_sa1_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_L4_TYPEf, &ipv6_sa2_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA2_RULE_MASKm, IPV6_SA2_RULE_MASK_L4_TYPEf, &ipv6_sa2_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_L4_TYPEf, &ipv6_sa3_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA3_RULE_MASKm, IPV6_SA3_RULE_MASK_L4_TYPEf, &ipv6_sa3_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_L4_TYPEf, &ether_type_value_rule, pCurData->data[0]);
                HAL_FIELD_SET(ETHER_TYPE_VALUE_RULE_MASKm, ETHER_TYPE_VALUE_RULE_MASK_L4_TYPEf, &ether_type_value_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_IP_FRAGMENT:
                HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_IP_FRAGMENTf, &ipv4_da_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV4_DA_RULE_MASKm, IPV4_DA_RULE_MASK_IP_FRAGMENTf, &ipv4_da_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_IP_FRAGMENTf, &ipv4_sa_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV4_SA_RULE_MASKm, IPV4_SA_RULE_MASK_IP_FRAGMENTf, &ipv4_sa_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_IP_FRAGMENTf, &l4_port_rule, pCurData->data[0]);
                HAL_FIELD_SET(L4_PORT_RULE_MASKm, L4_PORT_RULE_MASK_IP_FRAGMENTf, &l4_port_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_IP_FRAGMENTf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_IP_FRAGMENTf, &misc_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_IP_FRAGMENTf, &ipv6_da3_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA3_RULE_MASKm, IPV6_DA3_RULE_MASK_IP_FRAGMENTf, &ipv6_da3_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_IP_FRAGMENTf, &ipv6_sa3_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA3_RULE_MASKm, IPV6_SA3_RULE_MASK_IP_FRAGMENTf, &ipv6_sa3_rule_mask, pCurData->mask[0]);

                break;
            case YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT:
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_IP_1ST_FRAGMENTf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_IP_1ST_FRAGMENTf, &misc_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_IP_FIRST_FRAGMENTf, &ipv6_da2_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA2_RULE_MASKm, IPV6_DA2_RULE_MASK_IP_FIRST_FRAGMENTf, &ipv6_da2_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_IP_FIRST_FRAGMENTf, &ipv6_sa2_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA2_RULE_MASKm, IPV6_SA2_RULE_MASK_IP_FIRST_FRAGMENTf, &ipv6_sa2_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_IPV6_DA:
                HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_IPV6_DA0f, &ipv6_da0_rule,
                    ((pCurData->data[0] << 24) | (pCurData->data[1] << 16)
                        | (pCurData->data[2] << 8) |pCurData->data[3]));
                HAL_FIELD_SET(IPV6_DA0_RULE_MASKm, IPV6_DA0_RULE_MASK_IPV6_DA0f, &ipv6_da0_rule_mask,
                    ((pCurData->mask[0] << 24) | (pCurData->mask[1] << 16)
                        | (pCurData->mask[2] << 8) |pCurData->mask[3]));

                HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_IPV6_DA1f, &ipv6_da1_rule,
                    ((pCurData->data[4] << 24) | (pCurData->data[5] << 16)
                        | (pCurData->data[6] << 8) |pCurData->data[7]));
                HAL_FIELD_SET(IPV6_DA1_RULE_MASKm, IPV6_DA1_RULE_MASK_IPV6_DA1f, &ipv6_da1_rule_mask,
                    ((pCurData->mask[4] << 24) | (pCurData->mask[5] << 16)
                        | (pCurData->mask[6] << 8) |pCurData->mask[7]));

                HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_IPV6_DA2f, &ipv6_da2_rule,
                    ((pCurData->data[8] << 24) | (pCurData->data[9] << 16)
                        | (pCurData->data[10] << 8) |pCurData->data[11]));
                HAL_FIELD_SET(IPV6_DA2_RULE_MASKm, IPV6_DA2_RULE_MASK_IPV6_DA2f, &ipv6_da2_rule_mask,
                    ((pCurData->mask[8] << 24) | (pCurData->mask[9] << 16)
                        | (pCurData->mask[10] << 8) |pCurData->mask[11]));

                HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_IPV6_DA3f, &ipv6_da3_rule,
                    ((pCurData->data[12] << 24) | (pCurData->data[13] << 16)
                        | (pCurData->data[14] << 8) |pCurData->data[15]));
                HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_RANGE0_ENf, &ipv6_da3_rule, pCurData->data[16]);
                HAL_FIELD_SET(IPV6_DA3_RULE_MASKm, IPV6_DA3_RULE_MASK_IPV6_DA3f, &ipv6_da3_rule_mask,
                    ((pCurData->mask[12] << 24) | (pCurData->mask[13] << 16)
                        | (pCurData->mask[14] << 8) |pCurData->mask[15]));
                break;
            case YT_IGRACL_TEMPLATE_IPV6_SA:
                HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_IPV6_SA0f, &ipv6_sa0_rule,
                    ((pCurData->data[0] << 24) | (pCurData->data[1] << 16)
                        | (pCurData->data[2] << 8) |pCurData->data[3]));
                HAL_FIELD_SET(IPV6_SA0_RULE_MASKm, IPV6_SA0_RULE_MASK_IPV6_SA0f, &ipv6_sa0_rule_mask,
                    ((pCurData->mask[0] << 24) | (pCurData->mask[1] << 16)
                        | (pCurData->mask[2] << 8) |pCurData->mask[3]));

                HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_IPV6_SA1f, &ipv6_sa1_rule,
                    ((pCurData->data[4] << 24) | (pCurData->data[5] << 16)
                        | (pCurData->data[6] << 8) |pCurData->data[7]));
                HAL_FIELD_SET(IPV6_SA1_RULE_MASKm, IPV6_SA1_RULE_MASK_IPV6_SA1f, &ipv6_sa1_rule_mask,
                    ((pCurData->mask[4] << 24) | (pCurData->mask[5] << 16)
                        | (pCurData->mask[6] << 8) |pCurData->mask[7]));

                HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_IPV6_SA2f, &ipv6_sa2_rule,
                    ((pCurData->data[8] << 24) | (pCurData->data[9] << 16)
                        | (pCurData->data[10] << 8) |pCurData->data[11]));
                HAL_FIELD_SET(IPV6_SA2_RULE_MASKm, IPV6_SA2_RULE_MASK_IPV6_SA2f, &ipv6_sa2_rule_mask,
                    ((pCurData->mask[8] << 24) | (pCurData->mask[9] << 16)
                        | (pCurData->mask[10] << 8) |pCurData->mask[11]));

                HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_IPV6_SA3f, &ipv6_sa3_rule,
                    ((pCurData->data[12] << 24) | (pCurData->data[13] << 16)
                        | (pCurData->data[14] << 8) |pCurData->data[15]));
                HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_RANGE0_ENf, &ipv6_sa3_rule, pCurData->data[16]);
                HAL_FIELD_SET(IPV6_SA3_RULE_MASKm, IPV6_SA3_RULE_MASK_IPV6_SA3f, &ipv6_sa3_rule_mask,
                    ((pCurData->mask[12] << 24) | (pCurData->mask[13] << 16)
                        | (pCurData->mask[14] << 8) |pCurData->mask[15]));
                break;
            case YT_IGRACL_TEMPLATE_IP_OPTION:
                HAL_FIELD_SET(MISC_RULEm, MISC_RULE_IP_OPTIONf, &misc_rule, pCurData->data[0]);
                HAL_FIELD_SET(MISC_RULE_MASKm, MISC_RULE_MASK_IP_OPTIONf, &misc_rule_mask, pCurData->mask[0]);

                HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_IP_OPTIONf, &ipv6_da1_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_DA1_RULE_MASKm, IPV6_DA1_RULE_MASK_IP_OPTIONf, &ipv6_da1_rule_mask, pCurData->mask[0]);
                HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_IP_OPTIONf, &ipv6_sa1_rule, pCurData->data[0]);
                HAL_FIELD_SET(IPV6_SA1_RULE_MASKm, IPV6_SA1_RULE_MASK_IP_OPTIONf, &ipv6_sa1_rule_mask, pCurData->mask[0]);
                break;
            
            case YT_IGRACL_TEMPLATE_IS_IGMP:
                for(index2=0; index2<8; index2++)
                {
                    HAL_FIELD_SET(UDF_RULEm, UDF_RULE_IS_IGMPf, &udf_rule[index2], pCurData->data[0]);
                    HAL_FIELD_SET(UDF_RULE_MASKm, UDF_RULE_MASK_IS_IGMPf, &udf_rule_mask[index2], pCurData->mask[0]);
                }
                break;
            case YT_IGRACL_TEMPLATE_UDF_0:
            case YT_IGRACL_TEMPLATE_UDF_1:
            case YT_IGRACL_TEMPLATE_UDF_2:
            case YT_IGRACL_TEMPLATE_UDF_3:
            case YT_IGRACL_TEMPLATE_UDF_4:
            case YT_IGRACL_TEMPLATE_UDF_5:
            case YT_IGRACL_TEMPLATE_UDF_6:
            case YT_IGRACL_TEMPLATE_UDF_7:
                HAL_FIELD_SET(UDF_RULEm, UDF_RULE_UDF0f, &udf_rule[pCurData->type-YT_IGRACL_TEMPLATE_UDF_0],
                    ((pCurData->data[0] << 8) |pCurData->data[1]));
                HAL_FIELD_SET(UDF_RULE_MASKm, UDF_RULE_MASK_UDF0f, &udf_rule_mask[pCurData->type-YT_IGRACL_TEMPLATE_UDF_0],
                    ((pCurData->mask[0] << 8) |pCurData->mask[1]));
                HAL_FIELD_SET(UDF_RULEm, UDF_RULE_UDF1f, &udf_rule[pCurData->type-YT_IGRACL_TEMPLATE_UDF_0],
                    ((pCurData->data[2] << 8) |pCurData->data[3]));
                HAL_FIELD_SET(UDF_RULE_MASKm, UDF_RULE_MASK_UDF1f, &udf_rule_mask[pCurData->type-YT_IGRACL_TEMPLATE_UDF_0],
                    ((pCurData->mask[2] << 8) |pCurData->mask[3]));
                break;
            case YT_IGRACL_TEMPLATE_INNER_CVID:
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_CVIDf, &vid_pri_dei_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_RANGE0_ENf, &vid_pri_dei_rule, pCurData->data[2]);
                HAL_FIELD_SET(VID_PRI_DEI_RULE_MASKm, VID_PRI_DEI_RULE_MASK_CVIDf, &vid_pri_dei_rule_mask,
                    ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_INNER_SVID:
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_SVIDf, &vid_pri_dei_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_RANGE1_ENf, &vid_pri_dei_rule, pCurData->data[2]);
                HAL_FIELD_SET(VID_PRI_DEI_RULE_MASKm, VID_PRI_DEI_RULE_MASK_SVIDf, &vid_pri_dei_rule_mask,
                    ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            case YT_IGRACL_TEMPLATE_INNER_SPRI:
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_SPRIf, &vid_pri_dei_rule, pCurData->data[0]);
                HAL_FIELD_SET(VID_PRI_DEI_RULE_MASKm, VID_PRI_DEI_RULE_MASK_SPRIf, &vid_pri_dei_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_INNER_CPRI:
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_CPRIf, &vid_pri_dei_rule, pCurData->data[0]);
                HAL_FIELD_SET(VID_PRI_DEI_RULE_MASKm, VID_PRI_DEI_RULE_MASK_CPRIf, &vid_pri_dei_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_INNER_SDEI:
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_SDEIf, &vid_pri_dei_rule, pCurData->data[0]);
                HAL_FIELD_SET(VID_PRI_DEI_RULE_MASKm, VID_PRI_DEI_RULE_MASK_SDEIf, &vid_pri_dei_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_INNER_CDEI:
                HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_CDEIf, &vid_pri_dei_rule, pCurData->data[0]);
                HAL_FIELD_SET(VID_PRI_DEI_RULE_MASKm, VID_PRI_DEI_RULE_MASK_CDEIf, &vid_pri_dei_rule_mask, pCurData->mask[0]);
                break;
            case YT_IGRACL_TEMPLATE_ETHER_TYPE:
                HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_ETHER_TYPE_VALUEf, &ether_type_value_rule, ((pCurData->data[1] << 8) |pCurData->data[0]));
                HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_RANGE_ENf, &ether_type_value_rule, pCurData->data[2]);
                HAL_FIELD_SET(ETHER_TYPE_VALUE_RULE_MASKm, ETHER_TYPE_VALUE_RULE_MASK_ETHER_TYPE_VALUEf, &ether_type_value_rule_mask,
                    ((pCurData->mask[1] << 8) |pCurData->mask[0]));
                break;
            default:
                break;
        }

        if(pCurData)
        {
            pCurData = pCurData->pNext;
        }
    }

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_flags_set(igrAcl_data_t *pData, uint8_t num_element)
{
    igrAcl_data_t   *pCurData;
    uint8_t index = 0;

    pCurData = pData;

    for(index=0; index<num_element; index++)
    {
        switch(pCurData->type)
        {
            case YT_IGRACL_TEMPLATE_MAC_DA:
                if((pCurData->mask[0]) || (pCurData->mask[1]) ||(pCurData->mask[2])||(pCurData->mask[3]))
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_MAC_DA],  0);
                }
                if((pCurData->mask[4]) || (pCurData->mask[5]))
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_MAC_DA],  1);
                }
                if(flags[YT_IGRACL_TEMPLATE_MAC_DA] == 0)
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_MAC_DA],  0);
                }
                break;
                
            case YT_IGRACL_TEMPLATE_MAC_SA:
                if((pCurData->mask[0]) || (pCurData->mask[1]) ||(pCurData->mask[2])||(pCurData->mask[3]))
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_MAC_SA],  0);
                }
                if((pCurData->mask[4]) || (pCurData->mask[5]))
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_MAC_SA],  1);
                }
                if(flags[YT_IGRACL_TEMPLATE_MAC_SA] == 0)
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_MAC_SA],  0);
                }
                break;
                
            case YT_IGRACL_TEMPLATE_L2_TYPE:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_L2_TYPE],  0);
                break;      
            case YT_IGRACL_TEMPLATE_CDEI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_CDEI],  0);
                break;
            case YT_IGRACL_TEMPLATE_CPRI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_CPRI],  0);
                break;
            case YT_IGRACL_TEMPLATE_CTAG_FMT:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_CTAG_FMT],  0);
                break;
            case YT_IGRACL_TEMPLATE_SDEI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_SDEI],  0);
                break;
            case YT_IGRACL_TEMPLATE_SPRI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_SPRI],  0);
                break;
            case YT_IGRACL_TEMPLATE_STAG_FMT:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_STAG_FMT],  0);
                break;
            case YT_IGRACL_TEMPLATE_SVID:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_SVID],  0);
                break;
            case YT_IGRACL_TEMPLATE_CVID:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_CVID],  0);
                break;
            case YT_IGRACL_TEMPLATE_IPV4_DA:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV4_DA],  0);
                break;
            case YT_IGRACL_TEMPLATE_IPV4_SA:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV4_SA],  0);
                break;
            case YT_IGRACL_TEMPLATE_L4_DPORT:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_L4_DPORT],  0);
                break;
            case YT_IGRACL_TEMPLATE_L4_SPORT:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_L4_SPORT],  0);
                break;
            case YT_IGRACL_TEMPLATE_PPPOE_FLAG:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_PPPOE_FLAG],  0);
                break;
            case YT_IGRACL_TEMPLATE_TCP_FLAGS:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_TCP_FLAGS],  0);
                break;
            case YT_IGRACL_TEMPLATE_IP_PROTOCOL:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IP_PROTOCOL],  0);
                break;
            case YT_IGRACL_TEMPLATE_TOS:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_TOS],  0);
                break;
            case YT_IGRACL_TEMPLATE_L3_TYPE:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_L3_TYPE],  0);
                break;
            case YT_IGRACL_TEMPLATE_L4_TYPE:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_L4_TYPE],  0);
                break;
            case YT_IGRACL_TEMPLATE_IP_FRAGMENT:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IP_FRAGMENT],  0);
                break;
            case YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT],  0);
                break;
            case YT_IGRACL_TEMPLATE_IPV6_DA:
                if((pCurData->mask[0]) || (pCurData->mask[1]) ||(pCurData->mask[2])||(pCurData->mask[3]))
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_DA],  0);
                }
                if(pCurData->mask[4] || pCurData->mask[5] || pCurData->mask[6] || pCurData->mask[7] )
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_DA],  1);
                }
                if(pCurData->mask[8] || pCurData->mask[9] || pCurData->mask[10] || pCurData->mask[11] )
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_DA],  2);
                }
                if(pCurData->mask[12] || pCurData->mask[13] || pCurData->mask[14] || pCurData->mask[15] )
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_DA],  3);
                }
                break;
            case YT_IGRACL_TEMPLATE_IPV6_SA:
                if((pCurData->mask[0]) || (pCurData->mask[1]) ||(pCurData->mask[2])||(pCurData->mask[3]))
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_SA],  0);
                }
                if(pCurData->mask[4] || pCurData->mask[5] || pCurData->mask[6] || pCurData->mask[7] )
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_SA],  1);
                }
                if(pCurData->mask[8] || pCurData->mask[9] || pCurData->mask[10] || pCurData->mask[11] )
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_SA],  2);
                }
                if(pCurData->mask[12] || pCurData->mask[13] || pCurData->mask[14] || pCurData->mask[15] )
                {
                    SET_BIT( flags[YT_IGRACL_TEMPLATE_IPV6_SA],  3);
                }
                break;
            case YT_IGRACL_TEMPLATE_IP_OPTION:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IP_OPTION],  0);
                break;

            case YT_IGRACL_TEMPLATE_IS_IGMP:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_IS_IGMP],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_0:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_0],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_1:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_1],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_2:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_2],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_3:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_3],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_4:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_4],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_5:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_5],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_6:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_6],  0);
                break;
            case YT_IGRACL_TEMPLATE_UDF_7:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_UDF_7],  0);
                break;

            case YT_IGRACL_TEMPLATE_INNER_CVID:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_INNER_CVID],  0);
                break;
            case YT_IGRACL_TEMPLATE_INNER_SVID:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_INNER_SVID],  0);
                break;
            case YT_IGRACL_TEMPLATE_INNER_SPRI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_INNER_SPRI],  0);
                break;
            case YT_IGRACL_TEMPLATE_INNER_CPRI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_INNER_CPRI],  0);
                break;
            case YT_IGRACL_TEMPLATE_INNER_SDEI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_INNER_SDEI],  0);
                break;
            case YT_IGRACL_TEMPLATE_INNER_CDEI:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_INNER_CDEI],  0);
                break;
            case YT_IGRACL_TEMPLATE_ETHER_TYPE:
                SET_BIT( flags[YT_IGRACL_TEMPLATE_ETHER_TYPE],  0);
                break;
            default:
                break;
        }

        if(pCurData)
        {
            pCurData = pCurData->pNext;
        }
    }
    return CMM_ERR_OK;
}
static uint32_t  fal_tiger_acl_data_init(uint16_t  prio, yt_port_mask_t portmask, yt_bool_t revert_en)
{
    uint8_t index = 0;

    head.size   =   0;
    head.head  =    NULL;

    osal_memset(flags, sizeof(flags), 0, sizeof(flags));
    
    osal_memset(&mac_da0_rule0, sizeof(mac_da0_rule0), 0, sizeof(mac_da0_rule0));
    HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_PRIOf, &mac_da0_rule0, prio);
    HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_LOGIC_NOTf, &mac_da0_rule0, revert_en);
    HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_SRC_PORT_MASKf, &mac_da0_rule0, portmask.portbits[0]);
    HAL_FIELD_SET(MAC_DA0_RULEm, MAC_DA0_RULE_RULE_TYPEf, &mac_da0_rule0, ACL_RULE_MAC_DA0);
    osal_memset(&mac_da0_mask, sizeof(mac_da0_mask), 0, sizeof(mac_da0_mask));
    
    osal_memset(&mac_sa0_rule0, sizeof(mac_sa0_rule0), 0, sizeof(mac_sa0_rule0));
    HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_PRIOf, &mac_sa0_rule0, prio);
    HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_LOGIC_NOTf, &mac_sa0_rule0, revert_en);
    HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_SRC_PORT_MASKf, &mac_sa0_rule0, portmask.portbits[0]);
    HAL_FIELD_SET(MAC_SA0_RULEm, MAC_SA0_RULE_RULE_TYPEf, &mac_sa0_rule0, ACL_RULE_MAC_SA0);
    osal_memset(&mac_sa0_rule_mask, sizeof(mac_sa0_rule_mask), 0, sizeof(mac_sa0_rule_mask));
    
    osal_memset(&mac_da1_sa1_rule, sizeof(mac_da1_sa1_rule), 0, sizeof(mac_da1_sa1_rule));
    HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_PRIOf, &mac_da1_sa1_rule, prio);
    HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_LOGIC_NOTf, &mac_da1_sa1_rule, revert_en);
    HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_SRC_PORT_MASKf, &mac_da1_sa1_rule, portmask.portbits[0]);
    HAL_FIELD_SET(MAC_DA1_SA1_RULEm, MAC_DA1_SA1_RULE_RULE_TYPEf, &mac_da1_sa1_rule, ACL_RULE_MAC_DA1_SA1);
    osal_memset(&mac_da1_sa1_rule_mask, sizeof(mac_da1_sa1_rule_mask), 0, sizeof(mac_da1_sa1_rule_mask));
    
    osal_memset(&vlan_rule, sizeof(vlan_rule), 0, sizeof(vlan_rule));
    HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_PRIOf, &vlan_rule, prio);
    HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_LOGIC_NOTf, &vlan_rule, revert_en);
    HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_SRC_PORT_MASKf, &vlan_rule, portmask.portbits[0]);
    HAL_FIELD_SET(VLAN_RULEm, VLAN_RULE_RULE_TYPEf, &vlan_rule, ACL_RULE_VLAN_TAG);
    osal_memset(&vlan_rule_mask, sizeof(vlan_rule_mask), 0, sizeof(vlan_rule_mask));
    
    osal_memset(&ipv4_da_rule, sizeof(ipv4_da_rule), 0, sizeof(ipv4_da_rule));
    HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_PRIOf, &ipv4_da_rule, prio);
    HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_LOGIC_NOTf, &ipv4_da_rule, revert_en);
    HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_SRC_PORT_MASKf, &ipv4_da_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV4_DA_RULEm, IPV4_DA_RULE_RULE_TYPEf, &ipv4_da_rule, ACL_RULE_IPV4_DA);
    osal_memset(&ipv4_da_rule_mask, sizeof(ipv4_da_rule_mask), 0, sizeof(ipv4_da_rule_mask));
    
    osal_memset(&ipv4_sa_rule, sizeof(ipv4_sa_rule), 0, sizeof(ipv4_sa_rule));
    HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_PRIOf, &ipv4_sa_rule, prio);
    HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_LOGIC_NOTf, &ipv4_sa_rule, revert_en);
    HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_SRC_PORT_MASKf, &ipv4_sa_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV4_SA_RULEm, IPV4_SA_RULE_RULE_TYPEf, &ipv4_sa_rule, ACL_RULE_IPV4_SA);
    osal_memset(&ipv4_sa_rule_mask, sizeof(ipv4_sa_rule_mask), 0, sizeof(ipv4_sa_rule_mask));
    
    osal_memset(&l4_port_rule, sizeof(l4_port_rule), 0, sizeof(l4_port_rule));
    HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_PRIOf, &l4_port_rule, prio);
    HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_LOGIC_NOTf, &l4_port_rule, revert_en);
    HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_SRC_PORT_MASKf, &l4_port_rule, portmask.portbits[0]);
    HAL_FIELD_SET(L4_PORT_RULEm, L4_PORT_RULE_RULE_TYPEf, &l4_port_rule, ACL_RULE_L4_PORT);
    osal_memset(&l4_port_rule_mask, sizeof(l4_port_rule_mask), 0, sizeof(l4_port_rule_mask));
    
    osal_memset(&misc_rule, sizeof(misc_rule), 0, sizeof(misc_rule));
    HAL_FIELD_SET(MISC_RULEm, MISC_RULE_PRIOf, &misc_rule, prio);
    HAL_FIELD_SET(MISC_RULEm, MISC_RULE_LOGIC_NOTf, &misc_rule, revert_en);
    HAL_FIELD_SET(MISC_RULEm, MISC_RULE_SRC_PORT_MASKf, &misc_rule, portmask.portbits[0]);
    HAL_FIELD_SET(MISC_RULEm, MISC_RULE_RULE_TYPEf, &misc_rule, ACL_RULE_MISC);
    osal_memset(&misc_rule_mask, sizeof(misc_rule_mask), 0, sizeof(misc_rule_mask));
    
    osal_memset(&vid_pri_dei_rule, sizeof(vid_pri_dei_rule), 0, sizeof(vid_pri_dei_rule));
    HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_PRIOf, &vid_pri_dei_rule, prio);
    HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_LOGIC_NOTf, &vid_pri_dei_rule, revert_en);
    HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_SRC_PORT_MASKf, &vid_pri_dei_rule, portmask.portbits[0]);
    HAL_FIELD_SET(VID_PRI_DEI_RULEm, VID_PRI_DEI_RULE_RULE_TYPEf, &vid_pri_dei_rule, ACL_RULE_VID_PRI_DEI);
    osal_memset(&vid_pri_dei_rule_mask, sizeof(vid_pri_dei_rule_mask), 0, sizeof(vid_pri_dei_rule_mask));
    
    osal_memset(&ipv6_da0_rule, sizeof(ipv6_da0_rule), 0, sizeof(ipv6_da0_rule));
    HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_PRIOf, &ipv6_da0_rule, prio);
    HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_LOGIC_NOTf, &ipv6_da0_rule, revert_en);
    HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_SRC_PORT_MASKf, &ipv6_da0_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_DA0_RULEm, IPV6_DA0_RULE_RULE_TYPEf, &ipv6_da0_rule, ACL_RULE_IPV6_DA0);
    osal_memset(&ipv6_da0_rule_mask, sizeof(ipv6_da0_rule_mask), 0, sizeof(ipv6_da0_rule_mask));
    
    osal_memset(&ipv6_da1_rule, sizeof(ipv6_da1_rule), 0, sizeof(ipv6_da1_rule));
    HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_PRIOf, &ipv6_da1_rule, prio);
    HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_LOGIC_NOTf, &ipv6_da1_rule, revert_en);
    HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_SRC_PORT_MASKf, &ipv6_da1_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_DA1_RULEm, IPV6_DA1_RULE_RULE_TYPEf, &ipv6_da1_rule, ACL_RULE_IPV6_DA1);
    osal_memset(&ipv6_da1_rule_mask, sizeof(ipv6_da1_rule_mask), 0, sizeof(ipv6_da1_rule_mask));
    
    osal_memset(&ipv6_da2_rule, sizeof(ipv6_da2_rule), 0, sizeof(ipv6_da2_rule));
    HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_PRIOf, &ipv6_da2_rule, prio);
    HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_LOGIC_NOTf, &ipv6_da2_rule, revert_en);
    HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_SRC_PORT_MASKf, &ipv6_da2_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_DA2_RULEm, IPV6_DA2_RULE_RULE_TYPEf, &ipv6_da2_rule, ACL_RULE_IPV6_DA2);
    osal_memset(&ipv6_da2_rule_mask, sizeof(ipv6_da2_rule_mask), 0, sizeof(ipv6_da2_rule_mask));
    
    osal_memset(&ipv6_da3_rule, sizeof(ipv6_da3_rule), 0, sizeof(ipv6_da3_rule));
    HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_PRIOf, &ipv6_da3_rule, prio);
    HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_LOGIC_NOTf, &ipv6_da3_rule, revert_en);
    HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_SRC_PORT_MASKf, &ipv6_da3_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_DA3_RULEm, IPV6_DA3_RULE_RULE_TYPEf, &ipv6_da3_rule, ACL_RULE_IPV6_DA3);
    osal_memset(&ipv6_da3_rule_mask, sizeof(ipv6_da3_rule_mask), 0, sizeof(ipv6_da3_rule_mask));

    osal_memset(&ipv6_sa0_rule, sizeof(ipv6_sa0_rule), 0, sizeof(ipv6_sa0_rule));
    HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_PRIOf, &ipv6_sa0_rule, prio);
    HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_LOGIC_NOTf, &ipv6_sa0_rule, revert_en);
    HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_SRC_PORT_MASKf, &ipv6_sa0_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_SA0_RULEm, IPV6_SA0_RULE_RULE_TYPEf, &ipv6_sa0_rule, ACL_RULE_IPV6_SA0);
    osal_memset(&ipv6_sa0_rule_mask, sizeof(ipv6_sa0_rule_mask), 0, sizeof(ipv6_sa0_rule_mask));
    
    osal_memset(&ipv6_sa1_rule, sizeof(ipv6_sa1_rule), 0, sizeof(ipv6_sa1_rule));
    HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_PRIOf, &ipv6_sa1_rule, prio);
    HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_LOGIC_NOTf, &ipv6_sa1_rule, revert_en);
    HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_SRC_PORT_MASKf, &ipv6_sa1_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_SA1_RULEm, IPV6_SA1_RULE_RULE_TYPEf, &ipv6_sa1_rule, ACL_RULE_IPV6_SA1);
    osal_memset(&ipv6_sa1_rule_mask, sizeof(ipv6_sa1_rule_mask), 0, sizeof(ipv6_sa1_rule_mask));
    
    osal_memset(&ipv6_sa2_rule, sizeof(ipv6_sa2_rule), 0, sizeof(ipv6_sa2_rule));
    HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_PRIOf, &ipv6_sa2_rule, prio);
    HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_LOGIC_NOTf, &ipv6_sa2_rule, revert_en);
    HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_SRC_PORT_MASKf, &ipv6_sa2_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_SA2_RULEm, IPV6_SA2_RULE_RULE_TYPEf, &ipv6_sa2_rule, ACL_RULE_IPV6_SA2);
    osal_memset(&ipv6_sa2_rule_mask, sizeof(ipv6_sa2_rule_mask), 0, sizeof(ipv6_sa2_rule_mask));
    
    osal_memset(&ipv6_sa3_rule, sizeof(ipv6_sa3_rule), 0, sizeof(ipv6_sa3_rule));
    HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_PRIOf, &ipv6_sa3_rule, prio);
    HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_LOGIC_NOTf, &ipv6_sa3_rule, revert_en);
    HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_SRC_PORT_MASKf, &ipv6_sa3_rule, portmask.portbits[0]);
    HAL_FIELD_SET(IPV6_SA3_RULEm, IPV6_SA3_RULE_RULE_TYPEf, &ipv6_sa3_rule, ACL_RULE_IPV6_SA3);
    osal_memset(&ipv6_sa3_rule_mask, sizeof(ipv6_sa3_rule_mask), 0, sizeof(ipv6_sa3_rule_mask));
    
    for(index=0; index <8; index++)
    {
        osal_memset(&udf_rule[index], sizeof(udf_rule[index]), 0, sizeof(udf_rule[index]));
        HAL_FIELD_SET(UDF_RULEm, UDF_RULE_PRIOf, &udf_rule[index], prio);
        HAL_FIELD_SET(UDF_RULEm, UDF_RULE_LOGIC_NOTf, &udf_rule[index], revert_en);
        HAL_FIELD_SET(UDF_RULEm, UDF_RULE_SRC_PORT_MASKf, &udf_rule[index], portmask.portbits[0]);
        HAL_FIELD_SET(UDF_RULEm, UDF_RULE_RULE_TYPEf, &udf_rule[index], ACL_RULE_UDF0 + index);
        osal_memset(&udf_rule_mask[index], sizeof(udf_rule_mask[index]), 0, sizeof(udf_rule_mask[index]));
    }
    osal_memset(&ether_type_value_rule, sizeof(ether_type_value_rule), 0, sizeof(ether_type_value_rule));
    HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_PRIOf, &ether_type_value_rule, prio);
    HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_LOGIC_NOTf, &ether_type_value_rule, revert_en);
    HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_SRC_PORT_MASKf, &ether_type_value_rule, portmask.portbits[0]);
    HAL_FIELD_SET(ETHER_TYPE_VALUE_RULEm, ETHER_TYPE_VALUE_RULE_RULE_TYPEf, &ether_type_value_rule, ACL_RULE_ETHERTYPE_VALUE);
    osal_memset(&ether_type_value_rule_mask, sizeof(ether_type_value_rule_mask), 0, sizeof(ether_type_value_rule_mask));

    return CMM_ERR_OK;
}
static uint32_t fal_tiger_acl_data_rangeSet(yt_unit_t unit,uint8_t group, uint8_t offset, uint8_t groupID)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32 rule_ext_ctrl = 0;
    uint8_t grpId = 0;
    uint8_t binId = 0;
    acl_blk_keep_ctrl_t keep_ctrl;
    data_store_t *entryPtr;

    CMM_UNUSED_PARAM(offset);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, ACL_BLK_KEEP_CTRLm, 0, sizeof(keep_ctrl), &keep_ctrl), ret);

    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP0f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP1f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP2f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP3f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP4f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP5f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP6f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP7f, &keep_ctrl, 1);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RULE_EXT_CTRLm, group, sizeof(rule_ext_ctrl), &rule_ext_ctrl), ret);

    grpId   =   groupID;
    entryPtr = database_header[unit][group].head;
    while(entryPtr)
    {
        if(entryPtr->grpID == grpId)
        {
            if (group == 0)
                binId = entryPtr->entryIdx+1;
            else
                binId = entryPtr->entryIdx;
            rule_ext_ctrl |= ((grpId<<((binId<<2)+1))|(1<<(binId<<2)));
            switch(binId)
            {
                case 0:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP0f, &keep_ctrl, 0);
                    break;
                case 1:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP1f, &keep_ctrl, 0);
                    break;
                case 2:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP2f, &keep_ctrl, 0);
                    break;
                case 3:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP3f, &keep_ctrl, 0);
                    break;
                case 4:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP4f, &keep_ctrl, 0);
                    break;
                case 5:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP5f, &keep_ctrl, 0);
                    break;
                case 6:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP6f, &keep_ctrl, 0);
                    break;
                case 7:
                    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP7f, &keep_ctrl, 0);
                    break;
            }
        }
        entryPtr = entryPtr->next;
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_BLK_KEEP_CTRLm, 0, sizeof(keep_ctrl), &keep_ctrl), ret);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RULE_EXT_CTRLm, group, sizeof(rule_ext_ctrl), &rule_ext_ctrl), ret);
    
    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_data_rangeUnSet(yt_unit_t unit,uint8_t group, uint8_t offset)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32 rule_ext_ctrl = 0;
    uint8_t binId = 0;
    acl_blk_keep_ctrl_t keep_ctrl;

    if (group == 0)
    {
        binId = offset+1;
    }
    else
    {
        binId = offset;
    }
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP0f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP1f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP2f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP3f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP4f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP5f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP6f, &keep_ctrl, 1);
    HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP7f, &keep_ctrl, 1);
    
    switch(binId)
    {
        case 0:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP0f, &keep_ctrl, 0);
            break;
        case 1:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP1f, &keep_ctrl, 0);
            break;
        case 2:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP2f, &keep_ctrl, 0);
            break;
        case 3:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP3f, &keep_ctrl, 0);
            break;
        case 4:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP4f, &keep_ctrl, 0);
            break;
        case 5:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP5f, &keep_ctrl, 0);
            break;
        case 6:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP6f, &keep_ctrl, 0);
            break;
        case 7:
            HAL_FIELD_SET(ACL_BLK_KEEP_CTRLm, ACL_BLK_KEEP_CTRL_KEEP7f, &keep_ctrl, 0);
            break;
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_BLK_KEEP_CTRLm, 0, sizeof(keep_ctrl), &keep_ctrl), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, RULE_EXT_CTRLm, group, sizeof(rule_ext_ctrl), &rule_ext_ctrl), ret);
    rule_ext_ctrl &= ~(0xF <<(binId<<2));
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, RULE_EXT_CTRLm, group, sizeof(rule_ext_ctrl), &rule_ext_ctrl), ret);
    return CMM_ERR_OK;
}

static uint32_t _fal_tiger_acl_action_set(yt_unit_t unit,acl_action_t action,uint8_t group, uint8_t offset)
{
    cmm_err_t ret = CMM_ERR_OK;
    acl_action_tbl_t entry;
    uint32_t    lookup_index_base;
    uint32      gpio_pin = 0;

    if (group == 0)
        lookup_index_base = (group<<3)+offset+1;
    else
        lookup_index_base = (group<<3)+offset;

    gpio_pin = (((action.RESERVED0 & ACL_1_BIT_MASK) << 3) | ((action.FWD_SVID_EN & ACL_1_BIT_MASK) << 2) |
        ((action.FWD_CVID_EN & ACL_1_BIT_MASK) << 1) | (action.CPU_CTRL_BYPASS & ACL_1_BIT_MASK));
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_INTR_ENf, &entry, action.INTR_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_GPIO_PINf, &entry, gpio_pin);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_GPIO_ENf, &entry, action.GPIO_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_FLOW_STATS_PTRf, &entry, action.FLOW_STATS_PTR);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_FLOW_STATS_ENf, &entry, action.FLOW_STATS_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_MIRROR_ENf, &entry, action.MIRROR_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_METER_ENf, &entry, action.METER_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_METER_IDf, &entry, action.METER_ID);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_DSCP_REPLACE_ENf, &entry, action.DSCP_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_DSCPf, &entry, action.DSCP);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_INT_DP_VALIDf, &entry, action.INT_DP_VALID);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_INT_DPf, &entry, action.INT_DP);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_INT_PRI_VALIDf, &entry, action.INT_PRI_VALID);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_INT_PRI_0f, &entry, (action.INT_PRI &0x1));
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_INT_PRI_1f, &entry, (action.INT_PRI >>1));
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CVID_REPLACE_ENf, &entry, action.CVID_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CVIDf, &entry, action.CVID);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CPRI_REPLACE_ENf, &entry, action.CPRI_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CPRIf, &entry, action.CPRI);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CDEI_REPLACE_ENf, &entry, action.CDEI_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CDEIf, &entry, action.CDEI);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SVID_REPLACE_ENf, &entry, action.SVID_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SVID_0f, &entry, (action.SVID & 0x1FF));
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SVID_1f, &entry, (action.SVID>>9));
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SPRI_REPLACE_ENf, &entry, action.SPRI_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SPRIf, &entry, action.SPRI);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SDEI_REPLACE_ENf, &entry, action.SDEI_REPLACE_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_SDEIf, &entry, action.SDEI);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_FWD_DECISION_ENf, &entry, action.FWD_DECISION_EN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_FWD_DECISION_TYPEf, &entry, action.FWD_DECISION_TYPE);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_DEST_PORT_MASKf, &entry, action.DEST_PORT_MASK);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_CTAG_ASSIGNf, &entry, action.CTAG_ASSIGN);
    HAL_FIELD_SET(ACL_ACTION_TBLm, ACL_ACTION_TBL_STAG_ASSIGNf, &entry, action.STAG_ASSIGN);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_ACTION_TBLm, lookup_index_base, sizeof(entry), &entry), ret);
    return CMM_ERR_OK;
}

/*
static void showFlags()
{
    
    uint16_t index = 0;

    osal_printf("\n---------------------flags------------------\n");
    for(index=0; index<YT_IGRACL_TEMPLATE_MAX; index++)
    {
        osal_printf("%d ", flags[index]);
    }
    osal_printf("\n-----------------------------------------\n");
}
*/

///////////////////////////////////////////////////////////////////////////

static uint32_t  fal_tiger_acl_store_check(yt_unit_t unit, uint8_t *pgroup, uint8_t *poffset)
{
    data_store_t *node = NULL;
    uint8_t firstOffset = 0xF;
    uint8_t grpId = 0;
    uint8_t binIdx = 0;
    data_store_t *entryPtr, *tmpNext;

    for(binIdx=0; binIdx<ACL_LINE_NUM; binIdx++)
    {
        entryPtr = database_header[unit][binIdx].head;
        while(entryPtr)
        {
            tmpNext = entryPtr;
            grpId = tmpNext->grpID;
            node = head.head;
            firstOffset = 0xF;
            while(node)
            {
                if(tmpNext == NULL)
                {
                    return CMM_ERR_FAIL;
                }
                if(!((grpId == tmpNext->grpID)
                        &&(tmpNext->data_entry[0]==node->data_entry[0]) 
                        && (tmpNext->data_entry[1]==node->data_entry[1])
                        && (tmpNext->data_mask[0]==node->data_mask[0])
                        &&(tmpNext->data_mask[1]==node->data_mask[1])))
                    {        
                        tmpNext = tmpNext->next;
                        continue;
                    }
                if(firstOffset == 0xF)
                {
                    firstOffset =   tmpNext->entryIdx;
                }
                node = node->next;
                tmpNext = tmpNext->next;
            }
            if(node == NULL)
            {
                *pgroup  =    binIdx;
                *poffset    =   firstOffset;
                return CMM_ERR_OK;
            }
            entryPtr = entryPtr->next;
        }
    }
   return CMM_ERR_FAIL;
}

static uint32_t fal_tiger_acl_positon_search(yt_unit_t unit, uint8_t needNums, uint8_t *pgroup, uint8_t *pindex)
{
    uint8_t usedNums = 0;
    uint8_t binIdx = 0;
    data_store_t *entryPtr;
    uint8_t maxNums = EXT_NUM;

    for(binIdx=0; binIdx<ACL_LINE_NUM; binIdx++)
    {
        usedNums = 0;
        if (binIdx == 0)
            maxNums = EXT_NUM-1;
        else
            maxNums = EXT_NUM;
        if((maxNums-database_header[unit][binIdx].size) >=needNums)
        {
            entryPtr = database_header[unit][binIdx].head;
            if(entryPtr ==NULL)
            {
                *pgroup =   binIdx;
                *pindex = 0;
                return CMM_ERR_OK;
            }
            while(entryPtr)
            {
                if(entryPtr->entryIdx != usedNums)
                {
                    *pgroup =   binIdx;
                    *pindex =   usedNums;
                    return CMM_ERR_OK;
                }
                usedNums++;
                entryPtr = entryPtr->next;
            }
            if(entryPtr == NULL)
            {
                *pgroup =   binIdx;
                *pindex =   usedNums;
                return CMM_ERR_OK;
            }
        }
   }
    return CMM_ERR_FAIL;
}

static uint32_t fal_tiger_acl_tbl_write(yt_unit_t unit, uint8_t rule_type, uint8_t group, uint8_t offset)
{
    cmm_err_t ret = CMM_ERR_OK;

    uint32_t acl_rule_bin_idx[] =  { 
        ACL_RULE_BIN0m, ACL_RULE_BIN1m, ACL_RULE_BIN2m, ACL_RULE_BIN3m,
        ACL_RULE_BIN4m, ACL_RULE_BIN5m, ACL_RULE_BIN6m, ACL_RULE_BIN7m,
    };
    
    uint32_t acl_rule_msk_bin_idx[] =  { 
        ACL_RULE_MASK_BIN0m, ACL_RULE_MASK_BIN1m, ACL_RULE_MASK_BIN2m, ACL_RULE_MASK_BIN3m,
        ACL_RULE_MASK_BIN4m, ACL_RULE_MASK_BIN5m, ACL_RULE_MASK_BIN6m, ACL_RULE_MASK_BIN7m,
    };

    switch(rule_type)
    {
    
        case ACL_RULE_MAC_DA0:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(mac_da0_rule0), &mac_da0_rule0), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(mac_da0_mask), &mac_da0_mask), ret);
            break;
            
        case ACL_RULE_MAC_SA0:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(mac_sa0_rule0), &mac_sa0_rule0), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(mac_sa0_rule_mask), &mac_sa0_rule_mask), ret);
            break;
        case ACL_RULE_MAC_DA1_SA1:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(mac_da1_sa1_rule), &mac_da1_sa1_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(mac_da1_sa1_rule_mask), &mac_da1_sa1_rule_mask), ret);
            break;
        case ACL_RULE_VLAN_TAG:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(vlan_rule), &vlan_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(vlan_rule_mask), &vlan_rule_mask), ret);
            break;
        case ACL_RULE_IPV4_DA:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv4_da_rule), &ipv4_da_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv4_da_rule_mask), &ipv4_da_rule_mask), ret);
            break;
        case ACL_RULE_IPV4_SA:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv4_sa_rule), &ipv4_sa_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv4_sa_rule_mask), &ipv4_sa_rule_mask), ret);
            break;
        case ACL_RULE_L4_PORT:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(l4_port_rule), &l4_port_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(l4_port_rule_mask), &l4_port_rule_mask), ret);
            break;
        case ACL_RULE_MISC:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(misc_rule), &misc_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(misc_rule_mask), &misc_rule_mask), ret);
            break;
        case ACL_RULE_IPV6_DA0:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_da0_rule), &ipv6_da0_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_da0_rule_mask), &ipv6_da0_rule_mask), ret);
            break;
        case ACL_RULE_IPV6_DA1:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_da1_rule), &ipv6_da1_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_da1_rule_mask), &ipv6_da1_rule_mask), ret);
            break;
         case ACL_RULE_IPV6_DA2:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_da2_rule), &ipv6_da2_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_da2_rule_mask), &ipv6_da2_rule_mask), ret);
            break;
         case ACL_RULE_IPV6_DA3:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_da3_rule), &ipv6_da3_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_da3_rule_mask), &ipv6_da3_rule_mask), ret);
            break;
        case ACL_RULE_IPV6_SA0:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_sa0_rule), &ipv6_sa0_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_sa0_rule_mask), &ipv6_sa0_rule_mask), ret);
            break;
        case ACL_RULE_IPV6_SA1:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_sa1_rule), &ipv6_sa1_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_sa1_rule_mask), &ipv6_sa1_rule_mask), ret);
            break;
        case ACL_RULE_IPV6_SA2:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_sa2_rule), &ipv6_sa2_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_sa2_rule_mask), &ipv6_sa2_rule_mask), ret);
            break;
        case ACL_RULE_IPV6_SA3:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ipv6_sa3_rule), &ipv6_sa3_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ipv6_sa3_rule_mask), &ipv6_sa3_rule_mask), ret);
            break;
        case ACL_RULE_VID_PRI_DEI:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(vid_pri_dei_rule), &vid_pri_dei_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(vid_pri_dei_rule_mask), &vid_pri_dei_rule_mask), ret);
            break;
        case ACL_RULE_ETHERTYPE_VALUE:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(ether_type_value_rule), &ether_type_value_rule), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(ether_type_value_rule_mask), &ether_type_value_rule_mask), ret);
            break;
        case ACL_RULE_UDF0:
        case ACL_RULE_UDF1:
        case ACL_RULE_UDF2:
        case ACL_RULE_UDF3:
        case ACL_RULE_UDF4:
        case ACL_RULE_UDF5:
        case ACL_RULE_UDF6:
        case ACL_RULE_UDF7:
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[offset], group, sizeof(udf_rule[rule_type-ACL_RULE_UDF0]), &udf_rule[rule_type-ACL_RULE_UDF0]), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[offset], group, sizeof(udf_rule_mask[rule_type-ACL_RULE_UDF0]), &udf_rule_mask[rule_type-ACL_RULE_UDF0]), ret);
            break;
    }
    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_groupID_gen(yt_unit_t unit,uint8_t group)
{
    data_store_t *entryPtr;
    uint8_t tmpOffset = 0;
    uint8_t  groupId = 0xF;
    uint8_t  usedFlag = 0;

    entryPtr = database_header[unit][group].head;
    while(entryPtr)
    {
        usedFlag |= 1<<(entryPtr->grpID);
        entryPtr = entryPtr->next;
    }

    for(tmpOffset=0; tmpOffset<EXT_NUM; tmpOffset++)
    {
        if((usedFlag & (1<<tmpOffset))==0)
        {
            groupId = tmpOffset;
            break;
        }
    }
  
    return groupId;
}
static uint32_t fal_tiger_acl_data_store(yt_unit_t unit,uint8_t number,uint8_t group, uint8_t offset, uint8_t *groupID)
{
    data_store_t *node = NULL;
    uint8_t tmpOffset = 0;
    uint8_t  groupId = 0;
    data_store_t *entryPtr = NULL, *entryPrev = NULL, *tmpNode = NULL;

    CMM_UNUSED_PARAM(number);
    CMM_UNUSED_PARAM(offset);

    node = head.head;
    if(!node)
    {
        return CMM_ERR_FAIL;
    }
    groupId =  fal_tiger_acl_groupID_gen(unit, group);
    *groupID = groupId;
    if(groupId == 0xF)
    {
        return CMM_ERR_FAIL;
    }
    entryPtr = database_header[unit][group].head;
    if(entryPtr == NULL)
    {
        tmpOffset = 0;
        database_header[unit][group].head = node;
        while(node)
        {
            node->grpID = groupId;
            database_header[unit][group].size++;
            if (group == 0)
                fal_tiger_acl_tbl_write(unit, node->rule_type, group, tmpOffset+1);
            else
                fal_tiger_acl_tbl_write(unit, node->rule_type, group, tmpOffset);
            tmpOffset++;
            node = node->next;
        }
        return CMM_ERR_OK;
    }
    tmpOffset = 0;
    while(entryPtr && node)
    {  
        if(entryPtr->entryIdx != tmpOffset)
        {
            tmpNode = node;
            node = node->next;
            
            tmpNode->entryIdx = tmpOffset;
            tmpNode->grpID = groupId;
            tmpNode->next = NULL;
            if(tmpOffset == 0)
            {
                database_header[unit][group].head = tmpNode;
            }
            if(entryPrev != NULL)
            {
                entryPrev->next = tmpNode;
            }

            if (group == 0)
                fal_tiger_acl_tbl_write(unit, tmpNode->rule_type, group, tmpOffset+1);
            else
                fal_tiger_acl_tbl_write(unit, tmpNode->rule_type, group, tmpOffset);
            tmpNode->next = entryPtr;
            tmpOffset++;
            continue;
        }
        tmpOffset++;
        entryPrev = entryPtr;
        entryPtr = entryPtr->next;
    }
    if(entryPtr == NULL)
    {
        entryPrev->next = node;
        while(node)
        {
            node->entryIdx = tmpOffset;
            node->grpID = groupId;
            if (group == 0)
                fal_tiger_acl_tbl_write(unit, node->rule_type, group,  tmpOffset+1);
            else
                fal_tiger_acl_tbl_write(unit, node->rule_type, group,  tmpOffset);
            tmpOffset++;
            node = node->next;
        }
    }
    database_header[unit][group].size += head.size;
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_acl_table_del_entry(yt_unit_t unit, uint32_t id)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t index = 0;
    uint8_t group = 0;
    uint8_t binId = 0;
    uint8_t groupId = 0xF;
    acl_action_t action;
    acl_blk_cmd_t blk_cmd;
    uint32_t actualId = 0;
    uint32 data_entry[ACL_ENTRY_WIDTH_PER32BITS] = {0};
    uint32 mask_entry[ACL_MASK_WIDTH_PER32BITS] = {0};
    uint32_t acl_rule_bin_idx[] =  { 
        ACL_RULE_BIN0m, ACL_RULE_BIN1m, ACL_RULE_BIN2m, ACL_RULE_BIN3m,
        ACL_RULE_BIN4m, ACL_RULE_BIN5m, ACL_RULE_BIN6m, ACL_RULE_BIN7m,
    };
    
    uint32_t acl_rule_msk_bin_idx[] =  { 
        ACL_RULE_MASK_BIN0m, ACL_RULE_MASK_BIN1m, ACL_RULE_MASK_BIN2m, ACL_RULE_MASK_BIN3m,
        ACL_RULE_MASK_BIN4m, ACL_RULE_MASK_BIN5m, ACL_RULE_MASK_BIN6m, ACL_RULE_MASK_BIN7m,
    };
    data_store_t *entryPtr, *entryNext, *entryHeader= NULL, *entryPrev = NULL;

    actualId = (id < 7) ? id : (id+1);
    group = (actualId >> 3) & 0x3F;
    index = actualId & 0x7;
    if(group >= ACL_LINE_NUM)
    {
        return CMM_ERR_FAIL;
    }

    HAL_FIELD_SET(ACL_BLK_CMDm, ACL_BLK_CMD_CMDf, &blk_cmd, 1);
    HAL_FIELD_SET(ACL_BLK_CMDm, ACL_BLK_CMD_BLKIDf, &blk_cmd, group);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_BLK_CMDm, 0, sizeof(blk_cmd), &blk_cmd), ret);
    entryPtr = database_header[unit][group].head;
    entryHeader = entryPtr;
    while(entryPtr)
    {
        entryNext = entryPtr;
        if(entryNext->entryIdx == index)
        {
            groupId = entryNext->grpID;
        }
        if((groupId != 0xF)&& (entryNext->grpID == groupId))
        {
            CMM_ERR_CHK(fal_tiger_acl_data_rangeUnSet(unit, group, entryNext->entryIdx), ret);
            if (group == 0)
                binId = entryNext->entryIdx+1;
            else
                binId = entryNext->entryIdx;
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_bin_idx[binId], group, sizeof(data_entry), &data_entry), ret);
            CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, acl_rule_msk_bin_idx[binId], group, sizeof(mask_entry), &mask_entry), ret);
            if(entryHeader->entryIdx == entryNext->entryIdx)
            {
                entryHeader = entryPtr->next;
                entryPrev = NULL;
            }
            if(entryPrev != NULL)
            {
                entryPrev->next = entryPtr->next;
            }
            entryPtr= entryPtr->next;
            osal_free(entryNext, YT_MEM_MALLOC_MODULE_ACL);
            entryNext = NULL;
            database_header[unit][group].size--;
            continue;
        }
        entryPrev = entryPtr;
        entryPtr= entryPtr->next;
    }
    database_header[unit][group].head = entryHeader;
    HAL_FIELD_SET(ACL_BLK_CMDm, ACL_BLK_CMD_CMDf, &blk_cmd, 0);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_BLK_CMDm, 0, sizeof(blk_cmd), &blk_cmd), ret);
    if(groupId == 0xF)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }
    osal_memset(&action, sizeof(action), 0, sizeof(action));
    _fal_tiger_acl_action_set(unit, action, group, index);
    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_data_reset(yt_unit_t unit)
{
    yt_igrAcl_data_tlv_t *pTmpData = NULL;
    yt_igrAcl_data_tlv_t *pTmpNext = NULL;

    CMM_UNUSED_PARAM(unit);

    if(pgAclDataList == NULL ||
        pgAclAction == NULL)
    {
        return CMM_ERR_OK;
    }
    ACL_LOCK();
    /*free key list*/
    pTmpData = pgAclDataList;
    while(pTmpData)
    {
        pTmpNext = pTmpData->pNext;
        osal_memset(pTmpData, sizeof(yt_igrAcl_data_tlv_t), 0, sizeof(yt_igrAcl_data_tlv_t));
        osal_free(pTmpData, YT_MEM_MALLOC_MODULE_ACL);
        pTmpData = pTmpNext;
    }
    pgAclDataList = NULL;

    /*reset action list*/
    osal_memset(pgAclAction, sizeof(acl_action_t), 0, sizeof(acl_action_t));
    osal_free(pgAclAction, YT_MEM_MALLOC_MODULE_ACL);
    pgAclAction = NULL;

    gKeyDataNum = 0;
    gSrcPortSetFlag = FALSE;
    /*set all port*/
    CAL_YTPLIST_TO_MLIST(unit, CAL_YT_ALL_PORT_MASK(unit), gSrcPortMask);

    ACL_UNLOCK();

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_action_set(yt_unit_t unit, uint32_t id, acl_action_t *pAction)
{
    uint8_t index = 0;
    uint8_t group = 0;
    uint8_t groupId = 0xF;
    data_store_t *entryPtr;
    uint32_t actualId = 0;

    actualId = (id < 7) ? id : (id+1);
    group = (actualId >> 3) & 0x3F;
    index = actualId & 0x7;
    if(group >= ACL_LINE_NUM)
    {
        return CMM_ERR_INPUT;
    }

    entryPtr = database_header[unit][group].head;
    while(entryPtr)
    {
        if(entryPtr->entryIdx == index)
        {
            groupId = entryPtr->grpID;
        }
        entryPtr = entryPtr->next;
    }
    if(groupId == 0xF)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    _fal_tiger_acl_action_set(unit, *pAction, group, index);

    fal_tiger_acl_data_reset(unit);
    return CMM_ERR_OK;
}

static uint32_t fal_tiger_acl_keys_valid_check(yt_unit_t unit)
{
    yt_igrAcl_data_tlv_t *pTmpData = NULL;

    CMM_UNUSED_PARAM(unit);

    /* check if only one key of cpri or dei */
    ACL_LOCK();
    if(gKeyDataNum == 1)
    {
        pTmpData = pgAclDataList;
    }
    ACL_UNLOCK();

    if(pTmpData == NULL)
    {
        return CMM_ERR_OK;
    }

    switch(pTmpData->type)
    {
        case YT_IGRACL_TEMPLATE_CDEI:
        case YT_IGRACL_TEMPLATE_SDEI:
            if(pTmpData->data.dei.dei_data ==0)
            {
                return CMM_ERR_TOO_LESS_INFO;
            }
            break;
        case YT_IGRACL_TEMPLATE_CPRI:
        case YT_IGRACL_TEMPLATE_SPRI:
            if(pTmpData->data.pri.pri_data ==0)
            {
                return CMM_ERR_TOO_LESS_INFO;
            }
            break;
        default:
            break;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_acl_udf_rule_set(yt_unit_t unit, uint8_t index, yt_acl_udf_type_t type, uint8_t offset)
{
    cmm_err_t ret = CMM_ERR_OK;
    udf_ctrln_t udf_ctrln;

    if(YT_ACL_UDF_NUM_MAX <= index)
    {
        return CMM_ERR_REG_TABLE_IDX;
    }

    HAL_FIELD_SET(UDF_CTRLNm, UDF_CTRLN_UDF_TYPEf, &udf_ctrln, type);
    HAL_FIELD_SET(UDF_CTRLNm, UDF_CTRLN_UDF_OFFSETf, &udf_ctrln, (offset & 0x7F));
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, UDF_CTRLNm, index, sizeof(udf_ctrln), &udf_ctrln), ret);

    return CMM_ERR_OK;
}

/*create data list and action*/
yt_ret_t fal_tiger_acl_rule_init(yt_unit_t unit)
{
    /*create key list*/
    if(pgAclDataList == NULL)
    {
        pgAclDataList = (yt_igrAcl_data_tlv_t *)osal_malloc(sizeof(yt_igrAcl_data_tlv_t), YT_MEM_MALLOC_MODULE_ACL);
        if(pgAclDataList == NULL)
        {
            return CMM_ERR_FAIL;
        }
        osal_memset(pgAclDataList, sizeof(yt_igrAcl_data_tlv_t), 0, sizeof(yt_igrAcl_data_tlv_t));
        gKeyDataNum = 0;
        gSrcPortSetFlag = FALSE;
        /*set all port*/
        CAL_YTPLIST_TO_MLIST(unit, CAL_YT_ALL_PORT_MASK(unit), gSrcPortMask);
    }
    else
    {
        fal_tiger_acl_rule_reset(unit);
    }
    /*reset action list*/
    if(pgAclAction == NULL)
    {
        pgAclAction = (acl_action_t *)osal_malloc(sizeof(acl_action_t), YT_MEM_MALLOC_MODULE_ACL);
        if(pgAclAction == NULL)
        {
            osal_free(pgAclDataList, YT_MEM_MALLOC_MODULE_ACL);
            pgAclDataList = NULL;
            return CMM_ERR_FAIL;
        }
        osal_memset(pgAclAction, sizeof(acl_action_t), 0, sizeof(acl_action_t));
    }
    else
    {
        fal_tiger_acl_rule_reset(unit);
    }

    return CMM_ERR_OK;
}



yt_ret_t fal_tiger_acl_rule_reset(yt_unit_t unit)
{
    fal_tiger_acl_data_reset(unit);

    return CMM_ERR_OK;
}

/*
    check if exist same type;
    check if data entry reaches max num;
    get free or malloc new key data;
*/
static uint32_t fal_tiger_acl_valid_keylist_get(yt_unit_t unit, yt_igrAcl_key_type_t type, yt_igrAcl_data_tlv_t **pKeyTlv, uint8_t *pKey_num)
{
    yt_igrAcl_data_tlv_t *pTmpKey = NULL;
    yt_igrAcl_data_tlv_t *pTmpCur = NULL;
    uint8_t keyNum = 0;
    yt_bool_t getValid = 0;

    CMM_UNUSED_PARAM(unit);

    if(pgAclDataList == NULL)
    {
        return CMM_ERR_NOT_INIT;
    }

    pTmpKey = pgAclDataList;
    while(pTmpKey)
    {
        if(ACL_KEY_MAX_NUM == keyNum++)
        {
            return CMM_ERR_ENTRY_FULL;
        }
        /*get empty entry*/
        if(pTmpKey->type == 0)
        {
            getValid = 1;
            break;
        }

        if(pTmpKey->type == type)
        {
            return CMM_ERR_SAMEENTRY_EXIST;
        }

        pTmpCur = pTmpKey;/*record current one*/
        pTmpKey = pTmpKey->pNext;
    }

    if(getValid)
    {
        *pKey_num = keyNum;
        *pKeyTlv = pTmpKey;
    }
    else /*need to create new one*/
    {
        pTmpKey = (yt_igrAcl_data_tlv_t *)osal_malloc(sizeof(yt_igrAcl_data_tlv_t), YT_MEM_MALLOC_MODULE_ACL);
        if(pTmpKey == NULL)
        {
            return CMM_ERR_FAIL;
        }
        osal_memset(pTmpKey, sizeof(yt_igrAcl_data_tlv_t), 0, sizeof(yt_igrAcl_data_tlv_t));
        pTmpCur->pNext = pTmpKey;
        *pKey_num = keyNum + 1;
        *pKeyTlv = pTmpKey;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_acl_rule_key_add(yt_unit_t unit, yt_igrAcl_key_type_t type, yt_acl_comm_key_t *pKey_data)
{
    yt_igrAcl_data_tlv_t *pKeyData = NULL;
    yt_ret_t ret = CMM_ERR_OK;

    /*create and add key to list*/
    if(pgAclDataList == NULL)
    {
        return CMM_ERR_NOT_INIT;
    }

    if(type < YT_IGRACL_TEMPLATE_MAC_DA ||
        type >= YT_IGRACL_TEMPLATE_MAX)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    ACL_LOCK();

    /* special type handle,does need to record in datalist */
    switch(type)
    {
        case YT_IGRACL_TEMPLATE_SRC_PORTMASK:
        {
            yt_port_mask_t *pPortMask = (yt_port_mask_t *)pKey_data;
            if(CMM_PLIST_VALID(unit, (*pPortMask)) == FALSE)
            {
                ACL_UNLOCK();
                return CMM_ERR_PORTLIST;
            }
            CAL_YTPLIST_TO_MLIST(unit, (*pPortMask), gSrcPortMask);
            gSrcPortSetFlag = TRUE;
            ACL_UNLOCK();
            return CMM_ERR_OK;
        }
        default:
            break;
    }

    ret = fal_tiger_acl_valid_keylist_get(unit, type, &pKeyData, &gKeyDataNum);
    if(ret != CMM_ERR_OK)
    {
        ACL_UNLOCK();
        return ret;
    }

    pKeyData->type = type;
    switch(type)
    {
        case YT_IGRACL_TEMPLATE_MAC_DA:
        case YT_IGRACL_TEMPLATE_MAC_SA:
        {      
            yt_acl_key_mac_t *pKeyMac = (yt_acl_key_mac_t *)pKey_data;
            osal_memcpy(pKeyData->data.mac.mac_data, MAC_ADDR_LEN, pKeyMac->mac_data.addr, MAC_ADDR_LEN);
            osal_memcpy(pKeyData->data.mac.mac_mask, MAC_ADDR_LEN, pKeyMac->mac_mask.addr, MAC_ADDR_LEN);
        }
            break;
        case YT_IGRACL_TEMPLATE_L2_TYPE:
        {      
            yt_acl_key_l2Type_t *pKeyL2 = (yt_acl_key_l2Type_t *)pKey_data;
            pKeyData->data.l2_3_4_type.type_data = (pKeyL2->type_data & ACL_L2_TYPE_BIT_MASK);
            pKeyData->data.l2_3_4_type.type_mask = (pKeyL2->type_mask & ACL_L2_TYPE_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_L3_TYPE:
        {
            yt_acl_key_l3Type_t *pKeyL3 = (yt_acl_key_l3Type_t *)pKey_data;
            pKeyData->data.l2_3_4_type.type_data = (pKeyL3->type_data & ACL_L3_TYPE_BIT_MASK);
            pKeyData->data.l2_3_4_type.type_mask = (pKeyL3->type_mask & ACL_L3_TYPE_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_L4_TYPE:
        {
            yt_acl_key_l4Type_t *pKeyL4 = (yt_acl_key_l4Type_t *)pKey_data;
            pKeyData->data.l2_3_4_type.type_data = (pKeyL4->type_data & ACL_L4_TYPE_BIT_MASK);
            pKeyData->data.l2_3_4_type.type_mask = (pKeyL4->type_mask & ACL_L4_TYPE_BIT_MASK);
        }
            break;  
        case YT_IGRACL_TEMPLATE_CDEI:
        case YT_IGRACL_TEMPLATE_SDEI:
        case YT_IGRACL_TEMPLATE_INNER_SDEI:
        case YT_IGRACL_TEMPLATE_INNER_CDEI:
        {
            yt_acl_key_dei_t *pKeyDei = (yt_acl_key_dei_t *)pKey_data;
            pKeyData->data.dei.dei_data = (pKeyDei->dei_data & ACL_VLAN_DEI_BIT_MASK);
            pKeyData->data.dei.dei_mask = (pKeyDei->dei_mask & ACL_VLAN_DEI_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_CPRI:
        case YT_IGRACL_TEMPLATE_SPRI:
        case YT_IGRACL_TEMPLATE_INNER_SPRI:
        case YT_IGRACL_TEMPLATE_INNER_CPRI:
        {
            yt_acl_key_pri_t *pKeyPri = (yt_acl_key_pri_t *)pKey_data;
            pKeyData->data.pri.pri_data = (pKeyPri->pri_data & ACL_VLAN_PRI_BIT_MASK);
            pKeyData->data.pri.pri_mask = (pKeyPri->pri_mask & ACL_VLAN_PRI_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_CTAG_FMT:
        case YT_IGRACL_TEMPLATE_STAG_FMT:
        {
            yt_acl_key_vlanFmt_t *pKeyFmt = (yt_acl_key_vlanFmt_t *)pKey_data;
            pKeyData->data.tag_format.tagfmt_data = (pKeyFmt->tagfmt_data & ACL_VLAN_FMT_BIT_MASK);
            pKeyData->data.tag_format.tagfmt_mask = (pKeyFmt->tagfmt_mask & ACL_VLAN_FMT_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_SVID:
        case YT_IGRACL_TEMPLATE_CVID:
        case YT_IGRACL_TEMPLATE_INNER_CVID:
        case YT_IGRACL_TEMPLATE_INNER_SVID:
        {
            yt_acl_key_vid_t *pKeyVid = (yt_acl_key_vid_t *)pKey_data;
            pKeyData->data.vid.vid_data = (pKeyVid->vid_data_min & ACL_VLAN_ID_BIT_MASK);
            pKeyData->data.vid.vid_mask = (pKeyVid->vid_mask_max & ACL_VLAN_ID_BIT_MASK);
            pKeyData->data.vid.range_en = pKeyVid->range_en;
        }
            break;
        case YT_IGRACL_TEMPLATE_IPV4_DA:
        case YT_IGRACL_TEMPLATE_IPV4_SA:
        {
            yt_acl_key_ipv4_t *pKeyIp = (yt_acl_key_ipv4_t *)pKey_data;
            pKeyData->data.ipv4.ip_data = pKeyIp->ip_data_min;
            pKeyData->data.ipv4.ip_mask = pKeyIp->ip_mask_max;
            pKeyData->data.ipv4.range_en = pKeyIp->range_en;
        }
            break;
        case YT_IGRACL_TEMPLATE_L4_DPORT:
        case YT_IGRACL_TEMPLATE_L4_SPORT:
        {
            yt_acl_key_l4Port_t *pKeyL4Port = (yt_acl_key_l4Port_t *)pKey_data;
            pKeyData->data.l4_port.port_data = pKeyL4Port->port_data_min;
            pKeyData->data.l4_port.port_mask = pKeyL4Port->port_mask_max;
            pKeyData->data.l4_port.range_en = pKeyL4Port->range_en;
        }
            break;
        case YT_IGRACL_TEMPLATE_IP_FRAGMENT:
        {
            yt_acl_key_ipFrag_t *pKeyFrag = (yt_acl_key_ipFrag_t *)pKey_data;
            pKeyData->data.is_flag.flag_data = (pKeyFrag->is_frag_data & ACL_IP_FRAG_BIT_MASK);
            pKeyData->data.is_flag.flag_mask = (pKeyFrag->is_frag_mask & ACL_IP_FRAG_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_IP_1ST_FRAGMENT:
        {
            yt_acl_key_1stIpFrag_t *pKey1stFrag = (yt_acl_key_1stIpFrag_t *)pKey_data;
            pKeyData->data.is_flag.flag_data = (pKey1stFrag->is_1stFrag_data & ACL_IP_FIR_FRAG_BIT_MASK);
            pKeyData->data.is_flag.flag_mask = (pKey1stFrag->is_1stFrag_mask & ACL_IP_FIR_FRAG_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_IPV6_DA:
        case YT_IGRACL_TEMPLATE_IPV6_SA:
        {
            yt_acl_key_ipv6_t *pKeyIp6 = (yt_acl_key_ipv6_t *)pKey_data;
            osal_memcpy(pKeyData->data.ipv6.ipv6_data, IP6_ADDR_LEN, pKeyIp6->ipv6_data_min, IP6_ADDR_LEN);
            osal_memcpy(pKeyData->data.ipv6.ipv6_mask, IP6_ADDR_LEN, pKeyIp6->ipv6_mask_max, IP6_ADDR_LEN);
            pKeyData->data.ipv6.range_en = pKeyIp6->range_en;
        }
            break;
        case YT_IGRACL_TEMPLATE_IP_OPTION:
        {
            yt_acl_key_ipOption_t *pKeyIpOpt = (yt_acl_key_ipOption_t *)pKey_data;
            pKeyData->data.is_flag.flag_data = (pKeyIpOpt->is_option_data & ACL_IP_OPTION_BIT_MASK);
            pKeyData->data.is_flag.flag_mask = (pKeyIpOpt->is_option_mask & ACL_IP_OPTION_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_TCP_FLAGS:
        {
            yt_acl_key_tcpFlag_t *pKeyTcp = (yt_acl_key_tcpFlag_t *)pKey_data;
            pKeyData->data.tcp_flag.flag_data = (pKeyTcp->flag_data & ACL_TCP_FLAG_BIT_MASK);
            pKeyData->data.tcp_flag.flag_mask = (pKeyTcp->flag_mask & ACL_TCP_FLAG_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_IP_PROTOCOL:
        {
            yt_acl_key_ipProtocol_t *pKeyProt = (yt_acl_key_ipProtocol_t *)pKey_data;
            pKeyData->data.ip_protocol.protocol_data = (pKeyProt->protocol_data & ACL_IP_PROT_BIT_MASK);
            pKeyData->data.ip_protocol.protocol_mask = (pKeyProt->protocol_mask & ACL_IP_PROT_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_TOS:
        {
            yt_acl_key_ipTOS_t *pKeyTos = (yt_acl_key_ipTOS_t *)pKey_data;
            pKeyData->data.ip_tos.tos_data = (pKeyTos->tos_data & ACL_IP_TOS_BIT_MASK);
            pKeyData->data.ip_tos.tos_mask = (pKeyTos->tos_mask & ACL_IP_TOS_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_PPPOE_FLAG:
        {
            yt_acl_key_isPppoe_t *pKeyPppoe = (yt_acl_key_isPppoe_t *)pKey_data;
            pKeyData->data.is_flag.flag_data = (pKeyPppoe->is_pppoe_data & ACL_PPPOE_FLAG_BIT_MASK);
            pKeyData->data.is_flag.flag_mask = (pKeyPppoe->is_pppoe_mask & ACL_PPPOE_FLAG_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_IS_IGMP:
        {
            yt_acl_key_isIgmp_t *pKeyIgmp = (yt_acl_key_isIgmp_t *)pKey_data;
            pKeyData->data.is_flag.flag_data = (pKeyIgmp->is_igmp_data & ACL_IS_IGMP_BIT_MASK);
            pKeyData->data.is_flag.flag_mask = (pKeyIgmp->is_igmp_mask & ACL_IS_IGMP_BIT_MASK);
        }
            break;
        case YT_IGRACL_TEMPLATE_UDF_0:
        case YT_IGRACL_TEMPLATE_UDF_1:
        case YT_IGRACL_TEMPLATE_UDF_2:
        case YT_IGRACL_TEMPLATE_UDF_3:
        case YT_IGRACL_TEMPLATE_UDF_4:
        case YT_IGRACL_TEMPLATE_UDF_5:
        case YT_IGRACL_TEMPLATE_UDF_6:
        case YT_IGRACL_TEMPLATE_UDF_7:
        {
            yt_acl_key_udf_t *pKeyUdf = (yt_acl_key_udf_t *)pKey_data;
            osal_memcpy(pKeyData->data.udf.udf_data, YT_ACL_UDF_DATA_LEN_MAX, pKeyUdf->udf_data, YT_ACL_UDF_DATA_LEN_MAX);
            osal_memcpy(pKeyData->data.udf.udf_mask, YT_ACL_UDF_DATA_LEN_MAX, pKeyUdf->udf_mask, YT_ACL_UDF_DATA_LEN_MAX);
        }
            break;          
        case YT_IGRACL_TEMPLATE_ETHER_TYPE:
        {
            yt_acl_key_etherType_t *pKeyEth = (yt_acl_key_etherType_t *)pKey_data;
            pKeyData->data.ether_type.type_data = pKeyEth->type_data_min;
            pKeyData->data.ether_type.type_mask = pKeyEth->type_mask_max;
            pKeyData->data.ether_type.range_en = pKeyEth->range_en;
        }
            break;
        default:
            ACL_UNLOCK();
            pKeyData->type = 0;/*left one empty entry in pgAclDataList*/
            return CMM_ERR_NOT_SUPPORT;
    }
    ACL_UNLOCK();

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_acl_rule_action_add(yt_unit_t unit, yt_acl_action_type_t type, yt_acl_comm_act_t *pAction)
{
    yt_port_mask_t macmask;

    if(pgAclAction == NULL)
    {
        return CMM_ERR_NOT_INIT;
    }
    /*add action to action list*/
    switch(type)
    {
        case YT_ACL_ACT_TYPE_FWD:
        {
            yt_acl_action_fwd_t *pFwdAct = (yt_acl_action_fwd_t *)pAction;
            pgAclAction->FWD_DECISION_EN = pFwdAct->fwd_en;
            pgAclAction->FWD_DECISION_TYPE = pFwdAct->fwd_type;
            if(pFwdAct->fwd_type == YT_ACL_FWD_TYPE_DROP)
            {
                pgAclAction->FWD_DECISION_TYPE = YT_ACL_FWD_TYPE_REDIRECT;
                CMM_CLEAR_MEMBER_PORT(macmask);/*treate no dst portmask as drop*/
            }
            else
            {
                CAL_YTPLIST_TO_MLIST(unit, pFwdAct->dst_portmask, macmask);
            }
            pgAclAction->DEST_PORT_MASK = macmask.portbits[0];
        }
            break;
        case YT_ACL_ACT_TYPE_INTPRI_MAP:
        {
            yt_acl_action_internalPriMap_t *pInnerPri = (yt_acl_action_internalPriMap_t *)pAction;
            pgAclAction->INT_DP_VALID = pInnerPri->int_dp_en;
            pgAclAction->INT_DP = pInnerPri->int_dp;
            pgAclAction->INT_PRI_VALID = pInnerPri->int_pri_en;
            pgAclAction->INT_PRI = pInnerPri->int_pri;
        }
            break;
        case YT_ACL_ACT_TYPE_VID_REPLACE:
        {
            yt_acl_action_vid_replace_t *pVidRep = (yt_acl_action_vid_replace_t *)pAction;
            pgAclAction->CVID_REPLACE_EN = pVidRep->cvid_replace_en;
            pgAclAction->CVID = pVidRep->cvid;
            pgAclAction->SVID_REPLACE_EN = pVidRep->svid_replace_en;
            pgAclAction->SVID = pVidRep->svid;
            pgAclAction->FWD_CVID_EN = pVidRep->fwd_cvid_en;
            pgAclAction->FWD_SVID_EN = pVidRep->fwd_svid_en;
        }
            break;
        case YT_ACL_ACT_TYPE_PRI_REPLACE:
        {
            yt_acl_action_pri_replace_t *pPriRep = (yt_acl_action_pri_replace_t *)pAction;
            pgAclAction->CPRI_REPLACE_EN = pPriRep->cpri_replace_en;
            pgAclAction->CDEI_REPLACE_EN = pPriRep->cdei_replace_en;
            pgAclAction->CPRI = pPriRep->cpri;
            pgAclAction->CDEI = pPriRep->cdei;
            pgAclAction->SPRI_REPLACE_EN = pPriRep->spri_replace_en;
            pgAclAction->SDEI_REPLACE_EN = pPriRep->sdei_replace_en;
            pgAclAction->SPRI = pPriRep->spri;
            pgAclAction->SDEI = pPriRep->sdei;
        }
            break;
        case YT_ACL_ACT_TYPE_VLAN_ASSIGN:
        {
            yt_acl_action_vlan_assign_t *pVlanAssign = (yt_acl_action_vlan_assign_t *)pAction;
            pgAclAction->CTAG_ASSIGN= pVlanAssign->ctag_assign_mode;
            pgAclAction->STAG_ASSIGN = pVlanAssign->stag_assign_mode;
        }
            break;
        case YT_ACL_ACT_TYPE_DSCP_REPLACE:
        {
            yt_acl_action_dscp_replace_t *pDscpRep = (yt_acl_action_dscp_replace_t *)pAction;
            pgAclAction->DSCP_REPLACE_EN = pDscpRep->dscp_replace_en;
            pgAclAction->DSCP = pDscpRep->dscp;
        }
            break;
        case YT_ACL_ACT_TYPE_METER_ASSIGN:
        {
            yt_acl_action_meter_assign_t *pMeterAssign = (yt_acl_action_meter_assign_t *)pAction;
            pgAclAction->METER_EN = pMeterAssign->meter_en;
            pgAclAction->METER_ID = pMeterAssign->meter_id;
        }
            break;
        case YT_ACL_ACT_TYPE_FLOWSTAT:
        {
            yt_acl_action_flowStats_t *pFlowStat = (yt_acl_action_flowStats_t *)pAction;
            pgAclAction->FLOW_STATS_EN = pFlowStat->flow_stats_en;
            pgAclAction->FLOW_STATS_PTR = pFlowStat->flow_stats_id;
        }
            break;
        case YT_ACL_ACT_TYPE_MIRROR_ENABLE:
        {
            yt_acl_action_mirrorEn_t *pMirrorEn = (yt_acl_action_mirrorEn_t *)pAction;
            pgAclAction->MIRROR_EN = pMirrorEn->mirror_en;
        }
            break;
        case YT_ACL_ACT_TYPE_BYPASS_ENABLE:
        {
            yt_acl_action_bypass_t *pBypassEn = (yt_acl_action_bypass_t *)pAction;
            pgAclAction->CPU_CTRL_BYPASS = pBypassEn->cpu_ctrl_bypass_en;
        }
            break;
        default:
            return CMM_ERR_NOT_SUPPORT;
    }
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_acl_rule_create(yt_unit_t unit, uint16_t rulePri, yt_bool_t ruleReverse, uint32_t *pId)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t index = 0;
    uint8_t group = 0;
    uint8_t entryNum = 0;
    uint8_t groupId = 0xF;
    uint32_t lookup_index_base = 0xFF;
    acl_blk_cmd_t blk_cmd;
    yt_acl_key_vid_t vlan_data;

    igrAcl_data_t *pData = NULL;
    yt_bool_t revert_en;

    CMM_PARAM_CHK(YT_ACL_IDX_MAX <= rulePri, CMM_ERR_INPUT);
    osal_memset(&vlan_data, sizeof(yt_acl_key_vid_t), 0, sizeof(yt_acl_key_vid_t));
    ret = fal_tiger_acl_keys_valid_check(unit);
    if(ret != CMM_ERR_OK)
    {
        return ret;
    }

    pData = (igrAcl_data_t *)pgAclDataList;
    revert_en = ruleReverse;

    CMM_PARAM_CHK(NULL == pData, CMM_ERR_NOT_INIT);
    if (gKeyDataNum == 0 && gSrcPortSetFlag == TRUE)
    {
        fal_tiger_acl_rule_key_add(unit, YT_IGRACL_TEMPLATE_CVID, (yt_acl_comm_key_t *)&vlan_data);
    }
    ACL_LOCK();
    fal_tiger_acl_data_init(rulePri, gSrcPortMask, revert_en);
    fal_tiger_acl_flags_set(pData, gKeyDataNum);
    /*showFlags();*/
    fal_tiger_acl_data_prepare(pData, gKeyDataNum);

    fal_tiger_acl_data_list_create();
    /*showFlags();*/
    entryNum    =   head.size;
    ret = fal_tiger_acl_store_check(unit, &group, &index);
    if(ret != CMM_ERR_OK)
    {
        if(CMM_ERR_OK != fal_tiger_acl_positon_search(unit, entryNum, &group, &index))
        {
            fal_tiger_acl_list_destory();
            ACL_UNLOCK();
            return CMM_ERR_TABLE_FULL;
        }
        /*group 0 only used 7 entries*/
        lookup_index_base = (group == 0)?((group<<3)+index):((group<<3)+index-1);
        *pId = lookup_index_base;
        HAL_FIELD_SET(ACL_BLK_CMDm, ACL_BLK_CMD_CMDf, &blk_cmd, 1);
        HAL_FIELD_SET(ACL_BLK_CMDm, ACL_BLK_CMD_BLKIDf, &blk_cmd, group);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_BLK_CMDm, 0, sizeof(blk_cmd), &blk_cmd), ret);
        ret = fal_tiger_acl_data_store(unit, entryNum, group, index, &groupId);
        CMM_ERR_CHK(fal_tiger_acl_data_rangeSet(unit, group, index, groupId), ret);
        HAL_FIELD_SET(ACL_BLK_CMDm, ACL_BLK_CMD_CMDf, &blk_cmd, 0);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, ACL_BLK_CMDm, 0, sizeof(blk_cmd), &blk_cmd), ret);
    }
    else
    {
        lookup_index_base = (group == 0)?((group<<3)+index):((group<<3)+index-1);
        *pId = lookup_index_base;
        fal_tiger_acl_list_destory();
        ACL_UNLOCK();
        return CMM_ERR_SAMEENTRY_EXIST;
    }
    ret = fal_tiger_acl_id_entry_add(unit, *pId);
    ACL_UNLOCK();

    return ret;
}

yt_ret_t fal_tiger_acl_rule_active(yt_unit_t unit, uint32_t id)
{
    yt_ret_t ret = CMM_ERR_OK;

    ret = fal_tiger_acl_action_set(unit, id, pgAclAction);
    

    return ret;
}

yt_ret_t fal_tiger_acl_rule_del(yt_unit_t unit, uint32_t id)
{
    uint8_t count = 0;
    uint8_t bitId = 0;
    uint8_t aclId = 0;
    yt_ret_t ret = CMM_ERR_OK;

    ACL_LOCK();
    if (id == ACL_IDX_INVALID)
    {
        for (count = 0; count < YT_ACL_KEY_BIT_SIZE(YT_ACL_IDX_MAX); count++)
        {
            if (acl_id[unit][count] == 0)
            {
                continue;
            }
            else
            {
                for (bitId = 0; bitId < YT_ACL_BITWID; bitId++)
                {
                    if (!(acl_id[unit][count] & (1<<bitId)))
                    {
                        continue;
                    }

                    aclId = acl_id[unit][count]*YT_ACL_BITWID + bitId;
                    ret = fal_tiger_acl_table_del_entry(unit, aclId);
                    if (CMM_ERR_OK == ret)
                    {
                        acl_id[unit][count] &= ~(1<<bitId);
                    }
                }
            }
        }
    }
    else
    {
        ret = fal_tiger_acl_table_del_entry(unit, id);
        if (CMM_ERR_OK == ret)
        {
            ret = fal_tiger_acl_id_entry_del(unit, id);
        }
    }
    ACL_UNLOCK();

    return ret;
}

uint32_t fal_tiger_acl_reset(yt_unit_t unit)
{
    uint8_t binIdx;
    data_store_t *entryPtr, *entryNext;

    for(binIdx=0; binIdx<ACL_LINE_NUM; binIdx++)
    {
        entryPtr = database_header[unit][binIdx].head;
        while(entryPtr)
        {
            entryNext = entryPtr->next;
            osal_free(entryPtr, YT_MEM_MALLOC_MODULE_ACL);
            entryPtr = NULL;
            entryPtr = entryNext;
        }
        database_header[unit][binIdx].head = NULL;
        database_header[unit][binIdx].size = 0;
    }

    fal_tiger_acl_data_reset(unit);

    return CMM_ERR_OK;
}
