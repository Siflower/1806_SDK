/*
 * init_caches.S
 *
  *  Common Cache initialization
*/
/*
Copyright (c) 2015, Imagination Technologies Limited and/or its affiliated group companies

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and/or other materials
provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be
used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <regdef.h>	// #defines for GPRs
#include <m32c0.h>		// #defines for CP0 registers
#include <boot.h>

#define LINE_SIZE_v1		v1
#define BYTES_PER_LOOP_v0	v0
#define SET_SIZE_a0			a0
#define ASSOC_a1			a1
#define CONFIG1_a2			a2
#define END_ADDR_a3			a3
#define TOTAL_BYTES			$12	// t4 o32 a4 n32/64
#define	CURRENT_ADDR		$13	// t5 o32 a5 n32/64
#define TEMP1				$14 // t6 o32 a6 n32/64
#define TEMP2				$15 // t7 o32 a7 n32/64

#define LINES_PER_ITER 8 // number of cach instructions per loop
#define ILINE_SIZE 32 // L1 instruction cache line size
#define DLINE_SIZE 32 // L1 data cache line size
#define HCI 18		  // Hardware Cache Initialization bit

	.set	noreorder           // Don't allow the assembler to reorder instructions.
	.set	noat                // Don't allow the assembler to use r1(at) for synthetic instr.

/**************************************************************************************
**************************************************************************************/

LEAF(init_icache)
#ifndef SIMULATION
	// Can be skipped if Config7[HCI] set
	mfc0    TEMP1, C0_CONFIG, 7						// Read CP0 Config7
	ext		TEMP1, TEMP1, HCI, 1					// extract HCI
	bne		TEMP1, zero, done_icache
	nop

	// Determine how big the I$ is
	mfc0	CONFIG1_a2, C0_CONFIG1						// read C0_Config1

	// Isolate I$ Line Size
	ext	    LINE_SIZE_v1, CONFIG1_a2, CFG1_ILSHIFT, 3		// extract IL

	// Skip ahead if No I$
	beq	    LINE_SIZE_v1, zero, done_icache
	nop

	li	    TEMP1, 2
	sllv    LINE_SIZE_v1, TEMP1, LINE_SIZE_v1				// Now have true I$ line size in bytes

	ext	    SET_SIZE_a0, CONFIG1_a2, CFG1_ISSHIFT, 3		// extract IS
	li	    TEMP1, 64
	sllv    SET_SIZE_a0, TEMP1, SET_SIZE_a0					// I$ Sets per way

	// Config1IA == I$ Assoc - 1
	ext	    ASSOC_a1, CONFIG1_a2, CFG1_IASHIFT, 3			// extract IA
	addiu	ASSOC_a1, ASSOC_a1, 1

	li 	TEMP1, (LINES_PER_ITER)

	mul	    SET_SIZE_a0, SET_SIZE_a0, ASSOC_a1				// Total number of sets
	mul		TOTAL_BYTES, SET_SIZE_a0, LINE_SIZE_v1		// Total number of bytes
	mul		BYTES_PER_LOOP_v0, LINE_SIZE_v1, TEMP1		// Total bytes per loop

	// Set the starting address at the beginning of kgeg0 (0x80000000) which will corresponds to
	// way 0 index 0 of the cache and position so starting address is in the
	// middle of the first bytes per loop because the code will use + and - offsets
	li	    CURRENT_ADDR, 0x0000000080000000
	srl	TEMP1, BYTES_PER_LOOP_v0, 1
	addu	CURRENT_ADDR, TEMP1, CURRENT_ADDR
	addu	END_ADDR_a3, CURRENT_ADDR, TOTAL_BYTES		// make ending address
	subu	END_ADDR_a3, END_ADDR_a3, BYTES_PER_LOOP_v0	// -1

	// Clear TagLo/TagHi registers
	mtc0    zero, C0_TAGLO		// write C0_ITagLo
	mtc0    zero, C0_TAGHI		// Note: not all implementations will have a tag Hi but writes will be ignored if not present

// due to offset field restrictions code assumes line size will not be more that 128 bytes for a r5 or less core
// or not more than 64 bytes for a r6 and greater core

