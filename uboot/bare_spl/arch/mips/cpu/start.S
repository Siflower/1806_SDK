#include <boot.h>
#include <m32c0.h>
#include <regdef.h>
#include <cps.h>
#include <sf_mmap.h>

#define PAD_BASE            (SIFLOWER_SYSCTL_BASE+0x30000)
#define PAD_FUCN_SEL        (PAD_BASE+0xFF60)
#define PAD_MODE_SEL        (PAD_BASE+0xFC00)
#define PAD_IO_GPIOEN		(PAD_BASE + 0xFEE0)

	.set	noreorder           // Don't allow the assembler to reorder instructions.
	.set	noat                // Don't allow the assembler to use r1(at) for synthetic instr.
/**************************************************************************************
    R E S E T   E X C E P T I O N   H A N D L E R
**************************************************************************************/

LEAF(__reset_vector)
	la	a2, reset
	jr	a2
	mtc0	zero, C0_COUNT		// Clear cp0 Count (Used to measure boot time.)
END(__reset_vector)

reset:
	//disable cache
	mfc0	a2, C0_CONFIG
	li	a0, 2
	ins	a2, a0, 0, 3
	mtc0	a2, C0_CONFIG

	mfc0    a0, C0_EBASE			// Read CP0 EBase
	ext	r23_cpu_num, a0, 0, 4			// Extract CPUNum
	mfc0    a0, C0_TCBIND					// Read CP0 TCBind
	ext	r9_vpe_num, a0, 0, 4				// Extract CurVPE
	li      r22_gcr_addr, GCR_CONFIG_ADDR
	lw      r8_core_num, (CORE_LOCAL_CONTROL_BLOCK + GCR_CL_ID) (r22_gcr_addr) // Load GCR_CL_ID

	bnez    r8_core_num, all_done//  Only done from core 0.
	nop

	la      gp, _gp                // All vpe share globals.
	li      sp, STACK_BASE_ADDR     // Each vpe gets it's own stack.

	la	a2, init_cp0    // Init CP0 Status, Count, Compare, Watch*, and Cause.
	jalr	a2
	nop

	bnez    r9_vpe_num, init_done // If we are not a vpe0 then we are done.
	nop

	bnez    r8_core_num, init_sys_resources	//  Only done from core 0.
	nop

	la a2,     disable_L2   // Disable L2 caches
	jalr a2
	nop

	la a2,     init_icache // Initialize the L1 instruction cache. (Executing using I$ on return.)
	jalr a2
	nop

	// The changing of Kernel mode cacheability must be done from KSEG1
	// Since the code is executing from KSEG0 It needs to do a jump to KSEG1 change
	// K0 and jump back to KSEG0
	la		a2,		change_k0_cca
	li		a1, 0xf
	ins		a2, a1, 29, 1 // changed to KSEG1 address by setting bit 29
	jalr	a2
	nop

	la a2,     init_dcache // Initialize the L1 data cache
	jalr a2
	nop

init_sys_resources:     // Only done by core0 vpe0.
	la	a2, copy_c2_ram // Copy "C" code and data to RAM and zero bss (uncached.)
	jalr	a2
	nop

	la a2,     init_L2   // Initialize the unified L2 cache for non UP cores after L1 caches have been initialized
	jalr a2
	nop

	la		a2, enable_L2   // enable L2 cache
	jalr	a2
	nop

#ifdef SMP
	la a2,     release_mp  // Release other cores to execute this boot code.
	jalr a2
	nop
#endif

init_done:
	la      ra, all_done	// If main return then go to all_done:.
	la      a1, main
	mtc0    a1, C0_ERRPC			// ErrorEPC
	move	a0, r23_cpu_num

	eret    // Exit reset exception handler for this vpe and start execution of main().

/**************************************************************************************
**************************************************************************************/
all_done:
	// Looks like main returned. Just busy wait spin.
	b     all_done
	nop
