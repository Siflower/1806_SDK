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

#ifdef __KERNEL__
#include <linux/math64.h>
#endif
#include "jl_base.h"
#include "vlan.h"
#include "driver/jl_reg_io.h"
#include "jl51xx/jl51xx_drv_l2.h"
#include "jl51xx/jl51xx_drv_vlan.h"

static inline jl_uint32 __l2_hash(jl_l2_fib_key_t *pkey)
{
	jl_uint64 k;
	jl_uint8 i;
	jl_uint32 hashval = 0;		/* size: 52(48 + 4) bits */
	jl_uint64 hash_size = 9;
	jl_uint64 mask_hash = 0x1FF;	/* 9 bits */
	jl_uint8 rounds = 5;			/* math.floor(52 / 9) */

	k = (jl_uint64)(pkey->fid & 0xf) << 48;
	k |= pkey->mac_addr.val & JL_L2_MAC_MASK;

	hashval = (jl_uint32)k;		/* size: 52(48 + 4) bits */

	for (i = 0; i < rounds; i++) {
		hashval = hashval & mask_hash;
		k >>= hash_size;
		hashval ^= k;
	}

	return hashval & mask_hash;
}

static inline jl_ret_t __l2_fib_key_check(jl_l2_fib_key_t *pkey)
{
	if (!pkey || pkey->fid > VLAN_IDX_MAX)
		return JL_ERR_PARAM;

	if (!pkey->is_collision && pkey->entry_index >= JL_L2_LOOKUP_TABLE_SIZE)
		return JL_ERR_PARAM;

	if (pkey->is_collision && pkey->entry_index >= JL_L2_COLLISION_TABLE_SIZE)
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

static inline jl_uint32 __l2_fib_hash_index(jl_uint32 hash, jl_uint32 entry)
{
	return hash | (entry << 9);
}

static inline jl_bool __l2_fib_key_match(jl_l2_fib_key_t *a, jl_l2_fib_key_t *b)
{
	if (a == NULL || b == NULL)
		return FALSE;

	if (a->mac_addr.val != b->mac_addr.val)
		return FALSE;

	if (a->fid != b->fid)
		return FALSE;

	return TRUE;
}

static inline jl_uint32 __l2_fib_key_index(jl_l2_fib_key_t *pkey)
{
	if (pkey->is_collision) {
		return pkey->entry_index;
	} else {
		jl_uint32 hash_index = __l2_hash(pkey);

		return __l2_fib_hash_index(hash_index, pkey->entry_index);
	}
}

static inline jl_ret_t __l2_fib_dest_check(jl_l2_fib_dest_config_t *pdest_cfg)
{
	if (!pdest_cfg || pdest_cfg->is_unicast >= JL_ENABLE_END ||
			pdest_cfg->pkt_drop >= JL_ENABLE_END || pdest_cfg->port > 0x3F)
		return JL_ERR_PARAM;
	return JL_ERR_OK;
}

static inline jl_ret_t __l2_fib_aging_check(jl_l2_fib_aging_config_t *paging)
{
	if (!paging || paging->is_valid >= JL_ENABLE_END ||
				paging->is_static >= JL_ENABLE_END)
		return JL_ERR_PARAM;
	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_aging_table_get(jl_uint32 index, jl_l2_fib_aging_config_t *paging_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_AGING_TABLE_NUM)
		return JL_ERR_PARAM;
	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_AGING_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &regval);

	paging_cfg->is_static = (regval & BIT(1)) ? 1 : 0;
	paging_cfg->is_valid = regval & BIT(0);

	return ret;
}

jl_ret_t jl51xx_l2_fib_aging_table_set(jl_uint32 index, jl_l2_fib_aging_config_t *paging_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_AGING_TABLE_NUM)
		return JL_ERR_PARAM;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_AGING_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;
	/* always setup hit with 1 to prevent new itme be aged */
	regval |= BIT(2);
	if (paging_cfg->is_valid == 1)
		SET_BIT(regval, 0);
	else
		CLR_BIT(regval, 0);

	if (paging_cfg->is_static == 1)
		SET_BIT(regval, 1);
	else
		CLR_BIT(regval, 1);

	ret = jl_apb_reg_write(regaddr, regval);

	return ret;
}

