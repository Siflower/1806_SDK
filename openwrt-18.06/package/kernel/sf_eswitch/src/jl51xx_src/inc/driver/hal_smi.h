#ifndef __JL_SMI_H__
#define __JL_SMI_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct smi_device_s {
	void *device;
	void *phy_mutex;
	void *apb_mutex;
};

extern struct smi_device_s g_smi_dev;

jl_api_ret_t jl_smi_init(void);
jl_api_ret_t jl_smi_deinit(void);

// void jl_smi_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
// jl_uint16 jl_smi_read(jl_uint8 phy, jl_uint8 reg);

#ifdef __cplusplus
}
#endif

#endif /* __JL_SMI_H__ */
