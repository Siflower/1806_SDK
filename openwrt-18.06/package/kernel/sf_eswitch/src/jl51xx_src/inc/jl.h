
#ifndef JL_H
#define JL_H

#include "acl.h"
#include "cpu.h"
#include "fc.h"
#include "jl_base.h"
#include "jl_config.h"
#include "jl_debug.h"
#include "jl_error.h"
#include "jl_switch.h"
#include "jl_types.h"
#include "l2.h"
#include "led.h"
#include "lpd.h"
#include "mib.h"
#include "mirror.h"
#include "port.h"
#include "qos.h"
#include "sram_load.h"
#include "storm.h"
#include "stp.h"
#include "version.h"
#include "vlan.h"
#include "jl51xx/jl51xx_drv_acl.h"
#include "jl51xx/jl51xx_drv_cpu.h"
#include "jl51xx/jl51xx_drv_fc.h"
#include "jl51xx/jl51xx_drv_l2.h"
#include "jl51xx/jl51xx_drv_led.h"
#include "jl51xx/jl51xx_drv_lpd.h"
#include "jl51xx/jl51xx_drv_mib.h"
#include "jl51xx/jl51xx_drv_mirror.h"
#include "jl51xx/jl51xx_drv_phy.h"
#include "jl51xx/jl51xx_drv_port.h"
#include "jl51xx/jl51xx_drv_qos.h"
#include "jl51xx/jl51xx_drv_sram_load.h"
#include "jl51xx/jl51xx_drv_storm.h"
#include "jl51xx/jl51xx_drv_stp.h"
#include "jl51xx/jl51xx_drv_switch.h"
#include "jl51xx/jl51xx_drv_vlan.h"
#include "driver/jl_reg_io.h"

void port_udelay(jl_uint32 us)
{
	udelay(us);
}

#endif