jl_ret_t jl51xx_l2_fib_aging_sha_table_set(jl_uint32 index, jl_l2_fib_aging_config_t *paging_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_AGING_STATUS_SHADOW_TABLE_NUM)
		return JL_ERR_PARAM;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(
			L2_AGING_STATUS_SHADOW_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;

	if (paging_cfg->is_valid == 1)
		SET_BIT(regval, 0);
	else
		CLR_BIT(regval, 0);

	ret = jl_apb_reg_write(regaddr, regval);

	return ret;
}

jl_ret_t jl51xx_l2_col_aging_table_get(jl_uint32 index, jl_l2_fib_aging_config_t *paging_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_AGING_COLLISION_TABLE_NUM)
		return JL_ERR_PARAM;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_AGING_COLLISION_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &regval);

	paging_cfg->is_static = (regval & BIT(1)) ? 1 : 0;
	paging_cfg->is_valid = regval & BIT(0);
	return ret;
}

jl_ret_t jl51xx_l2_fib_col_aging_table_set(jl_uint32 index, jl_l2_fib_aging_config_t *paging_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_AGING_COLLISION_TABLE_NUM)
		return JL_ERR_PARAM;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_AGING_COLLISION_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;
	/* always setup hit with 1 to prevent new itme be aged */
	regval |= BIT(2);
	if (paging_cfg->is_valid == 1)
		SET_BIT(regval, 0);
	else
		CLR_BIT(regval, 0);

	if (paging_cfg->is_static == 1)
		SET_BIT(regval, 1);
	else
		CLR_BIT(regval, 1);

	ret = jl_apb_reg_write(regaddr, regval);

	return ret;
}

jl_ret_t jl51xx_l2_fib_col_aging_sha_table_set(jl_uint32 index, jl_l2_fib_aging_config_t *paging_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_AGING_COLLISION_SHADOW_TABLE_NUM)
		return JL_ERR_PARAM;
	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_AGING_COLLISION_SHADOW_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;
	/* always setup hit with 1 to prevent new itme be aged */
	regval |= BIT(2);
	if (paging_cfg->is_valid == 1)
		SET_BIT(regval, 0);
	else
		CLR_BIT(regval, 0);

	if (paging_cfg->is_static == 1)
		SET_BIT(regval, 1);
	else
		CLR_BIT(regval, 1);

	ret = jl_apb_reg_write(regaddr, regval);

	return ret;
}

jl_ret_t jl51xx_l2_mc_port_mask_get(jl_uint32 index, jl_uint32 *pregval)
{
	jl_ret_t ret;
	jl_uint32 phy_mask;
	jl_uint32 mac_mask;
	reg_addr_t regaddr;

	if (index >= L2_MULTICAST_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_MULTICAST_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &mac_mask);

	phy_mask = JL_PORTMASK_M2P(mac_mask);

	*pregval = phy_mask;

	return ret;
}

jl_ret_t jl51xx_l2_mc_port_mask_set(jl_uint32 index, jl_uint32 regval)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 val;
	jl_uint32 mac_mask;

	if (index >= L2_MULTICAST_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	if (regval >= BIT(10))
		return JL_ERR_PARAM;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_MULTICAST_TABLE_OFFSET, index, 1);

	ret = jl_apb_reg_read(regaddr, &val);
	if (ret)
		return ret;

	mac_mask = JL_PORTMASK_P2M(regval);

	WRITE_BITS(val, 0, 9, mac_mask);

	ret = jl_apb_reg_write(regaddr, val);

	return ret;
}

