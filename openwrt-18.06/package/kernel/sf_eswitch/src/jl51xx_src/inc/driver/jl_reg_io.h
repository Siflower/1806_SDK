/*
 * Copyright (c) 2014-2024 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */
#ifndef __JL_REG_ACC_H__
#define __JL_REG_ACC_H__

#include "jl_base.h"
#include "jl_types.h"
#include "jl_error.h"
#include "jl_debug.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"
#include "jl51xx/reg_ethmac.h"
#include "jl51xx/reg_mag.h"
#include "jl51xx/reg_swcore.h"
#include "jl51xx/reg_phy.h"
#include "jl51xx/reg_fephy.h"
#include "jl51xx/reg_top.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TODO only for Little Endian, bits count should not bigger than 64 */
JL_API void jl_read_bits(jl_uint8 *buffer, jl_uint64 *bits,
					jl_uint8 start, jl_uint8 count);
JL_API void jl_write_bits(jl_uint8 *buffer, jl_uint64 bits,
					jl_uint8 start, jl_uint8 count);

JL_API jl_uint16 jl_phy_reg_read_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg);
JL_API void jl_phy_reg_write_ext(jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint16 val);
JL_API jl_api_ret_t jl_apb_reg_read(jl_uint32 reg, jl_uint32 *val);
JL_API jl_api_ret_t jl_apb_reg_burst_read(jl_uint32 reg, jl_uint32 *buf, jl_uint32 size);
JL_API jl_api_ret_t jl_apb_reg_write(jl_uint32 reg, jl_uint32 val);
JL_API jl_api_ret_t jl_apb_reg_burst_write(jl_uint32 reg, jl_uint32 *buf, jl_uint32 size);
JL_API jl_api_ret_t jl_reg_io_init(void);
JL_API jl_api_ret_t jl_reg_io_deinit(void);

#ifdef CONFIG_JLFE_DUMP_REG_WRITE
JL_API jl_api_ret_t jl_dump_file_open(void);
JL_API void jl_dump_file_close(void);
JL_API jl_api_ret_t jl_dump_direct_write(jl_uint32 phy_id, jl_uint32 page_id,
					jl_uint32 reg_id, jl_uint16 val);
JL_API jl_api_ret_t jl_dump_indirect_write(jl_uint32 addr, jl_uint32 val);
JL_API jl_api_ret_t jl_dump_indirect_burst_write(jl_uint32 addr,
					jl_uint32 size, jl_uint32 *pval);
JL_API jl_api_ret_t jl_dump_add_delay(jl_uint32 us);
JL_API jl_api_ret_t jl_dump_add_comment(char *comment);
JL_API void jl_dump_enable(void);
JL_API void jl_dump_disable(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __JL_REG_ACC_H__ */
