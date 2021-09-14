#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>

#include <asm/cacheflush.h>
#include <asm/dma-coherence.h>
#include <asm/fw/fw.h>
#include <asm/mips-boards/generic.h>
#include <asm/mips-cps.h>
#include <asm/prom.h>
#include <asm/smp-ops.h>
#include <asm/traps.h>

void __init plat_mem_setup(void)
{
	set_io_port_base(KSEG1);
#ifdef CONFIG_BUILTIN_DTB
	__dt_setup_arch(__dtb_start);
#endif
}

static void __init mips_nmi_setup(void)
{
	void *base;
	extern char except_vec_nmi;

	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa80) :
		(void *)(CAC_BASE + 0x380);
	memcpy(base, &except_vec_nmi, 0x80);
	flush_icache_range((unsigned long)base,
			   (unsigned long)base + 0x80);
}

static void __init mips_ejtag_setup(void)
{
	void *base;
	extern char except_vec_ejtag_debug;

	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa00) :
		(void *)(CAC_BASE + 0x300);
	memcpy(base, &except_vec_ejtag_debug, 0x80);
	flush_icache_range((unsigned long)base,
			   (unsigned long)base + 0x80);
}

void __init prom_init(void)
{
	board_nmi_handler_setup = mips_nmi_setup;
	board_ejtag_handler_setup = mips_ejtag_setup;

	fw_init_cmdline();
    fw_meminit();

	mips_cm_probe();
	mips_cpc_probe();
	register_cps_smp_ops();
}

void __init device_tree_init(void)
{
	if (!initial_boot_params)
		return;

	unflatten_and_copy_device_tree();
}

const char *get_system_type(void)
{
	return "MIPS sf19a28";
}
