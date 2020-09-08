#ifndef SF_ETH_ADDR
#define SF_ETH_ADDR

/********************sys ctl address ************************/
#define SIFLOWER_SYSCTL_BASE              0xB9E00000

#define SIFLOWER_NPU_HIF_IRQ             16
#define SIFLOWER_NPU_BMU_IRQ             24

#define SIFLOWER_NPU_PHY_IRQ_BASE        33
//NPU
#define NPU_SYSM_RESET                   (SIFLOWER_SYSCTL_BASE + 0x2C44)
#define NPU_SOFT_RESET                   (SIFLOWER_SYSCTL_BASE + 0x4000)
#define NPU_SOFT_CLK_EN                  (SIFLOWER_SYSCTL_BASE + 0x4004)
#define NPU_SOFT_BOE                     (SIFLOWER_SYSCTL_BASE + 0x400C)
#define NPU_HW_RST_PGM_EN                (SIFLOWER_SYSCTL_BASE + 0x40CC)
//LED
#define EMAC1_LED_OFF_COUNT_LOW          (SIFLOWER_SYSCTL_BASE + 0x41b0)
#define EMAC2_LED_OFF_COUNT_LOW          (SIFLOWER_SYSCTL_BASE + 0x41c0)
#define EMAC3_LED_OFF_COUNT_LOW          (SIFLOWER_SYSCTL_BASE + 0x41d0)
#define EMAC4_LED_OFF_COUNT_LOW          (SIFLOWER_SYSCTL_BASE + 0x41e0)
#define EMAC5_LED_OFF_COUNT_LOW          (SIFLOWER_SYSCTL_BASE + 0x41f0)


/********************Npu address ************************/
#define NPU_ADDR_BASE                     0xB0000000

/***********************BMU CSR*************************/
#define BMU_BASE_OFFSET                          0x240000
#define REG_BMU_CTRL                             BMU_BASE_OFFSET  + 0x04
#define REG_BMU_UCAST_CONFIG                     BMU_BASE_OFFSET  + 0x08
#define REG_BMU_UCAST_BASEADDR                   BMU_BASE_OFFSET  + 0x0c
#define REG_BMU_BUF_SIZE                         BMU_BASE_OFFSET  + 0x10
#define REG_BMU_BUF_CNT							 BMU_BASE_OFFSET  + 0x14
#define REG_BMU_REM_BUF_CNT						 BMU_BASE_OFFSET  + 0x48
#define REG_BMU_THRES                            BMU_BASE_OFFSET  + 0x18
#define REG_BMU_INT_SRC                          BMU_BASE_OFFSET  + 0x20
#define REG_BMU_INT_ENABLE                       BMU_BASE_OFFSET  + 0x24
#define REG_BMU_ALLOC_CTRL                       BMU_BASE_OFFSET  + 0x30
#define REG_BMU_LOW_WATERMARK					 BMU_BASE_OFFSET  + 0x50
#define REG_BMU_HIGH_WATERMARK					 BMU_BASE_OFFSET  + 0x54

/***********************EMAC CSR*************************/
#define EMAC0_BASE_OFFSET				            0x200000
#define EMAC1_BASE_OFFSET							0x220000
#define EMAC2_BASE_OFFSET							0x680000
#define EMAC3_BASE_OFFSET							0x690000
#define EMAC4_BASE_OFFSET							0x6a0000

#define REG_EMAC_CFG(X)                          EMAC##X##_BASE_OFFSET + 0x0000
#define REG_EMAC_FILTER(X)                       EMAC##X##_BASE_OFFSET + 0x0004
#define REG_EMAC_GMII_ADDRESS(X)                 EMAC##X##_BASE_OFFSET + 0x0010
#define REG_EMAC_GMII_DATA(X)                    EMAC##X##_BASE_OFFSET + 0x0014
#define REG_EMAC_FLOWCTL(X)                      EMAC##X##_BASE_OFFSET + 0x0018
#define REG_EMAC_ADDRESS0_16_H(X)                EMAC##X##_BASE_OFFSET + 0x0040
#define REG_EMAC_ADDRESS0_32_L(X)                EMAC##X##_BASE_OFFSET + 0x0044
#define REG_EMAC_ADDRESS1_16_H(X)                EMAC##X##_BASE_OFFSET + 0x0048
#define REG_EMAC_ADDRESS1_32_L(X)                EMAC##X##_BASE_OFFSET + 0x004c
#define REG_EMAC_VLAN_CONFIG(X)                  EMAC##X##_BASE_OFFSET + 0x0584
//emac_control_reg to control emac signals
#define REG_EMAC_CFG_GLOBAL(X)					 EMAC##X##_BASE_OFFSET + 0x0B80

