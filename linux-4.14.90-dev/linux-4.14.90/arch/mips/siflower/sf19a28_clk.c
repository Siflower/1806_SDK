/*
 * SF19A28 clocks.
 *
 * Exposes all configurable internal clock sources to the clk framework.
 *
 * We have:
 *  - Root source, usually 12MHz supplied by an external crystal
 *  - 4 PLLs which generate multiples of root rate [CPU, DDR, CMN, SPC]
 *
 * Dividers:
 *  - 22 clock dividers with:
 *   * selectable source [one of the PLLs],
 *   * output divided between [1 .. 256]
 *   * can be enabled individually.
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/kernel.h>
//#include <linux/clk-private.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/of_address.h>

typedef struct sf_clk_data {
	spinlock_t *lock;
	unsigned long clk_flags;
} sf_clk_data_t;

/* CM_CFG */
static DEFINE_SPINLOCK(cm_cfg_lock);
static DEFINE_SPINLOCK(cm_cfg_lock_cpu);

typedef enum cm_cfg_t {
	BUS1_CFG,
	BUS2_CFG,
	BUS3_CFG,
	CPU_CFG,
	PBUS_CFG,
	MEMPHY_CFG,
	UART_CFG,
#ifdef CONFIG_SF19A28_MPW1
	ETH_REF_CFG,
	ETH_BYP_REF_CFG,
#endif
#ifdef CONFIG_SF19A28_FULLMASK
	GMAC_BYP_REF_CFG,
#endif
	ETH_TSU_CFG,
	M_SFDSP_0_CFG,
	M_SFDSP_1_CFG,
	WLAN24_CFG,
	WLAN5_CFG,
	USBPHY_CFG,
	TCLK_CFG,
	CATIP_LP_CFG //TODO: catip_lp_clk is not a divider clk.
} sf19a28_cfg;

static DEFINE_SPINLOCK(cm_pll_lock_cpu);
static DEFINE_SPINLOCK(cm_pll_lock);
static DEFINE_SPINLOCK(cm_pll_lock_ddr);

static sf_clk_data_t sf19a28_clk_data = {
	.lock = &cm_cfg_lock,
	.clk_flags = CLK_GET_RATE_NOCACHE,
};

static sf_clk_data_t sf19a28_clk_data_on = {
	.lock = &cm_cfg_lock,
	.clk_flags = CLK_IGNORE_UNUSED |
		     CLK_GET_RATE_NOCACHE,
};

static sf_clk_data_t sf19a28_clk_data_bus = {
	.lock = &cm_cfg_lock,
	.clk_flags = CLK_GET_RATE_NOCACHE |
		     CLK_SET_RATE_NO_REPARENT,
};

static sf_clk_data_t sf19a28_clk_data_cpu = {
	.lock = &cm_cfg_lock_cpu,
	.clk_flags = CLK_SET_RATE_NO_REPARENT |
		     CLK_IGNORE_UNUSED |
		     CLK_SET_RATE_PARENT |
		     CLK_GET_RATE_NOCACHE,
};

/* CM_PLL */
#define MIN_PLL_FREQ	16000000 /* 16MHz to 1600MHz */
#define PLL_PWR_DOWN	0
#define PLL_PARA	0x4
#define PLL_LOAD	0x1c
#define PLL_LOCK	0x20
#define PLL_REF_CLK	0x24
/* cpu clk src register, 0-cpupll 1-ddrpll 2-cmnpll 3-spcpll */
#define CPU_CLK_SRC	0xb9e01500

typedef enum cm_pll_t {
	CPU_PLL,
	DDR_PLL,
	CMN_PLL,
} sf19a28_pll;

struct sf_clk_pll {
	struct clk_hw	hw;
	void __iomem *base;
	unsigned int max_rate;
	spinlock_t lock;
};

static sf_clk_data_t sf19a28_pll_data_cpu = {
	.lock = &cm_pll_lock_cpu,
	.clk_flags = CLK_SET_RATE_NO_REPARENT |
		     CLK_IGNORE_UNUSED |
		     CLK_GET_RATE_NOCACHE,
};

static sf_clk_data_t sf19a28_pll_data_ddr = {
	.lock = &cm_pll_lock_ddr,
	.clk_flags = CLK_SET_RATE_NO_REPARENT |
		     CLK_IGNORE_UNUSED,
};

