/**
 ******************************************************************************
 * @file  mmc.c
 * @author  Mark.Cai
 * @version  V3.0.0
 * @date  02/08/2018
 * @brief  This file provides part of the mmc firmware functions.
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <types.h>
#include <io.h>
#include <sf_mmap.h>
#include <irq_handle.h>
#include <clk.h>
#include <errorno.h>
#include <mmc.h>
#include <malloc.h>

#ifdef CLIB
#include <stdio.h>
#endif


#define COUNT_MAX1 200
static u32 sd_version_2 = 1;
static u32 mmc_rca = 0;
static u32 mmc_no = 0;

extern void *memcpy(void *dest, const void *src, size_t count);
/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to wait the busy bit to be cleared.         *
 *                                                                  *
 ********************************************************************
 */
static void mmc_wait_busy(void)
{
	u32 reg;
	u32 count = 0;

	reg = readl(SDIO0_STATUS + mmc_no * SIFLOWER_SDIO_OFFSET);
	while(reg & ((1<<9)|(1<<10))){
		reg = readl(SDIO0_STATUS + mmc_no * SIFLOWER_SDIO_OFFSET);
		count++;
		u_delay(10);
		if(count > COUNT_MAX1*100){
			printf("mmc%d wait_busy_timeout!!!\n",mmc_no);
			break;
		}
	}

}


/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to wait the start bit to be cleared.        *
 *    it means the CMD is taken by CIU.                             *
 *                                                                  *
 ********************************************************************
 */