#define REG_EMAC_CONTROL							0xB80
#define REG_EMAC_TIMESTAMP_CONTROL					0x700
#define REG_EMAC_FLOW_CONTROL						0x0018
#define REG_MAC_ADDRESS_HIGH(X)					EMAC##X##_BASE_OFFSET + 0x0040
#define REG_MAC_ADDRESS_LOW(X)					EMAC##X##_BASE_OFFSET + 0x0044

#define CONFIG_EMAC_SECTION(X) \
	{ REG_EMAC_CFG(X),				0x0210cc80}, \
	{ REG_EMAC_FILTER(X),			0x80000040}, \
	{ REG_EMAC_FLOWCTL(X),			0xFFFF0006}, \
	{ REG_EMAC_CFG_GLOBAL(X),		0x0CC20000},
	 // REG_EMAC_VLAN_CONFIG(X),      0x50000},

/***********************Classifier CSR*************************/
#define CLASS_BASE_OFFSET							0x320000

#define REG_CLASS_TX_CTRL                        CLASS_BASE_OFFSET  + 0x04
#define REG_CLASS_HDR_SIZE                       CLASS_BASE_OFFSET  + 0x14
#define REG_CLASS_TM_INQ_ADDR					 CLASS_BASE_OFFSET  + 0x114
#define REG_CLASS_BMU_BUF_FREE                   CLASS_BASE_OFFSET  + 0x24c
#define REG_CLASS_USE_TMU_INQ                    CLASS_BASE_OFFSET    + 0x250
#define REG_CLASS_SNOOP_SPL_MCAST_ADDR1_LSB      (CLASS_BASE_OFFSET  + 0x368)
#define REG_CLASS_SNOOP_SPL_MCAST_ADDR1_MSB      (CLASS_BASE_OFFSET  + 0x36c)
#define REG_CLASS_SNOOP_SPL_MCAST_ADDR2_LSB      (CLASS_BASE_OFFSET  + 0x370)
#define REG_CLASS_SNOOP_SPL_MCAST_ADDR2_MSB      (CLASS_BASE_OFFSET  + 0x374)
#define REG_CLASS_SNOOP_SPL_MCAST_MASK1_LSB      (CLASS_BASE_OFFSET  + 0x378)
#define REG_CLASS_SNOOP_SPL_MCAST_MASK1_MSB      (CLASS_BASE_OFFSET  + 0x37C)
#define REG_CLASS_SNOOP_SPL_MCAST_MASK2_LSB      (CLASS_BASE_OFFSET  + 0x380)
#define REG_CLASS_SNOOP_SPL_MCAST_MASK2_MSB      (CLASS_BASE_OFFSET  + 0x384)
#define REG_CLASS_GLOBAL_CFG                     CLASS_BASE_OFFSET  + 0x4ac

#define REG_CLASS_SNOOP_CTRL					 (CLASS_BASE_OFFSET  + 0x90)
#define REG_CLASS_SNOOP_SPL_ETYPE_REG01          (CLASS_BASE_OFFSET  + 0x88)
#define REG_CLASS_SNOOP_SPL_ETYPE_REG23          (CLASS_BASE_OFFSET  + 0x8c)

