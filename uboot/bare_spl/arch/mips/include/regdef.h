/*
 * Copyright (c) 2014, Imagination Technologies LLC and Imagination
 * Technologies Limited.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted under the terms of the MIPS Free To Use 1.0
 * license that you will have received with this package. If you haven't
 * received this file, please contact Imagination Technologies or see the
 * following URL for details.
 * http://codescape-mips-sdk.imgtec.com/license/IMG-free-to-use-on-MIPS-license
 *
 */

#ifndef _MIPS_REGDEF_H_
#define _MIPS_REGDEF_H_

/*
 * regdef.h : MIPS Rx000 symbolic register names for assembler
 */


#define zero	$0

#define AT	$1

#define v0 	$2
#define v1	$3

#define a0	$4
#define a1	$5
#define a2	$6
#define	a3	$7

#if _MIPS_SIM==_ABIN32 || _MIPS_SIM==_ABI64 || _MIPS_SIM==_ABIEABI
#define a4	$8
#define a5	$9
#define a6	$10
#define	a7	$11
#define t0	$12
#define t1	$13
#define t2	$14
#define t3	$15
#define ta0	$8	/* alias for $a4 */
#define ta1	$9	/* alias for $a5 */
#define ta2	$10	/* alias for $a6 */
#define ta3	$11	/* alias for $a7 */
#else
#define t0	$8
#define t1	$9
#define t2	$10
#define t3	$11
#define t4	$12
#define t5	$13
#define t6	$14
#define t7	$15
#define ta0	$12	/* alias for $t4 */
#define ta1	$13	/* alias for $t5 */
#define ta2	$14	/* alias for $t6 */
#define ta3	$15	/* alias for $t7 */
#endif

#define s0	$16
#define s1	$17
#define s2	$18
#define s3	$19
#define s4	$20
#define s5	$21
#define s6	$22
#define s7	$23
#define s8	$30		/* == fp */

#define t8	$24
#define t9	$25
#define k0	$26
#define k1	$27

#define gp	$28

#define sp	$29
#define fp	$30
#define ra	$31

#endif /*_MIPS_REGDEF_H_*/