jl_ret_t jl51xx_l2_mc_port_learning_set(jl_port_t port, jl_enable_t en_learning)
{
	jl_ret_t ret;
	jl_uint64 default_learning;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	default_learning = en_learning;
	jl_write_bits(preg_val, default_learning, 34, 1);

	ret = jl_apb_reg_burst_write(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_mc_port_learning_get(jl_port_t port, jl_enable_t *pen_learning)
{
	jl_ret_t ret;
	jl_uint64 default_learning;
	jl_uint32 src_port_tab_val[2];
	jl_uint32 reg_addr = SWCORE_BASE + SOURCE_PORT_TABLE_OFFSET + JL_PORT_P2M(port) * 2;
	jl_uint8 *preg_val = (jl_uint8 *)&src_port_tab_val[0];

	ret = jl_apb_reg_burst_read(reg_addr, &src_port_tab_val[0], 2);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &default_learning, 34, 1);
	*pen_learning = default_learning;
	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_coltable_get(jl_uint32 index, jl_l2_fib_key_t *pkey)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint8 *ptr;
	jl_uint32 buf[2] = {0};
	jl_uint64 gid, mac;

	if (index >= L2_LOOKUP_COLLISION_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_LOOKUP_COLLISION_TABLE_OFFSET, index, 2);

	ret = jl_apb_reg_burst_read(regaddr, &buf[0], 2);
	if (ret)
		return ret;
	ptr = (jl_uint8 *)&buf[0];

	jl_read_bits(ptr, &mac, 0, 48);
	jl_read_bits(ptr, &gid, 48, 4);

	pkey->mac_addr.val = mac;
	pkey->fid = (jl_uint8)gid;

	return ret;
}

jl_ret_t jl51xx_l2_coltable_set(jl_uint32 index, jl_l2_fib_key_t *pkey)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint8 *ptr;
	jl_uint32 buf[2];
	jl_uint64 gid, mac_addr;

	if (index >= L2_LOOKUP_COLLISION_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_LOOKUP_COLLISION_TABLE_OFFSET, index, 2);

	ret = jl_apb_reg_burst_read(regaddr, &buf[0], 2);
	if (ret)
		return ret;

	ptr = (jl_uint8 *)&buf[0];
	gid = pkey->fid & VLAN_IDX_MAX;
	mac_addr = pkey->mac_addr.val;

	jl_write_bits(ptr, mac_addr, 0, 48);
	jl_write_bits(ptr, gid, 48, 4);

	ret = jl_apb_reg_burst_write(regaddr, &buf[0], 2);
	if (ret)
		return ret;

	return ret;
}

jl_ret_t jl51xx_l2_hashtable_get(jl_uint32 index, jl_l2_fib_key_t *pkey)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint8 *ptr;
	jl_uint32 buf[2] = {0};
	jl_uint64 gid, mac;

	if (index >= L2_DA_HASH_LOOKUP_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_DA_HASH_LOOKUP_TABLE_OFFSET, index, 2);
	ret = jl_apb_reg_burst_read(regaddr, buf, 2);

	ptr = (jl_uint8 *)&buf[0];

	jl_read_bits(ptr, &mac, 0, 48);
	jl_read_bits(ptr, &gid, 48, 4);

	pkey->mac_addr.val = mac;
	pkey->fid = (jl_uint8)gid & VLAN_IDX_MAX;

	return ret;
}

jl_ret_t jl51xx_l2_hashtable_set(jl_uint32 index, jl_l2_fib_key_t *pkey)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint8 *ptr;
	jl_uint32 buf[2];
	jl_uint64 gid, mac_addr;

	if (index >= L2_DA_HASH_LOOKUP_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_DA_HASH_LOOKUP_TABLE_OFFSET, index, 2);
	ret = jl_apb_reg_burst_read(regaddr, &buf[0], 2);
	if (ret)
		return ret;

	ptr = (jl_uint8 *)&buf[0];
	gid = pkey->fid & VLAN_IDX_MAX;
	mac_addr = pkey->mac_addr.val;

	jl_write_bits(ptr, mac_addr, 0, 48);
	jl_write_bits(ptr, gid, 48, 4);

	ret = jl_apb_reg_burst_write(regaddr, &buf[0], 2);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_dest_set(jl_uint32 index, jl_l2_fib_dest_config_t *pdest_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;
	jl_uint8 *ptr;
	jl_uint64 uc, port, drop;

	if (index >= L2_DESTINATION_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_DESTINATION_TABLE_OFFSET, index, 1);
	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;

	ptr = (jl_uint8 *)&regval;
	uc = pdest_cfg->is_unicast;
	port = JL_PORT_P2M(pdest_cfg->port);
	drop = pdest_cfg->pkt_drop;
	jl_write_bits(ptr, uc, 0, 1);
	jl_write_bits(ptr, port, 1, 6);
	jl_write_bits(ptr, drop, 7, 1);

	ret = jl_apb_reg_write(regaddr, regval);

	return ret;
}

