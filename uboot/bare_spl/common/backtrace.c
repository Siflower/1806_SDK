#include <types.h>
#include <io.h>
#include <sf_mmap.h>
#include <sys_manager.h>
#include <backtrace.h>

#include <common/stdiol.h>

extern void *memcpy(void *dest, const void *src, size_t n);

static inline int get_mem(unsigned long addr, unsigned long *result)
{
	unsigned long *address = (unsigned long *) addr;
//	if (!access_ok(VERIFY_READ, addr, sizeof(unsigned long)))
//		return -1;
	memcpy(result, address, sizeof(unsigned long));
//	if (__copy_from_user_inatomic(result, address, sizeof(unsigned long)))
//		return -3;
	return 0;
}

/*
 * These two instruction helpers were taken from process.c
 */
static inline int is_ra_save_ins(union mips_instruction *ip)
{
	/* sw / sd $ra, offset($sp) */
	return (ip->i_format.opcode == sw_op || ip->i_format.opcode == sd_op)
		&& ip->i_format.rs == 29 && ip->i_format.rt == 31;
}

static inline int is_sp_move_ins(union mips_instruction *ip)
{
	/* addiu/daddiu sp,sp,-imm */
	if (ip->i_format.rs != 29 || ip->i_format.rt != 29)
		return 0;
	if (ip->i_format.opcode == addiu_op || ip->i_format.opcode == daddiu_op)
		return 1;
	return 0;
}

/*
 * Looks for specific instructions that mark the end of a function.
 * This usually means we ran into the code area of the previous function.
 */
static inline int is_end_of_function_marker(union mips_instruction *ip)
{
	/* jr ra */
	if (ip->r_format.func == jr_op && ip->r_format.rs == 31 &&
			ip->r_format.opcode == 0)
		return 1;
	/* lui gp */
	if (ip->i_format.opcode == lui_op && ip->i_format.rt == 28)
		return 1;
	return 0;
}

/*
 * TODO for userspace stack unwinding:
 * - handle cases where the stack is adjusted inside a function
 *     (generally doesn't happen)
 * - find optimal value for max_instr_check
 * - try to find a way to handle leaf functions
 */

static inline int unwind_user_frame(struct stackframe *old_frame,
				    const unsigned int max_instr_check)
{
	struct stackframe new_frame = *old_frame;
	long ra_offset = 0;
	unsigned long stack_size = 0;
	unsigned long addr;

	if (old_frame->pc == 0 || old_frame->sp == 0 || old_frame->ra == 0)
		return -9;

	for (addr = new_frame.pc; (addr + max_instr_check > new_frame.pc)
		&& (!stack_size); addr-=4) {
		union mips_instruction ip;

		if (get_mem(addr, (unsigned long *) &ip))
			return -11;

		if (is_sp_move_ins(&ip)) {
			int stack_adjustment = ip.i_format.simmediate;
			if (stack_adjustment > 0)
				/* This marks the end of the previous function,
				   which means we overran. */
				break;
			stack_size = (unsigned long) stack_adjustment;
		} else if (is_end_of_function_marker(&ip))
			break;
	}

	for (addr = new_frame.ra; (addr + max_instr_check > new_frame.ra)
		&& (!ra_offset); addr-=4) {
		union mips_instruction ip;

		if (get_mem(addr, (unsigned long *) &ip))
			return -11;

		if (is_ra_save_ins(&ip)) {
			int ra_slot = ip.i_format.simmediate;
			if (ra_slot < 0)
				/* This shouldn't happen. */
				break;
			ra_offset = ra_slot;
		} else if (is_end_of_function_marker(&ip))
			break;
	}


	if (!ra_offset || !stack_size)
		return -1;

	if (stack_size) {
		new_frame.sp = old_frame->sp - stack_size;
	//	if (get_mem(new_frame.sp, &(new_frame.sp)))
	//		return -14;
	}

	if (ra_offset) {
		new_frame.ra = new_frame.sp + ra_offset;
		if (get_mem(new_frame.ra, &(new_frame.ra)))
			return -13;
	}

	if (new_frame.sp < old_frame->sp)
		return -2;

	new_frame.pc = old_frame->ra;
	*old_frame = new_frame;

	return 0;
}

static inline void do_user_backtrace(unsigned long high_addr,
				     struct stackframe *frame,
				     unsigned int depth)
{
	const unsigned int max_instr_check = 1024;
	const unsigned long low_addr = high_addr - SIFLOWER_STACK_SIZE;

	pr_info("%x<-\n",(unsigned int)frame->pc);
	while (depth-- && !unwind_user_frame(frame, max_instr_check)) {
		//oprofile_add_trace(frame->ra);
		if (frame->sp < low_addr || frame->sp > high_addr)
			break;
		pr_info("%x<-\n",(unsigned int)frame->pc);
	}
}

/*
 *	Get mips PC.
 */
static unsigned long __attribute__((optimize("O0"))) __getpc(void)
{
	unsigned long rtaddr;
	asm volatile("move %0,$31":"=r"(rtaddr):);
	return rtaddr;
}

void notrace op_mips_backtrace(void)
{
	struct stackframe frame;
	asm volatile("move %0,$29":"=r"(frame.sp):);
//	asm volatile("mfc0 %0,$14":"=r"(frame.pc):);
	asm volatile("move %0,$31":"=r"(frame.ra):);
	frame.pc = __getpc();
	pr_info("Call stack:\n");
	do_user_backtrace((unsigned long)SIFLOWER_STACK_BASE,&frame,10);
	pr_info("main.c\n");
}
/*
#ifndef CONFIG_KALLSYMS
static inline void do_kernel_backtrace(unsigned long low_addr,
				       struct stackframe *frame,
				       unsigned int depth) { }
#else
static inline void do_kernel_backtrace(unsigned long low_addr,
				       struct stackframe *frame,
				       unsigned int depth)
{
	while (depth-- && frame->pc) {
		frame->pc = unwind_stack_by_address(low_addr,
						    &(frame->sp),
						    frame->pc,
						    &(frame->ra));
		oprofile_add_trace(frame->ra);
	}
}
#endif

void notrace op_mips_backtrace(struct pt_regs *const regs, unsigned int depth)
{
	struct stackframe frame = { .sp = regs->regs[29],
				    .pc = regs->cp0_epc,
				    .ra = regs->regs[31] };
	const int userspace = user_mode(regs);
	const unsigned long low_addr = ALIGN(frame.sp, THREAD_SIZE);

	if (userspace)
		do_user_backtrace(low_addr, &frame, depth);
	else
		do_kernel_backtrace(low_addr, &frame, depth);
}
*/
