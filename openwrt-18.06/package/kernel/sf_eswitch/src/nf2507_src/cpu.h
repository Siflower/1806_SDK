/**  @file
  *  @brief    brief  description
  *  @author   
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#ifndef __L2SW_API_CPU_H__
#define __L2SW_API_CPU_H__

#define SF2507_TOP_REG_BASE         0x90900000
#define FTDMAC020_REG_BASE          0x50200000
#define SDRAM_MEM_BASE 			    0x30000000
#define NFW_FTDMAC020_REG_BASE      0x50500000
//注意，如果运行程序的rom空间+运行时占用的ram空间大于0x6000了，则这个地址值需要同步增大
#define SEND_PKT_SRAM_BASE_ADDR     0x6000 
#define RECEIVE_PKT_SRAM_BASE_ADDR	(SEND_PKT_SRAM_BASE_ADDR+0x1000)  //给发送预留了0x1000的buffer空间

#define MCU_INT                     0x3d00  
#define RX_ADDR0                    0x3d02  //sram addr lower 16bit
#define RX_ADDR1                    0x3d03  //sram addr higher 16bit
#define RX_ADDR_RENEW               0x3d04  //store first addr fifo's full or empty, when status bit is 0, set sram addr
#define TX_PKT_BD                   0x3d05  //send ptk config register
#define TX_PKT_START                0x3d06  //sned ptk start register
#define RX_TIMER1                	0x3d15  //RX fifo 异常判断时延，将此寄存器设置为非0值可使得堵住的CB释放
#define RX_STATUS 					0x3d16  //{rx_fifo_overflow,rx_fifo_underflow,tx_fifo_overflow,tx_fifo_underflow}
#define RX_BUF_LEN                  0x800   //2048 bytes
#define MAX_RETRY_NUM               3

#define C0_CSR                      0x100
#define	C0_CFG                      0x104
#define	C0_SrcAddr                  0x108
#define	C0_DstAddr                  0x10c
#define	C0_LLP                      0x110
#define	C0_SIZE                     0x114

#define	INT                         0x0
#define	INT_TC                      0x04
#define	INT_TC_CLR                  0x08
#define	INT_ERR	                    0x0c
#define	INT_ERR_CLR	                0x10
#define	TC                          0x14
#define	ERR                         0x18
#define	CH_EN                       0x1c
#define	CH_BUSY                     0x20
#define	CSR	                        0x24
#define	SYNC                        0x28

//dma
#define		MODE_NORMAL		0x00000000
#define     MODE_HARD       0x00000080

#define		SRC_SIZE4	0x00010000

#define		SRCAD_CTL_INC	0x00000000
#define 	SRCAD_CTL_DEC	0x00000020
#define		SRCAD_CTL_FIX	0x00000040
        	
#define		DSTAD_CTL_INC	0x00000000
#define		DSTAD_CTL_DEC	0x00000008
#define		DSTAD_CTL_FIX	0x00000010

#define		CHAN_DIS	0x00000000
#define		CHAN_EN		0x00000001

#define		SRC_SEL_M0	0x00000000
#define		SRC_SEL_M1	0x00000004
        	
#define		DST_SEL_M0	0x00000000
#define 	DST_SEL_M1	0x00000002

#define		SRC_WIDTH8	0x00000000
#define		SRC_WIDTH16	0x00000800
#define		SRC_WIDTH32	0x00001000
        	
#define		DST_WIDTH8	0x00000000
#define		DST_WIDTH16	0x00000100
#define		DST_WIDTH32	0x00000200

#define 	CHPRILO		0x00000000

/*
 * Data Type Declaration
 */
typedef enum l2sw_cpu_insert_e
{
    CPU_INSERT_TO_ALL = 0,
    CPU_INSERT_TO_TRAPPING,
    CPU_INSERT_TO_NONE,
    CPU_INSERT_END
}l2sw_cpu_insert_t;

typedef enum l2sw_cpu_position_e
{
    CPU_POS_AFTER_SA = 0,
    CPU_POS_BEFORE_CRC,
    CPU_POS_END
}l2sw_cpu_position_t;