#define REG_CLASS_QOS_PORT0_PCP2TC_MAP           (CLASS_BASE_OFFSET  + 0x44)
#define REG_CLASS_QOS_PORT1_PCP2TC_MAP           (CLASS_BASE_OFFSET  + 0x48)
#define REG_CLASS_QOS_PORT2_PCP2TC_MAP           (CLASS_BASE_OFFSET  + 0x4c)
#define REG_CLASS_QOS_PORT3_PCP2TC_MAP           (CLASS_BASE_OFFSET  + 0x50)
#define REG_CLASS_QOS_PORT4_PCP2TC_MAP           (CLASS_BASE_OFFSET  + 0x54)
#define REG_CLASS_QOS_PORT5_PCP2TC_MAP           (CLASS_BASE_OFFSET  + 0x58)
#define REG_CLASS_QOS_PORT_PID2TC_MAP            (CLASS_BASE_OFFSET  + 0x64)

#define REG_CLASS_QOS_PORT01_TC_SEL              (CLASS_BASE_OFFSET  + 0x18)
#define REG_CLASS_QOS_PORT23_TC_SEL              (CLASS_BASE_OFFSET  + 0x1c)
#define REG_CLASS_QOS_PORT45_TC_SEL              (CLASS_BASE_OFFSET  + 0x20)

//DOS
#define REG_CLASS_DOS_CONTRL1                    (CLASS_BASE_OFFSET  + 0x280)
#define REG_CLASS_DOS_CONTRL2                    (CLASS_BASE_OFFSET  + 0x284)
#define REG_CLASS_DOS_TCP_FLAGCHK_COMB_VALUE1    (CLASS_BASE_OFFSET  + 0x288)
#define REG_CLASS_DOS_ICMPV4_MAX_PKTLEN          (CLASS_BASE_OFFSET  + 0x28c)

#define REG_CLASS_DAMACHASH_HOST_CMD_REG         CLASS_BASE_OFFSET  + 0x390
#define REG_CLASS_DAVLANHASH_HOST_CMD_REG        CLASS_BASE_OFFSET  + 0x3e4
#define REG_CLASS_DAMACHASH_FREELIST_ENTRIES     CLASS_BASE_OFFSET  + 0x3d8
#define REG_CLASS_DAMACHASH_FREELIST_HEAD_PTR    CLASS_BASE_OFFSET  + 0x3dc
#define REG_CLASS_DAMACHASH_FREELIST_TAIL_PTR    CLASS_BASE_OFFSET  + 0x3e0
#define REG_CLASS_DAVLANHASH_FREELIST_ENTRIES    CLASS_BASE_OFFSET  + 0x42c
#define REG_CLASS_DAVLANHASH_FREELIST_HEAD_PTR   CLASS_BASE_OFFSET  + 0x430
#define REG_CLASS_DAVLANHASH_FREELIST_TAIL_PTR   CLASS_BASE_OFFSET  + 0x434

#define REG_CLASS_HW_PORT0_STRUC1      (CLASS_BASE_OFFSET + 0x0000046c)
#define REG_CLASS_HW_PORT0_STRUC2      (CLASS_BASE_OFFSET + 0x00000470)
#define REG_CLASS_HW_PORT1_STRUC1      (CLASS_BASE_OFFSET + 0x00000474)
#define REG_CLASS_HW_PORT1_STRUC2      (CLASS_BASE_OFFSET + 0x00000478)
#define REG_CLASS_HW_PORT2_STRUC1      (CLASS_BASE_OFFSET + 0x0000047c)
#define REG_CLASS_HW_PORT2_STRUC2      (CLASS_BASE_OFFSET + 0x00000480)
#define REG_CLASS_HW_PORT3_STRUC1      (CLASS_BASE_OFFSET + 0x00000484)
#define REG_CLASS_HW_PORT3_STRUC2      (CLASS_BASE_OFFSET + 0x00000488)
#define REG_CLASS_HW_PORT4_STRUC1      (CLASS_BASE_OFFSET + 0x0000048c)
#define REG_CLASS_HW_PORT4_STRUC2      (CLASS_BASE_OFFSET + 0x00000490)
#define REG_CLASS_HW_PORT5_STRUC1      (CLASS_BASE_OFFSET + 0x00000494)
#define REG_CLASS_HW_PORT5_STRUC2      (CLASS_BASE_OFFSET + 0x00000498)
#define REG_CLASS_HW_PORT6_STRUC1      (CLASS_BASE_OFFSET + 0x0000049c)
#define REG_CLASS_HW_PORT6_STRUC2      (CLASS_BASE_OFFSET + 0x000004a0)
#define REG_CLASS_HW_PORT7_STRUC1      (CLASS_BASE_OFFSET + 0x000004a4)
#define REG_CLASS_HW_PORT7_STRUC2			(CLASS_BASE_OFFSET + 0x000004a8)
#define REG_CLASS_HW_GLOBAL_CUTTHRU_REG		(CLASS_BASE_OFFSET + 0x000004b0)
// TX CHECKSUM UPDATE
#define REG_CLASS_HW_GLOBAL_CUTTHRU_REG		(CLASS_BASE_OFFSET + 0x000004b0)
#define REG_CLASS_TCP_OFFLOAD_PKTLEN_THRESH	(CLASS_BASE_OFFSET + 0x000004f8)
#define REG_CLASS_PORT_TCP_CHKSUM_OFFLOAD	(CLASS_BASE_OFFSET + 0x000004c4)

