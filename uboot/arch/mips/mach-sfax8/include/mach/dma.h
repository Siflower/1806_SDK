#ifndef __GDMA_H__
#define __GDMA_H__

#define GDMA_CHANNEL_MAX 7
#define GDMA_INSN_LEN_MAN 128
#define SECURE_VAL 1
#define PL330_MAX_LOOPS		256

// register offset setting
#define GDMA_DMS       0x000            // DMA Manager Status Register
#define GDMA_DPC       0x004            // DMA Program Counter Register
#define GDMA_IE        0x020            // Interrupt Enable Register
#define GDMA_EIRS      0x024            // Event-Interrupt Raw Status Register
#define GDMA_IS        0x028            // Interrupt Status Register
#define GDMA_IC        0x02c            // Interrupt Clear Register
#define GDMA_FSDM      0x030            // Fault Status DMA Manager Register
#define GDMA_FSDC      0x034            // Fault Status DMA Channel Register
#define GDMA_FTDM      0x038            // Fault Type DMA Manager Register
#define GDMA_FTDC(n)   (0x040+n*0x4)    // Fault Type DMA Channel Registers
#define GDMA_CS(n)     (0x100+n*0x8)    // Channel Status Registers
#define GDMA_CPC(n)    (0x104+n*0x4)    // Channel Program Counter Registers
#define GDMA_SAR(n)    (0x400+n*0x20)   // Source Address Registers
#define GDMA_DAR(n)    (0x404+n*0x20)   // Destination Address Registers
#define GDMA_CCR(n)    (0x408+n*0x20)   // Channel Control Registers
#define GDMA_LC0(n)    (0x40c+n*0x20)   // Loop Counter 0 Registers
#define GDMA_LC1(n)    (0x410+n*0x20)   // Loop Counter 1 Registers
#define GDMA_DS        0xd00            // Debug Status Register
#define GDMA_DC        0xd04            // Debug Command Register
#define GDMA_DI0       0xd08            // Debug Instruction-0 Register
#define GDMA_DI1       0xd0c            // Debug Instruction-1 Register

#define GDMA_CFG0      0xe00            // Configuration Register 0
#define GDMA_CFG1      0xe04            // Configuration Register 1
#define GDMA_CFG2      0xe08            // Configuration Register 2
#define GDMA_CFG3      0xe0c            // Configuration Register 3
#define GDMA_CFG4      0xe10            // Configuration Register 4
#define GDMA_DCFG      0xe14            // DMA Configuration Register
#define GDMA_WD        0xe80            // Watchdog Register

#define GDMA_PIR       0xfe0            // Peripheral Identification Registers
#define GDMA_CIR       0xff0            // Component Identification Registers

//gdma system register
#define GDMA_SR        0x00
#define GDMA_CGE       0x04
#define GDMA_PCR       0x08
#define GDMA_GM        0x0c
#define GDMA_SC        0x20


/* Register and Bit field Definitions */
#define DS_ST_STOP		0x0
#define DS_ST_EXEC		0x1
#define DS_ST_CMISS		0x2
#define DS_ST_UPDTPC	0x3
#define DS_ST_WFE		0x4
#define DS_ST_ATBRR		0x5
#define DS_ST_QBUSY		0x6
#define DS_ST_WFP		0x7
#define DS_ST_KILL		0x8
#define DS_ST_CMPLT		0x9
#define DS_ST_FLTCMP	0xe
#define DS_ST_FAULT		0xf


// GDMA_CCR: register settin
#define CCR_SRCINC                    (1 << 0)
#define CCR_DSTINC                    (1 << 14)
#define CCR_SRCPRI                    (1 << 8)
#define CCR_DSTPRI                    (1 << 22)
#define CCR_SRCNS                     (1 << 9)
#define CCR_DSTNS                     (1 << 23)
#define CCR_SRCIA                     (1 << 10)
#define CCR_DSTIA                     (1 << 24)

#define CCR_SRCBRSTLEN_SHFT           4
#define CCR_DSTBRSTLEN_SHFT           18
#define CCR_SRCBRSTSIZE_SHFT          1
#define CCR_DSTBRSTSIZE_SHFT          15
#define CCR_SRCCCTRL_SHFT             11
#define CCR_SRCCCTRL_MASK             0x7
#define CCR_DSTCCTRL_SHFT             25
#define CCR_DRCCCTRL_MASK             0x7
#define CCR_SWAP_SHFT                 28