static sf_clk_data_t sf19a28_pll_data = {
	.lock = &cm_pll_lock,
	.clk_flags = CLK_SET_RATE_NO_REPARENT |
		     CLK_GET_RATE_NOCACHE |
		     CLK_SET_RATE_GATE,
};

static const struct of_device_id sf19a28_cfg_of_match[] = {
	/* clk */
	{
		.compatible = "siflower,sf19a28-bus1-clk",
		.data = &sf19a28_clk_data_bus,
	},
	{
		.compatible = "siflower,sf19a28-bus2-clk",
		.data = &sf19a28_clk_data_bus,
	},
	{
		.compatible = "siflower,sf19a28-bus3-clk",
		.data = &sf19a28_clk_data_bus,
	},
	{
		.compatible = "siflower,sf19a28-cpu-clk",
		.data = &sf19a28_clk_data_cpu,
	},
	{
		.compatible = "siflower,sf19a28-pbus-clk",
		.data = &sf19a28_clk_data_on,
	},
	{
		.compatible = "siflower,sf19a28-memphy-clk",
		.data = &sf19a28_clk_data_on,
	},
	{
		.compatible = "siflower,sf19a28-uart-clk",
		.data = &sf19a28_clk_data,
	},
#ifdef CONFIG_SF19A28_MPW1
	{
		.compatible = "siflower,sf19a28-eth-ref-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-eth-byp-ref-clk",
		.data = &sf19a28_clk_data,
	},
#endif
#ifdef CONFIG_SF19A28_FULLMASK
	{
		.compatible = "siflower,sf19a28-gmac-byp-ref-clk",
		.data = &sf19a28_clk_data,
	},
#endif
	{
		.compatible = "siflower,sf19a28-eth-tsu-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-m_SFDSP-0-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-m_SFDSP-1-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-wlan24-mac-wt-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-wlan5-mac-wt-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-usbphy-ref-clk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-tclk",
		.data = &sf19a28_clk_data,
	},
	{
		.compatible = "siflower,sf19a28-catip-lp-clk",
		.data = &sf19a28_clk_data,
	},
	/* pll */
	{
		.compatible = "siflower,sf19a28-cpu-pll",
		.data = &sf19a28_pll_data_cpu,
	},
	{
		.compatible = "siflower,sf19a28-ddr-pll",
		.data = &sf19a28_pll_data_ddr,
	},
	{
		.compatible = "siflower,sf19a28-cmn-pll",
		.data = &sf19a28_pll_data,
	},
	{}
};

static const struct of_device_id *sf_get_match(int index, struct device_node *np)
{
	return of_match_node(&sf19a28_cfg_of_match[index], np);
}

/*
 *	CM_CFG_CLK init.
 */
static const char ** __init clk_mux_get_parents(struct device_node *np,
						unsigned char *num_parents)
{
	const char **parents;
	int nparents, i;

	nparents = of_count_phandle_with_args(np, "clocks", "#clock-cells");
	if (WARN_ON(nparents <= 0))
		return ERR_PTR(-EINVAL);

	parents = kzalloc(nparents * sizeof(const char *), GFP_KERNEL);
	if (!parents)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < nparents; i++)
		parents[i] = of_clk_get_parent_name(np, i);

	*num_parents = nparents;
	return parents;
}

