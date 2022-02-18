/*
 * memset.S: fast memset for MIPS
 */

#include <regdef.h>
#include <m32c0.h>
#include <boot.h>

	.set	nomips16

#if __mips >= 3 && __mips != 32
/* we're a leaf function, so its safe to use 64-bit ops, as
   an optimisation even if -mgp32 was used. */
	.set gp64
# undef __mips64
# define __mips64 1
#endif

#if __mips64
#define S	sd
#define SL	sdl
#define SR	sdr
#define RS	8
#else
#define S	sw
#define SL	swl
#define SR	swr
#define RS	4
#endif

#ifdef MIPSEL
#	define	SHI	SR
#	define	SLO	SL
#endif
#ifdef MIPSEB
#	define	SHI	SL
#	define	SLO	SR
#endif

/* memset(to, val, n) */
LEAF(memset)
	.set	noreorder
	move	v0, a0			# save to for return
	beq	a2, zero, ..ret
	sltu	t2, a2, 16
	bne	t2, zero, ..small	# do small blocks byte at a time

	/* replicate fill byte into register */
	and	a1, 0xff
#if __mips64
	dsll	t2, a1, 8
	or	a1, t2
	dsll	t2, a1, 16
	or	a1, t2
	dsll	t2, a1, 32
	or	a1, t2
#else
	sll	t2, a1, 8
	or	a1, t2
	sll	t2, a1, 16
	or	a1, t2
#endif

	and	v1, a0, RS-1		# get unaligned address
	beq	v1, zero, 1f		# skip if already aligned
	li	a3, RS			# calculate...
	subu	a3, v1			#  number of bytes to align

	subu	a2, a3			# subtract from remaining count
	SHI	a1, 0(a0)		# store 1..RS bytes to align a0
	addu	a0, a3			# bump pointer

	/* Try a 4X unrolled block fill */
1:	and	v1, a2, (RS*4)-1	# remaining size % blocksize
	subu	a3, a2, v1		# size of remaining blocks
	beq	a3, zero, 1f		# none?
	move	a2, v1			# bytes remaining after block copy
	addu	a3, a0			# compute ending address

2:	S	a1, RS*0(a0)
	S	a1, RS*1(a0)
	S	a1, RS*2(a0)
	addu	a0, RS*4
	bne	a0, a3, 2b
	S	a1, -RS(a0)

	/* Try a word at a time */
1:	and	v1, a2, RS-1		# remaining size % word size
	subu	a3, a2, v1		# size of remaining words
	beq	a3, zero, ..small	# none?
	move	a2, v1			# bytes remaining after word copy
	addu	a3, a0			# compute ending address

2:	addu	a0, RS
	bne	a0, a3, 2b
	S	a1, -RS(a0)

..small:
	/* Last resort: byte at a time */
	beq	a2, zero, ..ret
	addu	a3, a2, a0		# compute ending address

1:	addu	a0, 1
	bne	a0, a3, 1b
	sb	a1, -1(a0)

..ret:	j	ra
	nop
	.set	reorder
END(memset)