/* PL330_DBGSTATUS */
#define DBG_BUSY		(1 << 0)


/* instruction set opcode */
#define DMAADDH			(0x54)
#define DMAEND			(0x00)
#define DMAFLUSHHP		(0x35)
#define DMAGO			(0xa0)
#define DMALD			(0x04)
#define DMALDS			(0x05)
#define DMALDB			(0x07)
#define DMALDPS			(0x25)
#define DMALDPB			(0x27)
#define DMALP			(0x20)
#define DMALPEND		(0x38)
#define DMALPENDS		(0x39)
#define DMALPENDB		(0x3b)
#define DMALPFE			(0x28)
#define DMAKILL			(0x01)
#define DMAMOV			(0xbc)
#define DMANOP			(0xbc)
#define DMARMB			(0x12)
#define DMASEV			(0x34)
#define DMAST			(0x08)
#define DMASTS			(0x09)
#define DMASTB			(0x0b)
#define DMASTPS			(0x29)
#define DMASTPB			(0x2b)
#define DMASTZ			(0x0c)
#define DMAWFE			(0x36)
#define DMAWFPS			(0x30)
#define DMAWFPB			(0x32)
#define DMAWFPP			(0x31)
#define DMAWMB			(0x13)

/* ra DMAADDH */
#define RA_SA			0
#define RA_DA			1

/* ns DMAGO */
#define NS_SECURE		0
#define NS_NONSECURE	1

/* lc DMALP* */
#define LC_0			0
#define LC_1			1

/* rd DMAMOV */
#define RD_SAR			0
#define RD_CCR			1
#define RD_DAR			2

/* invalid DMAWFE */
#define INVALID_OFF		0
#define INVALID_ON		1



#define BRST_LEN(ccr)   ((((ccr) >> CCR_SRCBRSTLEN_SHFT) & 0xf) + 1)
#define BRST_SIZE(ccr)  (1 << (((ccr) >> CCR_SRCBRSTSIZE_SHFT) & 0x7))
#define BYTE_TO_LOOP_COUNT(b, ccr)  ((b) / BRST_SIZE(ccr) / BRST_LEN(ccr))


#define PL330_STATE_STOPPED		          (1 << 0)
#define PL330_STATE_EXECUTING		      (1 << 1)
#define PL330_STATE_WFE			          (1 << 2)
#define PL330_STATE_FAULTING		      (1 << 3)
#define PL330_STATE_COMPLETING	          (1 << 4)
#define PL330_STATE_WFP			          (1 << 5)
#define PL330_STATE_KILLING               (1 << 6)
#define PL330_STATE_FAULT_COMPLETING      (1 << 7)
#define PL330_STATE_CACHEMISS             (1 << 8)
#define PL330_STATE_UPDTPC                (1 << 9)
#define PL330_STATE_ATBARRIER		      (1 << 10)
#define PL330_STATE_QUEUEBUSY		      (1 << 11)
#define PL330_STATE_INVALID               (1 << 15)


/* struct for PL330_CC Register */
struct pl330_register_cc {
	unsigned int src_inc:1;
	unsigned int src_burst_size:3;
	unsigned int src_burst_len:4;
	unsigned int src_prot_ctrl:3;
	unsigned int src_cache_ctrl:3;
	unsigned int dst_inc:1;
	unsigned int dst_burst_size:3;
	unsigned int dst_burst_len:4;
	unsigned int dst_prot_ctrl:3;
	unsigned int dst_cache_ctrl:3;
	unsigned int endian_swqp_size:4;
};


/* dma transfer mode*/
typedef enum  dma_tersfer_mode_t {
	DMA_NORMAL_MODE,
	DMA_LLI_MODE,
}dma_tersfer_mode;

/*
 * PL330 can assign any channel to communicate with
 * any of the peripherals attched to the DMAC.
 * For the sake of consistency across client drivers,
 * We keep the channel names unchanged and only add
 * missing peripherals are added.
 * Order is not important since SFDMA PL330 API driver
 * use these just as IDs.
 */
