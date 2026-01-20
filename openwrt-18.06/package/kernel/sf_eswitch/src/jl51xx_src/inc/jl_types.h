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

#ifndef __JL_TYPES_H__
#define __JL_TYPES_H__

/* Differentiate between built-in compile and out-of-tree compile for
 * Linux kernel, for normal build, always include jl autoconf */
#ifdef __KERNEL__
#ifdef MODULE
#include <linux/kconfig.h>
#else
#include "jl_autoconf.h"
#endif /* MODULE */
#else /* KERNEL */
#include "jl_autoconf.h"
#endif

typedef unsigned long long		jl_uint64;
typedef long long				jl_int64;
typedef unsigned int			jl_uint32;
typedef int						jl_int32;
typedef unsigned short			jl_uint16;
typedef short					jl_int16;
typedef unsigned char			jl_uint8;
typedef signed char				jl_int8;
typedef unsigned int			jl_bool;

typedef unsigned int			jl_uintptr;

#define CONST_T     const

#if defined(WIN32)
#if defined(CONFIG_JL_DLL_EXPORT_SYMBOL)
#define JL_API          __declspec(dllexport)
#elif defined(CONFIG_JL_STATIC_LINK)
#define JL_API
#else
#define JL_API          __declspec(dllimport)
#endif
#else/* WIN32*/
#define JL_API
#endif


#ifndef __KERNEL__
#define attribute_deprecated	__attribute__((deprecated))
#define EXPORT_SYMBOL(x)
#else
/* avoid error in kernel build */
#define attribute_deprecated
#endif

typedef jl_uint32 jl_portmask_t;

typedef enum jl_enable_e {
	DISABLED = 0,
	ENABLED,
	JL_ENABLE_END
} jl_enable_t;

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN      (6) /**< ETHER_ADDR_LEN */
#endif

/**
 * @struct jl_mac_s
 * @brief mac address in network byte order
 */
struct jl_mac_s {
	jl_uint8 octet[ETHER_ADDR_LEN]; /**< MAC data */
};

/**
 * @public typedef jl_mac_t
 */
typedef struct jl_mac_s jl_mac_t;


typedef jl_uint32				ipaddr_t;

typedef	jl_uint32				jl_pri_t;		/* priority vlaue */
typedef jl_uint32				jl_qid_t;		/* queue id type */
typedef jl_uint32				jl_vlan_t;		/* vlan id type */

typedef enum jl_port_e {
	UTP_PORT0 = 0,
	UTP_PORT1,
	UTP_PORT2,
	UTP_PORT3,
	UTP_PORT4,
	UTP_PORT5,
	UTP_PORT6,
	UTP_PORT7,

	EXT_PORT0 = 8,
	EXT_PORT1,

	JL_PORT_MAX = 10,
	UNDEF_PORT = 0xff
} jl_port_t;

/**
 * @enum jl_port_dir_e
 * @brief port direction
 */
enum jl_port_dir_e {
	PORT_DIR_INGRESS = 0,/**< ingress direction */
	PORT_DIR_EGRESS = 1/**< egress direction */
};

/**
 * @public typedef jl_port_dir_t
 */
typedef enum jl_port_dir_e jl_port_dir_t;

/* type abstraction */
typedef jl_int32				jl_api_ret_t;
typedef jl_int32				jl_ret_t;
typedef jl_uint64				jl_u_long_t;

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif /* __JL_TYPES_H__ */
