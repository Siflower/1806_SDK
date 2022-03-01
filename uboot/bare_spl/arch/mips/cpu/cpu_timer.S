#include <boot.h>
#include <m32c0.h>
#include <regdef.h>
#include <clk.h>

/*C0_COUNT change FREQ,SF_CPU_CLK/1MHZ/2.*/
#define CPU_USEC (SF_CPU_CLK)/2000000

LEAF(u_delay_cpu)
	mtc0 zero,C0_COUNT
	li a1,CPU_USEC
	mul a1,a0
1:
	mfc0 a0,C0_COUNT
	subu	a2,a1,a0
	bgez a2,1b
	nop

	jr	ra
	nop
END(u_delay_cpu)

LEAF(m_delay_cpu)
	//attention: gpr a3 mustn't be used in u_delay!
	move a3,ra

#ifdef SIMULATION
	li	a2,1
#else
	li	a2,1000
#endif
	mul	a0,a2

	la	a2,u_delay_cpu
	jalr a2
	nop

	jr	a3
	nop
END(m_delay_cpu)
