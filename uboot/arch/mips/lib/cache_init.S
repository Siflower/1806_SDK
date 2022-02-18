/*
 *  Cache-handling routined for MIPS CPUs
 *
 *  Copyright (c) 2003	Wolfgang Denk <wd@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm-offsets.h>
#include <config.h>
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>

#ifdef CONFIG_SFA18
#include <mach/sfax8.h>
#endif


#ifndef CONFIG_SYS_MIPS_CACHE_MODE
#define CONFIG_SYS_MIPS_CACHE_MODE CONF_CM_CACHABLE_NONCOHERENT
#endif

#define INDEX_BASE	CKSEG0

	.macro	f_fill64 dst, offset, val
	LONG_S	\val, (\offset +  0 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  1 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  2 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  3 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  4 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  5 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  6 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  7 * LONGSIZE)(\dst)
#if LONGSIZE == 4
	LONG_S	\val, (\offset +  8 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset +  9 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset + 10 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset + 11 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset + 12 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset + 13 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset + 14 * LONGSIZE)(\dst)
	LONG_S	\val, (\offset + 15 * LONGSIZE)(\dst)
#endif
	.endm

	.macro cache_loop	curr, end, line_sz, op
10:	cache		\op, 0(\curr)
	PTR_ADDU	\curr, \curr, \line_sz
	bne		\curr, \end, 10b
	.endm

	.macro	l1_info		sz, line_sz, off
	.set	push
	.set	noat

	mfc0	$1, CP0_CONFIG, 1

	/* detect line size */
	srl	\line_sz, $1, \off + MIPS_CONF1_DL_SHF - MIPS_CONF1_DA_SHF
	andi	\line_sz, \line_sz, (MIPS_CONF1_DL >> MIPS_CONF1_DL_SHF)
	move	\sz, zero
	beqz	\line_sz, 10f
	li	\sz, 2
	sllv	\line_sz, \sz, \line_sz

	/* detect associativity */
	srl	\sz, $1, \off + MIPS_CONF1_DA_SHF - MIPS_CONF1_DA_SHF
	andi	\sz, \sz, (MIPS_CONF1_DA >> MIPS_CONF1_DA_SHF)
	addiu	\sz, \sz, 1

	/* sz *= line_sz */
	mul	\sz, \sz, \line_sz

	/* detect log32(sets) */
	srl	$1, $1, \off + MIPS_CONF1_DS_SHF - MIPS_CONF1_DA_SHF
	andi	$1, $1, (MIPS_CONF1_DS >> MIPS_CONF1_DS_SHF)
	addiu	$1, $1, 1
	andi	$1, $1, 0x7

	/* sz <<= log32(sets) */
	sllv	\sz, \sz, $1

	/* sz *= 32 */
	li	$1, 32
	mul	\sz, \sz, $1
10:
	.set	pop
	.endm
/*
 * mips_cache_reset - low level initialisation of the primary caches
 *
 * This routine initialises the primary caches to ensure that they have good
 * parity.  It must be called by the ROM before any cached locations are used
 * to prevent the possibility of data with bad parity being written to memory.
 *
 * To initialise the instruction cache it is essential that a source of data
 * with good parity is available. This routine will initialise an area of
 * memory starting at location zero to be used as a source of parity.
 *
 * RETURNS: N/A
 *
 */
LEAF(mips_cache_reset)
#ifndef CONFIG_SYS_CACHE_SIZE_AUTO
	li	t2, CONFIG_SYS_ICACHE_SIZE
	li	t8, CONFIG_SYS_ICACHE_LINE_SIZE
#else
	l1_info	t2, t8, MIPS_CONF1_IA_SHF
#endif

#ifndef CONFIG_SYS_CACHE_SIZE_AUTO
	li	t3, CONFIG_SYS_DCACHE_SIZE
	li	t9, CONFIG_SYS_DCACHE_LINE_SIZE
#else
	l1_info	t3, t9, MIPS_CONF1_DA_SHF
#endif

#ifdef CONFIG_SYS_MIPS_CACHE_INIT_RAM_LOAD

	/* Determine the largest L1 cache size */
#ifndef CONFIG_SYS_CACHE_SIZE_AUTO
#if CONFIG_SYS_ICACHE_SIZE > CONFIG_SYS_DCACHE_SIZE
	li	v0, CONFIG_SYS_ICACHE_SIZE
#else
	li	v0, CONFIG_SYS_DCACHE_SIZE
#endif
#else
	move	v0, t2
	sltu	t1, t2, t3
	movn	v0, t3, t1
#endif
	/*
	 * Now clear that much memory starting from zero.
	 */
	PTR_LI		a0, CKSEG1
	PTR_ADDU	a1, a0, v0
2:	PTR_ADDIU	a0, 64
	f_fill64	a0, -64, zero
	bne		a0, a1, 2b

#endif /* CONFIG_SYS_MIPS_CACHE_INIT_RAM_LOAD */

	/*
	 * The TagLo registers used depend upon the CPU implementation, but the
	 * architecture requires that it is safe for software to write to both
	 * TagLo selects 0 & 2 covering supported cases.
	 */
	mtc0		zero, CP0_TAGLO
	mtc0		zero, CP0_TAGLO, 2

	/*
	 * The caches are probably in an indeterminate state, so we force good
	 * parity into them by doing an invalidate for each line. If
	 * CONFIG_SYS_MIPS_CACHE_INIT_RAM_LOAD is set then we'll proceed to
	 * perform a load/fill & a further invalidate for each line, assuming
	 * that the bottom of RAM (having just been cleared) will generate good
	 * parity for the cache.
	 */

	/*
	 * Initialize the I-cache first,
	 */
	blez		t2, 1f
	PTR_LI		t0, INDEX_BASE
	PTR_ADDU	t1, t0, t2
	/* clear tag to invalidate */
	cache_loop	t0, t1, t8, INDEX_STORE_TAG_I
