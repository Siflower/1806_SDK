/*
 * =====================================================================================
 *
 *       Filename:  siwifi_lmac_glue_barematel.c
 *
 *    Description:  for lmac baremetal glue
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */


#include "siwifi_lmac_glue.h"
#include "ipc_user_event.h"
#include "reg_access.h"
#include "siwifi_defs.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#ifdef CONFIG_SFA28_FULLMASK
#include <sf19a28.h>
#endif

#ifdef CONFIG_SF16A18_LMAC_USE_M_SFDSP
static char *g_fw_lb = "sf1688_lb_fmac.bin";
static char *g_fw_hb = "sf1688_hb_fmac.bin";
#endif

#if defined(CONFIG_SFA28_FULLMASK)
//iram for lb/hb ipc_share_env base
#define LB_IRAM_BASE 0xbc000000
#define HB_IRAM_BASE 0xbc008000
#endif

static int g_task_id_lb = -1;
static int g_task_id_hb = -1;

extern int load_task(char *path, int cpu, struct device *device);
extern int start_task(int task_id);
extern int stop_task(int task_id);
extern void remove_task(int task_id);
extern void task_dump_registers(int task_id,uint32_t fexception_base);

void notify_lmac_complete_ipc(struct siwifi_hw *siwifi_hw)
{
    struct v1_plat_data *plat_priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;
    plat_priv->lmac_prepared = 1;
    wake_up_all(&plat_priv->lmac_wq);
    printk("lmac init complete(%d)\n", siwifi_hw->mod_params->is_hb);
}

//must be atomic
void notify_lmac_la_init_ipc(struct siwifi_hw *siwifi_hw, int8_t type, int8_t enable)
{
    struct v1_plat_data *priv = (struct v1_plat_data *)&siwifi_hw->plat->priv;
    uint8_t data;
    int ret;
    struct ipc_shared_env_tag *ipc_shenv;

    //check if conflicts happen for different type, now :
    //LA/IQDUMP/NORMAL FUNCTION can not be worked together, so if one is enable, must denied the others's request
    if(enable &&
            priv->deep_debug_type != 0 &&
            priv->deep_debug_type != type){
        printk("deep_debug %d has been enabled, can not enable another type : %d\n", priv->deep_debug_type, type);
        ret = 0;
        goto EXIT;
    }

    if(type == DEEP_DEBUG_LA){
#ifndef CONFIG_SF16A18_WIFI_LA_ENABLE
        ret = 0;
        printk("la not support(%d)\n", siwifi_hw->mod_params->is_hb);
        goto EXIT;
#endif
		data = readb((void *)REG_SYSM_SHARE_RAM_SEL);
        if(enable)
            data |= ((priv->band & LB_MODULE) ? ( 1 << 0 ) : ( 1 << 1 ));
        else
            data &= ~((priv->band & LB_MODULE) ? ( 1 << 0 ) : ( 1 << 1 ));

        writeb(data, (void *)REG_SYSM_SHARE_RAM_SEL);
		writeb((2 << 4) | 0xd,(void *)((priv->band & LB_MODULE) ? WIFI_1_LA_THRES : WIFI_2_LA_THRES));

#ifdef CONFIG_SFA28_FULLMASK
	    //clk selection
        //use the 240M clk provided by rf
        //la_clk_sel[0:1] 2bits
        //[000] ---> mpif_clk
        //[001] --->phy_gclk
        //[010] --->bdtx_gclk
        //[011] --->bdrx_gclk
		//[1xx] --->plf_clk
        writeb(0x4, (void *)((priv->band & LB_MODULE) ? REG_SYSM_WIFI1_LA_CLK_SEL : REG_SYSM_WIFI2_LA_CLK_SEL));
        priv->la_clk = 300;
#endif
    }else if(type == DEEP_DEBUG_IQDUMP){
        //TODO

    }else{
        printk("%s invalid type : %d\n", __func__, type);
        ret = 0;
        goto EXIT;
    }

    if(enable)
        priv->deep_debug_type |= type;
    else
        priv->deep_debug_type &= ~type;

    ret = 1;
EXIT:
#if defined(CONFIG_SFA28_FULLMASK)
    ipc_shenv = (struct ipc_shared_env_tag *)((priv->band == LB_MODULE) ? LB_IRAM_BASE : HB_IRAM_BASE);
#else
    ipc_shenv = (struct ipc_shared_env_tag *)((priv->band == LB_MODULE) ? MEM_LB_SHARE_MEM_BASE : MEM_HB_SHARE_MEM_BASE);
#endif
    clear_ipc_event_user_bit(ipc_shenv, IPC_USER_EVENT_DEEP_DEBUG_SET);
    set_ipc_event_user_reply(ipc_shenv, ret);
}

