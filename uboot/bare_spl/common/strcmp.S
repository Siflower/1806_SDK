/*
 * Unpublished work (c) MIPS Technologies, Inc.  All rights reserved.
 * Unpublished rights reserved under the copyright laws of the United
 * States of America and other countries.
 *
 * This code is confidential and proprietary to MIPS Technologies,
 * Inc. ("MIPS Technologies") and may be disclosed only as permitted in
 * writing by MIPS Technologies. Any copying, reproducing, modifying,
 * use or disclosure of this code (in whole or in part) that is not
 * expressly permitted in writing by MIPS Technologies is strictly
 * prohibited. At a minimum, this code is protected under trade secret,
 * unfair competition, and copyright laws. Violations thereof may result
 * in criminal penalties and fines.
 *
 * MIPS Technologies reserves the right to change this code to improve
 * function, design or otherwise. MIPS Technologies does not assume any
 * liability arising out of the application or use of this code, or of
 * any error or omission in such code.  Any warranties, whether express,
 * statutory, implied or otherwise, including but not limited to the
 * implied warranties of merchantability or fitness for a particular
 * purpose, are excluded.  Except as expressly provided in any written
 * license agreement from MIPS Technologies, the furnishing of this
 * code does not give recipient any license to any intellectual property
 * rights, including any patent rights, that cover this code.
 *
 * This code shall not be exported, reexported, transferred, or released,
 * directly or indirectly, in violation of the law of any country or
 * international law, regulation, treaty, Executive Order, statute,
 * amendments or supplements thereto.  Should a conflict arise regarding
 * the export, reexport, transfer, or release of this code, the laws of
 * the United States of America shall be the governing law.
 *
 * This code may only be disclosed to the United States government
 * ("Government"), or to Government users, with prior written consent
 * from MIPS Technologies.  This code constitutes one or more of the
 * following: commercial computer software, commercial computer software
 * documentation or other commercial items.  If the user of this code,
 * or any related documentation of any kind, including related technical
 * data or manuals, is an agency, department, or other entity of the
 * Government, the use, duplication, reproduction, release, modification,
 * disclosure, or transfer of this code, or any related documentation
 * of any kind, is restricted in accordance with Federal Acquisition
 * Regulation 12.212 for civilian agencies and Defense Federal Acquisition
 * Regulation Supplement 227.7202 for military agencies.  The use of this
 * code by the Government is further restricted in accordance with the
 * terms of the license agreement(s) and/or applicable contract terms
 * and conditions covering this code from MIPS Technologies.
 *
 *
 */

/*
 * Originally written by Kjeld Borch Egevang <kjelde@mips.com>, 2002.
 */

#include <regdef.h>
#include <m32c0.h>
#include <boot.h>

	.set	nomips16

/* int strcmp(const char *s1, const char *s2); */

#if __mips >= 3 && __mips != 32
#define MIPS64 1
#undef MIPS32
#else
#undef MIPS64
#define MIPS32 1
#endif

#if __mips > 1 && __mips != 32 && __mips != 64
#define BRANCHLIKELY 1
#endif
#if __mips == 1
#define LOADDELAY 1
#endif


#ifdef MIPS64
.set gp64
#define	L	ld
#define	LL	ldl
#define	LR	ldr
#define word_bits 0x3
#define word_mask 0x7
#define word_size 0x8
#define	SUBU	dsubu
#define	ADDIU	daddiu
#define	SLL	dsll
#endif


#ifdef MIPS32
#define	L	lw
#define	LL	lwl
#define	LR	lwr
#define word_bits 0x2
#define word_mask 0x3
#define word_size 0x4
#define	SUBU	subu
#define	ADDIU	addiu
#define	SLL	sll
#endif

#if __STDC__
#define CONCAT(a,b)	a##b
#else
#define CONCAT(a,b)	a/**/b
#endif

#ifndef BRANCHLIKELY

#define	STRCMP_WORD(OFS)			\
CONCAT(..strcmp_word,OFS):			;\
3:	SUBU	t0, v0, t1			;\
	beq	v0, v1, 4f			;\
	and	t0, t7				;\
	ADDIU	a0, OFS*word_size		;\
	b	..strcmp_byte			;\
	ADDIU	a1, OFS*word_size		;\