static void sf19a28_cfg_setup(struct device_node *np, int cfg_type)
{
	unsigned char num_parents;
	const struct of_device_id *match;
	struct clk *clk;
	const char **parents;
	struct clk_gate *gate;
	struct clk_divider *div;
	struct clk_mux *mux;
	void __iomem *reg = NULL;
	const char *clk_name;
	struct clk_onecell_data *clk_data;
	sf_clk_data_t *data;

	/* of_match_node() will check device node. */
	match = sf_get_match(cfg_type, np);
	if (!match)
		goto err;

	data = (sf_clk_data_t *)match->data;

	reg = of_iomap(np, 0);
	if (!reg)
		goto err;

	if (of_property_read_string(np, "clock-output-names", &clk_name))
		goto err;

	parents = clk_mux_get_parents(np, &num_parents);

	if(!parents)
		goto err;

	clk_data = kzalloc(sizeof(struct clk_onecell_data), GFP_KERNEL);
	if (!clk_data)
		goto err;

	clk_data->clk_num = 1;
	clk_data->clks = kzalloc(clk_data->clk_num * sizeof(struct clk *),
			GFP_KERNEL);

	if (!clk_data->clks)
		goto err;

	gate = kzalloc(sizeof(struct clk_gate), GFP_KERNEL);
	if (!gate)
		goto err;

	div = kzalloc(sizeof(struct clk_divider), GFP_KERNEL);
	if (!div) {
		kfree(gate);
		goto err;
	}

	mux = kzalloc(sizeof(struct clk_mux), GFP_KERNEL);
	if (!mux) {
		kfree(gate);
		kfree(div);
		goto err;
	}

	gate->reg = reg + 0xc;
	gate->bit_idx = 0;
	gate->flags = 0;
	gate->lock = data->lock;

	div->reg = reg + 0x4;
	div->shift = 0;
	div->width = 8;
	div->lock = data->lock;
	div->flags = CLK_DIVIDER_ROUND_CLOSEST | CLK_DIVIDER_ALLOW_ZERO;

	mux->reg = reg;
	mux->shift = 0;
	mux->mask = 0x7;
	mux->lock = data->lock;
	mux->flags = 0;

	clk = clk_register_composite(NULL,clk_name,parents,num_parents,
			&mux->hw,&clk_mux_ops,
			&div->hw,&clk_divider_ops,
			&gate->hw,&clk_gate_ops,
			data->clk_flags);

	if (!IS_ERR(clk)) {
		clk_data->clks[0] = clk;
		of_clk_add_provider(np, of_clk_src_onecell_get, clk_data);
		return;
	} else {
		kfree(gate);
		kfree(div);
		kfree(mux);
		if (clk_data)
			kfree(clk_data->clks);
		kfree(clk_data);
		kfree(parents);
	}

err:
	pr_err("clk %d init failed!\n", cfg_type);
	return;
}

unsigned char sf_pll_read(void *base, unsigned int off)
{
	return readb((void*)((unsigned int)base + off));
}

void sf_pll_write(unsigned char val, void *base, unsigned int off)
{
	writeb(val, (void*)((unsigned int)base + off));
	return;
}

static int sf_pll_enable(struct clk_hw *hw)
{
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);
	unsigned char enable;

	enable = sf_pll_read(pll->base, PLL_PWR_DOWN);
	sf_pll_write(enable & ~BIT(0), pll->base, PLL_PWR_DOWN);

	return 0;
}

static void sf_pll_disable(struct clk_hw *hw)
{
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);
	unsigned char enable;

	return;

	enable = sf_pll_read(pll->base, PLL_PWR_DOWN);
	sf_pll_write(enable | BIT(0), pll->base, PLL_PWR_DOWN);

	return;
}

static int sf_pll_is_enabled(struct clk_hw *hw)
{
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);

	return !(sf_pll_read(pll->base, PLL_PWR_DOWN) & BIT(0));
}

unsigned long get_oscclk_rate(struct clk_hw *hw)
{
	unsigned int ref_clk_sel, pll_src_sel, osc_rate;
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);
	struct clk *osc_clk = clk_get_parent(hw->clk);

	pll_src_sel = sf_pll_read(pll->base, PLL_LOAD) >> 1 & 0x3;
	ref_clk_sel = sf_pll_read(pll->base, PLL_REF_CLK);
	if (pll_src_sel | ref_clk_sel) {
		osc_rate = 40000000;
	} else {
		osc_rate = 12000000;
	}
	printk_once("%susing %dMHz osc clk now!\n",
			osc_rate == clk_get_rate(osc_clk) ? "":"Warning: ",
			osc_rate / 1000000);
	return osc_rate;
}