#define REG_CLASS_GLOBAL_MGMT_REG			(CLASS_BASE_OFFSET + 0x00000200)
#define REG_CLASS_PPPOE_CFG					(CLASS_BASE_OFFSET + 0x00000524)

/***********************EGPI CSR*************************/
#define EGPI0_BASE_OFFSET							0x210000
#define EGPI1_BASE_OFFSET							0x230000
#define EGPI2_BASE_OFFSET							0x6B0000
#define EGPI3_BASE_OFFSET							0x6C0000
#define EGPI4_BASE_OFFSET							0x620000

#define REG_GPI_INQ_PKTPTR0						0x210030
#define REG_GPI_INQ_PKTPTR1						0x230030
#define REG_GPI_INQ_PKTPTR2						0x6b0030
#define REG_GPI_INQ_PKTPTR3						0x6c0030
#define REG_GPI_INQ_PKTPTR4						0x620030
#define REG_GPI_INQ_PKTPTR5						0x340030

#define  REG_EGPI_PORT_SHP_CONFIG				0x128
#define  REG_EGPI_PORT_SHP0_CTRL				0x098
#define  REG_EGPI_PORT_SHP0_WGHT				0x09c
#define  REG_EGPI_PORT_SHP0_MIN_CREDOT			0x140
#define  REG_EGPI_PORT_SHP1_CTRL				0x11c
#define  REG_EGPI_PORT_SHP1_WGHT				0x120
#define  REG_EGPI_PORT_SHP1_MIN_CREDOT			0x144

#define  REG_GPI_BMU1_PHY_LOW_WATERMARK_OFFSET                0x104
#define  REG_GPI_BMU1_PHY_HIGH_WATERMARK_OFFSET               0x108
#define  REG_GPI_FW_CONTROL_OFFSET							  0x114

#define  REG_GPI_CTRL(X)                        EGPI##X##_BASE_OFFSET + 0x04
#define  REG_GPI_RX_CONFIG(X)                   EGPI##X##_BASE_OFFSET + 0x08
#define  REG_GPI_HDR_SIZE(X)                    EGPI##X##_BASE_OFFSET + 0x0c
#define  REG_GPI_BUF_SIZE(X)                    EGPI##X##_BASE_OFFSET + 0x10
#define  REG_GPI_LMEM_ALLOC_ADDR(X)             EGPI##X##_BASE_OFFSET + 0x14
#define  REG_GPI_LMEM_FREE_ADDR(X)              EGPI##X##_BASE_OFFSET + 0x18
#define  REG_GPI_CLASS_ADDR(X)                  EGPI##X##_BASE_OFFSET + 0x24
#define  REG_GPI_LMEM_SEC_BUF_DATA_ADDR(X)		EGPI##X##_BASE_OFFSET + 0x60
#define  REG_GPI_TMLF_TX(X)                     EGPI##X##_BASE_OFFSET + 0x4c
#define  REG_GPI_DTX_ASEQ(X)                    EGPI##X##_BASE_OFFSET + 0x50

//TODO: for chksum update
#define  REG_GPI_CSR_TOE_CHKSUM_EN(X)           EGPI##X##_BASE_OFFSET + 0x68