typedef enum l2sw_cpu_tag_length_e
{
    CPU_LEN_8BYTES = 0,
    CPU_LEN_4BYTES,
    CPU_LEN_END
}l2sw_cpu_tag_length_t;


typedef enum l2sw_cpu_rx_length_e
{
    CPU_RX_72BYTES = 0,
    CPU_RX_64BYTES,
    CPU_RX_END
}l2sw_cpu_rx_length_t;


/* Function Name:
 *      l2sw_cpu_enable_set
 * Description:
 *      Set CPU port function enable/disable.
 * Input:
 *      enable - CPU port function enable
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can set CPU port function enable/disable.
 */
extern l2sw_api_ret_t l2sw_cpu_enable_set(l2sw_uint8 units, l2sw_enable_t enable);

/* Function Name:
 *      l2sw_cpu_enable_get
 * Description:
 *      Get CPU port and its setting.
 * Input:
 *      None
 * Output:
 *      pEnable - CPU port function enable
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_L2_NO_CPU_PORT   - CPU port is not exist
 * Note:
 *      The API can get CPU port function enable/disable.
 */
extern l2sw_api_ret_t l2sw_cpu_enable_get(l2sw_uint8 units, l2sw_enable_t *pEnable);

/* Function Name:
 *      l2sw_cpu_tagPort_set
 * Description:
 *      Set CPU port and CPU tag insert mode.
 * Input:
 *      port - Port id.
 *      mode - CPU tag insert for packets egress from CPU port.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can set CPU port and inserting proprietary CPU tag mode (Length/Type 0x8899)
 *      to the frame that transmitting to CPU port.
 *      The inset cpu tag mode is as following:
 *      - CPU_INSEL2SW_TO_ALL
 *      - CPU_INSEL2SW_TO_TRAPPING
 *      - CPU_INSEL2SW_TO_NONE
 */
extern l2sw_api_ret_t l2sw_cpu_tagPort_set(l2sw_uint8 units, l2sw_port_t port, l2sw_cpu_insert_t mode);

/* Function Name:
 *      l2sw_cpu_tagPort_get
 * Description:
 *      Get CPU port and CPU tag insert mode.
 * Input:
 *      None
 * Output:
 *      pPort - Port id.
 *      pMode - CPU tag insert for packets egress from CPU port, 0:all insert 1:Only for trapped packets 2:no insert.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_L2_NO_CPU_PORT   - CPU port is not exist
 * Note:
 *      The API can get configured CPU port and its setting.
 *      The inset cpu tag mode is as following:
 *      - CPU_INSEL2SW_TO_ALL
 *      - CPU_INSEL2SW_TO_TRAPPING
 *      - CPU_INSEL2SW_TO_NONE
 */
extern l2sw_api_ret_t l2sw_cpu_tagPort_get(l2sw_uint8 units, l2sw_port_t *pPort, l2sw_cpu_insert_t *pMode);

/* Function Name:
 *      l2sw_cpu_awarePort_set
 * Description:
 *      Set CPU aware port mask.
 * Input:
 *      portmask - Port mask.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK      - Invalid port mask.
 * Note:
 *      The API can set configured CPU aware port mask.
 */
extern l2sw_api_ret_t l2sw_cpu_awarePort_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask);


/* Function Name:
 *      l2sw_cpu_awarePort_get
 * Description:
 *      Get CPU aware port mask.
 * Input:
 *      None
 * Output:
 *      pPortmask - Port mask.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 * Note:
 *      The API can get configured CPU aware port mask.
 */
extern l2sw_api_ret_t l2sw_cpu_awarePort_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask);

/* Function Name:
 *      l2sw_cpu_tagPosition_set
 * Description:
 *      Set CPU tag position.
 * Input:
 *      position - CPU tag position.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can set CPU tag position.
 */
extern l2sw_api_ret_t l2sw_cpu_tagPosition_set(l2sw_uint8 units, l2sw_cpu_position_t position);

/* Function Name:
 *      l2sw_cpu_tagPosition_get
 * Description:
 *      Get CPU tag position.
 * Input:
 *      None
 * Output:
 *      pPosition - CPU tag position.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can get CPU tag position.
 */
