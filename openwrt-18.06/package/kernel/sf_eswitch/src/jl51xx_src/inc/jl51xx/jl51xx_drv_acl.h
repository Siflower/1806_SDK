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

#ifndef __JL51XX_DRV_ACL_H__
#define __JL51XX_DRV_ACL_H__

#include "jl_error.h"
#include "acl.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_acl_set(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action);
jl_ret_t jl51xx_acl_get(jl_uint8 entry_index, jl_acl_rule_t *acl_rule, jl_acl_action_t *acl_action);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_ACL_H__ */