next_icache_tag:
	// Index Store Tag Cache Op
	// Will invalidate the tag entry, clear the lock bit, and clear the LRF bit
    cache 0x8, (ILINE_SIZE*-2)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*-1)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*0)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*1)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*-4)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*-3)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*2)(CURRENT_ADDR)
    cache 0x8, (ILINE_SIZE*3)(CURRENT_ADDR)
	bne	    	CURRENT_ADDR, END_ADDR_a3, next_icache_tag 	// Done yet?
	addu	    CURRENT_ADDR, BYTES_PER_LOOP_v0				// Get next starting line address

done_icache:
#endif /* SIMULATION */
    jalr	zero,      ra
    nop
END(init_icache)

/**************************************************************************************
* init_dcache invalidates all data cache entries
**************************************************************************************/

LEAF(init_dcache)
#ifndef SIMULATION
	// Can be skipped if Config7[HCI] set
	mfc0    TEMP1, C0_CONFIG, 7						// Read CP0 Config7
	ext		TEMP1, TEMP1, HCI, 1					// extract HCI
	bne		TEMP1, zero, done_dcache
	nop

	mfc0	CONFIG1_a2, C0_CONFIG1					// read C0_Config1

	// Isolate D$ Line Size
	ext	    LINE_SIZE_v1, CONFIG1_a2, CFG1_DLSHIFT, 3	// extract DL

	// Skip ahead if No D$
	beq	    LINE_SIZE_v1, zero, done_dcache
	nop

	li	    TEMP1, 2
	sllv	LINE_SIZE_v1, TEMP1, LINE_SIZE_v1			// Now have true D$ line size in bytes

	ext	    SET_SIZE_a0, CONFIG1_a2, CFG1_DSSHIFT, 3	// extract DS
	li	    TEMP1, 64
	sllv	SET_SIZE_a0, TEMP1, SET_SIZE_a0				// D$ Sets per way

	// Config1DA == D$ Assoc - 1
	ext	    ASSOC_a1, CONFIG1_a2, CFG1_DASHIFT, 3		// extract DA
	addiu	ASSOC_a1, 1

	li 	TEMP1, (LINES_PER_ITER)

	mul	    SET_SIZE_a0, SET_SIZE_a0, ASSOC_a1			// Total number of sets
	mul		TOTAL_BYTES, SET_SIZE_a0, LINE_SIZE_v1		// Total number of bytes
	mul		BYTES_PER_LOOP_v0, LINE_SIZE_v1, TEMP1		// Total bytes per loop

	// Set the starting address at the beginning of kgeg0 (0x80000000) which will corresponds to
	// way 0 index 0 of the cache and position so starting address is in the
	// middle of the first bytes per loop because the code will use + and - offsets
	lui	    CURRENT_ADDR, 0x8000
	srl		TEMP1, BYTES_PER_LOOP_v0, 1
	addu	CURRENT_ADDR, TEMP1, CURRENT_ADDR

	addu	END_ADDR_a3, CURRENT_ADDR, TOTAL_BYTES		// make ending address
	subu	END_ADDR_a3, END_ADDR_a3, BYTES_PER_LOOP_v0	// -1

	// Clear TagLo/TagHi registers
	mtc0	zero, C0_TAGLO, 2	// write C0_DTagLo
	mtc0	zero, C0_TAGHI, 2	// Note: not all implementations will have a tag Hi but writes will be ignored if not present

// due to offset field restrictions code assumes line size will not be more that 128 bytes for a r5 or less core
// or not more than 64 bytes for a r6 and greater core

next_dcache_tag:
	// Index Store Tag Cache Op
	// Will invalidate the tag entry, clear the lock bit, and clear the LRF bit
    cache 0x9, (DLINE_SIZE*-2)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*-1)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*0)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*1)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*-4)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*-3)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*2)(CURRENT_ADDR)
    cache 0x9, (DLINE_SIZE*3)(CURRENT_ADDR)
	bne	    	CURRENT_ADDR, END_ADDR_a3, next_dcache_tag 	// Done yet?
	addu	    CURRENT_ADDR, BYTES_PER_LOOP_v0			// Get next starting line address

