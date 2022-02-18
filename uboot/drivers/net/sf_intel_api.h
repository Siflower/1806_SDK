#ifndef _SF_INTEL_API_H_
#define _SF_INTEL_API_H_

//intel gsw150 regs
#define SMDIO_WRADDR    (0x1F)
#define SMDIO_RDADDR    (0x0)
/* Bit: 'MBUSY' */
/* Description: 'MDIO Busy' */
#define MMDIO_CTRL_MBUSY_OFFSET 0xF408
#define MMDIO_CTRL_MBUSY_SHIFT  12
#define MMDIO_CTRL_MBUSY_SIZE   1
/* Bit: 'REGAD' */
/* Description: 'Register Address' */
#define MMDIO_CTRL_REGAD_OFFSET 0xF408
#define MMDIO_CTRL_REGAD_SHIFT  0
#define MMDIO_CTRL_REGAD_SIZE   5
#define MMDIO_CTRL_REG_SIZE   16
/* Bit: 'RDATA' */
/* Description: 'Read Data' */
#define MMDIO_READ_RDATA_OFFSET 0xF409
#define MMDIO_READ_RDATA_SHIFT  0
#define MMDIO_READ_RDATA_SIZE   16
/* Bit: 'WDATA' */
/* Description: 'Write Data' */
#define MMDIO_WRITE_WDATA_OFFSET    0xF40A
#define MMDIO_WRITE_WDATA_SHIFT 0
#define MMDIO_WRITE_WDATA_SIZE  16

#define LED_MD_CFG_LED5_OFFSET 0xF3E0
#define LED_MD_CFG_LED5_SHIFT 5
#define LED_MD_CFG_LED5_SIZE  1

#define LED_MD_CFG_LED6_OFFSET 0xF3E0
#define LED_MD_CFG_LED6_SHIFT 6
#define LED_MD_CFG_LED6_SIZE  1

#define LED_MD_CFG_LED7_OFFSET 0xF3E0
#define LED_MD_CFG_LED7_SHIFT 7
#define LED_MD_CFG_LED7_SIZE  1

#define LED_MD_CFG_LED8_OFFSET 0xF3E0
#define LED_MD_CFG_LED8_SHIFT 8
#define LED_MD_CFG_LED8_SIZE  1

#define LED_MD_CFG_LED9_OFFSET 0xF3E0
#define LED_MD_CFG_LED9_SHIFT 9
#define LED_MD_CFG_LED9_SIZE  1

#define INTEL_PHY_PORT_NUM  5

#define ETHSW_CAP_1_PPORTS_OFFSET	0x0007
#define ETHSW_CAP_1_PPORTS_SHIFT	0
#define ETHSW_CAP_1_PPORTS_SIZE		4

#define PCE_TBL_CTRL_BAS_OFFSET	0x44F
#define PCE_TBL_CTRL_BAS_SHIFT	15
#define PCE_TBL_CTRL_BAS_SIZE		1

#define PCE_TBL_CTRL_ADDR_OFFSET	0x44F
#define PCE_TBL_CTRL_ADDR_SHIFT		0
#define PCE_TBL_CTRL_ADDR_SIZE		5
#define PCE_TBL_CTRL_ADDR_OPMOD_SIZE 7
#define PCE_TBL_CTRL_ADDR_REG_SIZE 16

#define PCE_TBL_CTRL_VLD_OFFSET 0x44F
#define PCE_TBL_CTRL_VLD_SHIFT  12
#define PCE_TBL_CTRL_VLD_SIZE       1


#define PCE_TBL_ADDR_ADDR_OFFSET	0x44E
#define PCE_TBL_ADDR_ADDR_SHIFT		0
#define PCE_TBL_ADDR_ADDR_SIZE		12