/* Get pll rate */
static unsigned long sf_pll_recalc_rate(struct clk_hw *hw,
		unsigned long parent_rate)
{
	unsigned long long temp, refdiv, fbdiv, frac, postdiv1, postdiv2;
	unsigned long rate;
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);
	fbdiv = (sf_pll_read(pll->base, PLL_PARA + 0x4) << 8) | sf_pll_read(pll->base, PLL_PARA);
	fbdiv &= 0xFFF;
	refdiv = sf_pll_read(pll->base, PLL_PARA + 0x14) >> 2;
	refdiv &= 0x3F;
	if (readl(pll->base) & 0x20) {
		frac = 0;
	} else {
		frac = sf_pll_read(pll->base, PLL_PARA + 0x10) & 0xF;
		frac = frac << 8;
		frac |= sf_pll_read(pll->base, PLL_PARA + 0xC);
		frac = frac << 8;
		frac |= sf_pll_read(pll->base, PLL_PARA + 0x8);
		frac = frac << 4;
		frac |= (sf_pll_read(pll->base, PLL_PARA + 0x4) >> 4);
	}
	postdiv1 = (sf_pll_read(pll->base, PLL_PARA + 0x10) >> 4) & 0x7;
	postdiv2 = (sf_pll_read(pll->base, PLL_PARA + 0x14) & 0x3) << 1;
	postdiv2 |= sf_pll_read(pll->base, PLL_PARA + 0x10) >> 7;

	// 12M osc
	if (frac) {
		temp = (get_oscclk_rate(hw) * ((fbdiv << 24) + frac)) >> 12;
		rate = DIV_ROUND_UP_ULL(temp, (refdiv * postdiv1 * postdiv2 * (1 << 12)));
	} else {
		rate = get_oscclk_rate(hw) * fbdiv;
		rate = DIV_ROUND_UP_ULL(rate, (refdiv * postdiv1 * postdiv2));
	}

	/* FIXME: cpu-pll is regarded as cpu-clk because of dvfs bug. */
	if (strcmp(hw->init->name, "cpupll") == 0)
		return rate / 2;
	else
		return rate;
}

static long sf_pll_round_rate(struct clk_hw *hw, unsigned long rate,
					unsigned long *parent_rate)
{
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);

	if((rate < MIN_PLL_FREQ) || (rate > pll->max_rate))
		return -EINVAL;

	return (long)rate;
}

static int sf_pll_set_rate(struct clk_hw *hw, unsigned long rate,
					unsigned long parent_rate)
{
	struct sf_clk_pll *pll = container_of(hw, struct sf_clk_pll, hw);
	unsigned long long pll_para = 0, tmp;
	unsigned long flags, div;
	int i;
	unsigned char postdiv;

	spin_lock_irqsave(&pll->lock, flags);

#ifdef CONFIG_SF16A18_MPW0
	//6M osc
	div = rate / 1000000 / 6;
#else
	//12M osc
	div = rate / get_oscclk_rate(hw);
#endif
	//read postdiv from bit 32-39
	tmp = sf_pll_read(pll->base, PLL_PARA + 4 * 4);
	postdiv = (tmp >> 4) & 0x7;
	div = div * postdiv;

	//A rate change on cpu-pll need switch cpu-clk to other plls first.
	//switch cpu clk to cmn pll
	if(strcmp(hw->init->name, "cpupll") == 0) {
		writeb(1, (void*)CPU_CLK_SRC); /* ddr_pll */
#ifndef CONFIG_SF16A18_FULL_MASK
		/* FIXME: cpu-pll is regarded as cpu-clk because of dvfs bug. */
		div = div * 2;
#endif /* CONFIG_SF16A18_FULL_MASK */
	}

	//power down pll
	sf_pll_disable(hw);

	//clear load bit
	sf_pll_write(sf_pll_read(pll->base, PLL_LOAD) & ~BIT(5),
			pll->base, PLL_LOAD);

	pll_para = (tmp << 32) + div;

	//set parameter
	for(i = 0; i < 5; i++) {
		sf_pll_write(pll_para, pll->base, PLL_PARA + 4 * i);
		pll_para = pll_para >> 8;
	}

	//set load bit
	sf_pll_write(sf_pll_read(pll->base, PLL_LOAD) | BIT(5),
						pll->base, PLL_LOAD);

	//clear power down
	sf_pll_enable(hw);

	//check pll locked and stable
	while(!sf_pll_read(pll->base, PLL_LOCK));

	//swtich cpu clk back to cpu pll
	if(strcmp(hw->init->name, "cpupll") == 0)
		writeb(0, (void*)CPU_CLK_SRC);

	spin_unlock_irqrestore(&pll->lock, flags);

	return 0;
}