extern l2sw_api_ret_t l2sw_cpu_tagPosition_get(l2sw_uint8 units, l2sw_cpu_position_t *pPosition);

/* Function Name:
 *      l2sw_cpu_tagLength_set
 * Description:
 *      Set CPU tag length.
 * Input:
 *      length - CPU tag length.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can set CPU tag length.
 */
extern l2sw_api_ret_t l2sw_cpu_tagLength_set(l2sw_uint8 units, l2sw_cpu_tag_length_t length);

/* Function Name:
 *      l2sw_cpu_tagLength_get
 * Description:
 *      Get CPU tag length.
 * Input:
 *      None
 * Output:
 *      pLength - CPU tag length.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can get CPU tag length.
 */
extern l2sw_api_ret_t l2sw_cpu_tagLength_get(l2sw_uint8 units, l2sw_cpu_tag_length_t *pLength);

/* Function Name:
 *      l2sw_cpu_acceptLength_set
 * Description:
 *      Set CPU accept  length.
 * Input:
 *      length - CPU tag length.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can set CPU accept length.
 */
extern l2sw_api_ret_t l2sw_cpu_acceptLength_set(l2sw_uint8 units, l2sw_cpu_rx_length_t length);

/* Function Name:
 *      l2sw_cpu_acceptLength_get
 * Description:
 *      Get CPU accept length.
 * Input:
 *      None
 * Output:
 *      pLength - CPU tag length.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can get CPU accept length.
 */
extern l2sw_api_ret_t l2sw_cpu_acceptLength_get(l2sw_uint8 units, l2sw_cpu_rx_length_t *pLength);

/* Function Name:
 *      l2sw_cpu_priRemap_set
 * Description:
 *      Configure CPU priorities mapping to internal absolute priority.
 * Input:
 *      int_pri     - internal priority value.
 *      new_pri    - new internal priority value.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid 1p priority.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      Priority of CPU tag assignment for internal asic priority, and it is used for queue usage and packet scheduling.
 */
extern l2sw_api_ret_t l2sw_cpu_priRemap_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t new_pri);

/* Function Name:
 *      l2sw_cpu_priRemap_get
 * Description:
 *      Configure CPU priorities mapping to internal absolute priority.
 * Input:
 *      int_pri     - internal priority value.
 * Output:
 *      pNew_pri    - new internal priority value.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid 1p priority.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      Priority of CPU tag assignment for internal asic priority, and it is used for queue usage and packet scheduling.
 */
extern l2sw_api_ret_t l2sw_cpu_priRemap_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t *pNew_pri);

/* Function Name:
 *      l2sw_cpu_tag_ethertype_set
 * Description:
 *      Set CPU tag ethertype value.
 * Input:
 *      value
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can set CPU tag ethertype value.
 */
extern l2sw_api_ret_t l2sw_cpu_tag_ethertype_set(l2sw_uint8 units, l2sw_uint16 value);

/* Function Name:
 *      l2sw_cpu_tag_ethertype_get
 * Description:
 *      Get CPU tag ethertype value.
 * Input:
 *      None
 * Output:
 *      value
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can get CPU tag ethertype value.
 */
extern l2sw_api_ret_t l2sw_cpu_tag_ethertype_get(l2sw_uint8 units, l2sw_uint16 *value);

/* Function Name:
 *      l2sw_cpu_send_pkt
 * Description:
 *      Send pkt from CPU.
 * Input:
 *      pkt_len
 *      pkt
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can make CPU send pkt.
 */
extern l2sw_api_ret_t l2sw_cpu_send_pkt(l2sw_uint8 units, l2sw_uint32 pkt_len, l2sw_uint8 *pkt);

/* Function Name:
 *      l2sw_cpu_get_pkt
 * Description:
 *      Get the pkt whitch is send to CPU.
 * Input:
 *      None
 * Output:
 *      pkt_len
 *      pkt
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT      - Invalid input.
 * Note:
 *      The API can get pkt whitch is send to CPU.
 */
extern l2sw_api_ret_t l2sw_cpu_get_pkt(l2sw_uint8 units, l2sw_uint32 *pkt_len, l2sw_uint8 *pkt);

#endif /* __L2SW_API_CPU_H__ */