#define PCE_TBL_VAL_0_VAL0_OFFSET   0x44D
#define PCE_TBL_VAL_0_VAL0_SHIFT    0
#define PCE_TBL_VAL_0_VAL0_SIZE     16
#define PCE_TBL_VAL_1_VAL1_OFFSET   0x44C
#define PCE_TBL_VAL_1_VAL1_SHIFT    0
#define PCE_TBL_VAL_1_VAL1_SIZE     16
#define PCE_TBL_VAL_3_VAL3_OFFSET	0x44A
#define PCE_TBL_VAL_3_VAL3_SHIFT	0
#define PCE_TBL_VAL_3_VAL3_SIZE		16

#define BM_RAM_VAL_0_VAL0_OFFSET	0x0043
#define BM_RAM_VAL_0_VAL0_SHIFT		0
#define BM_RAM_VAL_0_VAL0_SIZE		16
#define BM_RAM_VAL_1_VAL1_OFFSET	0x0042
#define BM_RAM_VAL_1_VAL1_SHIFT		0
#define BM_RAM_VAL_1_VAL1_SIZE		16
#define BM_PCFG_CNTEN_OFFSET		0x080
#define BM_PCFG_CNTEN_SHIFT			0
#define BM_PCFG_CNTEN_SIZE			1

#define PCE_TBL_VAL_2_VAL2_OFFSET	0x44B
#define PCE_TBL_VAL_2_VAL2_SHIFT	0
#define PCE_TBL_VAL_2_VAL2_SIZE		16
#define VLAN_MAP_TBL_SIZE			4096

#define PCE_DEFPVID_PVID_OFFSET		0x486
#define PCE_DEFPVID_PVID_SHIFT		0
#define PCE_DEFPVID_PVID_SIZE		12

#define PCE_VCTRL_UVR_OFFSET		0x485
#define PCE_VCTRL_UVR_SHIFT			0
#define PCE_VCTRL_UVR_SIZE			1

#define PCE_VCTRL_VSR_OFFSET		0x485
#define PCE_VCTRL_VSR_SHIFT			5
#define PCE_VCTRL_VSR_SIZE			1

#define PCE_VCTRL_VIMR_OFFSET		0x485
#define PCE_VCTRL_VIMR_SHIFT		3
#define PCE_VCTRL_VIMR_SIZE			1
#define PCE_VCTRL_VIMR_VEMR_SIZE	2

#define PCE_VCTRL_VINR_OFFSET		0x485
#define PCE_VCTRL_VINR_SHIFT		1
#define PCE_VCTRL_VINR_SIZE			2

#define PCE_PCTRL_0_TVM_OFFSET		0x480
#define PCE_PCTRL_0_TVM_SHIFT		5
#define PCE_PCTRL_0_TVM_SIZE		1

#define PCE_PCTRL_0_TVM_OFFSET		0x480
#define PCE_PCTRL_0_TVM_SHIFT		5
#define PCE_PCTRL_0_TVM_SIZE		1

#define PCE_GCTRL_0_VLAN_OFFSET		0x456
#define PCE_GCTRL_0_VLAN_SHIFT		14
#define PCE_GCTRL_0_VLAN_SIZE       1

#define PCE_GCTRL_1_VLANMD_OFFSET	0x457
#define PCE_GCTRL_1_VLANMD_SHIFT	9
#define PCE_GCTRL_1_VLANMD_SIZE		1

#define PCE_GCTRL_0_MC_VALID_OFFSET	0x456
#define PCE_GCTRL_0_MC_VALID_SHIFT	3
#define PCE_GCTRL_0_MC_VALID_SIZE	1

#define MANU_ID_PNUML_OFFSET		0xFA10
#define MANU_ID_PNUML_SHIFT			12
#define MANU_ID_PNUML_SIZE			4

#define FDMA_PCTRL_EN_OFFSET	0xA80
#define FDMA_PCTRL_EN_SHIFT		0
#define FDMA_PCTRL_EN_SIZE		1

#define SDMA_PCTRL_PEN_OFFSET	0xBC0
#define SDMA_PCTRL_PEN_SHIFT	0
#define SDMA_PCTRL_PEN_SIZE		1


