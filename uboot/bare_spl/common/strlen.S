
/*
 * Originally written by Kjeld Borch Egevang <kjelde@mips.com>, 2002.
 */

#include <regdef.h>
#include <m32c0.h>
#include <boot.h>


	.set	nomips16

/* size_t strlen(const char *s); */

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


#if (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN) || defined(__MIPSEB__)
#define LHI	LL		/* high part is left in big-endian      */
#endif


#if (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN) || defined(__MIPSEL__)
#define LHI	LR		/* high part is right in little-endian  */
#endif


#ifndef BRANCHLIKELY

#ifdef LOADDELAY

#define	STRLEN_BYTE(OFS)		\
	bne	t2, zero, 2f		;\
	nop				;\
	jr	ra			;\
	li	v0, OFS			;\
2:	lbu	t2, OFS+1(a0)		;\
	nop				;\

#define	STRLEN_BYTE_END(OFS)		\
	bne	t2, zero, 2f		;\
	nop				;\
	ADDIU	a0, OFS			;\
	jr	ra			;\
	SUBU	v0, a0, v0		;\
2:	lbu	t2, OFS+1(a0)		;\
	nop				;\

#else /* ! LOADDELAY */

#define	STRLEN_BYTE(OFS)		\
	bne	t2, zero, 2f		;\
	nop				;\
	jr	ra			;\
	li	v0, OFS			;\
2:	lbu	t2, OFS+1(a0)		;\

#define	STRLEN_BYTE_END(OFS)		\
	bne	t2, zero, 2f		;\
	nop				;\
	ADDIU	a0, OFS			;\
	jr	ra			;\
	SUBU	v0, a0, v0		;\
2:	lbu	t2, OFS+1(a0)		;\

#endif /* LOADDELAY */

#define STRLEN_WORD(OFS,r1,r2)			\
	SUBU	t0, r1, t1			;\
	and	t0, t7				;\
	beq	t0, zero, 4f			;\
	and	t4, t0, r1			;\
	beq	t4, t0, 4f			;\
	nop					;\
	lbu	t2, OFS*word_size(a0)		;\
	b	..strlen_byte			;\
    	ADDIU	a0, OFS*word_size		;\
4:	L	r2, (OFS+1)*word_size(a0)	;\

#else /* BRANCHLIKELY */

#define	STRLEN_BYTE(OFS)		\
2:	bnel	t2, zero, 2f		;\
	lbu	t2, OFS+1(a0)		;\
	jr	ra			;\
	li	v0, OFS

#define	STRLEN_BYTE_END(OFS)		\
2:	bnel	t2, zero, 2f		;\
	lbu	t2, OFS+1(a0)		;\
	ADDIU	a0, OFS			;\
	jr	ra			;\
	SUBU	v0, a0, v0

#define STRLEN_WORD(OFS,r1,r2)			\
4:	SUBU	t0, r1, t1			;\
	and	t0, t7				;\
	beql	t0, zero, 4f			;\
	L	r2, (OFS+1)*word_size(a0)	;\
	and	t4, t0, r1			;\
	beql	t4, t0, 4f			;\
	L	r2, (OFS+1)*word_size(a0)	;\
    	ADDIU	a0, OFS*word_size		;\
	b	..strlen_byte			;\
	lbu	t2, 0(a0)			;\

#endif /* BRANCHLIKELY */


LEAF(strlen)
	.set	noreorder

	lbu	t2, 0x0(a0)
    	move	v0, a0

	STRLEN_BYTE(0)
	STRLEN_BYTE(1)
	STRLEN_BYTE(2)
#ifdef  MIPS64
	STRLEN_BYTE(3)
	STRLEN_BYTE(4)
	STRLEN_BYTE(5)
	STRLEN_BYTE(6)
#endif
2:	bne	t2, zero, 2f
	srl	a0, word_bits
	jr	ra
	li	v0, word_mask
2:	addiu	a0, 1
	sll	a0, word_bits
	L	t2, 0x0(a0)
	lui	t1, 0x0101
	ori	t1, 0x0101
#ifdef	MIPS64
	dsll	t7, t1, 32
	or	t1, t7
#endif
	SLL	t7, t1, 7

3:    	STRLEN_WORD(0,t2,t3)
    	STRLEN_WORD(1,t3,t2)
    	STRLEN_WORD(2,t2,t3)
    	STRLEN_WORD(3,t3,t2)
4:	b	3b
	ADDIU	a0, 4*word_size

..strlen_byte:				/* Normal, byte-oriented strlen() */
	STRLEN_BYTE_END(0)
	STRLEN_BYTE_END(1)
	STRLEN_BYTE_END(2)
#ifdef  MIPS64
	STRLEN_BYTE_END(3)
	STRLEN_BYTE_END(4)
	STRLEN_BYTE_END(5)
	STRLEN_BYTE_END(6)
#endif
2:	ADDIU	a0, word_mask
	jr	ra
	SUBU	v0, a0, v0
END(strlen)

