/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
#include "fal_tiger_nic.h"
#include "fal_tiger_port.h"
#include "fal_tiger_l2.h"

yt_ret_t  fal_tiger_nic_init(yt_unit_t unit)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2_learn_per_port_ctrl;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, FAL_INTERNAL_CPU_MACID, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);
    /* disable internal cpu port learn fdb*/
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2_learn_per_port_ctrl, YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, FAL_INTERNAL_CPU_MACID, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode)
{
    cpu_copy_dst_ctrl_t tmp_entry;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&tmp_entry, 0, sizeof(cpu_copy_dst_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CPU_COPY_DST_CTRLm, 0, sizeof(cpu_copy_dst_ctrl_t), &tmp_entry), ret);
    if (mode == CPUPORT_MODE_INTERNAL)
    {
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_INT_CPUf, &tmp_entry, YT_ENABLE);
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_EXT_CPUf, &tmp_entry, YT_DISABLE);
    }
    else if (mode == CPUPORT_MODE_EXTERNAL)
    {
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_INT_CPUf, &tmp_entry, YT_DISABLE);
        HAL_FIELD_SET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_EXT_CPUf, &tmp_entry, YT_ENABLE);
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, CPU_COPY_DST_CTRLm, 0, sizeof(cpu_copy_dst_ctrl_t), &tmp_entry), ret);  

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pmode)
{
    cpu_copy_dst_ctrl_t tmp_entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t to_ext_cpu;

    osal_memset(&tmp_entry, 0, sizeof(cpu_copy_dst_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CPU_COPY_DST_CTRLm, 0, sizeof(cpu_copy_dst_ctrl_t), &tmp_entry), ret);
    HAL_FIELD_GET(CPU_COPY_DST_CTRLm, CPU_COPY_DST_CTRL_TO_EXT_CPUf, &tmp_entry, &to_ext_cpu);
    *pmode = to_ext_cpu ? CPUPORT_MODE_EXTERNAL : CPUPORT_MODE_INTERNAL;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_t ext_cpuport;
    uint32_t macid;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EXT_CPU_PORT_CTRLm, 0, sizeof(ext_cpu_port_ctrl_t), &entry), ret);

    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_NUMf, &entry, &macid);
    CAL_MAC_TO_YTP(unit, macid, ext_cpuport);
    /* enable the backpress on cpu port */
#ifdef PORT_INCLUDED
    fal_tiger_port_backpress_enable_set(unit, ext_cpuport, (enable?YT_DISABLE:YT_ENABLE));
#endif
    /* l2 learn on port */
    fal_tiger_l2_port_learn_en_set(unit, ext_cpuport, (enable?YT_DISABLE:YT_ENABLE));
    /* udpate yt ext cpu port bit in portmask
    CMM_PLIST_UPDATE(unit, ext_cpuport, (enable?FALSE:TRUE)); */
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t enable;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_NUMf, &entry, macid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EXT_CPU_PORT_CTRLm, 0, sizeof(ext_cpu_port_ctrl_t), &entry), ret);

    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_ENf, &entry, &enable);
    if(enable == YT_ENABLE)
    {
        /* disable the backpress on cpu port */
#ifdef PORT_INCLUDED
        fal_tiger_port_backpress_enable_set(unit, port, YT_DISABLE);
#endif
        /* l2 learn on port */
        fal_tiger_l2_port_learn_en_set(unit, port, YT_DISABLE);
        /* udpate yt ext cpu port bit in portmask
        CMM_PLIST_UPDATE(unit, port, FALSE); */
     }
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pport)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t macid;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_PORT_NUMf, &entry, &macid);
    CAL_MAC_TO_YTP(unit, macid, (*pport));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid)
{
    cpu_tag_tpid_t cpu_tag_tpid;
    cmm_err_t ret = CMM_ERR_OK;

    HAL_FIELD_SET(CPU_TAG_TPIDm, CPU_TAG_TPID_TPIDf, &cpu_tag_tpid, tpid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, CPU_TAG_TPIDm, 0, sizeof(cpu_tag_tpid_t), &cpu_tag_tpid), ret);
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *ptpid)
{
    cpu_tag_tpid_t cpu_tag_tpid;
    uint32_t tpid;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, CPU_TAG_TPIDm, 0, sizeof(cpu_tag_tpid_t), &cpu_tag_tpid), ret);
    HAL_FIELD_GET(CPU_TAG_TPIDm, CPU_TAG_TPID_TPIDf, &cpu_tag_tpid, &tpid);
    *ptpid = tpid;
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable)
{
    ext_cpu_port_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_SET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_TAG_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EXT_CPU_PORT_CTRLm, 0, sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable)
{
    ext_cpu_port_ctrl_t entry;
    uint32_t enable;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EXT_CPU_PORT_CTRLm, 0,sizeof(ext_cpu_port_ctrl_t), &entry), ret);
    HAL_FIELD_GET(EXT_CPU_PORT_CTRLm, EXT_CPU_PORT_CTRL_EXT_CPU_TAG_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}
