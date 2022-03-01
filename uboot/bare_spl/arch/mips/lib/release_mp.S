#include <boot.h>
#include <regdef.h>
#include <cps.h>

	.set	noreorder           // Don't allow the assembler to reorder instructions.
	.set	noat                // Don't allow the assembler to use r1(at) for synthetic instr.
/**************************************************************************************
**************************************************************************************/
LEAF(release_mp)

    blez    r19_more_cores, done_release_mp     // If no more cores then we are done.
    li      a3, 1

    beqz    r30_cpc_addr, release_next_core     // If no CPC then use GCR_CO_RESET_RELEASE
    nop
										// else use CPC Power Up command.
    move    a0, a3
    sll     a0, 16
    sw      a0, (CPS_CORE_LOCAL_CONTROL_BLOCK | CPC_OTHERL_REG)(r30_cpc_addr)
    li      a0, PWR_DOWN							// "PwrUp" power domain command.
    sw      a0, (CPS_CORE_OTHER_CONTROL_BLOCK | CPC_CMDO_REG)(r30_cpc_addr)
    nop
    move      a0, zero
    sw      a0, (CPS_CORE_LOCAL_CONTROL_BLOCK | CPC_OTHERL_REG)(r30_cpc_addr)
    nop

    sll     a0, a3, 16
    sw      a0, (CORE_LOCAL_CONTROL_BLOCK | GCR_CL_OTHER)(r22_gcr_addr) // GCR_CL_OTHER[CoreNum]
	li		a3,0xbc004000
	sw		a3,(CORE_OTHER_CONTROL_BLOCK | GCR_CO_RESET_BASE)(r22_gcr_addr)
	nop
	li		a3,1
powerup_next_core:
    // Send PwrUp command to next core causing execution at their reset exception vector.
    move    a0, a3
    sll     a0, 16
    sw      a0, (CPS_CORE_LOCAL_CONTROL_BLOCK | CPC_OTHERL_REG)(r30_cpc_addr)
    li      a0, PWR_UP							// "PwrUp" power domain command.
    sw      a0, (CPS_CORE_OTHER_CONTROL_BLOCK | CPC_CMDO_REG)(r30_cpc_addr)
    bne     r19_more_cores, a3, powerup_next_core
    addiu     a3, a3, 1

    jalr	zero, ra
    nop

release_next_core:
	sdbbp
	nop

done_release_mp:
    jalr	zero, ra
    nop
END(release_mp)
