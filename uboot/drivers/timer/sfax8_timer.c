#include <common.h>
#include <dm.h>
#include <errno.h>
#include <timer.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

/* control register */
#define SF_TIMER_START	1 << 0	/* Start timer */
#define SF_TIMER_STOP	0 << 0	/* Stop timer */

#define TIMER_CURRENTVAL	0x4
#define TIMER_CONTROL		0x8

struct sf_timer_platdata {
	u32 base;	// timer reg base
};


static int sf_timer_get_count(struct udevice *dev, u64 *count)
{
	struct sf_timer_platdata *plat = dev->platdata;
	u32 val;

	val = ~readl((void *)(plat->base + TIMER_CURRENTVAL));
	*count = timer_conv_64(val);
	return 0;
}

static int sf_timer_probe(struct udevice *dev)
{
	struct sf_timer_platdata *plat = dev->platdata;

	writel(SF_TIMER_STOP, (void *)(plat->base + TIMER_CONTROL));

	writel(SF_TIMER_START, (void *)(plat->base + TIMER_CONTROL));

	return 0;
}

static int sf_timer_ofdata_to_platdata(struct udevice *dev)
{
	struct sf_timer_platdata *plat = dev_get_platdata(dev);

	plat->base = dev_get_addr(dev);

	return 0;
}

static const struct timer_ops sf_timer_ops = {
	.get_count = sf_timer_get_count,
};

static const struct udevice_id sf_timer_ids[] = {
	{ .compatible = "siflower,timer0" },
	{}
};

U_BOOT_DRIVER(sf_timer) = {
	.name	= "sfax8_timer",
	.id	= UCLASS_TIMER,
	.of_match = sf_timer_ids,
	.ofdata_to_platdata = sf_timer_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct sf_timer_platdata),
	.probe = sf_timer_probe,
	.ops	= &sf_timer_ops,
	.flags = DM_FLAG_PRE_RELOC,
};
