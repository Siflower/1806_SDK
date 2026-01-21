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

#include "l2sw_switch.h"
#include "cpu.h"
#include "sal_string.h"
#include "l2sw_asicdrv_cputag.h"

static l2sw_api_ret_t _l2sw_cpu_enable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicCputagEnable(units, enable)) != L2SW_ERR_OK)
        return retVal;

    if (L2SW_DISABLED == enable)
    {
        if ((retVal = l2sw_setAsicCputagPortmask(units, 0)) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_cpu_enable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicCputagEnable(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tagPort_set(l2sw_uint8 units, l2sw_port_t port, l2sw_cpu_insert_t mode)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (mode >= CPU_INSERT_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicCputagPortmask(units, 1<<l2sw_switch_port_L2P_get(units, port))) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicCputagTrapPort(units, l2sw_switch_port_L2P_get(units, port))) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicCputagInsertMode(units, mode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tagPort_get(l2sw_uint8 units, l2sw_port_t *pPort, l2sw_cpu_insert_t *pMode)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk, port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPort)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pMode)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicCputagPortmask(units, &pmsk)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicCputagTrapPort(units, &port)) != L2SW_ERR_OK)
        return retVal;

    *pPort = l2sw_switch_port_P2L_get(units, port);

    if ((retVal = l2sw_getAsicCputagInsertMode(units, pMode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_awarePort_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 phyMbrPmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Valid port mask */
    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    /* Check port mask valid */
    L2SW_CHK_PORTMASK_VALID(units, pPortmask);

    if(l2sw_switch_portmask_L2P_get(units, pPortmask, &phyMbrPmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    if ((retVal = l2sw_setAsicCputagPortmask(units, phyMbrPmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_awarePort_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicCputagPortmask(units, &pmsk)) != L2SW_ERR_OK)
        return retVal;

    if(l2sw_switch_portmask_P2L_get(units, pmsk, pPortmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tagPosition_set(l2sw_uint8 units, l2sw_cpu_position_t position)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (position >= CPU_POS_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicCputagPosition(units, position)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tagPosition_get(l2sw_uint8 units, l2sw_cpu_position_t *pPosition)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPosition)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicCputagPosition(units, pPosition)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tagLength_set(l2sw_uint8 units, l2sw_cpu_tag_length_t length)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (length >= CPU_LEN_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicCputagMode(units, length)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tagLength_get(l2sw_uint8 units, l2sw_cpu_tag_length_t *pLength)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pLength)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicCputagMode(units, pLength)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_priRemap_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t new_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (new_pri > L2SW_PRIMAX || int_pri > L2SW_PRIMAX)
        return  L2SW_ERR_VLAN_PRIORITY;

    if ((retVal = l2sw_setAsicCputagPriorityRemapping(units, int_pri, new_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_priRemap_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t *pNew_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pNew_pri)
        return L2SW_ERR_NULL_POINTER;

    if (int_pri > L2SW_PRIMAX)
        return  L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_getAsicCputagPriorityRemapping(units, int_pri, pNew_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_acceptLength_set(l2sw_uint8 units, l2sw_cpu_rx_length_t length)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (length >= CPU_RX_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicCputagRxMinLength(units, length)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_acceptLength_get(l2sw_uint8 units, l2sw_cpu_rx_length_t *pLength)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pLength)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicCputagRxMinLength(units, pLength)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tag_ethertype_set(l2sw_uint8 units, l2sw_uint16 value)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (value > 0xFFFF)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicReg(units, L2SW_REG_CPU_FLAG_CTRL, (l2sw_uint32)value)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_tag_ethertype_get(l2sw_uint8 units, l2sw_uint16 *value)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == value)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicReg(units, L2SW_REG_CPU_FLAG_CTRL, (l2sw_uint32 *)value)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_send_pkt(l2sw_uint8 units, l2sw_uint32 len, l2sw_uint8 *pkt)
{
    l2sw_api_ret_t retVal;
    l2sw_int32 index = 0;
    l2sw_int32 csr_value0 = 0;
	l2sw_int32 transfer_beats = 0;
	l2sw_uint32 data_len = len;
	l2sw_uint32 tempread = 0;
	l2sw_uint32 value;
    l2sw_uint32 len_byte = len/4;
    int i = 0;
    l2sw_uint32 *buff;
	buff = (l2sw_uint32*)kmalloc(sizeof(l2sw_uint32)*380, GFP_KERNEL);
	memset(buff, 0, sizeof(l2sw_uint32)*380);

    L2SW_CHK_INIT_STATE(units);

    if(NULL == pkt)
        return L2SW_ERR_NULL_POINTER;

    //if pkt bytes % 4 != 0
    if(len%4!=0)
        len_byte++;

    for(i = 0; i < len_byte; i++)
    {
        buff[index] = ((pkt[index*4]<<24)|(pkt[index*4+1]<<16)|(pkt[index*4+2]<<8)|(pkt[index*4+3]));
        index++;
    }

    //data init
    index = 0;
    i = 0;
    while(len_byte--)
    {
        if ((retVal = l2sw_switch_ahb_write(units, SDRAM_MEM_BASE+SEND_PKT_SRAM_BASE_ADDR+index, buff[i])) != L2SW_ERR_OK)
            return retVal;
        index+=4;
        i++;
    }

    //dma Enable channels 0 synchronization circuit
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+SYNC, 0x1)) != L2SW_ERR_OK)
        return retVal;

	// Set source_address = SDRAM_MEM_BASE+0x10000
	//set memory addr into source addr
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_SrcAddr, SDRAM_MEM_BASE+SEND_PKT_SRAM_BASE_ADDR)) != L2SW_ERR_OK)
		return retVal;

    //user ahb master 0
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_LLP, 0x0)) != L2SW_ERR_OK)
		return retVal;

    transfer_beats = data_len %4==0 ? data_len /4 : data_len /4+1;

    //set beats to be transfered
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_SIZE, transfer_beats)) != L2SW_ERR_OK)
		return retVal;

	// Assign each channel's destination address, set destination addr
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_DstAddr, NFW_FTDMAC020_REG_BASE)) != L2SW_ERR_OK)
		return retVal;


	csr_value0 = (MODE_HARD	+ SRCAD_CTL_INC	+ DSTAD_CTL_INC	+ SRC_SEL_M0 + DST_SEL_M0 + CHAN_EN + SRC_SIZE4 + SRC_WIDTH32 + DST_WIDTH32 + CHPRILO);

	// Clear Cn_CSR all state
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_CSR, 0x0)) != L2SW_ERR_OK)
		return retVal;

	// Write data to Cn_CSR
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_CSR, csr_value0)) != L2SW_ERR_OK)
		return retVal;

	// Clear the all interrupt flags
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+INT_TC_CLR, 0x1)) != L2SW_ERR_OK)
		return retVal;

	// Enable C0_CFG INT_TC_MSK bit
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+C0_CFG, 0x0)) != L2SW_ERR_OK)
		return retVal;

	/*通知交换芯片pkt的相关信息，并start开始了，使得交换芯片准备好*/
	if ((retVal = l2sw_setAsicReg(units, TX_PKT_BD, data_len)) != L2SW_ERR_OK)
		return retVal;
	if ((retVal = l2sw_setAsicReg(units, TX_PKT_START, 0x1)) != L2SW_ERR_OK)
		return retVal;

	//启动DMA传输Set Master0 little-endian and Enable DMA controller
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+CSR, 0x1)) != L2SW_ERR_OK)
		return retVal;

    while(1)
    {
        if ((retVal = l2sw_switch_ahb_read(units, FTDMAC020_REG_BASE+CH_EN, &value)) != L2SW_ERR_OK)
            return retVal;

        if(value == 0)
            break;

       DELAY_MS(1);
	}

    //check switch chip get pkt or not

	//ahb slave 接收到一个包的中断 -表示SW收到报文
	if ((retVal = l2sw_getAsicReg(units, MCU_INT, &tempread)) != L2SW_ERR_OK)
        return retVal;

	//PRINT("\r\n check data MCU_INT %d.\r\n", tempread);
    while ((tempread&0x2) != 0x2)/*bit 1:ahb slave 接收到一个包的中断*/
    {
    	if ((retVal = l2sw_getAsicReg(units, MCU_INT, &tempread)) != L2SW_ERR_OK)
            return retVal;

		DELAY_MS(1);

		PRINT("wait send over\n");
    }

    //清中断
    if ((retVal = l2sw_setAsicReg(units, MCU_INT, 0x2)) != L2SW_ERR_OK)
        return retVal;

    //disable dma
	// Clear INT, INT_TC and TC flag
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+INT_TC_CLR, 0x1)) != L2SW_ERR_OK)
        return retVal;

	//Disable DMA
    if ((retVal = l2sw_switch_ahb_write(units, FTDMAC020_REG_BASE+CSR, 0x0)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_cpu_get_pkt(l2sw_uint8 units, l2sw_uint32 *len, l2sw_uint8 *pkt)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 readtemp;
    l2sw_uint32 try = 3;
    l2sw_uint32 pkt_info;
    l2sw_uint32 i, index;
    l2sw_uint32 recevice_cnt = 0;
    l2sw_uint8 *rxbuffer;
	rxbuffer = (l2sw_uint8*)kmalloc(RX_BUF_LEN, GFP_KERNEL);

    L2SW_CHK_INIT_STATE(units);

    if(NULL == pkt)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicReg(units, RX_ADDR_RENEW, &readtemp)) != L2SW_ERR_OK)
        return retVal;

    if((readtemp&(1<<0)) == 0x0)//rx_addr_fifo_full:store first addr fifo full or empty
    {
        if ((retVal = l2sw_setAsicReg(units, RX_ADDR0, (SDRAM_MEM_BASE+RECEIVE_PKT_SRAM_BASE_ADDR)&0x0000ffff)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicReg(units, RX_ADDR1, ((SDRAM_MEM_BASE+RECEIVE_PKT_SRAM_BASE_ADDR)&0xffff0000)>>16)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicReg(units, RX_ADDR_RENEW, 0x1)) != L2SW_ERR_OK)
            return retVal;

        while(try--)
        {
            if ((retVal = l2sw_getAsicReg(units, MCU_INT, &readtemp)) != L2SW_ERR_OK)
                return retVal;

    		/*0x3d00 interrupt status register bit0:pkt_upload_done, indicate switch have send packet success to soc*/
            if((readtemp&0x1) == 0x1)
            {
            	/*note: the first 4 byte is packet info:pkt lenth and src port*/
                if ((retVal = l2sw_switch_ahb_read(units, SDRAM_MEM_BASE+RECEIVE_PKT_SRAM_BASE_ADDR, &pkt_info)) != L2SW_ERR_OK)
                    return retVal;

    			PRINT("\r\nreceived pacekt len %d, src_port %d.\r\n", pkt_info&0xffff, ((pkt_info&0xff0000)>>16));

    			*len = pkt_info&0xffff;//{3'd0,rx_done,rx_status[3:0],src_port[7:0],pkt_len[15:0]}
    			PRINT("\r\nreceived data is:\r\n 0x%x:", 0x0);
    			i = 0x0;
    			while(i < *len)
    			{
    				/*note: big_end to little_end*/
    				if(0 == i%4)
    					index = i + 3;
    				else if(1 == i%4)
    					index = i + 1;
    				else if(2 == i%4)
    					index = i - 1;
    				else if(3 == i%4)
    					index = i - 3;
    				/*note: the first 4 byte is packet info, after is packet context*/
    				if ((retVal = l2sw_switch_ahb_read(units, SDRAM_MEM_BASE+RECEIVE_PKT_SRAM_BASE_ADDR+index+4, &readtemp)) != L2SW_ERR_OK)
                        return retVal;

                    if(0 == i%4)
                        rxbuffer[i] = (readtemp&0xff000000)>>24;
    				else if(1 == i%4)
    					rxbuffer[i] = (readtemp&0xff0000)>>16;
    				else if(2 == i%4)
    					rxbuffer[i] = (readtemp&0xff00)>>8;
    				else if(3 == i%4)
    					rxbuffer[i] = readtemp&0xff;

    				*pkt = rxbuffer[i];

    				if(rxbuffer[i] < 0x10)
                        PRINT("0x0%x,",rxbuffer[i]);
                    else
                        PRINT("0x%x,",rxbuffer[i]);

    				if(0 == (i+1)%16)
    					PRINT("\r\n 0x%x:", i+1);

    				i++;
    				pkt++;
    			}

    			//write 1 to clear ahb master interrupt
    			if ((retVal = l2sw_setAsicReg(units, MCU_INT, 0x1)) != L2SW_ERR_OK)
                    return retVal;
    			recevice_cnt++;
    			PRINT("\r\nrxbuffer reciver over.\r\n");
    			break;
            }
    		else
    		{
                DELAY_MS(1);
    		}
        }
    }
    return L2SW_ERR_OK;
}

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
l2sw_api_ret_t l2sw_cpu_enable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_enable_set(units, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_enable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_enable_get(units, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tagPort_set(l2sw_uint8 units, l2sw_port_t port, l2sw_cpu_insert_t mode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tagPort_set(units, port, mode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tagPort_get(l2sw_uint8 units, l2sw_port_t *pPort, l2sw_cpu_insert_t *pMode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tagPort_get(units, pPort, pMode);
    L2SW_API_UNLOCK(units);

    return retVal;
}


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
l2sw_api_ret_t l2sw_cpu_awarePort_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_awarePort_set(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_awarePort_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_awarePort_get(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tagPosition_set(l2sw_uint8 units, l2sw_cpu_position_t position)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tagPosition_set(units, position);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tagPosition_get(l2sw_uint8 units, l2sw_cpu_position_t *pPosition)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tagPosition_get(units, pPosition);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tagLength_set(l2sw_uint8 units, l2sw_cpu_tag_length_t length)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tagLength_set(units, length);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tagLength_get(l2sw_uint8 units, l2sw_cpu_tag_length_t *pLength)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tagLength_get(units, pLength);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_priRemap_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t new_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_priRemap_set(units, int_pri, new_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_priRemap_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t *pNew_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_priRemap_get(units, int_pri, pNew_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_acceptLength_set(l2sw_uint8 units, l2sw_cpu_rx_length_t length)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_acceptLength_set(units, length);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_acceptLength_get(l2sw_uint8 units, l2sw_cpu_rx_length_t *pLength)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_acceptLength_get(units, pLength);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tag_ethertype_set(l2sw_uint8 units, l2sw_uint16 value)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tag_ethertype_set(units, value);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_tag_ethertype_get(l2sw_uint8 units, l2sw_uint16 *value)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_tag_ethertype_get(units, value);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_send_pkt(l2sw_uint8 units, l2sw_uint32 pkt_len, l2sw_uint8 *pkt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_send_pkt(units, pkt_len, pkt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

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
l2sw_api_ret_t l2sw_cpu_get_pkt(l2sw_uint8 units, l2sw_uint32 *pkt_len, l2sw_uint8 *pkt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_cpu_get_pkt(units, pkt_len, pkt);
    L2SW_API_UNLOCK(units);

    return retVal;
}