#define GSWIP_CFG_SWRES_OFFSET	0xF400
#define GSWIP_CFG_SWRES_SHIFT		0
#define GSWIP_CFG_SWRES_SIZE		1

#define GSWIP_CFG_SE_OFFSET 0xF400
#define GSWIP_CFG_SE_SHIFT  15
#define GSWIP_CFG_SE_SIZE   1

#define GPHY0_FCR_FCR_OFFSET 0xF700
#define GPHY0_FCR_FCR_SHIFT 0
#define GPHY0_FCR_FCR_SIZE  14

#define PCE_MICRO_TABLE_SIZE 64

#define INSTR 0
#define IPV6 1
#define LENACCU 2

#define RST_REQ_RD0_OFFSET 0xFA01
#define RST_REQ_RD0_SHIFT 0
#define RST_REQ_RD0_SIZE  1

#define PCE_MC_M(val, msk, ns, out, len, type, flags, ipv4_len) \
	{ val, msk, (ns<<10 | out<<4 | len>>1),\
		(len&1)<<15 | type<<13 | flags<<9 | ipv4_len<<8 }


typedef struct {
	u16 val_3;
	u16 val_2;
	u16 val_1;
	u16 val_0;
} pce_uc_row_t;

typedef pce_uc_row_t PCE_MICROCODE[PCE_MICRO_TABLE_SIZE];

enum led_mode{
       LED_NORMAL = 0, // led on when link 10/100/1000Mbps, blink when tx/rx
       LED_ALL_ON,
       LED_ALL_OFF,
       LED_ALL_BLINK
 };

enum {
       GROUND_MODE = 0,
       POWER_MODE
};

typedef struct {
    u16 key[16];
    u16 mask[4];
    u16 val[16];
    u16 table;
    u16 pcindex;
    u16 opmode:2;
    u16 extop:1;
    u16 kformat:1;
    u16 type:1;
    u16 valid:1;
    u16 group:4;
} pctbl_prog_t;

typedef struct
{
   /** CTAG VLAN ID. The valid range is from 0 to 4095.
    ¦  An error code is delivered in case of range mismatch. */
   u16 nVId;
   /** Filtering Identifier (FID) (not supported by all switches). */
   u32 nFId;
} GSW_VLAN_IdCreate_t;

typedef enum {
    PCE_OPMODE_ADRD = 0,
    PCE_OPMODE_ADWR = 1,
    PCE_OPMODE_KSRD = 2,
    PCE_OPMODE_KSWR = 3
} ptbl_opcode_t;

enum {
	GOUT_MAC0 = 0,
	GOUT_MAC1,
	GOUT_MAC2,
	GOUT_MAC3,
	GOUT_MAC4,
	GOUT_MAC5,
	GOUT_ETHTYP,
	GOUT_VTAG0,
	GOUT_VTAG1,
	GOUT_ITAG0,
	GOUT_ITAG1,	/*10 */
	GOUT_ITAG2,
	GOUT_ITAG3,
	GOUT_IP0,
	GOUT_IP1,
	GOUT_IP2,
	GOUT_IP3,
	GOUT_SIP0,
	GOUT_SIP1,
	GOUT_SIP2,
	GOUT_SIP3,	/*20*/
	GOUT_SIP4,
	GOUT_SIP5,
	GOUT_SIP6,
	GOUT_SIP7,
	GOUT_DIP0,
	GOUT_DIP1,
	GOUT_DIP2,
	GOUT_DIP3,
	GOUT_DIP4,
	GOUT_DIP5,	/*30*/
	GOUT_DIP6,
	GOUT_DIP7,
	GOUT_SESID,
	GOUT_PROT,
	GOUT_APP0,
	GOUT_APP1,
	GOUT_IGMP0,
	GOUT_IGMP1,
	GOUT_STAG0 = 61,
	GOUT_STAG1 = 62,
	GOUT_NONE	=	63,
};