typedef enum requst_dev_type_t {
	SFDMA_I2S_MASTER_TX = 0,
	SFDMA_I2S_MASTER_RX,
	SFDMA_I2S_SLAVE_TX,
	SFDMA_I2S_SLAVE_RX,
	SFDMA_SPDIF_TX = 8,
	SFDMA_SPDIF_RX,
	SFDMA_SSP0_TX,
	SFDMA_SSP0_RX,
	SFDMA_SSP1_TX,
	SFDMA_SSP1_RX,
	SFDMA_UART1_TX,
	SFDMA_UART1_RX,
	SFDMA_UART2_TX,
	SFDMA_UART2_RX,
	SFDMA_UART3_TX,
	SFDMA_UART3_RX,
	SFDMA_PCM0_TX,
	SFDMA_PCM0_RX,
	SFDMA_PCM1_TX,
	SFDMA_PCM1_RX,
	SFDMA_MEM_MEM,
	SFDMA_REQUEST_MAX,
} dma_requst_type;


typedef enum srccachectrl_t {
    SCCTRL0 = 0, /*  Noncacheable and nonbufferable */
    SCCTRL1, /*  Bufferable only */
    SCCTRL2, /*  Cacheable, but do not allocate */
    SCCTRL3, /*  Cacheable and bufferable, but do not allocate */
    SINVALID1,
    SINVALID2,
    SCCTRL6, /*  Cacheable write-through, allocate on reads only */
    SCCTRL7, /*  Cacheable write-back, allocate on reads only */
}srccachectrl;

typedef enum dstcachectrl_t {
    DCCTRL0 = 0, /*  Noncacheable and nonbufferable */
    DCCTRL1, /*  Bufferable only */
    DCCTRL2, /*  Cacheable, but do not allocate */
    DCCTRL3, /*  Cacheable and bufferable, but do not allocate */
    DINVALID1 = 8,
    DINVALID2,
    DCCTRL6, /*  Cacheable write-through, allocate on writes only */
    DCCTRL7, /*  Cacheable write-back, allocate on writes only */
} dstcachectrl;

typedef enum byteswap_t {
    SWAP_NO = 0,
    SWAP_2,
    SWAP_4,
    SWAP_8,
    SWAP_16,
}byteswap;

/*
Endian swap size
Table 3-22 defines whether data can be swapped between little-endian (LE) and
byte-invariant big-endian (BE-8) formats, and if so, also defines the natural width of the
data independently of the source and destination transaction sizes.

[21:18] dst_burst_len For each burst, these bits program the number of data transfers that the DMAC performs
when it writes the destination data:
b0000 = 1 data transfer
b0001 = 2 data transfers
b0010 = 3 data transfers
.
.
.
b1111 = 16 data transfers.

dst_burst_size Programs the burst size that the DMAC uses when it writes the destination data:
b000 = 1 byte
b001 = 2 bytes
b010 = 4 bytes
b011 = 8 bytes
b100 = 16 bytes
b101 = 32 bytes
b110 = 64 bytes
b111 = 128 bytes.

[14] dst_inc Programs the burst type that the DMAC performs when it writes the destination data:
0 = fixed-address burst. The DMAC signals AWBURST[0] LOW.
1 = incrementing-address burst. The DMAC signals AWBURST[0] HIGH.
*/

typedef struct gdma_reqcfg_t {
    u32 port_num;

	u32 src;
	u32 ccr;
	u32 dst;

	u32 tran_len;//bytes

	int direct;
	dma_requst_type periph;

    /*  Address Incrementing */
	u32 src_inc;
    u32 dst_inc;


    /*
     ** For now, the SRC & DST protection levels
     ** and burst size/length are assumed same.
     */
    u32 nonsecure;
    u32 privileged;
    u32 insnaccess;
    u32 brst_len;
    u32 brst_size; /*  in power of 2 */

	u32 is_busy;
	u32 ev_num;

    dstcachectrl dcctl;
    srccachectrl scctl;
    byteswap swap;

} gdma_reqcfg,gdma_reqcfg_p;


#define BRST_LEN(ccr)   ((((ccr) >> CCR_SRCBRSTLEN_SHFT) & 0xf) + 1)
#define BRST_SIZE(ccr)  (1 << (((ccr) >> CCR_SRCBRSTSIZE_SHFT) & 0x7))
#define BYTE_TO_BURST(b, ccr)  ((b) / BRST_SIZE(ccr) / BRST_LEN(ccr))

#endif
