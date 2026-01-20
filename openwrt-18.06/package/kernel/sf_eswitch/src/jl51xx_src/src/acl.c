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

#include "jl_base.h"
#include "jl51xx/jl51xx_drv_acl.h"
#include "acl.h"


jl_ret_t _jl_acl_set(jl_uint8 entry_index, jl_acl_rule_t *acl_rule,
					jl_acl_action_t *acl_action)
{
	return jl51xx_acl_set(entry_index, acl_rule, acl_action);
}

jl_ret_t _jl_acl_get(jl_uint8 entry_index, jl_acl_rule_t *acl_rule,
					jl_acl_action_t *acl_action)
{
	return jl51xx_acl_get(entry_index, acl_rule, acl_action);
}

jl_api_ret_t jl_acl_set(jl_uint8 entry_index, jl_acl_rule_t *acl_rule,
					jl_acl_action_t *acl_action)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	JL_API_MUTEX_LOCK();
	ret = _jl_acl_set(entry_index, acl_rule, acl_action);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_acl_set);

jl_api_ret_t jl_acl_get(jl_uint8 entry_index, jl_acl_rule_t *acl_rule,
					jl_acl_action_t *acl_action)
{
	jl_api_ret_t ret;

	JL_CHECK_STAT();
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	JL_API_MUTEX_LOCK();
	ret = _jl_acl_get(entry_index, acl_rule, acl_action);
	JL_API_MUTEX_UNLOCK();

	return ret;
}
EXPORT_SYMBOL(jl_acl_get);