#define CONFIG_EGPI_SECTION(X) \
    { REG_GPI_RX_CONFIG(X),			0x02000001}, /*bit 0 – LMEM buffer enable ;bit 31:16 – Retry count for LMEM buffers*/\
    { REG_GPI_HDR_SIZE(X),			0x00000030}, /*bit 7:0 – LMEM first buffer header size value*/\
    { REG_GPI_BUF_SIZE(X),			0x00000080}, /*bit 15:0 – LMEM buffer size value as 128 bytes*/\
    { REG_GPI_LMEM_ALLOC_ADDR(X),	0x00240030}, /*Address of BMU1, where buffer should be allocated.*/\
    { REG_GPI_LMEM_FREE_ADDR(X),	0x00240034}, /*Address of BMU1, where buffer should be freed*/\
    { REG_GPI_CLASS_ADDR(X),		0x00320010}, /*Program address of the Class HW register where packet from peripherals are sent to*/\
    { REG_GPI_LMEM_SEC_BUF_DATA_ADDR(X), 0x00000010}, /*bit 15:0 - LMEM header size from second buffer onwards for each buffer in chain*/\
    { REG_GPI_TMLF_TX(X),			0x000000bc},/*bit 7:0 - Threshold number of TMLF words - 64bit size, to be in the TMLF FIFO before transmission starts*/\
    { REG_GPI_DTX_ASEQ(X),			0x00000050},/*Initial number of bytes read from received pointer in LMEM, to check for action fields*/ \
    { REG_GPI_CSR_TOE_CHKSUM_EN(X),	0x00000001},/* for checksum update*/


/***********************ETGPI CSR*************************/
#ifdef CONFIG_NET_SFAX8_PTP
#define ETGPI0_BASE_OFFSET							0x630000
#define ETGPI1_BASE_OFFSET							0x640000
#define ETGPI2_BASE_OFFSET							0x650000
#define ETGPI3_BASE_OFFSET							0x660000
#define ETGPI4_BASE_OFFSET							0x670000

#define  REG_TGPI_RX_CONFIG(X)                   ETGPI##X##_BASE_OFFSET + 0x08
#define  REG_TGPI_HDR_SIZE(X)                    ETGPI##X##_BASE_OFFSET + 0x0c
#define  REG_TGPI_BUF_SIZE(X)                    ETGPI##X##_BASE_OFFSET + 0x10
#define  REG_TGPI_LMEM_ALLOC_ADDR(X)             ETGPI##X##_BASE_OFFSET + 0x14
#define  REG_TGPI_LMEM_FREE_ADDR(X)              ETGPI##X##_BASE_OFFSET + 0x18
#define  REG_TGPI_CLASS_ADDR(X)                  ETGPI##X##_BASE_OFFSET + 0x24
#define  REG_TGPI_LMEM_SEC_BUF_DATA_ADDR(X)		 ETGPI##X##_BASE_OFFSET + 0x60
#define  REG_TGPI_TMLF_TX(X)                     ETGPI##X##_BASE_OFFSET + 0x4c
#define  REG_TGPI_DTX_ASEQ(X)                    ETGPI##X##_BASE_OFFSET + 0x50

#define CONFIG_ETGPI_SECTION(X) \
    { REG_TGPI_RX_CONFIG(X),     0x02000001}, /*bit 0 – LMEM buffer enable ;bit 31:16 – Retry count for LMEM buffers*/\
    { REG_TGPI_HDR_SIZE(X),      0x00000030}, /*bit 7:0 – LMEM first buffer header size value*/\
    { REG_TGPI_BUF_SIZE(X),      0x00000080}, /*bit 15:0 – LMEM buffer size value as 128 bytes*/\
    { REG_TGPI_LMEM_ALLOC_ADDR(X),  0x00240030}, /*Address of BMU1, where buffer should be allocated.*/\
    { REG_TGPI_LMEM_FREE_ADDR(X),   0x00240034}, /*Address of BMU1, where buffer should be freed*/\
    { REG_TGPI_CLASS_ADDR(X),       0x00320010}, /*Program address of the Class HW register where packet from peripherals are sent to*/\
    { REG_TGPI_LMEM_SEC_BUF_DATA_ADDR(X), 0x00000010}, /*bit 15:0 - LMEM header size from second buffer onwards for each buffer in chain*/\
    { REG_TGPI_TMLF_TX(X),       0x000000bc},/*bit 7:0 - Threshold number of TMLF words - 64bit size, to be in the TMLF FIFO before transmission starts*/\
    { REG_TGPI_DTX_ASEQ(X),      0x00000050},/*Initial number of bytes read from received pointer in LMEM, to check for action fields*/