/* parser's microcode flag type */
enum {
	GFLAG_ITAG = 0,
	GFLAG_VLAN,
	GFLAG_SNAP,
	GFLAG_PPPOE,
	GFLAG_IPV6,
	GFLAG_IPV6FL,
	GFLAG_IPV4,
	GFLAG_IGMP,
	GFLAG_TU,
	GFLAG_HOP,
	GFLAG_NN1,	/*10 */
	GFLAG_NN2,
	GFLAG_END,
	GFLAG_NO,	/*13*/
	GFLAG_SVLAN,  /*14 */
};

typedef enum {
	/** Parser microcode table */
	PCE_PARS_INDEX = 0x00,
	PCE_ACTVLAN_INDEX = 0x01,
	PCE_VLANMAP_INDEX = 0x02,
	PCE_PPPOE_INDEX = 0x03,
	PCE_PROTOCOL_INDEX = 0x04,
	PCE_APPLICATION_INDEX	= 0x05,
	PCE_IP_DASA_MSB_INDEX	= 0x06,
	PCE_IP_DASA_LSB_INDEX	= 0x07,
	PCE_PACKET_INDEX = 0x08,
	PCE_PCP_INDEX = 0x09,
	PCE_DSCP_INDEX = 0x0A,
	PCE_MAC_BRIDGE_INDEX	= 0x0B,
	PCE_MAC_DASA_INDEX = 0x0C,
	PCE_MULTICAST_SW_INDEX = 0x0D,
	PCE_MULTICAST_HW_INDEX = 0x0E,
	PCE_TFLOW_INDEX = 0x0F,
	PCE_REMARKING_INDEX = 0x10,
	PCE_QUEUE_MAP_INDEX = 0x11,
	PCE_METER_INS_0_INDEX	= 0x12,
	PCE_METER_INS_1_INDEX	= 0x13,
	PCE_IPDALSB_INDEX = 0x14,
	PCE_IPSALSB_INDEX = 0x15,
	PCE_MACDA_INDEX = 0x16,
	PCE_MACSA_INDEX = 0x17,
	PCE_PARSER_FLAGS_INDEX = 0x18,
	PCE_PARS_INDIVIDUAL_INDEX	= 0x19,
	PCE_SPCP_INDEX = 0x1A,
	PCE_MSTP_INDEX = 0x1B,
	PCE_EGREMARK_INDEX = 0x1C,
	PCE_PAYLOAD_INDEX = 0x1D,
	PCE_EG_VLAN_INDEX = 0x1E,
} ptbl_cmds_t;

typedef enum {
    /** Boolean False. */
    LTQ_FALSE       = 0,
    /** Boolean True. */
    LTQ_TRUE        = 1
} ltq_bool_t;

typedef struct
{
   /** CTAG VLAN ID. The valid range is from 0 to 4095.
       An error code is delivered in case of range mismatch. */
   u16 nVId;
   /** Ethernet Port number (zero-based counting). The valid range is hardware
       dependent. An error code is delivered if the selected port is not
       available.

       \remarks
       This field is used as portmap field, when the MSB bit is set.
       In portmap mode, every value bit represents an Ethernet port.
       LSB represents Port 0 with incrementing counting.
       The (MSB - 1) bit represent the last port.
       The macro \ref GSW_PORTMAP_FLAG_SET allows to set the MSB bit,
       marking it as portmap variable.
       Checking the portmap flag can be done by
       using the \ref GSW_PORTMAP_FLAG_GET macro. */
   u32 nPortId;
   /** Tag Member Egress. Enable egress tag-based support.
       If enabled, all port egress traffic
       from this CTAG VLAN group carries a CTAG VLAN group tag. */
   ltq_bool_t bVLAN_TagEgress;
} GSW_VLAN_portMemberAdd_t;

