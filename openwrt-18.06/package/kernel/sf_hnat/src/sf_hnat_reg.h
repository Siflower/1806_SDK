#ifndef _SF_HNAT_REG_H_
#define _SF_HNAT_REG_H_

#define HNAT_CSR_BASE								 0x4000
#define HNAT_TABLE_BASE								 0x6000

#define NAPT_TABLE_MAX 1024
#define VLAN_TABLE_MAX 128
#define DIP_TABLE_MAX 512
#define RT_PUB_NET_TABLE_MAX 16
#define PPPHD_TABLE_MAX 8
#define DMAC_TABLE_MAX 128
#define ROUTER_MAC_TABLE_MAX 8

//TABLE NUMBER
#define INAT_NAPT_HASH1_NO		0
#define INAT_NAPT_HASH2_NO		1
#define INAT_NAPT_HASH3_NO		2
#define ENAT_NAPT_HASH1_NO		3
#define ENAT_NAPT_HASH2_NO		4
#define ENAT_NAPT_HASH3_NO		5
#define NAPT_NO					6
#define NAPT_VLD_NO				7
#define RT_PUB_NET_NO			8
#define DIP_HASH1_NO			16
#define DIP_HASH2_NO			17
#define DIP_NO					18
#define DIP_VLD_NO				19
#define DMAC_NO					20
#define PPPOE_HD_NO				21
#define ROUTER_MAC_NO			22
#define VLAN_ID_NO				23
#define STATICS_NO				24
#define HNAT_TB_NO				25  //0-8,16-24 is vld,9-15,25-31 is reserved

/*regular use bit for hnat reg*/
#define HWNAT_REG1_TB_STATUS_BUSY       0x00000001  //busy bit
#define HWNAT_REG4_TB_OP_CODE_WR		1   //write action
#define HWNAT_REG4_TB_OP_CODE_RD		0   //read action
#define HWNAT_REG3_TB_ADD_NO_OFF		16  //offset of table number
#define HWNAT_TB_WRDATA_NO				5   //number of write data register
#define HWNAT_TB_RDDATA_NO				7   //number of read data register
#define HWNAT_REG2_TB_CONFIG_ENTB		0x0fff0000 //enable all tables
#define HWNAT_REG2_TB_CONFIG_USE_DIP	0x00000001 //use dip for enat arp search, default auto judge
#define HWNAT_REG2_TB_CONFIG_EN_BIG_ENDIAN		0x00000010 //enable big endian for wr and litte endian for read
#define HWNAT_REG2_TB_CONFIG_CRC3_SEL			0x00000400 //use 0x8005 for crc_key[2]
#define HWNAT_REG2_TB_CONFIG_READ_CLR			0x00000004 // host read clear


