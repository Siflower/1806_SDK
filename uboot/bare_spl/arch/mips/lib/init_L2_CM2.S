/*
 * init_L2_CM2.S
 *
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


#include <boot.h>
#include <regdef.h>
#include <m32c0.h>

#define LINE_SIZE_v1		v1
#define BYTES_PER_LOOP_v0 	v0
#define SET_SIZE_a0			a0
#define ASSOC_a1			a1
#define CONFIG_L2_a2 		a2
#define END_ADDR_a3			a3
#define TOTAL_BYTES			$12	// t4 o32 a4 n32/64
#define	CURRENT_ADDR		$13	// t5 o32 a5 n32/64
#define GCR_BASE			s0
#define TEMP_s1				s1
#define L2LINE_SIZE 32 // L2 line size could be 32 or 64

#define LINES_PER_ITER 8 // number of cach instructions per loop

	.set	noreorder           // Don't allow the assembler to reorder instructions.
	.set	noat                // Don't allow the assembler to use r1(at) for synthetic instr.

/**************************************************************************************
**************************************************************************************/
LEAF(disable_L2)

	bnez    r8_core_num, done_disable_L2	# Only done from core 0.

    // Use CCA Override disable the L2 cache
    // NOTE: If you have a L3 cache you must add code here
    // to disable it or initialize it if it can't be disabled.
    // Disable the L2 cache using CCA override by writing a 0x50 to
    // the GCR Base register. 0x50 enables the CCA override bit and sets
    // the CCA to uncached.

	lw	    GCR_BASE, 0x0008(r22_gcr_addr)  // Read GCR_BASE
    li      TEMP_s1, 0x50                  	// Enable CCA and set to uncached
    ins     GCR_BASE, TEMP_s1, 0, 8    		// Insert bits
    sw      GCR_BASE, 0x0008(r22_gcr_addr)  // Write GCR_BASE

done_disable_L2:
    jr      ra
    nop
END(disable_L2)
/**************************************************************************************
**************************************************************************************/
LEAF(enable_L2)

	bnez    r8_core_num, done_enable_L2	# Only done from core 0.

    // Use CCA Override disable the L2 cache
    // NOTE: If you have a L3 cache you must add code here
    // to enable it or initialize it if it can't be enabled.


	lw	    GCR_BASE, 0x0008(r22_gcr_addr)  // Read GCR_BASE
 	ins     GCR_BASE, zero, 0, 8    		  // CCA Override disabled
    sw      GCR_BASE, 0x0008(r22_gcr_addr)  // Write GCR_BASE

done_enable_L2:
    jr      ra
    nop
END(enable_L2)
/**************************************************************************************
* Initialize the L2
**************************************************************************************/
LEAF(init_L2)
#ifndef SIMULATION
	bnez    r8_core_num, done_L2_cach_init			# Only done from core 0.

	// L2 Cache initialization routine
	// Check L2 cache size
	mfc0	CONFIG_L2_a2, C0_CONFIG2		// Read C0_Config2
	// Isolate L2$ Line Size
	ext	    LINE_SIZE_v1, CONFIG_L2_a2, 4, 4		// extract L2 line size

	// Skip ahead if No L2$
	beq	    LINE_SIZE_v1, zero, done_L2_cach_init
	nop

	li	    TEMP_s1, 2
	sllv	LINE_SIZE_v1, TEMP_s1, LINE_SIZE_v1			// Now have true L2$ line size in bytes

	// Isolate L2 Sets per Way (cache lines per way)
	ext	    SET_SIZE_a0, CONFIG_L2_a2, 8, 4		// extrace sets per way encoding
	li	    TEMP_s1, 64
	sllv	SET_SIZE_a0, TEMP_s1, SET_SIZE_a0			// L2$ Sets per way

	// Isolate L2 Associativity (number of ways)
	// L2$ Assoc (-1)
	ext	    ASSOC_a1, CONFIG_L2_a2, 0, 4		// extract ways encoding
	addiu	ASSOC_a1, ASSOC_a1, 1			// Decode L2 number of ways

	li 	TEMP_s1, (LINES_PER_ITER)

	mul	    SET_SIZE_a0, SET_SIZE_a0, ASSOC_a1				// Total number of sets
	mul		TOTAL_BYTES, SET_SIZE_a0, LINE_SIZE_v1		// Total number of bytes
	mul		BYTES_PER_LOOP_v0, LINE_SIZE_v1, TEMP_s1		// Total bytes per loop

	// Set the starting address at the beginning of kgeg0 (0x80000000) which will corresponds to
	// way 0 index 0 of the cache and position so starting address is in the
	// middle of the first bytes per loop because the code will use + and - offsets
	lui	    CURRENT_ADDR, 0x8000
	srl		TEMP_s1, BYTES_PER_LOOP_v0, 1
	addu	CURRENT_ADDR, TEMP_s1, CURRENT_ADDR

	addu	END_ADDR_a3, CURRENT_ADDR, TOTAL_BYTES		// make ending address
	subu	END_ADDR_a3, END_ADDR_a3, BYTES_PER_LOOP_v0		// -1 bytes per loop interration

	// Clear L23TagLo/L23TagHi registers these are used to set the cache tag
	mtc0	zero, C0_TAGLO, 4
	mtc0	zero, C0_TAGHI, 4	// Note: not all implementations will have a L23 tag Hi but writes will be ignored if not present

// due to offset field restrictions code assumes line size will not be more that 128 bytes for a r5 or less core
// or not more than 64 bytes for a r6 and greater core

next_L2_cache_tag:
	// Index Store Tag Cache Op
	// Will invalidate the tag entry, clear the lock bit, and clear the LRF bit
    cache 0xB, (L2LINE_SIZE*-2)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*-1)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*0)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*1)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*-4)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*-3)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*2)(CURRENT_ADDR)
    cache 0xB, (L2LINE_SIZE*3)(CURRENT_ADDR)
	bne	    	CURRENT_ADDR, END_ADDR_a3, next_L2_cache_tag 	// Done yet?
	addu	    CURRENT_ADDR, BYTES_PER_LOOP_v0			// Get next starting line address

done_L2_cach_init:
#endif /* SIMULATION */
    jr      ra
    nop
END(init_L2)

#undef LINE_SIZE_v1
#undef BYTES_PER_LOOP_v0
#undef SET_SIZE_a0
#undef ASSOC_a1
#undef CONFIG_L2_a2
#undef END_ADDR_a3
#undef TOTAL_BYTES
#undef	CURRENT_ADDR
#undef TEMP_s1
#undef GCR_BASE
#undef LINES_PER_ITER