jl_ret_t jl51xx_l2_fib_dest_get(jl_uint32 index, jl_l2_fib_dest_config_t *pdest_cfg)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	if (index >= L2_DESTINATION_TABLE_NUM)
		return JL_ERR_OUT_OF_RANGE;

	regaddr = SWCORE_L2_REG_ENTRY_ADDR(L2_DESTINATION_TABLE_OFFSET, index, 1);
	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;

	pdest_cfg->is_unicast = regval & BIT(0);
	pdest_cfg->port = JL_PORT_M2P((regval >> 1) & 0x3F);
	pdest_cfg->pkt_drop = (regval & BIT(7)) ? 1 : 0;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_tick_configuration_get(jl_uint16 *pdiv, jl_uint8 *pstep)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	regaddr = SWCORE_L2_REG_ADDR(TICK_CONFIGURATION_OFFSET);

	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;
	*pdiv = regval & 0x3FFF;
	*pstep = (regval >> 14) & 0x0F;

	return JL_ERR_OK;
}

static jl_ret_t jl51xx_l2_time_to_age_set(jl_uint8 tick, jl_uint32 tick_cnt)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint8 *ptr;
	jl_uint32 buf[2];
	jl_uint64 tickcnt, tickindex;

	if (tick > 0x07) // 3bit
		return JL_ERR_PARAM;

	regaddr = SWCORE_L2_REG_ADDR(TIME_TO_AGE_OFFSET);
	ptr = (jl_uint8 *)&buf[0];
	ret = jl_apb_reg_burst_read(regaddr, &buf[0], 2);
	if (ret)
		return ret;

	tickcnt = tick_cnt;
	tickindex = tick;
	jl_write_bits(ptr, tickcnt, 0, 32);
	jl_write_bits(ptr, tickindex, 32, 3);
	ret = jl_apb_reg_burst_write(regaddr, &buf[0], 2);
	if (ret)
		return ret;

	return ret;
}

jl_ret_t jl51xx_l2_time_to_age_get(jl_uint8 *ptick, jl_uint32 *ptick_cnt)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 buf[2] = {0};

	regaddr = SWCORE_L2_REG_ADDR(TIME_TO_AGE_OFFSET);

	ret = jl_apb_reg_burst_read(regaddr, buf, 2);
	if (ret)
		return ret;

	*ptick = buf[1] & 0x7;
	*ptick_cnt = buf[0];

	return JL_ERR_OK;
}

static jl_ret_t _jl_l2_fib_aging_set(CONST_T jl_uint32 index, jl_l2_fib_aging_config_t *cfg)
{
	jl_ret_t ret;

	ret = __l2_fib_aging_check(cfg);
	if (ret)
		return ret;

	if ((ret = jl51xx_l2_fib_aging_table_set(index, cfg)) != JL_ERR_OK)
		return ret;

	if ((ret = jl51xx_l2_fib_aging_sha_table_set(index, cfg)) != JL_ERR_OK)
		return ret;
	return JL_ERR_OK;
}