#define HWNAT_REG0_CSR						HNAT_CSR_BASE + 0x0000
// not operate during data trans
#define HWNAT_REG1_CSR						HNAT_CSR_BASE + 0x0004
#define HWNAT_REG2_CSR						HNAT_CSR_BASE + 0x0008
#define HWNAT_REG3_CSR						HNAT_CSR_BASE + 0x000c
#define HWNAT_REG4_CSR						HNAT_CSR_BASE + 0x0010
#define HWNAT_REG5_CSR						HNAT_CSR_BASE + 0x0014
#define HWNAT_REG6_CSR						HNAT_CSR_BASE + 0x0018
#define HWNAT_REG7_CSR						HNAT_CSR_BASE + 0x001c
#define HWNAT_REG8_CSR						HNAT_CSR_BASE + 0x0020
#define HWNAT_REG16_CSR						HNAT_CSR_BASE + 0x0040
#define HWNAT_REG17_CSR	                    HNAT_CSR_BASE + 0x0044
#define HWNAT_REG18_CSR		                HNAT_CSR_BASE + 0x0048
#define HWNAT_REG19_CSR                     HNAT_CSR_BASE + 0x004C
#define HWNAT_REG20_CSR                     HNAT_CSR_BASE + 0x0050
#define HWNAT_REG21_CSR                     HNAT_CSR_BASE + 0x0054
#define HWNAT_REG22_CSR						HNAT_CSR_BASE + 0x0058
#define HWNAT_REG23_CSR                     HNAT_CSR_BASE + 0x005c
#define HWNAT_REG30_CSR                     HNAT_CSR_BASE + 0x0080
#define HWNAT_REG31_CSR                     HNAT_CSR_BASE + 0x0084
#define HWNAT_REG33_CSR                     HNAT_CSR_BASE + 0x008c
#define HWNAT_REG34_CSR                     HNAT_CSR_BASE + 0x0090
#define HWNAT_REG35_CSR                     HNAT_CSR_BASE + 0x0094
#define HWNAT_REG36_CSR                     HNAT_CSR_BASE + 0x0098
#define HWNAT_REG48_CSR						HNAT_CSR_BASE + 0x00c0
#define	HWNAT_REG49_CSR						HNAT_CSR_BASE + 0x00c4
#define	HWNAT_REG50_CSR						HNAT_CSR_BASE + 0x00c8
#define	HWNAT_REG51_CSR						HNAT_CSR_BASE + 0x00cc
#define	HWNAT_REG52_CSR						HNAT_CSR_BASE + 0x00d0
#define	HWNAT_REG53_CSR						HNAT_CSR_BASE + 0x00d4
#define	HWNAT_REG54_CSR						HNAT_CSR_BASE + 0x00d8
#define	HWNAT_REG55_CSR						HNAT_CSR_BASE + 0x00dc
#define	HWNAT_REG56_CSR						HNAT_CSR_BASE + 0x00e0
#define	HWNAT_REG57_CSR						HNAT_CSR_BASE + 0x00e4
#define	HWNAT_REG58_CSR						HNAT_CSR_BASE + 0x00e8
#define	HWNAT_REG59_CSR						HNAT_CSR_BASE + 0x00ec
#define	HWNAT_REG60_CSR						HNAT_CSR_BASE + 0x00f0
#define	HWNAT_REG61_CSR						HNAT_CSR_BASE + 0x00f4
#define	HWNAT_REG62_CSR						HNAT_CSR_BASE + 0x00f8
#define	HWNAT_REG70_CSR						HNAT_CSR_BASE + 0x01c4
#define	HWNAT_REG71_CSR						HNAT_CSR_BASE + 0x01c8
#define	HWNAT_REG72_CSR						HNAT_CSR_BASE + 0x01cc
#define	HWNAT_REG73_CSR						HNAT_CSR_BASE + 0x01d0
#define	HWNAT_REG74_CSR						HNAT_CSR_BASE + 0x01d4
#define	HWNAT_REG75_CSR						HNAT_CSR_BASE + 0x01d8
#define	HWNAT_REG76_CSR						HNAT_CSR_BASE + 0x01dc
#define	HWNAT_REG77_CSR						HNAT_CSR_BASE + 0x01e0
#define	HWNAT_RX_ENTER_SOF_CNT_L			HNAT_CSR_BASE + 0x0100
#define	HWNAT_RX_ENTER_SOF_CNT_H			HNAT_CSR_BASE + 0x0104
#define	HWNAT_RX_ENTER_EOF_CNT_L			HNAT_CSR_BASE + 0x0108
#define	HWNAT_RX_ENTER_EOF_CNT_H			HNAT_CSR_BASE + 0x010c
#define	HWNAT_RX_ENTER_DROP_CNT				HNAT_CSR_BASE + 0x0110
#define	HWNAT_RX_2HOST_FRAME_SOF_CNT_L		HNAT_CSR_BASE + 0x0114
#define	HWNAT_RX_2HOST_FRAME_SOF_CNT_H		HNAT_CSR_BASE + 0x0118
#define	HWNAT_RX_2HOST_FRAME_EOF_CNT_L		HNAT_CSR_BASE + 0x011c
#define	HWNAT_RX_2HOST_FRAME_EOF_CNT_H		HNAT_CSR_BASE + 0x0120
#define	HWNAT_RX2TX_DATA_FRAME_CNT			HNAT_CSR_BASE + 0x0124
#define	HWNAT_TX_RECEIVE_RX_FRAME_CNT		HNAT_CSR_BASE + 0x0128
#define	HWNAT_TX_SOF_FRAME_CNT				HNAT_CSR_BASE + 0x012c
#define	HWNAT_TX_EOF_FRAME_CNT				HNAT_CSR_BASE + 0x0130
#define	HWNAT_TX_EXIT_SOF_CNT_L				HNAT_CSR_BASE + 0x0134
#define	HWNAT_TX_EXIT_SOF_CNT_H				HNAT_CSR_BASE + 0x0138
#define	HWNAT_TX_EXIT_EOF_CNT_L				HNAT_CSR_BASE + 0x013c
#define	HWNAT_TX_EXIT_EOF_CNT_H				HNAT_CSR_BASE + 0x0140
#define	HWNAT_TX_NOHITS_FRAME_CNT			HNAT_CSR_BASE + 0x0144
#define HWNAT_PKT_ERR_CNT_SYNC				HNAT_CSR_BASE + 0X0148
#define	HWNAT_RCV_STATUS_CNT				HNAT_CSR_BASE + 0x014c
#define	HWNAT_TX_STATUS_CNT					HNAT_CSR_BASE + 0X0150
#define	HWNAT_RCV_TXACK_CNT					HNAT_CSR_BASE + 0X0154
#define	HWNAT_GEN_RXACK_CNT					HNAT_CSR_BASE + 0X0158
#define	HWNAT_2MTL_ACK_CNT					HNAT_CSR_BASE + 0X015C
#define	HWNAT_MTL_2HNAT_RDYN_CNT			HNAT_CSR_BASE + 0X0160
#define	HWNAT_TX_REC_RX_TXTRANS_DROP_CNT	HNAT_CSR_BASE + 0X0164
#define	HWNAT_TX_REC_RX_RXTRANS_DROP_CNT	HNAT_CSR_BASE + 0X0168
#define	HWNAT_TX_ENAT_CNT					HNAT_CSR_BASE + 0X016C
#define	HWNAT_RX_ENAT_CNT					HNAT_CSR_BASE + 0X0170
#define	HWNAT_RX_INAT_CNT					HNAT_CSR_BASE + 0x0174
#define	HWNAT_RX_TOTAL_ERR_CNT				HNAT_CSR_BASE + 0X0178
#define	HWNAT_RX_GMII_ERR_CNT				HNAT_CSR_BASE + 0X017C
#define	HWNAT_RX_CRC_ERR_CNT				HNAT_CSR_BASE + 0X0180
#define	HWNAT_RX_LENGTH_ERR_CNT				HNAT_CSR_BASE + 0X0184
#define	HWNAT_RX_IPHDR_ERR_CNT				HNAT_CSR_BASE + 0X0188
#define	HWNAT_RX_PAYLOAD_ERR_CNT			HNAT_CSR_BASE + 0X018C
#define	HWNAT_RX2TX2HOST_CNT				HNAT_CSR_BASE + 0X0190
#define	HWNAT_RX2TX_DROP_CNT				HNAT_CSR_BASE + 0X0194