done_dcache:

#endif /* SIMULATION */
    jalr	zero,      ra
    nop
END(init_dcache)

LEAF(change_k0_cca)
	// NOTE! This code must be executed in KSEG1 (not KSGE0 uncached)
    // Set CCA for kseg0 to cacheable
	mfc0	TEMP1, C0_CONFIG	// read C0_Config
	beqz    r11_is_cps, set_kseg0_cca
	li	    TEMP2, 3			// CCA for all others
	li	    TEMP2, 5			// CCA for coherent cores (fall through)

set_kseg0_cca:
    ins	    TEMP1, TEMP2, 0, 3	// instert K0
	mtc0	TEMP1, C0_CONFIG	// write C0_Config
	jalr.hb   zero, ra
    nop

END(change_k0_cca)


LEAF(flush_dcache)

	mfc0	CONFIG1_a2, C0_CONFIG1						// read C0_Config1

	// Isolate D$ Line Size
	ext	    LINE_SIZE_v1, CONFIG1_a2, CFG1_DLSHIFT, 3	// extract DL

	// Skip ahead if No D$
	beq	    LINE_SIZE_v1, zero, done_flush_dcache
	nop

	li	    TEMP1, 2
	sllv	LINE_SIZE_v1, TEMP1, LINE_SIZE_v1			// Now have true D$ line size in bytes

	ext	    SET_SIZE_a0, CONFIG1_a2, CFG1_DSSHIFT, 3	// extract DS
	li	    TEMP1, 64
	sllv	SET_SIZE_a0, TEMP1, SET_SIZE_a0				// D$ Sets per way

	// Config1DA == D$ Assoc - 1
	ext	    ASSOC_a1, CONFIG1_a2, CFG1_DASHIFT, 3		// extract DA
	addiu	ASSOC_a1, 1

	li 	TEMP1, (LINES_PER_ITER)

	mul	    SET_SIZE_a0, SET_SIZE_a0, ASSOC_a1			// Total number of sets
	mul		TOTAL_BYTES, SET_SIZE_a0, LINE_SIZE_v1		// Total number of bytes
	mul		BYTES_PER_LOOP_v0, LINE_SIZE_v1, TEMP1		// Total bytes per loop

	lui	    CURRENT_ADDR, 0x8000		    			// Get a KSeg0 address for cacheops
	srl		TEMP1, BYTES_PER_LOOP_v0, 1
	addu	CURRENT_ADDR, TEMP1, CURRENT_ADDR

	addu	END_ADDR_a3, CURRENT_ADDR, TOTAL_BYTES		// make ending address
	subu	END_ADDR_a3, END_ADDR_a3, BYTES_PER_LOOP_v0	// -1

	// Clear TagLo/TagHi registers
	mtc0	zero, C0_TAGLO, 2	// write C0_DTagLo
	mtc0	zero, C0_TAGHI, 2	// Note: not all implementations will have a tag Hi but writes will be ignored if not present

// due to offset field restrictions code assumes line size will not be more that 128 bytes for a r5 or less core
// or not more than 64 bytes for a r6 and greater core

fnext_dcache_tag:
	// Index writeback invalidate Cache Op
	// Will invalidate the tag entry, clear the lock bit, and clear the LRF bit
    cache 0x1, (DLINE_SIZE*-2)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*-1)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*0)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*1)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*-4)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*-3)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*2)(CURRENT_ADDR)
    cache 0x1, (DLINE_SIZE*3)(CURRENT_ADDR)
	bne	  CURRENT_ADDR, END_ADDR_a3, fnext_dcache_tag 	// Done yet?
	addu  CURRENT_ADDR, BYTES_PER_LOOP_v0				// Get next starting line address

done_flush_dcache:
	sync
    jalr	zero,      ra
    nop
END(flush_dcache)

#undef LINE_SIZE_v1
#undef BYTES_PER_LOOP_v0
#undef SET_SIZE_a0
#undef ASSOC_a1
#undef CONFIG1_a2
#undef END_ADDR_a3
#undef TOTAL_BYTES
#undef CURRENT_ADDR
#undef TEMP1
#undef TEMP2