static struct clk_ops sf_clk_pll_ops = {
	.enable		= sf_pll_enable,
	.disable	= sf_pll_disable,
	.is_enabled	= sf_pll_is_enabled,
	.recalc_rate	= sf_pll_recalc_rate,
	.round_rate	= sf_pll_round_rate,
	.set_rate	= sf_pll_set_rate,
};

static void sf19a28_pll_setup(struct device_node *np, int pll_type)
{
	struct clk *clk;
	struct clk_init_data *init;
	struct sf_clk_pll *pll;
	struct clk_onecell_data *clk_data;
	const struct of_device_id *match;
	const char **pll_name;
	sf_clk_data_t *data;

	pll = kzalloc(sizeof(*pll), GFP_KERNEL);
	if (!pll)
		goto out_of_mem;

	init = kzalloc(sizeof(*init), GFP_KERNEL);
	if (!init) {
		kfree(pll);
		goto out_of_mem;
	}

	pll_name = kzalloc(sizeof(char*), GFP_KERNEL);
	if (!pll_name) {
		kfree(pll);
		kfree(init);
		goto out_of_mem;
	}

	match = sf_get_match(pll_type, np);
	if (!match)
		goto err;

	data = (sf_clk_data_t *)match->data;

	if(of_property_read_string(np, "clock-output-names", pll_name))
		goto err;

	pll->base = of_iomap(np, 0);
	if (!pll->base)
		goto err;

	if(of_property_read_u32(np, "max-freq", &pll->max_rate))
		goto err;

	spin_lock_init(&pll->lock);

	init->name = *pll_name;
	init->flags = data->clk_flags | CLK_IS_BASIC;
	init->ops = &sf_clk_pll_ops;
	init->parent_names = clk_mux_get_parents(np, &init->num_parents);

	pll->hw.init = init;

	clk = clk_register(NULL, &pll->hw);
	if (IS_ERR(clk)) {
		kfree(init->parent_names);
		goto err;
	} else {
		clk_data = kzalloc(sizeof(struct clk_onecell_data), GFP_KERNEL);
		if (!clk_data)
			goto err;

		clk_data->clk_num = 1;
		clk_data->clks = kzalloc(clk_data->clk_num * sizeof(struct clk *),
				GFP_KERNEL);
		if (!clk_data->clks) {
			kfree(clk_data);
			goto err;
		}

		clk_data->clks[0] = clk;
		of_clk_add_provider(np, of_clk_src_onecell_get, clk_data);
		return;
	}
err:
	kfree(pll_name);
	kfree(init);
	kfree(pll);
out_of_mem:
	pr_err("%s failed!\n", __func__);
	return;
}

#define BUILD_SFCLK_SETUP(name, cfg)					\
static void __init sf19a28_##name##_cfg_setup(struct device_node *np)	\
{									\
	sf19a28_cfg_setup(np, cfg);					\
	return;								\
}

#define BUILD_SFPLL_SETUP(name, pll)					\
static void __init sf19a28_##name##_pll_setup(struct device_node *np)	\
{									\
	sf19a28_pll_setup(np, pll);					\
	return;								\
}

BUILD_SFCLK_SETUP(bus1, BUS1_CFG)
BUILD_SFCLK_SETUP(bus2, BUS2_CFG)
BUILD_SFCLK_SETUP(bus3, BUS3_CFG)
BUILD_SFCLK_SETUP(cpu, CPU_CFG)
BUILD_SFCLK_SETUP(pbus, PBUS_CFG)
BUILD_SFCLK_SETUP(memphy, MEMPHY_CFG)
BUILD_SFCLK_SETUP(uart, UART_CFG)
#ifdef CONFIG_SF19A28_MPW1
BUILD_SFCLK_SETUP(eth_ref, ETH_REF_CFG)
BUILD_SFCLK_SETUP(eth_byp_ref, ETH_BYP_REF_CFG)
#endif
#ifdef CONFIG_SF19A28_FULLMASK
BUILD_SFCLK_SETUP(gmac_byp_ref, GMAC_BYP_REF_CFG)
#endif
BUILD_SFCLK_SETUP(eth_tsu, ETH_TSU_CFG)
BUILD_SFCLK_SETUP(m_SFDSP_0, M_SFDSP_0_CFG)
BUILD_SFCLK_SETUP(m_SFDSP_1, M_SFDSP_1_CFG)
BUILD_SFCLK_SETUP(wlan24, WLAN24_CFG)
BUILD_SFCLK_SETUP(wlan5, WLAN5_CFG)
BUILD_SFCLK_SETUP(usbphy, USBPHY_CFG)
BUILD_SFCLK_SETUP(tclk, TCLK_CFG)
BUILD_SFCLK_SETUP(catip_lp, CATIP_LP_CFG)