#endif

/***********************HGPI CSR*************************/
#define HGPI_BASE_OFFSET							0x340000

#define REG_HGPI_CTRL                            HGPI_BASE_OFFSET + 0x04
#define REG_HGPI_RX_CONFIG                       HGPI_BASE_OFFSET + 0x08
#define REG_HGPI_HDR_SIZE                        HGPI_BASE_OFFSET + 0x0c
#define REG_HGPI_BUF_SIZE                        HGPI_BASE_OFFSET + 0x10
#define REG_HGPI_LMEM_ALLOC_ADDR			 	 HGPI_BASE_OFFSET + 0x14
#define REG_HGPI_LMEM_FREE_ADDR					 HGPI_BASE_OFFSET + 0x18
#define REG_HGPI_CLASS_ADDR						 HGPI_BASE_OFFSET + 0x24
#define REG_HGPI_INQ_PKTPTR                      HGPI_BASE_OFFSET + 0x30
#define REG_HGPI_LMEM_SEC_BUF_DATA_ADDR			 HGPI_BASE_OFFSET + 0x60
#define REG_HGPI_TMLF_TX                         HGPI_BASE_OFFSET + 0x4c
#define REG_HGPI_DTX_ASEQ                        HGPI_BASE_OFFSET + 0x50
#define REG_HGPI_PORT_SHP0_CTRL                  HGPI_BASE_OFFSET + 0x98
#define REG_HGPI_PORT_SHP0_WGHT                  HGPI_BASE_OFFSET + 0x9c
#define REG_HGPI_PORT_SHP1_CTRL                  HGPI_BASE_OFFSET + 0x11c
#define REG_HGPI_PORT_SHP1_WGHT                  HGPI_BASE_OFFSET + 0x120
#define REG_HGPI_PORT_SHP_CONFIG                 HGPI_BASE_OFFSET + 0x128

#define CONFIG_HGPI_SECTION() \
    { REG_HGPI_RX_CONFIG,     0x02000001}, /*bit 0 – LMEM buffer enable ;bit 31:16 – Retry count for LMEM buffers*/\
    { REG_HGPI_HDR_SIZE,      0x00000030}, /*bit 7:0 – LMEM first buffer header size value*/\
    { REG_HGPI_BUF_SIZE,      0x00000080}, /*bit 15:0 – LMEM buffer size value as 128 bytes*/\
    { REG_HGPI_LMEM_ALLOC_ADDR,  0x00240030}, /*Address of BMU1, where buffer should be allocated.*/\
    { REG_HGPI_LMEM_FREE_ADDR,   0x00240034}, /*Address of BMU1, where buffer should be freed*/\
    { REG_HGPI_CLASS_ADDR,       0x00320010}, /*Program address of the Class HW register where packet from peripherals are sent to*/\
    { REG_HGPI_LMEM_SEC_BUF_DATA_ADDR, 0x00000010}, /*bit 15:0 - LMEM header size from second buffer onwards for each buffer in chain*/\
    { REG_HGPI_TMLF_TX,       0x000000bc},/*bit 7:0 - Threshold number of TMLF words - 64bit size, to be in the TMLF FIFO before transmission starts*/\
    { REG_HGPI_DTX_ASEQ,      0x00000040},/*Initial number of bytes read from received pointer in LMEM, to check for action fields*/