typedef enum
{
   /** No VLAN member violation. Ingress and egress packets violating the
    ¦  membership pass and are not filtered out. */
   GSW_VLAN_MEMBER_VIOLATION_NO = 0,
   /** VLAN member violation for ingress packets. Ingress packets violating
    ¦  the membership are filtered out. Egress packets violating the
    ¦  membership are not filtered out. */
   GSW_VLAN_MEMBER_VIOLATION_INGRESS = 1,
   /** VLAN member violation for egress packets. Egress packets violating
    ¦  the membership are filtered out. Ingress packets violating the
    ¦  membership are not filtered out.*/
   GSW_VLAN_MEMBER_VIOLATION_EGRESS = 2,
   /** VLAN member violation for ingress and egress packets.
    ¦  Ingress and egress packets violating the membership are filtered out. */
   GSW_VLAN_MEMBER_VIOLATION_BOTH = 3
} GSW_VLAN_MemberViolation_t;

typedef enum
{
   /** Admit all. Tagged and untagged packets are allowed. */
   GSW_VLAN_ADMIT_ALL = 0,
   /** Untagged packets only (not supported yet). Tagged packets are dropped. */
   GSW_VLAN_ADMIT_UNTAGGED = 1,
   /** Tagged packets only. Untagged packets are dropped. */
   GSW_VLAN_ADMIT_TAGGED = 2
} GSW_VLAN_Admit_t;

typedef struct
{
   /** Ethernet Port number (zero-based counting). The valid range is hardware
       dependent. An error code is delivered if the selected port is not
       available. */
   u8 nPortId;
   /** Port CTAG VLAN ID (PVID). The software shall ensure that the used VID has
       been configured in advance on the hardware by
       using \ref GSW_VLAN_ID_CREATE. */
   u16 nPortVId;
    /** Drop ingress CTAG VLAN-tagged packets if the VLAN ID
        is not listed in the active VLAN set. If disabled, all incoming
        VLAN-tagged packets are forwarded using the FID tag members and
        the port members of the PVID.
        This parameter is only supported for devices which do not
        support 4k VLAN table entries (64 entries instead). */
   ltq_bool_t bVLAN_UnknownDrop;
   /** Reassign all ingress CTAG VLAN tagged packets to the port-based
       VLAN ID (PVID). */
   ltq_bool_t bVLAN_ReAssign;
   /** VLAN ingress and egress membership violation mode. Allows admittance of
       VLAN-tagged packets where the port is not a member of the VLAN ID
       carried in the received and sent packet. */
   GSW_VLAN_MemberViolation_t eVLAN_MemberViolation;
   /** Ingress VLAN-tagged or untagged packet filter configuration. */
   GSW_VLAN_Admit_t eAdmitMode;
   /** Transparent CTAG VLAN Mode (TVM). All packets are handled as untagged
       packets. Any existing tag is ignored and treated as packet payload. */
   ltq_bool_t bTVM;
} GSW_VLAN_portCfg_t;

typedef struct
{
    /** Device address on the MDIO interface */
    u8   nAddressDev;
    /** MMD Register address/offset inside the device. */
    u32  nAddressReg;
    /** Exchange data word with the device (read / write). */
    u16  nData;
} GSW_MMD_data_t;

typedef struct
{
   /** Device address on the MDIO interface */
   u8   nAddressDev;
   /** Register address inside the device. */
   u8   nAddressReg;
   /** Exchange data word with the device (read / write). */
   u16  nData;
} GSW_MDIO_data_t;

typedef unsigned int ur;

int gsw_reg_rd(struct sgmac_priv *priv,u16 Offset, u16 Shift, u16 Size, uint *value);
int gsw_reg_wr(struct sgmac_priv *priv,u16 Offset, u16 Shift, u16 Size, u32 value);
int GSW_MDIO_DataRead(struct sgmac_priv *priv, GSW_MDIO_data_t *parm);
int GSW_MDIO_DataWrite(struct sgmac_priv *priv, GSW_MDIO_data_t *parm);
int intel_rgmii_init(struct sgmac_priv *priv, int port);
int GSW_MmdDataWrite(GSW_MMD_data_t *parm);
void intel7084_led_init(int led_mode);
int intel7084_vlan_set(void);
int GSW_Enable(void);
#endif