CLK_OF_DECLARE(sf19a28_bus1_cfg,"siflower,sf19a28-bus1-clk",sf19a28_bus1_cfg_setup);
CLK_OF_DECLARE(sf19a28_bus2_cfg,"siflower,sf19a28-bus2-clk",sf19a28_bus2_cfg_setup);
CLK_OF_DECLARE(sf19a28_bus3_cfg,"siflower,sf19a28-bus3-clk",sf19a28_bus3_cfg_setup);
CLK_OF_DECLARE(sf19a28_cpu_cfg,"siflower,sf19a28-cpu-clk",sf19a28_cpu_cfg_setup);
CLK_OF_DECLARE(sf19a28_pbus_cfg,"siflower,sf19a28-pbus-clk",sf19a28_pbus_cfg_setup);
CLK_OF_DECLARE(sf19a28_memphy_cfg,"siflower,sf19a28-memphy-clk",sf19a28_memphy_cfg_setup);
CLK_OF_DECLARE(sf19a28_uart_cfg,"siflower,sf19a28-uart-clk",sf19a28_uart_cfg_setup);
#ifdef CONFIG_SF19A28_MPW1
CLK_OF_DECLARE(sf19a28_eth_ref_cfg,"siflower,sf19a28-eth-ref-clk",sf19a28_eth_ref_cfg_setup);
CLK_OF_DECLARE(sf19a28_eth_byp_ref_cfg,"siflower,sf19a28-eth-byp-ref-clk",sf19a28_eth_byp_ref_cfg_setup);
#endif
#ifdef CONFIG_SF19A28_FULLMASK
CLK_OF_DECLARE(sf19a28_gmac_byp_ref_cfg,"siflower,sf19a28-gmac-byp-ref-clk",sf19a28_gmac_byp_ref_cfg_setup);
#endif
CLK_OF_DECLARE(sf19a28_eth_tsu_cfg,"siflower,sf19a28-eth-tsu-clk",sf19a28_eth_tsu_cfg_setup);
CLK_OF_DECLARE(sf19a28_m_SFDSP_0_cfg,"siflower,sf19a28-m_SFDSP-0-clk",sf19a28_m_SFDSP_0_cfg_setup);
CLK_OF_DECLARE(sf19a28_m_SFDSP_1_cfg,"siflower,sf19a28-m_SFDSP-1-clk",sf19a28_m_SFDSP_1_cfg_setup);
CLK_OF_DECLARE(sf19a28_wlan24_cfg,"siflower,sf19a28-wlan24-mac-wt-clk",sf19a28_wlan24_cfg_setup);
CLK_OF_DECLARE(sf19a28_wlan5_cfg,"siflower,sf19a28-wlan5-mac-wt-clk",sf19a28_wlan5_cfg_setup);
CLK_OF_DECLARE(sf19a28_usbphy_cfg,"siflower,sf19a28-usbphy-ref-clk",sf19a28_usbphy_cfg_setup);
CLK_OF_DECLARE(sf19a28_tclk_cfg,"siflower,sf19a28-tclk",sf19a28_tclk_cfg_setup);
CLK_OF_DECLARE(sf19a28_catip_lp_cfg,"siflower,sf19a28-catip-lp-clk",sf19a28_catip_lp_cfg_setup);

BUILD_SFPLL_SETUP(cpu, CPU_PLL)
BUILD_SFPLL_SETUP(ddr, DDR_PLL)
BUILD_SFPLL_SETUP(cmn, CMN_PLL)

CLK_OF_DECLARE(sf19a28_cpu_pll,"siflower,sf19a28-cpu-pll",sf19a28_cpu_pll_setup);
CLK_OF_DECLARE(sf19a28_ddr_pll,"siflower,sf19a28-ddr-pll",sf19a28_ddr_pll_setup);
CLK_OF_DECLARE(sf19a28_cmn_pll,"siflower,sf19a28-cmn-pll",sf19a28_cmn_pll_setup);
