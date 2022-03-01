#include <boot.h>
#include <regdef.h>
#include <m32c0.h>

	.set	noreorder           // Don't allow the assembler to reorder instructions.
	.set	noat                // Don't allow the assembler to use r1(at) for synthetic instr.
/**************************************************************************************
**************************************************************************************/
LEAF(init_cp0)
	li	v1, 0x24400004		// (M_StatusERL | M_StatusIPL1 | M_StatusBEV)
	mtc0	v1, C0_STATUS		// write C0_Status

	// Initialize Watch registers if implemented.
	mfc0	v0, C0_CONFIG1		// read C0_Config1
	ext	v1, v0, 3, 1		// extract bit 3 WR (Watch registers implemented)
	beq 	v1, zero, done_wr
	li    	v1, 0x7				// (M_WatchHiI | M_WatchHiR | M_WatchHiW)

	// Clear Watch Status bits and disable watch exceptions
	mtc0	v1, C0_WATCHHI		// write C0_WatchHi0
	mtc0	zero, C0_WATCHLO	// write C0_WatchLo0

	mtc0	v1, C0_WATCHHI, 1	// write C0_WatchHi1
	mtc0	zero, C0_WATCHLO, 1	// write C0_WatchLo1

	mtc0	v1, C0_WATCHHI, 2	// write C0_WatchHi2
	mtc0	zero, C0_WATCHLO, 2	// write C0_WatchLo2

	mtc0	v1, C0_WATCHHI, 3	// write C0_WatchHi3
	mtc0	zero, C0_WATCHLO, 3	// write C0_WatchLo3

	mtc0	v1, C0_WATCHHI, 4	// write C0_WatchHi4
	mtc0	zero, C0_WATCHLO, 4	// write C0_WatchLo4

	mtc0	v1, C0_WATCHHI, 5	// write C0_WatchHi5
	mtc0	zero, C0_WATCHLO, 5	// write C0_WatchLo5

	mtc0	v1, C0_WATCHHI, 6	// write C0_WatchHi6
	mtc0	zero, C0_WATCHLO, 6	// write C0_WatchLo6

	mtc0	v1, C0_WATCHHI, 7	// write C0_WatchHi7
	mtc0	zero, C0_WATCHLO, 7	// write C0_WatchLo7
done_wr:
	mtc0	zero, C0_CAUSE		// write C0_Cause: Init AFTER init of CP0 WatchHi/Lo registers.
	mtc0	zero, C0_COMPARE	// write C0_Compare

	ehb
	jr      ra
	nop
END(init_cp0)