static jl_ret_t _jl_l2_fib_col_aging_set(CONST_T jl_uint32 index, jl_l2_fib_aging_config_t *cfg)
{
	jl_ret_t ret;

	ret = __l2_fib_aging_check(cfg);
	if (ret)
		return ret;

	if ((ret = jl51xx_l2_fib_col_aging_table_set(index, cfg)) != JL_ERR_OK)
		return ret;
	if ((ret = jl51xx_l2_fib_col_aging_sha_table_set(index, cfg)) != JL_ERR_OK)
		return ret;
	return JL_ERR_OK;
}

static jl_ret_t _jl_l2_fib_dest_set(CONST_T jl_uint32 index, jl_l2_fib_dest_config_t *cfg)
{
	jl_ret_t ret;

	ret = __l2_fib_dest_check(cfg);
	if (ret)
		return ret;

	return jl51xx_l2_dest_set(index, cfg);
}

static jl_ret_t _l2_fib_dest_get(jl_uint32 index, jl_l2_fib_dest_config_t *pdest)
{
	return jl51xx_l2_fib_dest_get(index, pdest);

}

static jl_ret_t _jl_l2_fib_col_dest_set(CONST_T jl_uint32 index, jl_l2_fib_dest_config_t *cfg)
{
	return _jl_l2_fib_dest_set(
			index + JL_L2_COL_DA_OFFSET,
			cfg);
}

static jl_ret_t _l2_fib_col_dest_get(CONST_T jl_uint32 index, jl_l2_fib_dest_config_t *cfg)
{
	return jl51xx_l2_fib_dest_get(
			index + JL_L2_COL_DA_OFFSET,
			cfg);
}

jl_ret_t jl51xx_l2_aging_time_set(jl_uint32 time, jl_uint8 unit)
{
	jl_ret_t ret;
	jl_uint32 period_ns = 160; /* 1 / core_freq(6.25M) */
	jl_uint64 max_tick_cnt = 0xFFFFFFFF; /* 2^32 - 1 */
	jl_uint8 max_tick_index = 4;
	jl_uint64 t_ns = time * 2 / 3;
	jl_uint16 div;
	jl_uint8 step;
	jl_uint64 tick0_period;
	jl_uint64 aim_tick_cnt;
	jl_uint8 tick_index;

	if (unit >= L2_AGING_END)
		return JL_ERR_PARAM;

	switch (unit) {
	case L2_AGING_S:
		t_ns *= 1000;
		/* fallthrough */
	case L2_AGING_MS:
		t_ns *= 1000;
		/* fallthrough */
	case L2_AGING_US:
		t_ns *= 1000;
		/* fallthrough */
	case L2_AGING_NS:
		break;
	}

	if ((ret = jl51xx_l2_tick_configuration_get(
					&div, &step)) != JL_ERR_OK)
		return ret;

	tick0_period = period_ns * div;
	#ifdef __KERNEL__
	aim_tick_cnt = div_u64(t_ns, tick0_period);
	#else
	aim_tick_cnt = t_ns / tick0_period;
	#endif

	tick_index = 0;
	while (aim_tick_cnt > max_tick_cnt) {
		#ifdef __KERNEL__
		aim_tick_cnt = div_u64(aim_tick_cnt, step);
		#else
		aim_tick_cnt /= step;
		#endif

		tick_index += 1;

		if (tick_index >= max_tick_index) {
			break;
		}
	}

	/* Set the minimum aging time to 1us when input time is not zero */
	if (time != 0 && tick_index == 0 && aim_tick_cnt == 0)
		aim_tick_cnt = 1;

	return jl51xx_l2_time_to_age_set(
			tick_index,
			(jl_uint32)aim_tick_cnt);
}

