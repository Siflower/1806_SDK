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

#ifndef __JL_CONFIG_H__
#define __JL_CONFIG_H__

/* #define	CONFIG_JLFE_API_MUTEX */
/* #define	CONFIG_JLFE_PHY_MUTEX */
/* #define	CONFIG_JLFE_APB_MUTEX */
/* XXX
 * Pthread mutex is used in jl_portable.c sample code.
 * But, MinGW not support pthread mutex yet, please comment all mutex options here.
 * If you are using other available mutex APIs in jl_portable.c just uncomment the  mutex as your need.
 */
#if (defined WIN32 && (defined CONFIG_JLFE_API_MUTEX || defined CONFIG_JLFE_PHY_MUTEX || defined CONFIG_JLFE_APB_MUTEX))
//#error "MinGW not support pthread yet"
#endif

/* uncomment this to printf log info */
#ifdef	CONFIG_JLFE_DEBUG
#define JL_BIT(n)				((0x1U)<<n)
#define JL_FLAG_SYS				JL_BIT(1)
#define JL_FLAG_PHY				JL_BIT(2)
#define JL_FLAG_SMI				JL_BIT(3)
#define JL_FLAG_MAC_UCTRL			JL_BIT(4)
#define JL_FLAG_EMAC				JL_BIT(5)
#define JL_FLAG_SWCORE				JL_BIT(6)
#define JL_FLAG_LED				JL_BIT(7)
#define JL_FLAG_ALL				(0xFFFFFFFFU)
enum {
	_DBG_OFF = 0,
	_DBG_ERROR,
	_DBG_WARNING,
	_DBG_INFO,
	_DBG_ON,
};
#define	JL_DBG_FLAG_MASK		JL_FLAG_ALL
#endif
#endif /* __JL_CONFIG_H__ */
