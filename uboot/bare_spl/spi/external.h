/**
* external.h
*
* Copyright (c) 2009-2015 Micron Technology, Inc.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#include <io.h>
#include <types.h>
#include "types.h"

#define le16_to_cpu(x) x
#define le32_to_cpu(x) x

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

/* Some compiler specific definitions are overwritten here
 * for Clang compiler
 */
#define uninitialized_var(x) x = *(&(x))

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#endif

#ifndef EUCLEAN
#define EUCLEAN 	117
#endif
//#define	EBADMSG		84	/* Not a data message */

extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s,int ch,size_t n);
extern void * malloc( size_t xWantedSize );
extern void free( void *pv );

struct spi_slave {
	u8 op_mode_rx;
	u8 op_mode_tx;
	unsigned int max_speed_hz;
	u8 option;
};
