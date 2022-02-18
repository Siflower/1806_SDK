#ifndef __BACKTRACE_H_
#define __BACKTRACE_H_

#define notrace __attribute__((no_instrument_function))

struct stackframe {
	unsigned long sp;
	unsigned long pc;
	unsigned long ra;
};
#define __BITFIELD_FIELD(field, more)                   \
    more                              \
    field;
enum major_op {
	spec_op, bcond_op, j_op, jal_op,
	beq_op, bne_op, blez_op, bgtz_op,
	addi_op, addiu_op, slti_op, sltiu_op,
	andi_op, ori_op, xori_op, lui_op,
	cop0_op, cop1_op, cop2_op, cop1x_op,
	beql_op, bnel_op, blezl_op, bgtzl_op,
	daddi_op, daddiu_op, ldl_op, ldr_op,
	spec2_op, jalx_op, mdmx_op, spec3_op,
	lb_op, lh_op, lwl_op, lw_op,
	lbu_op, lhu_op, lwr_op, lwu_op,
	sb_op, sh_op, swl_op, sw_op,
	sdl_op, sdr_op, swr_op, cache_op,
	ll_op, lwc1_op, lwc2_op, pref_op,
	lld_op, ldc1_op, ldc2_op, ld_op,
	sc_op, swc1_op, swc2_op, major_3b_op,
	scd_op, sdc1_op, sdc2_op, sd_op
};

enum spec_op {
	sll_op, movc_op, srl_op, sra_op,
	sllv_op, pmon_op, srlv_op, srav_op,
	jr_op, jalr_op, movz_op, movn_op,
	syscall_op, break_op, spim_op, sync_op,
	mfhi_op, mthi_op, mflo_op, mtlo_op,
	dsllv_op, spec2_unused_op, dsrlv_op, dsrav_op,
	mult_op, multu_op, div_op, divu_op,
	dmult_op, dmultu_op, ddiv_op, ddivu_op,
	add_op, addu_op, sub_op, subu_op,
	and_op, or_op, xor_op, nor_op,
	spec3_unused_op, spec4_unused_op, slt_op, sltu_op,
	dadd_op, daddu_op, dsub_op, dsubu_op,
	tge_op, tgeu_op, tlt_op, tltu_op,
	teq_op, spec5_unused_op, tne_op, spec6_unused_op,
	dsll_op, spec7_unused_op, dsrl_op, dsra_op,
	dsll32_op, spec8_unused_op, dsrl32_op, dsra32_op
};

struct i_format {			/* signed immediate format */
	__BITFIELD_FIELD(unsigned int opcode : 6,
	__BITFIELD_FIELD(unsigned int rs : 5,
	__BITFIELD_FIELD(unsigned int rt : 5,
	__BITFIELD_FIELD(signed int simmediate : 16,
	;))))
};

struct r_format {			/* Register format */
	__BITFIELD_FIELD(unsigned int opcode : 6,
	__BITFIELD_FIELD(unsigned int rs : 5,
	__BITFIELD_FIELD(unsigned int rt : 5,
	__BITFIELD_FIELD(unsigned int rd : 5,
	__BITFIELD_FIELD(unsigned int re : 5,
	__BITFIELD_FIELD(unsigned int func : 6,
	;))))))
};

union mips_instruction {
	unsigned int word;
	unsigned short halfword[2];
	unsigned char byte[4];
	struct i_format i_format;
	struct r_format r_format;
};

#endif