/***********************TMU CSR*************************/
#define TMU_BASE_OFFSET							0x310000
#define REG_TMU_PHY0_INQ_ADDR					TMU_BASE_OFFSET  + 0x200
#define REG_TMU_PHY1_INQ_ADDR					TMU_BASE_OFFSET  + 0x204
#define REG_TMU_PHY2_INQ_ADDR					TMU_BASE_OFFSET  + 0x208
#define REG_TMU_PHY3_INQ_ADDR					TMU_BASE_OFFSET  + 0x20c
#define REG_TMU_PHY4_INQ_ADDR					TMU_BASE_OFFSET  + 0x210
#define REG_TMU_PHY5_INQ_ADDR					TMU_BASE_OFFSET  + 0x214
#define REG_TMU_CNTX_ACCESS_CTRL                 TMU_BASE_OFFSET  + 0x280
#define REG_TMU_CNTX_ADDR		                (TMU_BASE_OFFSET  + 0x284)
#define REG_TMU_CNTX_DATA		                (TMU_BASE_OFFSET  + 0x288)
#define REG_TMU_CNTX_CMD		                (TMU_BASE_OFFSET  + 0x28c)
#define REG_TMU_PHY0_TDQ_CTRL                   (TMU_BASE_OFFSET  + 0x260)
#define REG_TMU_PHY1_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x264
#define REG_TMU_PHY2_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x268
#define REG_TMU_PHY3_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x26c
#define REG_TMU_PHY4_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x270
#define REG_TMU_PHY5_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x274
#define REG_TMU_BMU_INQ_ADDR					 TMU_BASE_OFFSET  + 0x100

#define TMU_TDQ_PHY0_CSR_BASE_ADDR               TMU_BASE_OFFSET + 0x1000
#define TMU_TDQ_PHY1_CSR_BASE_ADDR               TMU_BASE_OFFSET + 0x2000
#define TMU_TDQ_PHY2_CSR_BASE_ADDR               TMU_BASE_OFFSET + 0x3000
#define TMU_TDQ_PHY3_CSR_BASE_ADDR               TMU_BASE_OFFSET + 0x4000
#define TMU_TDQ_PHY4_CSR_BASE_ADDR               TMU_BASE_OFFSET + 0x5000

/* tmu_phy_sch_reg_add */
#define REG_TMU_PHY0_SCH0_CTRL					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x00)
#define REG_TMU_PHY0_SCH0_Q0_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x20)
#define REG_TMU_PHY0_SCH0_Q1_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x24)
#define REG_TMU_PHY0_SCH0_Q2_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x28)
#define REG_TMU_PHY0_SCH0_Q3_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x2c)
#define REG_TMU_PHY0_SCH0_Q4_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x30)
#define REG_TMU_PHY0_SCH0_Q5_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x34)
#define REG_TMU_PHY0_SCH0_Q6_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x38)
#define REG_TMU_PHY0_SCH0_Q7_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x3c)
#define REG_TMU_PHY0_SCH0_Q_ALLOC0				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x40)
#define REG_TMU_PHY0_SCH0_Q_ALLOC1				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x44)
#define REG_TMU_PHY0_SCH0_BIT_RATE				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x48)
#define REG_TMU_PHY0_SCH0_POS					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x54)

#define REG_TMU_PHY0_SCH1_CTRL					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x300)
#define REG_TMU_PHY0_SCH1_Q0_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x320)
#define REG_TMU_PHY0_SCH1_Q1_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x324)
#define REG_TMU_PHY0_SCH1_Q2_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x328)
#define REG_TMU_PHY0_SCH1_Q3_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x32c)
#define REG_TMU_PHY0_SCH1_Q4_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x330)
#define REG_TMU_PHY0_SCH1_Q5_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x334)
#define REG_TMU_PHY0_SCH1_Q6_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x338)
#define REG_TMU_PHY0_SCH1_Q7_WGHT				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x33c)
#define REG_TMU_PHY0_SCH1_Q_ALLOC0				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x340)
#define REG_TMU_PHY0_SCH1_Q_ALLOC1				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x344)
#define REG_TMU_PHY0_SCH1_BIT_RATE				(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x348)

/* tmu_phy_shp_reg_add  */
#define REG_TMU_PHY0_SHP0_CTRL                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x800)
#define REG_TMU_PHY0_SHP0_WGHT                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x804)
#define REG_TMU_PHY0_SHP0_MAX_CREDIT            (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x808)
#define REG_TMU_PHY0_SHP0_CTRL2					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x80c)
#define REG_TMU_PHY0_SHP0_MIN_CREDIT			(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x810)