#define TASK_LB 0
#define TASK_HB 1

int lmac_glue_init(struct v1_plat_data *priv, struct device *device)
{
    init_waitqueue_head(&priv->lmac_wq);
    priv->lmac_prepared = 0;
    priv->deep_debug_type = 0;

    if (priv->band == LB_MODULE)
    {
        g_task_id_lb = load_task(g_fw_lb, TASK_LB, device);
        printk("g_task_id lb: %d\n", g_task_id_lb);
        if (g_task_id_lb < 0)
        {
            return -1;
        }
    }
    else
    {
        g_task_id_hb = load_task(g_fw_hb, TASK_HB, device);
        printk("g_task_id hb: %d\n", g_task_id_hb);
        if (g_task_id_hb < 0)
        {
            return -1;
        }
    }

    return 0;
}

u8 *lmac_glue_share_mem_init(struct v1_plat_data *priv)
{
#if defined(CONFIG_SFA28_FULLMASK)
    return (u8 *)((priv->band == LB_MODULE) ? LB_IRAM_BASE : HB_IRAM_BASE);
#else
    return (u8 *)((priv->band == LB_MODULE) ? MEM_LB_SHARE_MEM_BASE : MEM_HB_SHARE_MEM_BASE);
#endif
}

//wait timeout for lmac init
#define LMAC_TIMEOUT_MS 30000


int lmac_glue_start(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    printk("lmac_glue_start(%d)\n", siwifi_hw->mod_params->is_hb);
    priv->lmac_prepared = 0;
    priv->deep_debug_type = 0;

    if (priv->band == LB_MODULE)
    {
        if (start_task(g_task_id_lb))
        {
            printk("start task failed!\n");
            return -1;
        }
    }
    else
    {
        if (start_task(g_task_id_hb))
        {
            printk("start task failed!\n");
            return -1;
        }
    }
    //wait the init complete
#ifdef CONFIG_SF16A18_RELEASE
    printk("wait lmac(%d) init with timeout:%ld ms>>>>>>>>>>>>>>>>>>>>>>>\n", siwifi_hw->mod_params->is_hb, msecs_to_jiffies(LMAC_TIMEOUT_MS));
    if (wait_event_interruptible_timeout(priv->lmac_wq, (priv->lmac_prepared == 1), msecs_to_jiffies(LMAC_TIMEOUT_MS)) == 0)
    {
        printk("wait lmac(%d) init timeout\n", siwifi_hw->mod_params->is_hb);
        return -1;
    };
#else
    printk("wait lmac init(%d)>>>>>>>>>>>>>>>>>>>>>>>\n", siwifi_hw->mod_params->is_hb);
    wait_event_interruptible(priv->lmac_wq, (priv->lmac_prepared == 1));
#endif

    printk("wait lmac over(%d)<<<<<<<<<<<<<<<<<<<<<<<\n", siwifi_hw->mod_params->is_hb);

    return 0;
}

void lmac_glue_stop(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv)
{
    printk("lmac_glue_stop(%d)\n", siwifi_hw->mod_params->is_hb);
    if (priv->band == LB_MODULE)
    {
        if (stop_task(g_task_id_lb))
        {
            printk("stop task faield!\n");
        }
    }
    else
    {
        if (stop_task(g_task_id_hb))
        {
            printk("stop task faield!\n");
        }
    }
    priv->lmac_prepared = 0;
    priv->deep_debug_type = 0;
}

void lmac_glue_deinit(struct v1_plat_data *priv)
{
    priv->deep_debug_type = 0;
    priv->lmac_prepared = 0;
    if (priv->band == LB_MODULE)
    {
        remove_task(g_task_id_lb);
        g_task_id_lb = -1;
    }
    else
    {
        remove_task(g_task_id_hb);
        g_task_id_hb = -1;
    }
}

void lmac_dump_registers(int band_type, uint32_t fexception_base)
{
    printk("lmac_dump_registers band=%d\n", band_type);
    task_dump_registers((band_type == 0) ? TASK_LB : TASK_HB, fexception_base);
}
