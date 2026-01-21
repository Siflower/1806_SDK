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

#ifndef __JL_ERROR_H__
#define __JL_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "jl_types.h"

typedef enum {
	JL_ERR_OK			= 0,
	JL_ERR_INTERNAL			= -1,
	JL_ERR_MEMORY			= -2,
	JL_ERR_UNIT			= -3,
	JL_ERR_PARAM			= -4,
	JL_ERR_EMPTY			= -5,
	JL_ERR_FULL			= -6,
	JL_ERR_NOT_FOUND		= -7,
	JL_ERR_EXISTS			= -8,
	JL_ERR_OUT_OF_RANGE		= -9,
	JL_ERR_TIMEOUT			= -10,
	JL_ERR_BUSY			= -11,
	JL_ERR_FAIL			= -12,
	JL_ERR_DISABLED			= -13,
	JL_ERR_BADID			= -14,
	JL_ERR_RESOURCE			= -15,
	JL_ERR_CONFIG			= -16,
	JL_ERR_UNAVAIL			= -17,
	JL_ERR_INIT			= -18,
	JL_ERR_PORT			= -19,
	JL_ERR_NOT_READY		= -20,
	JL_ERR_LIMIT			= -21		/* Must come last */
} jl_error_code_t;

#define _JL_ERRMSG_STR		{ \
	"Ok",					/* ERR_OK */ \
	"Internal error",			/* ERR_INTERNAL */ \
	"Out of memory",			/* ERR_MEMORY */ \
	"Invalid unit",				/* ERR_UNIT */ \
	"Invalid parameter",			/* ERR_PARAM */ \
	"Table empty",				/* ERR_EMPTY */ \
	"Table full",				/* ERR_FULL */ \
	"Entry not found",			/* ERR_NOT_FOUND */ \
	"Entry exists",				/* ERR_EXISTS */ \
	"Index out of range",			/* ERR_OUT_OF_RANGE */ \
	"Operation timed out",			/* ERR_TIMEOUT */ \
	"Operation still running",		/* ERR_BUSY */ \
	"Operation failed",			/* ERR_FAIL */ \
	"Operation disabled",			/* ERR_DISABLED */ \
	"Invalid identifier",			/* ERR_BADID */ \
	"No resources for operation",		/* ERR_RESOURCE */ \
	"Invalid configuration",		/* ERR_CONFIG */ \
	"Feature unavailable",			/* ERR_UNAVAIL */ \
	"Feature not initialized",		/* ERR_INIT */ \
	"Invalid port",				/* ERR_PORT */ \
	"Device not ready",			/* ERR_NOT_READY */ \
	"Unknown error"				/* ERR_LIMIT */ \
	}

JL_API extern char *_jl_errmsg[];

#define JL_ERRMSG(r)          \
	_jl_errmsg[(((jl_int32)r) <= 0 && ((int)r) > JL_ERR_LIMIT) ? -(r) : -JL_ERR_LIMIT]

#ifdef __cplusplus
}
#endif
#endif /* __JL_ERROR_H__ */