static int mmc_wait_START(void)
{
	u32 reg;
	u32 count = 0;

	reg = readl(SDIO0_CMD + mmc_no * SIFLOWER_SDIO_OFFSET);
	while(reg & (1<<31)){
		reg = readl(SDIO0_CMD + mmc_no * SIFLOWER_SDIO_OFFSET);
		count++;
		u_delay(5);
		if(count > COUNT_MAX1){
			return -EINVAL;
		}
	}

	return 0;
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to wait the response tramsmit over.         *
 *    use different ways to make sure that the response is end.     *
 *                                                                  *
 ********************************************************************
 */

static int mmc_wait_response(void)
{
	u32 reg = 0;
	u32 ret = 0;
	u32 count = 0;

	while(!(readl(SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET) & (1 << 2)))
	{
		u_delay(10);
		count++;
		if(count > COUNT_MAX1){
			printf("response wait timeout!!!!\n");
			return -EINVAL;
		}
	}

	reg = readl(SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET);

	writel((1 << 2),SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET);

	if(reg &((1<<1) | (1<<6) | (1<<8)))
	{
		ret = -EIO;
		writel(reg,SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET);
	}

	m_delay(1);

	return ret;
}

static void mmc_send_cmd(u32 arg, u32 cmd_index, u32 cmd_flag)
{
	if(cmd_index == MMC_CMD_GO_IDLE_STATE)
		cmd_flag |= (SDIO0_CMD_INIT | SDIO0_CMD_PRV_DAT_WAIT);
	else if(cmd_index == MMC_CMD_STOP_TRANSMISSION)
		cmd_flag |= (SDIO0_CMD_STOP | SDIO0_CMD_RESP_EXPT);
	else
		cmd_flag |= (SDIO0_CMD_PRV_DAT_WAIT | SDIO0_CMD_RESP_EXPT);

	writel(arg , SDIO0_CMDARG + mmc_no * SIFLOWER_SDIO_OFFSET);//0x1aa
	writel((cmd_index | cmd_flag | SDIO0_CMD_START | SDIO0_CMD_USEHOLD) , SDIO0_CMD + mmc_no * SIFLOWER_SDIO_OFFSET);

}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to read data from the card.                 *
 *    read four bytes one time.                                     *
 *                                                                  *
 ********************************************************************
 */
static void mci_pull_data32( void *buf, int len)
{
	u32 *pdata = (u32 *)buf;

	len = len >> 2;
	while(len > 0) {
		*pdata = readl(SDIO0_DATA + mmc_no * SIFLOWER_SDIO_OFFSET);
		pdata++;
		len--;
	}

}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to enable the power.                        *
 *                                                                  *
 *                                                                  *
 ********************************************************************
 */

static void mmc_power_enable(void)
{

	writel(0x1 , SDIO0_PWREN + mmc_no * SIFLOWER_SDIO_OFFSET);
	u_delay(50);
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to change the clock.                        *
 *                                                                  *
 *                                                                  *
 ********************************************************************
 */

static int mmc_clk_set(u32 div)
{
	u32 reg;
	u32 ret = 0;
	u32 delay = 0;
	u32 clock = 0;

	clock = (mmc_no == 1)?SD_CLOCK:EMMC_CLOCK;

	delay = (2*div*100*10^6)/clock+1;

	reg = readl(SDIO0_CLKENA + mmc_no * SIFLOWER_SDIO_OFFSET);
	reg &= 0xFFFF0000;
	writel(reg , SDIO0_CLKENA + mmc_no * SIFLOWER_SDIO_OFFSET);

	writel(SDIO0_CMD_START | SDIO0_CMD_UPD_CLK | SDIO0_CMD_PRV_DAT_WAIT , SDIO0_CMD + mmc_no * SIFLOWER_SDIO_OFFSET);

	ret = mmc_wait_START();
	if(ret){
		printf("mmc%d clk set error1!!\n",mmc_no);
		return -EINVAL;
	}

	writel(div , SDIO0_CLKDIV + mmc_no * SIFLOWER_SDIO_OFFSET);

	writel(0x0 , SDIO0_CLKSRC + mmc_no * SIFLOWER_SDIO_OFFSET);

	writel(SDIO0_CMD_START | SDIO0_CMD_UPD_CLK | SDIO0_CMD_PRV_DAT_WAIT , SDIO0_CMD + mmc_no * SIFLOWER_SDIO_OFFSET);

	ret = mmc_wait_START();
	if(ret){
		printf("mmc%d clk set error2!!\n",mmc_no);
		return -EINVAL;
	}

	writel((1 << 0) , SDIO0_CLKENA + mmc_no * SIFLOWER_SDIO_OFFSET);                 //config the clock enable register

	writel(SDIO0_CMD_START | SDIO0_CMD_UPD_CLK | SDIO0_CMD_PRV_DAT_WAIT , SDIO0_CMD + mmc_no * SIFLOWER_SDIO_OFFSET);

	ret = mmc_wait_START();
	if(ret){
		printf("mmc%d clk set error3!!\n",mmc_no);
		return -EINVAL;
	}

	u_delay(delay);

	return 0;
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to send the CMD0 to enter the IDLE status.  *
 *                                                                  *
 *                                                                  *
 ********************************************************************
 */

static int mmc_enter_idle(void)
{
	u32 ret = 0;
	u32 count=0;

	mmc_send_cmd(0x0, MMC_CMD_GO_IDLE_STATE, 0);
	//send CMD0 to enter the IDLE status.
	ret = mmc_wait_START();
	if(ret){
		printf("mmc%d enter idle wait start timeout !!!\n",mmc_no);
		return -EINVAL;
	}

	while(count < COUNT_MAX1)
	{
		if(readl(SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET)&(1 << 2))
		{
			writel((1<<2),SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET);
			break;
		}
		u_delay(50);
		count++;
	}

	if(count >= COUNT_MAX1){
		printf("mmc%d enter idle timeout !!!\n",mmc_no);
		return -EINVAL;
	}

	return 0;
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to match the voltage.                       *
 *    1.send the CMD8 to judge the card is high speed or not.       *
 *    2.send CMD55-ACMD41 to match the voltage.                     *
 *                                                                  *
 ********************************************************************
 */
static int mmc_voltage_match(void)
{
	u32 RES_0;
	u32 count = 0;
	u32 arg = 0;

	if(mmc_no == 1){
		//send CMD8 to get sd version and decide if need to set HCS.
		mmc_send_cmd(0x1aa, SD_CMD_SEND_IF_COND, 0);

		mmc_wait_response();

		RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);
		if((RES_0 & 0xff) == 0xaa)
			sd_version_2 = 1;
		else
			sd_version_2 = 0;

		//send ACMD41 to get voltage matched.
		do
		{
			//send CMD55 to tell the card the next CMD is ACMD.
			mmc_send_cmd(0x0, SD_CMD_APP_CMD, 0);

			mmc_wait_response();

			RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

			//send the ACMD41 to match the voltage.(2.7~3.6 V)
			if(sd_version_2)
				arg |= 0xFF8000 | OCR_HCS;
			else
				arg |= 0xFF8000;

			mmc_send_cmd(arg, SD_CMD_APP_SEND_OP_COND, 0);

			mmc_wait_response();

			RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

			u_delay(10);

			count++;
			if(count>COUNT_MAX1){
				printf("voltage match timeout !!!\n");
				break;
			}
		}
		while(!(RES_0 & OCR_BUSY));
	}
	else{
		sd_version_2 = 1;
		//send ACMD41 to get voltage matched.
		do
		{
			mmc_send_cmd(0x40FF8000, MMC_CMD_SEND_OP_COND, 0);

			mmc_wait_response();

			RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

			u_delay(10);

			count++;
			if(count>COUNT_MAX1){
				printf("match the voltage timeout!\n");
				break;
			}
		}
		while(!(RES_0 & OCR_BUSY));
	}

	return RES_0;
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to get the CID by send CMD2 and receive the *
 *  response.                                                       *
 *                                                                  *
 ********************************************************************
 */

static int mmc_get_cid(void)
{
	u32 RES_1,RES_2,RES_3;
	u32 cmd_flag = 0;

	//send CMD2 to get cid
	cmd_flag |= (SDIO0_CMD_RESP_LONG | SDIO0_CMD_RESP_CRC);
	mmc_send_cmd(0x0, SD_CMD_ALL_SEND_CID, cmd_flag);

	mmc_wait_response();

	RES_3 = readl(SDIO0_RESP3 + mmc_no * SIFLOWER_SDIO_OFFSET);//0x3C
	RES_2 = readl(SDIO0_RESP2 + mmc_no * SIFLOWER_SDIO_OFFSET);//0x38
	RES_1 = readl(SDIO0_RESP1 + mmc_no * SIFLOWER_SDIO_OFFSET);//0x34
	/*RES_0 = */readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);//0x30

	if(RES_3 == 0 && RES_2 == 0 && RES_1 == 0)
		return -EINVAL;

	return 0;
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to get the RCA by send CMD3 and receive the *
 *  response.                                                       *
 *                                                                  *
 ********************************************************************
 */

static int mmc_get_rca(void)
{
	u32 RCA;

	//send CMD3 to get rca
	mmc_send_cmd(0x0, SD_CMD_SEND_RELATIVE_ADDR, SDIO0_CMD_RESP_CRC);

	mmc_wait_response();

	RCA = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

	if(mmc_no == 1)
		return RCA;
	else
		return (1 << 16);
}


/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to select the card by CMD7 & RCA and then   *
 *  active or inactive the chosen card.                             *
 *                                                                  *
 ********************************************************************
 */

static int mmc_select_card(u32 RCA)
{
	u32 RES_0;
	u32 arg = 0;

	arg |= RCA & 0xffff0000;

	mmc_send_cmd(arg, SD_CMD_SELECT_CARD, 0);

	mmc_wait_response();
	RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

	return RES_0;
}

static int mmc_card_status_check(u32 RCA)
{
	u32 RES_0;
	u32 arg = 0;

	arg |= RCA & 0xffff0000;
	mmc_send_cmd(arg, SD_CMD_SEND_STATUS, SDIO0_CMD_RESP_CRC);

	mmc_wait_response();
	RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

	return RES_0;
}

static int mmc_card_status_wait(u32 masked, u32 experted)
{
	u32 RES_0;
	u32 count = 0;

	do
	{
		RES_0 = mmc_card_status_check(mmc_rca);
		count++;
		u_delay(10);
		if(count > COUNT_MAX1*10){
			printf("card status wait timeout!\n");
			break;
		}
	}while((RES_0 & masked) != experted);

	return 0;
}

/*!
 ********************************************************************
 *                                                                  *
 * -Description:                                                    *
 *    This part is used to set the bus width of the card and the    *
 *  the controller.                                                 *
 *                                                                  *
 ********************************************************************
 */

static int mmc_set_bus_width()
{
	u32 RES_0 = 0;
	u32 arg = 0;

	if(mmc_no == 1){
		//ACMD6 set bus width 1/4 bit
		//writel((0 << 0) , SDIO0_CTYPE + mmc_no * SIFLOWER_SDIO_OFFSET); //choose 1-bit mode
		writel((1 << 0) , SDIO0_CTYPE + mmc_no * SIFLOWER_SDIO_OFFSET); //choose 4-bit mode

		arg |= (mmc_rca & 0xffff0000);

		mmc_send_cmd(arg, SD_CMD_APP_CMD, 0);

		mmc_wait_response();

		RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

		//writel(0 , SDIO0_CMDARG + mmc_no * SIFLOWER_SDIO_OFFSET);//0x28 1-bit
		mmc_send_cmd(0x2, SD_CMD_APP_SET_BUS_WIDTH, SDIO0_CMD_RESP_CRC);

		mmc_wait_response();

		RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);
	}
	else{
		//writel(0x1 , SDIO0_CTYPE + mmc_no * SIFLOWER_SDIO_OFFSET);   //set controller bus width---four data line
		//arg = (0x3 << 24) | (0xB7 << 16) | (0x1 << 8);
		writel((1 << 16) , SDIO0_CTYPE + mmc_no * SIFLOWER_SDIO_OFFSET);   //set controller bus width---eight data line
		arg = (0x3 << 24) | (0xB7 << 16) | (0x2 << 8);
		//writel(0x0 , SDIO0_CTYPE + mmc_no * SIFLOWER_SDIO_OFFSET);   //set controller bus width---one data line
		//arg = (0x3 << 24) | (0xB7 << 16) | (0x0 << 8);

		//send CMD6 to set the Device bus width
		mmc_send_cmd(arg, MMC_CMD_SWITCH, 0);

		mmc_wait_response();
		/*RES_0 = */readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

		mmc_wait_busy();
	}

	return RES_0;
}


int mmc_initialization(u32 dev_choose)
{
	u32 RCA;
	u32 ret = 0;
	u32 div_1 = 0,div_2 = 0;
	u32 clock = 0;

	mmc_no = dev_choose;
	clock = (mmc_no == 1)?SD_CLOCK:EMMC_CLOCK;

	div_1 = (clock-1)/(MMC_INIT_RATE*2)+1;
	div_2 = (clock-1)/(MMC_WR_RATE*2)+1;

	mmc_power_enable();

	ret = mmc_clk_set(div_1);
	if(ret){
		return -EINVAL;
	}

	ret = mmc_enter_idle();
	if(ret){
		return -EINVAL;
	}

	mmc_voltage_match();

	ret = mmc_get_cid();

	if(ret){
		printf("MMC%d init cid Failed!!!\n",mmc_no);
		return -EINVAL;
	}

	RCA = mmc_get_rca();

	mmc_rca = RCA;

	mmc_select_card(RCA);

	mmc_set_bus_width();

	mmc_clk_set(div_2);

	writel((0x4 << 16)|(0x4 << 0), SDIO0_FIFOTH + mmc_no * SIFLOWER_SDIO_OFFSET);

	if(mmc_no == 1)
		return 0x1234;
	else
		return 0x4321;

}

static int mmc_set_block_length(u32 BLK_len)
{
	u32 RES_0 = 0;

	//CMD16 set the block length .
	mmc_send_cmd(BLK_len, MMC_CMD_SET_BLOCKLEN, SDIO0_CMD_RESP_CRC);

	mmc_wait_response();
	RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

	return RES_0;
}

static int mmc_set_block_count(u32 BLK_count)
{
	u32 RES_0 = 0;

	//CMD23 set the block count .
	mmc_send_cmd(BLK_count, MMC_CMD_SET_BLKCNT, SDIO0_CMD_RESP_CRC);

	mmc_wait_response();
	RES_0 = readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);

	return RES_0;
}

static int mmc_block_read(u32 ADDR,u8 * buf,u32 blk_size,u32 blk_count)
{
	u32 i = 0;
	u32 count = 0;
	u32 byte_count = 0;
	u32 tmp_size = 0;
	u32 cmd_index = 0;

	byte_count = blk_size*blk_count;
	cmd_index = (blk_count == 1)?SD_CMD_READ_SINGLE_BLOCK:SD_CMD_READ_MULTIPLE_BLOCK;

	tmp_size = byte_count;

	writel(blk_size , SDIO0_BLKSIZ + mmc_no * SIFLOWER_SDIO_OFFSET);
	writel(byte_count , SDIO0_BYTCNT + mmc_no * SIFLOWER_SDIO_OFFSET);

	writel((0x200 << 16) | 0x1 , SDIO0_BEP + mmc_no * SIFLOWER_SDIO_OFFSET);

	mmc_send_cmd(ADDR, cmd_index, SDIO0_CMD_DAT_EXP);

	mmc_wait_response();

	count = 0;

	while(byte_count)
	{
		while(readl(SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET) & ((1 << 5)|(1 << 10)))
		{
			writel((1 << 5)|(1 << 10) , SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET);

			if(byte_count < 4)
				break;

			mci_pull_data32(buf + i, 4);
			byte_count -= 4;
			i += 4;
		}

		if(readl(SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET) & (1 << 3))
		{
			writel((1<<3) , SDIO0_RINTSTS + mmc_no * SIFLOWER_SDIO_OFFSET);
			while(!(readl(SDIO0_STATUS + mmc_no * SIFLOWER_SDIO_OFFSET) & (1 << 2)))
			{
				if(byte_count < 4)
					break;

				mci_pull_data32(buf + i, 4);
				byte_count -= 4;
				i += 4;
			}
		}

		if(byte_count < 4)
			break;

		if(byte_count == tmp_size)
		{
			count++;
			u_delay(4);
			if(count > COUNT_MAX1){
				printf("MMC%d block read wait blk_size timeout!\n",mmc_no);
				break;
			}
		}
		else
		{
			count = 0;
			tmp_size = byte_count;
		}
	}

	if(blk_count > 1){
		mmc_send_cmd(0x0, SD_CMD_STOP_TRANSMISSION, 0);

		mmc_wait_response();
		readl(SDIO0_RESP0 + mmc_no * SIFLOWER_SDIO_OFFSET);
	}

	if(count > COUNT_MAX1)
		return -EINVAL;
	else
		return 0;
}

int mmc_data_read(u32 addr, u8 * buf, u32 blk_size , u32 byte_count)
{
	u32 blk_count;
	u32 offset = (sd_version_2 == 1)?blk_size:1;
	u8 *remained_data = 0;
	u32 remained_len = 0;

	if(addr%blk_size)
	{
		remained_data = (u8 *)malloc(blk_size * sizeof(char));
		remained_len = (blk_size - addr%blk_size);
		addr -= (addr%blk_size);
		if(mmc_no == 0){
			mmc_set_block_length(blk_size);
			mmc_set_block_count(1);
		}
		mmc_block_read(addr/blk_size,remained_data,blk_size,1);
		memcpy((void *)buf, (void *)(remained_data + (blk_size - remained_len)), remained_len);
		byte_count -= remained_len;
		addr += blk_size;
		buf += remained_len;
	}

	blk_count = byte_count/blk_size;
	if(byte_count > (blk_count * blk_size))
		blk_count ++;

	if(mmc_no == 0){
		mmc_set_block_length(blk_size);
		mmc_set_block_count(blk_count);
	}

	if(blk_count > 0){
		mmc_block_read(addr/offset,buf,blk_size,blk_count);
		mmc_card_status_wait(0x100, 0x100);
	}

	return 0;
}

