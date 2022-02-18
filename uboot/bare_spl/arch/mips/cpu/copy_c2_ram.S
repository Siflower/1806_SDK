/*
 * copy_c2_ram.S
 *
 *  Copy code and data to ram then clear BSS
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

#include <regdef.h>
#include <boot.h>
#include "sf_mmap.h"

#define all_ones_s1				s1   /* at Will hold 0xffffffff to simplify bit insertion of 1's. */
#define data_a0					a0   /* a0 data to be moved */
#define source_addr_a1			a1   /* from address */
#define destination_addr_a2		a2   /* to address */
#define end_addr_a3				a3   /* ending address */


	.set	noreorder           # Don't allow the assembler to reorder instructions.
	.set	noat                # Don't allow the assembler to use r1(at) for synthetic instr.

/**************************************************************************************
**************************************************************************************/
LEAF(copy_c2_ram)


    li  all_ones_s1, 0xffffffff

    # Copy code and read-only/initialized data from FLASH to (uncached) RAM.
    la      source_addr_a1, _start_ram_text
    la      destination_addr_a2, _fdata_ram
    la      end_addr_a3, _edata_ram
    #ifndef EVA  // NOTE EVA mode assumed to be uncached
    	// Switch address to uncached (kseg1) so copy will go directly
    	// to memory
    	ins     destination_addr_a2, all_ones_s1, 29, 1
    	ins     end_addr_a3, all_ones_s1, 29, 1
    #endif
    beq     destination_addr_a2, end_addr_a3, zero_bss
    nop
next_ram_word:
    lw      data_a0, 0(source_addr_a1)
    sw      data_a0, 0(destination_addr_a2)
    addiu   destination_addr_a2, 4
    bne     end_addr_a3, destination_addr_a2, next_ram_word
    addiu   source_addr_a1, 4

zero_bss:
    la      destination_addr_a2, _fbss
    la      end_addr_a3, _end
    #ifndef EVA  // NOTE EVA mode assumed to be uncached
    	// Switch address to uncached (kseg1) so copy will go directly
    	// to memory
	    ins     destination_addr_a2, all_ones_s1, 29, 1
	    ins     end_addr_a3, all_ones_s1, 29, 1
    #endif
    beq     destination_addr_a2, end_addr_a3, copy_c2_ram_done
    nop
next_bss_word:
    sw      zero, 0(destination_addr_a2)
    addiu   destination_addr_a2, 4
    bne     destination_addr_a2, end_addr_a3, next_bss_word
    nop

copy_c2_ram_done:
    jalr	zero,      ra
    nop

.set at
END(copy_c2_ram)

