#include <linux/clk.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

static int __init siflower_cpufreq_init(void)
{
	platform_device_register_simple("cpufreq-dt", -1, NULL, 0);
	return 0;
}
device_initcall(siflower_cpufreq_init);
