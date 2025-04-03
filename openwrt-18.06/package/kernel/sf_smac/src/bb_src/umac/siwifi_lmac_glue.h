/*
 * =====================================================================================
 *
 *       Filename:  siwifi_lmac_glue.h
 *
 *    Description:  header file for lmac glue
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */

#ifndef _SIWIFI_LMAC_GLUE_H_
#define _SIWIFI_LMAC_GLUE_H_

#include <linux/wait.h>
#include <linux/timer.h>
#include <linux/clk.h>
#include "siwifi_defs.h"

struct v1_plat_data {
    int deep_debug_type;

    //indicate that lmac has been prepared
    int lmac_prepared;
    //a wait queue used by lmac
    wait_queue_head_t lmac_wq;
    //irq num
    void __iomem *base;
    //umac irq number
    int umac_irq;

    //la clk number
    int32_t la_clk;

    struct clk *pl_clk;
    struct clk *bus_clk;
#if (defined(CONFIG_SF16A18_WIFI_LA_ENABLE) && (defined(CFG_A28_V_LA_CLK_BUG) || defined(CFG_A28_FULLMASK_LA_BUG)))
	struct clk *other_band_pl_clk;
    struct clk *other_band_bus_clk;
#endif

#ifdef CONFIG_SF16A18_LMAC_USE_M_SFDSP
    struct clk *m_SFDSP_clk;
#endif
    //low band or high band
    uint8_t band;
    //platform on or off
    uint8_t on;
};

int lmac_glue_init(struct v1_plat_data *priv, struct device *device);

u8 *lmac_glue_share_mem_init(struct v1_plat_data *priv);

int lmac_glue_start(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv);

void lmac_glue_stop(struct siwifi_hw *siwifi_hw, struct v1_plat_data *priv);

void lmac_glue_deinit(struct v1_plat_data *priv);

void notify_lmac_complete_ipc(struct siwifi_hw *siwifi_hw);

void notify_lmac_la_init_ipc(struct siwifi_hw *siwifi_hw, int8_t type,int8_t enable);
#endif