jl_ret_t jl51xx_l2_aging_time_get(jl_uint32 *ptime, jl_uint8 *punit)
{
	jl_ret_t ret;
	jl_uint32 period_ns = 160; /* 1 / core_freq(6.25M) */
	//jl_uint64 max_tick_cnt = 0xFFFFFFFF; /* 2^32 - 1 */
	jl_uint8 max_tick_index = 4;
	jl_uint64 t_ns = 0;
	jl_uint16 div;
	jl_uint8 step;
	jl_uint64 tick0_period;
	jl_uint32 cur_tick_cnt;
	jl_uint8 cur_tick_index;
	jl_uint64 tick_cnt = 0;

	if ((ret = jl51xx_l2_tick_configuration_get(&div, &step)) != JL_ERR_OK)
		return ret;

	tick0_period = period_ns * div;

	ret = jl51xx_l2_time_to_age_get(
			&cur_tick_index,
			&cur_tick_cnt);
	if (ret)
		return ret;

	if (cur_tick_index >= max_tick_index)
		return JL_ERR_PARAM;

	tick_cnt = cur_tick_cnt;
#ifdef __KERNEL__
	tick_cnt = div_u64(tick_cnt * 3, 2);
#else
	tick_cnt = tick_cnt * 3 / 2;
#endif
	while (cur_tick_index > 0) {
		tick_cnt = tick_cnt * step;
		cur_tick_index --;
	}

	t_ns = tick_cnt * tick0_period;

	if (t_ns < 1000)
		*punit = L2_AGING_NS;
	else if (t_ns < 1000000) {
		#ifdef __KERNEL__
		t_ns = div_u64(t_ns, 1000);
		#else
		t_ns /= 1000;
		#endif
		*punit = L2_AGING_US;
	} else if (t_ns < 1000000000) {
		#ifdef __KERNEL__
		t_ns = div_u64(t_ns, 1000000);
		#else
		t_ns /= 1000000;
		#endif
		*punit = L2_AGING_MS;
	} else {
		#ifdef __KERNEL__
		t_ns = div_u64(t_ns, 1000000000);
		#else
		t_ns /= 1000000000;
		#endif
		*punit = L2_AGING_S;
	}

	*ptime = (jl_uint32)t_ns;
	return JL_ERR_OK;
}