//HNAT TABLE Register
#define HWNAT_REG1_TB_STATUS				HNAT_TABLE_BASE + 0x0000
//not opera on data trans
#define HWNAT_REG2_TB_CONFIG				HNAT_TABLE_BASE + 0x0004
#define HWNAT_REG3_TB_ADDRESS				HNAT_TABLE_BASE + 0x0008
#define HWNAT_REG4_TB_OP_CODE				HNAT_TABLE_BASE + 0x000c
#define HWNAT_REG5_TB_WRDATA0				HNAT_TABLE_BASE + 0x0010
#define HWNAT_REG6_TB_WRDATA1				HNAT_TABLE_BASE + 0x0014
#define HWNAT_REG7_TB_WRDATA2				HNAT_TABLE_BASE + 0x0018
#define HWNAT_REG8_TB_WRDATA3				HNAT_TABLE_BASE + 0x001c
#define HWNAT_REG9_TB_WRDATA4				HNAT_TABLE_BASE + 0x0020
#define HWNAT_REG10_TB_RDDATA0				HNAT_TABLE_BASE + 0x0030
#define HWNAT_REG11_TB_RDDATA1				HNAT_TABLE_BASE + 0x0034
#define HWNAT_REG12_TB_RDDATA2				HNAT_TABLE_BASE + 0x0038
#define HWNAT_REG13_TB_RDDATA3				HNAT_TABLE_BASE + 0x003c
#define HWNAT_REG14_TB_RDDATA4				HNAT_TABLE_BASE + 0x0040
#define HWNAT_REG15_TB_RDDATA5				HNAT_TABLE_BASE + 0x0044
#define HWNAT_REG16_TB_RDDATA6				HNAT_TABLE_BASE + 0x0048
#define HWNAT_REG17_REG_BUF_THRESH			HNAT_TABLE_BASE + 0x0050
#define HWNAT_REG18_REG_TB_FC_CFG			HNAT_TABLE_BASE + 0x0060
#define HWNAT_REG19_REG_TB_RFC_TIMER		HNAT_TABLE_BASE + 0x0064
#define HWNAT_REG20_REG_TB_RFC_AGECNT		HNAT_TABLE_BASE + 0x0068
#define HWNAT_REG21_REG_TB_TFC_TIMER		HNAT_TABLE_BASE + 0x006C



