#include <common.h>
#include <asm/io.h>
#include <dma.h>
#include <dm/device.h>
#include <errno.h>
#include <mach/sfax8.h>
#include <mach/dma.h>


struct sfax8_priv {
	u32 base;
};

static int gdma_channel = 0;
static gdma_reqcfg req_cfg[GDMA_CHANNEL_MAX];

static inline int is_manager(u32 chanenl)
{
	/* MANAGER is indexed at the end */
	if (chanenl == req_cfg[chanenl].port_num)
		return 1;
	else
		return 0;
}

static inline u32 _state(u32 base,int channel)
{
	u32 val;

	if (is_manager(channel))
		val = readl((void*)(base + GDMA_DMS)) & 0xf;
	else
		val = readl((void*)(base + GDMA_CS(channel))) & 0xf;

	switch (val) {
		case DS_ST_STOP:
			return PL330_STATE_STOPPED;
		case DS_ST_EXEC:
			return PL330_STATE_EXECUTING;
		case DS_ST_CMISS:
			return PL330_STATE_CACHEMISS;
		case DS_ST_UPDTPC:
			return PL330_STATE_UPDTPC;
		case DS_ST_WFE:
			return PL330_STATE_WFE;
		case DS_ST_FAULT:
			return PL330_STATE_FAULTING;
		case DS_ST_ATBRR:
			if (is_manager(channel))
				return PL330_STATE_INVALID;
			else
				return PL330_STATE_ATBARRIER;
		case DS_ST_QBUSY:
			if (is_manager(channel))
				return PL330_STATE_INVALID;
			else
				return PL330_STATE_QUEUEBUSY;
		case DS_ST_WFP:
			if (is_manager(channel))
				return PL330_STATE_INVALID;
			else
				return PL330_STATE_WFP;
		case DS_ST_KILL:
			if (is_manager(channel))
				return PL330_STATE_INVALID;
			else
				return PL330_STATE_KILLING;
		case DS_ST_CMPLT:
			if (is_manager(channel))
				return PL330_STATE_INVALID;
			else
				return PL330_STATE_COMPLETING;
		case DS_ST_FLTCMP:
			if (is_manager(channel))
				return PL330_STATE_INVALID;
			else
				return PL330_STATE_FAULT_COMPLETING;
		default:
			return PL330_STATE_INVALID;
	}
}

static inline u32 _emit_KILL(u32 dry_run, u8 *buf)
{
	if (dry_run)
		return 1;

	buf[0] = DMAKILL;

	return 1;
}

static inline void _execute_DBGINSN(u32 base,int channel, u8 *buf, int as_manager)
{
	u32 val;

	val = (buf[0] << 16) | (buf[1] << 24);
	if (!as_manager) {
		val |= (1 << 0);
		val |= (channel << 8); /* Channel Number */
	}
	writel(val, (void*)(base + GDMA_DI0));

	val = (buf[2]) | (buf[3]<<8) | (buf[4]<<16) | (buf[5]<<24);

	//virtual addr convert to phy addr.
	val = (u32)CPHYSADDR(val);

	writel(val, (void*)(base + GDMA_DI1));

	while((readl((void*)(base + GDMA_DS)) & DBG_BUSY));

	/* Get going */
	writel(0,(void*)(base + GDMA_DC));
}

static void _stop(u32 base,int channel)
{
	u8 buf[6] = {0, 0, 0, 0, 0, 0};

	if (_state(base,channel) == PL330_STATE_FAULT_COMPLETING)
	{
		while (!(_state(base,channel) & (PL330_STATE_FAULTING | PL330_STATE_KILLING)))
		{
			if (_state(base,channel) == PL330_STATE_STOPPED)
				return;
		}
	}

	_emit_KILL(0, buf);

	/* Stop generating interrupts for SEV */
	writel(readl((void*)(base + GDMA_IE)) & ~(1 << req_cfg[channel].ev_num),
				(void*)(base + GDMA_IE));

	_execute_DBGINSN(base,channel, buf, is_manager(channel));
	writel(1 << req_cfg[channel].ev_num, (void*)(base + GDMA_IC));
}