jl_ret_t _l2_fib_config_set(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *cfg)
{
	jl_ret_t ret;
	jl_uint32 index;
	jl_enable_t cur_aging, cur_learning;

	index = pkey->entry_index;

	if ((ret = jl51xx_l2_aging_learning_get(&cur_aging,
					&cur_learning)) != JL_ERR_OK)
		return ret;

	if ((ret = jl51xx_l2_aging_learning_set(0, 0)) != JL_ERR_OK)
		return ret;

	if (pkey->is_collision) {
		//set collision hash table
		//set collision aging table
		//set collision dest table +2048

		if ((ret = jl51xx_l2_coltable_set(index, pkey)) != JL_ERR_OK)
			return ret;
		if ((ret = _jl_l2_fib_col_aging_set(index, &cfg->aging)) != JL_ERR_OK)
			return ret;
		if ((ret = _jl_l2_fib_col_dest_set(index, &cfg->dest)) != JL_ERR_OK)
			return ret;
	} else {
		// set lut hash table
		// set lut aging table
		// set dest table+0
		if ((ret = jl51xx_l2_hashtable_set(index, pkey)) != JL_ERR_OK)
			return ret;
		if ((ret = _jl_l2_fib_aging_set(index, &cfg->aging)) != JL_ERR_OK)
			return ret;
		if ((ret = _jl_l2_fib_dest_set(index, &cfg->dest)) != JL_ERR_OK)
			return ret;
	}

	if ((ret = jl51xx_l2_aging_learning_set(cur_aging, cur_learning)) != JL_ERR_OK)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_fib_config_set_auto(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *cfg)
{
	jl_ret_t ret;
	jl_uint32 hash = 0;
	jl_uint16 offset = 0;
	jl_int8 first_invalid = -1;
	jl_int8 first_match = -1;
	jl_l2_fib_key_t keys[JL_L2_LOOKUP_MAX_ENTRY];
	jl_l2_fib_aging_config_t agings[JL_L2_LOOKUP_MAX_ENTRY];
	jl_l2_fib_key_t *k = NULL;
	jl_l2_fib_aging_config_t *a = NULL;
	jl_int8 aim_index = -1;
	jl_uint8 i;

	if ((ret = __l2_fib_key_check(pkey)) != JL_ERR_OK)
		return ret;

	hash = __l2_hash(pkey);

	/* load keys from hash-table */
	for (i = 0; i < JL_L2_LOOKUP_TABLE_BUCKET_NUM; i++) {
		jl_uint32 index = __l2_fib_hash_index(hash, i);

		ret = jl51xx_l2_hashtable_get(
				index,
				&keys[offset]);
		if (ret)
			return ret;

		ret = jl51xx_l2_aging_table_get(
				index,
				&agings[offset]);
		if (ret)
			return ret;
		offset += 1;
	}

	/* load keys from col-table */
	for (i = 0; i < JL_L2_COLLISION_TABLE_ENTRY_NUM; i++) {
		ret = jl51xx_l2_coltable_get(i, &keys[offset]);
		if (ret)
			return ret;
		ret = jl51xx_l2_col_aging_table_get(
				i,
				&agings[offset]);
		if (ret)
			return ret;
		offset += 1;
	}

	/* find match or invalid slot */
	for (i = 0; i < JL_L2_LOOKUP_MAX_ENTRY; i++) {
		k = &keys[i];
		a = &agings[i];
		if (!a->is_valid) {
			if (first_invalid < 0) {
				first_invalid = i;
			}
			/* find next one */
			continue;
		}

		if (__l2_fib_key_match(pkey, k)) {

			if (first_match < 0) {
				first_match = i;
			}
			/* break, if found someone match */
			break;
		}

		/* do nothing for else */
	}

	/* check if we don't have site for this key */
	if (first_match >= 0) {
		aim_index = first_match;
	} else if (first_invalid >= 0) {
		aim_index = first_invalid;
	} else {
		return JL_ERR_OUT_OF_RANGE;
	}

	/* setup key automaticly */
	if (aim_index < JL_L2_LOOKUP_TABLE_BUCKET_NUM) {
		pkey->is_collision = FALSE;
		pkey->entry_index = __l2_fib_hash_index(hash, aim_index);
	} else {
		pkey->is_collision = TRUE;
		pkey->entry_index = aim_index - JL_L2_LOOKUP_TABLE_BUCKET_NUM;
	}

	ret = _l2_fib_config_set(pkey, cfg);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_fib_config_get_by_index(jl_uint32 entry_index, jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *pcfg)
{
	jl_ret_t ret;

	if (entry_index >= JL_L2_LOOKUP_TABLE_SIZE) {
		pkey->is_collision = 1;
		ret = jl51xx_l2_coltable_get(entry_index - JL_L2_LOOKUP_TABLE_SIZE, pkey);
		if (ret)
			return ret;

		ret = jl51xx_l2_col_aging_table_get(entry_index - JL_L2_LOOKUP_TABLE_SIZE, &pcfg->aging);
		if (ret)
			return ret;
	} else {
		pkey->is_collision = 0;
		ret = jl51xx_l2_hashtable_get(entry_index, pkey);
		if (ret)
			return ret;

		ret = jl51xx_l2_aging_table_get(entry_index, &pcfg->aging);
		if (ret)
			return ret;
	}

	pkey->entry_index = entry_index;
	ret = jl51xx_l2_fib_dest_get(entry_index, &pcfg->dest);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_fib_config_get(jl_l2_fib_key_t *pkey, jl_l2_fib_config_t *cfg)
{
	jl_ret_t ret;
	jl_uint32 index;
	jl_l2_fib_key_t tmp_key;
	jl_l2_fib_aging_config_t tmp_aging;
	jl_uint32 hash_index;
	jl_uint8 i = 0;
	jl_int32 match_idx = -1;

	hash_index = __l2_hash(pkey);

	/* search in 4 way hash lookup table */
	for (i = 0; i < JL_L2_LOOKUP_TABLE_BUCKET_NUM; i++) {
		index = __l2_fib_hash_index(hash_index, i);
		if ((ret = jl51xx_l2_aging_table_get(index, &tmp_aging)) != JL_ERR_OK)
			return ret;

		/* not valid in hash table, continue... */
		if (tmp_aging.is_valid != 1)
			continue;

		if ((ret = jl51xx_l2_hashtable_get(index, &tmp_key)) != JL_ERR_OK)
			return ret;

		/* mismatch in hash table, continue... */
		if (!__l2_fib_key_match(pkey, &tmp_key))
			continue;

		if (match_idx < 0) {
			match_idx = index;
			pkey->is_collision = 0;
			break;
		}
	}

	if (match_idx < 0) {
		for (i = 0; i < JL_L2_COLLISION_TABLE_ENTRY_NUM; i++) {

			if ((ret = jl51xx_l2_col_aging_table_get(i, &tmp_aging)) != JL_ERR_OK)
				return ret;

			/* not valid in collision table, continue */
			if (tmp_aging.is_valid != 1)
				continue;

			if ((ret = jl51xx_l2_coltable_get(i, &tmp_key)) != JL_ERR_OK)
				return ret;

			/* mismatch in collision table, continue... */
			if (!__l2_fib_key_match(pkey, &tmp_key))
				continue;

			if (match_idx < 0) {
				pkey->is_collision = 1;
				match_idx = i;
				break;
			}

		}
	}

	if (match_idx < 0) {
		/* not matched finally */
		return JL_ERR_NOT_FOUND;
	}

	pkey->entry_index = match_idx;

	if (pkey->is_collision) {
		if ((ret = jl51xx_l2_col_aging_table_get(match_idx, &cfg->aging)) != JL_ERR_OK)
			return ret;
		if ((ret = _l2_fib_col_dest_get(match_idx, &cfg->dest)) != JL_ERR_OK)
			return ret;
	} else {
		if ((ret = jl51xx_l2_aging_table_get(match_idx, &cfg->aging)) != JL_ERR_OK)
			return ret;
		if ((ret = _l2_fib_dest_get(match_idx, &cfg->dest)) != JL_ERR_OK)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_fib_hash_clear(void)
{
	jl_ret_t ret;
	jl_l2_fib_aging_config_t aging = {
		.is_valid = 0,
		.is_static = 0,
	};
	jl_l2_table_entry_idx_t i;

	for (i = 0; i < JL_L2_LOOKUP_TABLE_SIZE; i++) {
		ret = _jl_l2_fib_aging_set(i, &aging);
		if (ret)
			return ret;
	}
	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_fib_col_clear(void)
{
	jl_l2_fib_aging_config_t aging = {
		.is_valid = 0,
		.is_static = 0,
	};
	jl_l2_table_entry_idx_t i;

	for (i = 0; i < JL_L2_COLLISION_TABLE_SIZE; i++)
		_jl_l2_fib_col_aging_set(i, &aging);

	return JL_ERR_OK;
}

jl_ret_t jl51xx_l2_aging_learning_get(jl_enable_t *penable_aging, jl_enable_t *penable_learning)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	regaddr = SWCORE_L2_REG_ADDR(LEARNING_AND_AGING_ENABLE_OFFSET);

	ret = jl_apb_reg_read(regaddr, &regval);

	*penable_aging = regval & BIT(1) ? 1 : 0;
	*penable_learning = regval & BIT(0) ? 1 : 0;
	return ret;
}

jl_ret_t jl51xx_l2_aging_learning_set(jl_enable_t enable_aging, jl_enable_t enable_learning)
{
	jl_ret_t ret;
	reg_addr_t regaddr;
	jl_uint32 regval;

	regaddr = SWCORE_L2_REG_ADDR(LEARNING_AND_AGING_ENABLE_OFFSET);

	ret = jl_apb_reg_read(regaddr, &regval);
	if (ret)
		return ret;

	if (enable_aging == 0)
		CLR_BIT(regval, 1);
	else
		SET_BIT(regval, 1);

	if (enable_learning == 0)
		CLR_BIT(regval, 0);
	else
		SET_BIT(regval, 0);

	ret = jl_apb_reg_write(regaddr, regval);

	return ret;
}