typedef enum _sf_hnat_tb_name {
	INAT_NAPT_HASH1,
	INAT_NAPT_HASH2,
	INAT_NAPT_HASH3,
	ENAT_NAPT_HASH1,
	ENAT_NAPT_HASH2,
	ENAT_NAPT_HASH3,
	NAPT,
	NAPT_VLD,
	RT_PUB_NET,
	DIP_HASH1 = 16,
	DIP_HASH2,
	DIP,
	DIP_VLD,
	DMAC,
	PPPOE_HD,
	ROUTER_MAC,
	VLAN_ID,
	STATICS,
} sf_hnat_tb_name;

typedef struct _sf_napt_crc_info {
	unsigned short inat_crc_key[3];
	unsigned short enat_crc_key[3];

	unsigned char inat_ptr_index;//there are 8 pointers and 3 hash tables,index 0~7, hash1 has 0,1, hash2 has 2~5, hash3 has 6,7
	unsigned char enat_ptr_index;
	unsigned char conflict_flag;//bit1 inat; bit 2 enat
} sf_napt_crc_info;

typedef struct _sf_dip_crc_info {
	unsigned short dip_crc_key[2];
	unsigned char dip_ptr_index;
} sf_dip_crc_info;

//512
typedef struct _sf_dip_entry {
	unsigned int dip;
	unsigned char dmac_index;
	unsigned char vlan_index;
	unsigned char valid;
	unsigned short ref_count;//the number of the napt entry that use this dip entry
} sf_dip_entry;

//128
typedef struct _sf_dmac_entry {
	unsigned char dmac[6];
	unsigned char router_mac_index;
	unsigned char valid;
	unsigned short ref_count;
} sf_dmac_entry;

//router mac
typedef struct _sf_rmac_entry {
	unsigned char rmac[6];
	unsigned char valid;
	unsigned short ref_count;
} sf_rmac_entry;

// pppoe entry
typedef struct _sf_ppphd_entry {
	unsigned short ppphd_sid;
	unsigned char valid;
	unsigned short ref_count;
} sf_ppphd_entry;

typedef struct _sf_rt_pub_net_entry {
	unsigned int router_pub_ip;
	unsigned char rt_ip_mask;
	unsigned char pub_vlan_index;
	unsigned char valid;
	unsigned short ref_count;
} sf_rt_pub_net_entry;

typedef struct _sf_vlan_id_entry {
	unsigned short vlan_id;
	unsigned char valid;
	unsigned short ref_count;
} sf_vlan_entry;

#endif