static inline int pl330_dmaend(u8 *desc_pool_virt)
{
	u8 opcode = DMAEND;

	writeb(opcode, (void*)desc_pool_virt);
	return 1;
}

static inline int pl330_dmasev(u8 *desc_pool_virt, u8 event_num)
{
	u8 opcode = DMASEV;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(event_num << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmalpend(u8 *desc_pool_virt, u8 backwards_jump, u8 lc)
{
	u8 opcode = DMALPEND | (lc << 2);

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(backwards_jump, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmast(u8 *desc_pool_virt)
{
	u8 opcode = DMAST;

	writeb(opcode, (void*)desc_pool_virt);
	return 1;
}

static inline int pl330_dmaldps(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMALDPS;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmaldpb(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMALDPB;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmaflushp(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMAFLUSHHP;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmastps(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMASTPS;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmastpb(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMASTPB;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmald(u8 *desc_pool_virt)
{
	u8 opcode = DMALD;

	writeb(opcode, (void*)desc_pool_virt);
	return 1;
}

static inline int pl330_dmawfps(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMAWFPS;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmawfpb(u8 *desc_pool_virt, u8 periph)
{
	u8 opcode = DMAWFPB;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(periph << 3, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmalp(u8 *desc_pool_virt, u8 iter, u8 lc)
{
	u8 opcode = DMALP | (lc << 1);

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(iter, (void*)(desc_pool_virt));
	return 2;
}

static inline int pl330_dmamov(u8 *desc_pool_virt, u8 rd, u32 imm)
{
	u8 opcode = DMAMOV;

	writeb(opcode, (void*)(desc_pool_virt++));
	writeb(rd, (void*)(desc_pool_virt++));
	writeb(imm&0xff, (void*)(desc_pool_virt++));
	writeb((imm>>8)&0xff, (void*)(desc_pool_virt++));
	writeb((imm>>16)&0xff, (void*)(desc_pool_virt++));
	writeb((imm>>24)&0xff, (void*)(desc_pool_virt));

	return 6;
}

static void pl330_dmago(u32 base, gdma_reqcfg* preq_cfg, u32 inst_addr)
{
	u32 val;
	u8 opcode;
	opcode = DMAGO | (preq_cfg->nonsecure << 1);

	val = ((preq_cfg->port_num&0x7) << 24) | (opcode << 16) | ((preq_cfg->ev_num) << 8);
	writel(val, (void*)(base + GDMA_DI0));

	val = inst_addr;
	writel(val, (void*)(base + GDMA_DI1));

	writel(0, (void*)(base + GDMA_DC));
}

static int gdma_request(void)
{
	int index = 0;

	while(index < GDMA_CHANNEL_MAX)
	{
		if(!req_cfg[index].is_busy)
		{
			req_cfg[index].is_busy = 1;
			return index;
		}
		index++;
	}
	return -EBUSY;

}

static u32 _prepare_ccr(void)
{
	u32 ccr = 0;

	if (req_cfg[gdma_channel].src_inc)
		ccr |= CCR_SRCINC;

	if (req_cfg[gdma_channel].dst_inc)
		ccr |= CCR_DSTINC;

	/*  We set same protection levels for Src and DST for now */
	if (req_cfg[gdma_channel].privileged)
	{
		ccr |= CCR_SRCPRI;
		ccr |= CCR_DSTPRI;
	}

	if (req_cfg[gdma_channel].nonsecure)
		ccr |= CCR_SRCNS | CCR_DSTNS;
	if (req_cfg[gdma_channel].insnaccess)
		ccr |= CCR_SRCIA | CCR_DSTIA;

	ccr |= (((req_cfg[gdma_channel].brst_len - 1) & 0xf) << CCR_SRCBRSTLEN_SHFT);
	ccr |= (((req_cfg[gdma_channel].brst_len - 1) & 0xf) << CCR_DSTBRSTLEN_SHFT);

	ccr |= (req_cfg[gdma_channel].brst_size << CCR_SRCBRSTSIZE_SHFT);
	ccr |= (req_cfg[gdma_channel].brst_size << CCR_DSTBRSTSIZE_SHFT);

	ccr |= (req_cfg[gdma_channel].dcctl << CCR_SRCCCTRL_SHFT);
	ccr |= (req_cfg[gdma_channel].scctl << CCR_DSTCCTRL_SHFT);

	ccr |= (req_cfg[gdma_channel].swap << CCR_SWAP_SHFT);

	return ccr;
}

static u32 gdma_file_descriptor(gdma_reqcfg* preq_cfg, u8 *inst)
{
	u32 inst_size = 0;
	u32 loop_size = 0;
	u32 loop_size_rest = 0;
	u32 loop_count0 = 0;
	u32 loop_count1 = 0;
	u32 loop_count0_rest = 0;
	u32 loop_start0 = 0;
	u32 loop_start1 = 0;

	/* calculate loop count */
	loop_size = BRST_LEN(preq_cfg->ccr) * BRST_SIZE(preq_cfg->ccr);
	loop_count0 = (preq_cfg->tran_len / loop_size) - 1;
	loop_size_rest = preq_cfg->tran_len % loop_size;

	if (loop_size_rest) {
		printf( "Transfer length must be aligned to loop_size, loop_size_rest:0x%x!!!!!!!!\n", loop_size_rest);
		return -EINVAL;
	}

	if (loop_count0 >= PL330_MAX_LOOPS) {
		loop_count1 = (loop_count0 / PL330_MAX_LOOPS) - 1;
		loop_count0_rest = (loop_count0 % PL330_MAX_LOOPS) + 1;
		loop_count0 = PL330_MAX_LOOPS - 1;

		if (loop_count1 >= PL330_MAX_LOOPS)
		{
			printf( "loop_count1 overflow\n");
			loop_count1 = PL330_MAX_LOOPS - 1;
		}
	}

	/* write instruction sets on buffer */
	inst_size += pl330_dmamov(inst + inst_size, RD_DAR, preq_cfg->dst);
	inst_size += pl330_dmamov(inst + inst_size, RD_SAR, preq_cfg->src);
	inst_size += pl330_dmamov(inst + inst_size, RD_CCR, preq_cfg->ccr);

	if (loop_count1) {
		inst_size += pl330_dmalp(inst + inst_size, loop_count1, LC_1);
		loop_start1 = inst_size;
	}

	if (loop_count0) {
		inst_size += pl330_dmalp(inst + inst_size, loop_count0, LC_0);
		loop_start0 = inst_size;
	}

	if (preq_cfg->direct == DMA_MEM_TO_DEV)
	{
		if(preq_cfg->brst_len > 1)
			inst_size += pl330_dmawfpb(inst + inst_size, preq_cfg->periph);
		else
			inst_size += pl330_dmawfps(inst + inst_size, preq_cfg->periph);

		inst_size += pl330_dmald(inst + inst_size);

		if(preq_cfg->brst_len > 1)
			inst_size += pl330_dmastpb(inst + inst_size, preq_cfg->periph);
		else
			inst_size += pl330_dmastps(inst + inst_size, preq_cfg->periph);

		inst_size += pl330_dmaflushp(inst + inst_size, preq_cfg->periph);
	}
	else if (preq_cfg->direct == DMA_DEV_TO_MEM)
	{
		if(preq_cfg->brst_len > 1)
		{
			inst_size += pl330_dmawfpb(inst + inst_size, preq_cfg->periph);
			inst_size += pl330_dmaldpb(inst + inst_size, preq_cfg->periph);
		}
		else
		{
			inst_size += pl330_dmawfps(inst + inst_size, preq_cfg->periph);
			inst_size += pl330_dmaldps(inst + inst_size, preq_cfg->periph);
		}

		inst_size += pl330_dmast(inst + inst_size);
		inst_size += pl330_dmaflushp(inst + inst_size, preq_cfg->periph);
	}
	else
	{
		inst_size += pl330_dmald(inst + inst_size);
		inst_size += pl330_dmast(inst + inst_size);
	}

	if (loop_count0)
		inst_size += pl330_dmalpend(inst + inst_size, inst_size - loop_start0, LC_0);

	if (loop_count1)
		inst_size += pl330_dmalpend(inst + inst_size, inst_size - loop_start1, LC_1);

	if (loop_count0_rest)
	{
		inst_size += pl330_dmalp(inst + inst_size, loop_count0_rest - 1, LC_0);
		loop_start0 = inst_size;

		if (preq_cfg->direct == DMA_MEM_TO_DEV)
		{
			if(preq_cfg->brst_len > 1)
				inst_size += pl330_dmawfpb(inst + inst_size, preq_cfg->periph);
			else
				inst_size += pl330_dmawfps(inst + inst_size, preq_cfg->periph);

			inst_size += pl330_dmald(inst + inst_size);

			if(preq_cfg->brst_len > 1)
				inst_size += pl330_dmastpb(inst + inst_size, preq_cfg->periph);
			else
				inst_size += pl330_dmastps(inst + inst_size, preq_cfg->periph);

			inst_size += pl330_dmaflushp(inst + inst_size, preq_cfg->periph);
		}
		else if (preq_cfg->direct == DMA_DEV_TO_MEM)
		{
			if(preq_cfg->brst_len > 1)
			{
				inst_size += pl330_dmawfpb(inst + inst_size, preq_cfg->periph);
				inst_size += pl330_dmaldpb(inst + inst_size, preq_cfg->periph);
			}
			else
			{
				inst_size += pl330_dmawfps(inst + inst_size, preq_cfg->periph);
				inst_size += pl330_dmaldps(inst + inst_size, preq_cfg->periph);
			}

			inst_size += pl330_dmast(inst + inst_size);
			inst_size += pl330_dmaflushp(inst + inst_size, preq_cfg->periph);
		}
		else
		{
			inst_size += pl330_dmald(inst + inst_size);
			inst_size += pl330_dmast(inst + inst_size);
		}

		inst_size += pl330_dmalpend(inst + inst_size,inst_size - loop_start0, LC_0);
	}

	inst_size += pl330_dmasev(inst + inst_size, preq_cfg->ev_num);
	inst_size += pl330_dmaend(inst + inst_size);

	return inst_size;
}

static int gdma_move(u32 base, u32 dst, u32 src, u32 len)
{
	int unaligned_trans = len%128;
	int aligned_trans = len - unaligned_trans;
	u8 *insn = (u8*)malloc(GDMA_INSN_LEN_MAN);

	//set insn to uncached addr.
    insn = (u8*)((u32)insn | (1<<29));

	//use defualt value
	req_cfg[gdma_channel].nonsecure = SECURE_VAL;
	req_cfg[gdma_channel].privileged = 0;
	req_cfg[gdma_channel].insnaccess = 0;
	req_cfg[gdma_channel].dcctl = DCCTRL0;
	req_cfg[gdma_channel].scctl = SCCTRL0;
	req_cfg[gdma_channel].swap =SWAP_NO;

	req_cfg[gdma_channel].port_num = gdma_channel;

	//aligned transfer
	if(aligned_trans != 0)
	{
		req_cfg[gdma_channel].brst_len = 16;
		req_cfg[gdma_channel].brst_size = 3;
		req_cfg[gdma_channel].tran_len = aligned_trans;

		req_cfg[gdma_channel].ev_num = gdma_channel;
		req_cfg[gdma_channel].src = (u32)CPHYSADDR(src);
		req_cfg[gdma_channel].dst = (u32)CPHYSADDR(dst);
		req_cfg[gdma_channel].ccr = _prepare_ccr();

		gdma_file_descriptor(&req_cfg[gdma_channel],insn);

		writel(readl((void*)(base + GDMA_IE)) | (1 << req_cfg[gdma_channel].ev_num),
				(void*)(base + GDMA_IE));
		writel(1 << req_cfg[gdma_channel].ev_num,(void*)(base + GDMA_IC));

		pl330_dmago(base,&req_cfg[gdma_channel],(u32)CPHYSADDR(insn));
	}

	//unaligned transfer
	if(unaligned_trans != 0)
	{
		req_cfg[gdma_channel].brst_len = 1;
		req_cfg[gdma_channel].brst_size = 0;
		req_cfg[gdma_channel].tran_len = unaligned_trans;

		req_cfg[gdma_channel].ev_num = gdma_channel;
		req_cfg[gdma_channel].src = (u32)CPHYSADDR(src + aligned_trans);
		req_cfg[gdma_channel].dst = (u32)CPHYSADDR(dst + aligned_trans);
		req_cfg[gdma_channel].ccr = _prepare_ccr();

		gdma_file_descriptor(&req_cfg[gdma_channel],insn);

		writel(readl((void*)(base + GDMA_IE)) | (1 << req_cfg[gdma_channel].ev_num),
				(void*)(base + GDMA_IE));
		writel(1 << req_cfg[gdma_channel].ev_num,(void*)(base + GDMA_IC));

		pl330_dmago(base,&req_cfg[gdma_channel],(u32)CPHYSADDR(insn));
	}

	return 0;
}

static int gdma_sample_complete(u32 base,int eirs)
{
	u32 val;

	val = readl((void*)(base + GDMA_EIRS));

	if(val == eirs)
	{
		writel(val,(void*)(base + GDMA_IC));
	}
	else
	{
		do
		{
			val = readl((void*)(base + GDMA_FSDM)) & 0x1;
			if (val)
			{
				printf("dmac manager fault: 0x%x\n", val);
				break;
			}
			val = readl((void*)(base + GDMA_FSDC));
			if(val)
			{
				printf("dmac channel fault: 0x%x, 0x%x\n", val, readl((void*)(base + 0x40)));
				break;
			}
			val = readl((void*)(base + GDMA_EIRS));
			if(val == eirs)
			{
				writel(val, (void*)(base + GDMA_IC));
				break;
			}
		}while (val != eirs);
	}

	return 0;
}

static int gdma_release(u32 base)
{
	_stop(base,gdma_channel);

	req_cfg[gdma_channel].is_busy = 0;
	req_cfg[gdma_channel].ev_num = 0;

	return 0;
}

static int gdma_transfer(u32 base, u32 dst, u32 src, u32 len)
{
	gdma_move(base,dst,src,len);
	gdma_sample_complete(base,1 << gdma_channel);
	gdma_release(base);

	return 0;
}

static int sfax8_init_gdma(void)
{
	int index;

	for(index=0;index<GDMA_CHANNEL_MAX;index++)
	{
		req_cfg[index].is_busy = 0;
	}

	writel(0x1, (void *)REMAP_N);

	return 0;
}
static int sfax8_transfer(struct udevice *dev, int direction, void *dst,
		void *src, size_t len)
{
	struct sfax8_priv *priv = dev_get_priv(dev);

	gdma_channel = gdma_request();

	if(gdma_channel < 0)
	{
		return -EBUSY;
	}

	switch (direction) {
		case DMA_MEM_TO_MEM:
			req_cfg[gdma_channel].periph = SFDMA_MEM_MEM;
			req_cfg[gdma_channel].direct = DMA_MEM_TO_MEM;
			req_cfg[gdma_channel].src_inc = 1;
			req_cfg[gdma_channel].dst_inc = 1;
			break;
		default:
			error("Transfer type not implemented in DMA driver\n");
			break;
	}

	gdma_transfer(priv->base, (u32)dst, (u32)src, len);

	return 0;
}

static int sfax8_ofdata_to_platdata(struct udevice *dev)
{
	struct sfax8_priv *priv = dev_get_priv(dev);

	priv->base = dev_get_addr(dev);

	return 0;
}

static int sfax8_probe(struct udevice *dev)
{
	struct dma_dev_priv *uc_priv = dev_get_uclass_priv(dev);

	sfax8_init_gdma();

	uc_priv->supported = DMA_SUPPORTS_MEM_TO_MEM | DMA_SUPPORTS_MEM_TO_DEV
		| DMA_SUPPORTS_DEV_TO_MEM;

	return 0;
}

static const struct dma_ops sfax8_ops = {
	.transfer	= sfax8_transfer,
};

static const struct udevice_id sfax8_ids[] = {
	{ .compatible = "gdma" },
	{ }
};

U_BOOT_DRIVER(sfax8) = {
	.name	= "sfax8_gdma",
	.id	= UCLASS_DMA,
	.of_match = sfax8_ids,
	.ops	= &sfax8_ops,
	.ofdata_to_platdata = sfax8_ofdata_to_platdata,
	.probe	= sfax8_probe,
	.priv_auto_alloc_size = sizeof(struct sfax8_priv),
};