#ifdef CONFIG_SYS_MIPS_CACHE_INIT_RAM_LOAD
	/* fill once, so data field parity is correct */
	PTR_LI		t0, INDEX_BASE
	cache_loop	t0, t1, t8, FILL
	/* invalidate again - prudent but not strictly neccessary */
	PTR_LI		t0, INDEX_BASE
	cache_loop	t0, t1, t8, INDEX_STORE_TAG_I
#endif

	/*
	 * then initialize D-cache.
	 */
1:	blez		t3, 3f
	PTR_LI		t0, INDEX_BASE
	PTR_ADDU	t1, t0, t3
	/* clear all tags */
	cache_loop	t0, t1, t9, INDEX_STORE_TAG_D
#ifdef CONFIG_SYS_MIPS_CACHE_INIT_RAM_LOAD
	/* load from each line (in cached space) */
	PTR_LI		t0, INDEX_BASE
2:	LONG_L		zero, 0(t0)
	PTR_ADDU	t0, t9
	bne		t0, t1, 2b
	/* clear all tags */
	PTR_LI		t0, INDEX_BASE
	cache_loop	t0, t1, t9, INDEX_STORE_TAG_D
#endif

3:	jr	ra
	END(mips_cache_reset)

/*
 * dcache_status - get cache status
 *
 * RETURNS: 0 - cache disabled; 1 - cache enabled
 *
 */
LEAF(dcache_status)
	mfc0	t0, CP0_CONFIG
	li	t1, CONF_CM_UNCACHED
	andi	t0, t0, CONF_CM_CMASK
	move	v0, zero
	beq	t0, t1, 2f
	li	v0, 1
2:	jr	ra
	END(dcache_status)

/*
 * dcache_disable - disable cache
 *
 * RETURNS: N/A
 *
 */
LEAF(dcache_disable)
	mfc0	t0, CP0_CONFIG
	li	t1, -8
	and	t0, t0, t1
	ori	t0, t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG
	jr	ra
	END(dcache_disable)

/*
 * dcache_enable - enable cache
 *
 * RETURNS: N/A
 *
 */
LEAF(dcache_enable)
	mfc0	t0, CP0_CONFIG
	ori	t0, CONF_CM_CMASK
	xori	t0, CONF_CM_CMASK
	ori	t0, CONFIG_SYS_MIPS_CACHE_MODE
	mtc0	t0, CP0_CONFIG
	jr	ra
	END(dcache_enable)

/*
 * init_L2 - init L2 cache
 *
 * RETURNS: N/A
 *
 */
LEAF(init_L2)
	mfc0	a2, CP0_CONFIG, 2		// Read C0_Config2
	ext	    v1, a2, 4, 4		// extract L2 line size

	beq	    v1, zero, done_L2_cach_init
	nop

	li	    s1, 2
	sllv	v1, s1, v1			// Now have true L2$ line size in bytes

	ext	    a0, a2, 8, 4		// extrace sets per way encoding
	li	    s1, 64
	sllv	a0, s1, a0			// L2$ Sets per way

	ext	    a1, a2, 0, 4		// extract ways encoding
	addiu	a1, a1, 1			// Decode L2 number of ways

	li	s1, 8

	mul	    a0, a0, a1				// Total number of sets
	mul		t4, a0, v1		// Total number of bytes
	mul		v0, v1, s1		// Total bytes per loop

	lui	    t5, 0x8000
	srl		s1, v0, 1
	addu	t5, s1, t5

	addu	a3, t5, t4		// make ending address
	subu	a3, a3, v0		// -1 bytes per loop interration

	mtc0	zero, CP0_TAGLO, 4
	mtc0	zero, CP0_TAGHI, 4	// Note: not all implementations will have a L23 tag Hi but writes will be ignored if not present

	subu	t5, t5, v0

next_L2_cache_tag:
	addu	    t5, v0			// Get next starting line address
    cache 0xB, (32*-2)(t5)
    cache 0xB, (32*-1)(t5)
    cache 0xB, (32*0)(t5)
    cache 0xB, (32*1)(t5)
    cache 0xB, (32*-4)(t5)
    cache 0xB, (32*-3)(t5)
    cache 0xB, (32*2)(t5)
    cache 0xB, (32*3)(t5)
	bne		t5, a3, next_L2_cache_tag	// Done yet?
    nop

done_L2_cach_init:
    jr      ra
    nop

END(init_L2)

/*
 * enable_L2 - enable L2 cache
 *
 * RETURNS: N/A
 *
 */
LEAF(enable_L2)
	li	t0, GCR_CONFIG_ADDR
	lw	t1, 8(t0)	// Offset 0x8: GCR Base Register
	li	t2, 0x0	// Disable L2 CCA OVERRIDE
	ins	t1, t2, 0, 8
	sw	t1, 8(t0)
	jr	ra
	nop
END(enable_L2)