4:	beq	t0, zero, 5f			;\
	and	t4, t0, v1			;\
	beq	t4, t0, 5f			;\
	nop					;\
	jr	ra				;\
	move	v0, zero			;\
5:	L	v0, (OFS+1)*word_size(a0)	;\
	L	v1, (OFS+1)*word_size(a1)	;\

#else /* BRANCHDELAY */

#define	STRCMP_WORD(OFS)			\
CONCAT(..strcmp_word,OFS):			;\
3:	SUBU	t0, v0, t1			;\
	beq	v0, v1, 4f			;\
	and	t0, t7				;\
	ADDIU	a0, OFS*word_size		;\
	b	..strcmp_byte			;\
	ADDIU	a1, OFS*word_size		;\
4:	beql	t0, zero, 5f			;\
	L	v0, (OFS+1)*word_size(a0)	;\
	and	t4, t0, v1			;\
	beql	t4, t0, 3f			;\
	L	v1, (OFS+1)*word_size(a1)	;\
	jr	ra				;\
	move	v0, zero			;\
5:	L	v1, (OFS+1)*word_size(a1)	;\

#endif


LEAF(strcmp)
	.set	noreorder

	or	t0, a0, a1		/* Check for unaligned strings */
	andi	t0, word_mask

	bne	t0, zero, ..strcmp_byte
	lui	t1, 0x0101
	ori	t1, 0x0101
#ifdef	MIPS64
	dsll	t7, t1, 32
	or	t1, t7
#endif
	SLL	t7, t1, 7

..strcmp_loop:
	L	v0, 0x00(a0)
2:	L	v1, 0x00(a1)
	STRCMP_WORD(0)
	STRCMP_WORD(1)
	STRCMP_WORD(2)
3:	SUBU	t0, v0, t1
	beq	v0, v1, 4f
	and	t0, t7
	ADDIU	a0, 3*word_size
	b	..strcmp_byte
	ADDIU	a1, 3*word_size
4:	ADDIU	a0, 4*word_size
	beq	t0, zero, ..strcmp_loop
	ADDIU	a1, 4*word_size
	and	t4, t0, v1
	beq	t4, t0, 2b
	L	v0, 0x00(a0)
	jr	ra
	move	v0, zero

#ifdef BRANCHLIKELY
#define	STRCMP_T0T1(OFS)	\
	bnel	t0, zero, 8f	;\
	lbu	t2, OFS(a0)	;\
	jr	ra		;\
	SUBU	v0, t0, t1	;\
8:	beql	t0, t1, 8f	;\
	lbu	t3, OFS(a1)	;\
	jr	ra		;\
	SUBU	v0, t0, t1	;\
8:

#define	STRCMP_T2T3(OFS)	\
	bnel	t2, zero, 8f	;\
	lbu	t0, OFS(a0)	;\
	jr	ra		;\
	SUBU	v0, t2, t3	;\
8:	beql	t2, t3, 8f	;\
	lbu	t1, OFS(a1)	;\
	jr	ra		;\
	SUBU	v0, t2, t3	;\
8:
#endif /* BRANCHLIKELY */

..strcmp_byte:
#ifndef BRANCHLIKELY
1:	lbu	t0, 0(a0)
	lbu	t1, 0(a1)
	beq	t0, zero, 2f
    	ADDIU	a0, 1
	beq	t0, t1, 1b
    	ADDIU	a1, 1
2:	jr	ra
	SUBU	v0, t0, t1
#else /* BRANCHLIKELY */
	lbu	t0, 0(a0)
	lbu	t1, 0(a1)
1:	STRCMP_T0T1(1)
	STRCMP_T2T3(2)
	STRCMP_T0T1(3)
	STRCMP_T2T3(4)
	STRCMP_T0T1(5)
	STRCMP_T2T3(6)
	STRCMP_T0T1(7)
	bnel	t2, zero, 2f
	lbu	t0, 8(a0)
	jr	ra
	SUBU	v0, t2, t3
2:
	ADDIU	a0, 0x08
	ADDIU	a1, 0x08
	beql	t2, t3, 1b
	lbu	t1, 0(a1)
	jr	ra
	SUBU	v0, t2, t3
#endif /* BRANCHLIKELY */
END(strcmp)