#define REG_TMU_PHY0_SHP1_CTRL                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x900)
#define REG_TMU_PHY0_SHP1_WGHT                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x904)
#define REG_TMU_PHY0_SHP1_MAX_CREDIT            (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x908)
#define REG_TMU_PHY0_SHP1_CTRL2					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x90c)
#define REG_TMU_PHY0_SHP1_MIN_CREDIT			(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0x910)

#define REG_TMU_PHY0_SHP2_CTRL                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xa00)
#define REG_TMU_PHY0_SHP2_WGHT                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xa04)
#define REG_TMU_PHY0_SHP2_MAX_CREDIT            (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xa08)
#define REG_TMU_PHY0_SHP2_CTRL2					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xa0c)
#define REG_TMU_PHY0_SHP2_MIN_CREDIT			(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xa10)

#define REG_TMU_PHY0_SHP3_CTRL                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xb00)
#define REG_TMU_PHY0_SHP3_WGHT                  (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xb04)
#define REG_TMU_PHY0_SHP3_MAX_CREDIT            (TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xb08)
#define REG_TMU_PHY0_SHP3_CTRL2					(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xb0c)
#define REG_TMU_PHY0_SHP3_MIN_CREDIT			(TMU_TDQ_PHY0_CSR_BASE_ADDR + 0xb10)


#define REG_TMU_PHY_QUEUE_SEL_ADDR               TMU_BASE_OFFSET + 0x34
#define REG_TMU_CURQ_PTR_ADDR                    TMU_BASE_OFFSET + 0x38
#define REG_TMU_CURQ_PKT_CNT_ADDR                TMU_BASE_OFFSET + 0x3c
#define REG_TMU_CURQ_DROP_CNT_ADDR               TMU_BASE_OFFSET + 0x40
#define REG_TMU_CURQ_TRANS_CNT_ADDR              TMU_BASE_OFFSET + 0x44
#define REG_TMU_CURQ_QSTAT_ADDR                  TMU_BASE_OFFSET + 0x48
#define REG_TMU_HW_PROB_CFG_TBL0_ADDR            TMU_BASE_OFFSET + 0x4c
#define REG_TMU_HW_PROB_CFG_TBL1_ADDR            TMU_BASE_OFFSET + 0x50
#define REG_TMU_CURQ_DEBUG_ADDR                  TMU_BASE_OFFSET + 0x54


/***********************HIF CSR*************************/
#define HIF_BASE_OFFSET							0x330000

#define REG_HIF_TX_CTRL                          HIF_BASE_OFFSET  + 0x04
#define REG_HIF_TX_BDP_ADDR                      HIF_BASE_OFFSET  + 0x10
#define REG_HIF_TX_CURR_BD_ADDR					 HIF_BASE_OFFSET  + 0x08
#define REG_HIF_RX_CURR_BD_ADDR					 HIF_BASE_OFFSET  + 0x40

#define REG_HIF_TX_ALLOC                         HIF_BASE_OFFSET  + 0x0c
#define REG_HIF_RX_CTRL                          HIF_BASE_OFFSET  + 0x20
#define REG_HIF_RX_BDP_ADDR						 HIF_BASE_OFFSET  + 0x24
#define REG_HIF_INT_SRC                          HIF_BASE_OFFSET  + 0x34
#define REG_HIF_INT_EN                           HIF_BASE_OFFSET  + 0x38
#define REG_HIF_POLL_CTRL                        HIF_BASE_OFFSET  + 0x3c
#define REG_HIF_RX_ALLOC                         HIF_BASE_OFFSET  + 0x44
#define REG_HIF_INT_COAL                         HIF_BASE_OFFSET  + 0x50

/***********************WSP CSR*************************/
#define WSP_GLOBAL_BASE_OFFSET                     0x600000
#define REG_WSP_SYS_GENERIC_CONTROL              WSP_GLOBAL_BASE_OFFSET + 0x20

typedef struct hwinit_data {
   unsigned int offset;
   unsigned int data;
} hwinit_data_t;



#endif /* ifndef SF_ETH_ADDR*/
