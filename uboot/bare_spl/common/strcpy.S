
/*
 * Originally written by Kjeld Borch Egevang <kjelde@mips.com>, 2002.
 */

#include <regdef.h>
#include <m32c0.h>
#include <boot.h>


	.set	nomips16

/* char *strcpy(char *dest, const char *src); */

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
#define	S	sd
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
#define	S	sw
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


#ifndef BRANCHLIKELY

#define STRCPY_WORD(OFS,r1,r2)			\
3:	SUBU	t0, r1, t1			;\
	and	t0, t7				;\
	beq	t0, zero, 4f			;\
	and	t4, t0, r1			;\
	beq	t4, t0, 4f			;\
	nop					;\
    	ADDIU	a1, OFS*word_size		;\
	lbu	t2, 0(a1)			;\
	b	..strcpy_byte			;\
    	ADDIU	a0, OFS*word_size		;\
4:	L	r2, (OFS+1)*word_size(a1)	;\
	S	r1, OFS*word_size(a0)		;\

#ifdef LOADDELAY

#define STRCPY_BYTE(OFS,r1,r2)			\
	bne	r1, zero, 4f			;\
	sb	r1, OFS(a0)			;\
	jr	ra				;\
	nop					;\
4:	lbu	r2, OFS+1(a1)			;\
	nop					;\

#else /* ! LOADDELAY */

#define STRCPY_BYTE(OFS,r1,r2)			\
	bne	r1, zero, 4f			;\
	sb	r1, OFS(a0)			;\
	jr	ra				;\
	nop					;\
4:	lbu	r2, OFS+1(a1)			;\

#endif /* LOADDELAY */

#else /* BRANCHLIKELY */

#define STRCPY_WORD(OFS,r1,r2)			\
3:	SUBU	t0, r1, t1			;\
	and	t0, t7				;\
	beql	t0, zero, 4f			;\
	L	r2, (OFS+1)*word_size(a1)	;\
	and	t4, t0, r1			;\
	beql	t4, t0, 4f			;\
	L	r2, (OFS+1)*word_size(a1)	;\
    	ADDIU	a0, OFS*word_size		;\
    	ADDIU	a1, OFS*word_size		;\
	b	..strcpy_byte			;\
	lbu	t2, 0(a1)			;\
4:	S	r1, OFS*word_size(a0)		;\

#define STRCPY_BYTE(OFS,r1,r2)			\
	sb	r1, OFS(a0)			;\
	bnel	r1, zero, 4f			;\
	lbu	r2, OFS+1(a1)			;\
	jr	ra				;\
	nop					;\
4:						;\

#endif /* BRANCHLIKELY */


LEAF(strcpy)
	.set	noreorder

	or	t0, a0, a1		/* Check for unaligned strings */
	andi	t0, word_mask
	beq	t0, zero, 1f
	move	v0, a0
#ifdef LOADDELAY
	lbu	t2, 0(a1)
	b	..strcpy_byte
	nop
#else
	b	..strcpy_byte
	lbu	t2, 0(a1)
#endif

1:	L	t2, 0x0(a1)
	lui	t1, 0x0101
	ori	t1, 0x0101
#ifdef	MIPS64
	dsll	t7, t1, 32
	or	t1, t7
#endif
	SLL	t7, t1, 7

2:	STRCPY_WORD(0,t2,t3)
	STRCPY_WORD(1,t3,t2)
	STRCPY_WORD(2,t2,t3)
	STRCPY_WORD(3,t3,t2)

	ADDIU	a0, 4*word_size
	b	2b
	ADDIU	a1, 4*word_size

..strcpy_byte:
	STRCPY_BYTE(0,t2,t3)
	STRCPY_BYTE(1,t3,t2)
	STRCPY_BYTE(2,t2,t3)
	STRCPY_BYTE(3,t3,t2)
	STRCPY_BYTE(4,t2,t3)
	STRCPY_BYTE(5,t3,t2)
	STRCPY_BYTE(6,t2,t3)
	STRCPY_BYTE(7,t3,t2)
    	ADDIU	a0, 8
	b	..strcpy_byte
    	ADDIU	a1, 8
END(strcpy)

