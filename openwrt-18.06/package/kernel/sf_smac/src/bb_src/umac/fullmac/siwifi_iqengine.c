/*
 * =====================================================================================
 *
 *       Filename:  iqengine.c
 *
 *    Description: q
 *
 *        Version:  1.0
 *        Created:  11/29/2022 09:54:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include "siwifi_mod_params.h"
#include "reg_access.h"
#include "siwifi_mem.h"
#include "siwifi_iqengine.h"

static int iqe_buffer[SIWIFI_IQ_BUFFER_MAX];
static irqreturn_t iqe_handle_irq(int irq, void *dev_id)
{
    uint32_t value;
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)dev_id;
    value = REG_PL_RD8(0xB9E03C94);
    if (value & 0xC)
    {
        if (value == 4)
            printk("Recorder over, 0x%x\n", value);
        else if (value == 8)
            printk("Player over, 0x%x\n", value);
        siwifi_hw->iqe.error = value;
        //clear the iq error
        REG_PL_WR8(0xB9E03C98, 0xC);
    }
    return IRQ_HANDLED;
}
/**
 *  func: init iq engine to make it can record or player iq while combined with RF or bb
 *  params:
 *      <priv>          siwifi_hw context
 *      <iqlength>      how many iq we need to play or record
 *      <mode>          IQE_BB_PLAYER   baseband player
 *                      IQE_RF_PLAYER   rf player
 *                      IQE_BB_RECORDER baseband recorder
 *                      IQE_RF_RECORDER rf recorder
 *  return:
 *      0  initilization is OK, otherwise failure
 */
int iqe_init(struct siwifi_hw *priv, uint32_t iqlength, int mode)
{
    struct siwifi_iqe *iqe = &priv->iqe;
    char  *buf;
    u32 iq_buffer_phy;
    int ret;
    // check if it is already initialized
    if (iqe->init) {
        printk("iqe has bene already initialized\n");
        return 0;
    }
    buf = (void *)iqe_buffer;
    memset(buf, 0, iqlength);
    if (!buf)
    {
        printk("can not alloc memory with size : %d\n", iqlength);
        return -1;
    }
    iq_buffer_phy = dma_map_single(priv->dev, buf, iqlength, DMA_BIDIRECTIONAL);
    if (dma_mapping_error(priv->dev, iqe->iq_buffer_phy)) {
        printk("DMA mapping failed\n");
        siwifi_kfree(buf);
        return -1;
    }
    iqe->iq_buffer = (void*)buf;
    iqe->iq_buffer_len = iqlength;
    iqe->iq_buffer_phy = iq_buffer_phy;
    iqe->mode = mode;
    printk("phy addr:%x buf addr: %p length: %d\n", iqe->iq_buffer_phy, iqe->iq_buffer, iqlength);
    // load iq text into allocated memory
    // if we are recorder, write the iq to file system
    if (iqe->mode == IQE_BB_PLAYER || iqe->mode == IQE_RF_PLAYER)
    {
        mm_segment_t old_fs;
        loff_t pos = 0;
        uint32_t iq_pairs = 0;
        char wbuf[32];
        uint16_t *iq_buf = NULL;
        uint32_t i_data, q_data;
        struct file *file = filp_open(SIWIFI_IQ_INPUT_FILE, O_RDWR | O_CREAT, 0);
        if (IS_ERR(file)) {
            printk("can not open file %s\n", SIWIFI_IQ_INPUT_FILE);
            siwifi_kfree(buf);
            return -1;
        }
        //make should the iq_buffer_len should be the 4 * iq_pairs
        old_fs = get_fs();
        set_fs(KERNEL_DS);
        // write the IQ file line by line to output file
        iq_buf = (uint16_t *)iqe->iq_buffer;
        do {
            //0x1234 0x5678\n should be 14 bytes per line
            //vfs_read(file, wbuf, 14, &pos);
            kernel_read(file, wbuf, 14, &pos);
            sscanf(wbuf, "0x%x 0x%x\n", &i_data, &q_data);
            iq_buf[iq_pairs] = i_data;
            iq_buf[iq_pairs + 1] = q_data;
            iq_pairs += 2;
            memset(wbuf, 0, 32);
        } while(iq_pairs < (iqe->iq_buffer_len >> 1));
        printk("read %lld bytes into file\n", pos);
        set_fs(old_fs);
        filp_close(file, 0);
        dma_sync_single_for_device(priv->dev, iqe->iq_buffer_phy,
                iqlength, DMA_BIDIRECTIONAL);
    }
    //1, config fe clock as recorder' write clock
    //2, config b2m3 bus0 clock as recorder's read clock
    if (!priv->mod_params->is_hb)
        REG_PL_WR(0xB1100874, 0x01000000 | REG_PL_RD(0xB1100874));
    else
        REG_PL_WR(0xB7900874, 0x01000000 | REG_PL_RD(0xB7900874));
    //bit0 is b2m3 bm_clock[0]
    REG_PL_WR8(0xB9E08804, 0x1 | REG_PL_RD8(0xB9E08804));
    //2, config iq engine clear register, hold
    REG_PL_WR8(0xB9E03CA4,0xF);
    //3, release iq engine dma reset
    REG_PL_WR8(0xB9E03CA8, 0x1);
    //4, release iq engine buffer reset
    REG_PL_WR8(0xB9E03CAC, 0x1);
    //5, config iq engine clear register, release
    REG_PL_WR8(0xB9E03CA4, 0x0);
    //6, config IQ engine mode register
    // 3'b000 data from engine to ddr
    // 3'b001 data from ddr to engine
    // 3'b100 data from la to ddr
    if (mode == IQE_BB_PLAYER || mode == IQE_RF_PLAYER) {
        REG_PL_WR8(0xB9E03C00, 0x1);
    } else {
        REG_PL_WR8(0xB9E03C00, 0x0);
    }
    //7, config ddr source address(iq should be load already)
    if (mode == IQE_BB_PLAYER || mode == IQE_RF_PLAYER) {
        REG_PL_WR8(0xB9E03C14, iqe->iq_buffer_phy & 0xFF);
        REG_PL_WR8(0xB9E03C18, (iqe->iq_buffer_phy & 0xFF00) >> 8);
        REG_PL_WR8(0xB9E03C1C, (iqe->iq_buffer_phy & 0xFF0000) >> 16);
        REG_PL_WR8(0xB9E03C20, ((iqe->iq_buffer_phy + SIWIFI_IQ_BASE_ADDR) & 0xFF000000) >> 24);
    } else {
        REG_PL_WR8(0xB9E03C04, iqe->iq_buffer_phy & 0xFF);
        REG_PL_WR8(0xB9E03C08, (iqe->iq_buffer_phy & 0xFF00) >> 8);
        REG_PL_WR8(0xB9E03C0C, (iqe->iq_buffer_phy & 0xFF0000) >> 16);
        REG_PL_WR8(0xB9E03C10, ((iqe->iq_buffer_phy + SIWIFI_IQ_BASE_ADDR) & 0xFF000000) >> 24);
    }
    //8, config iq overflow interrupt
    iqe->iq_irq = platform_get_irq(priv->plat->pdev, 2);
    if (iqe->iq_irq == -ENXIO) {
        printk("can not get the iqengine irq from system!\n");
        return -ENXIO;
    }
    ret = request_irq(iqe->iq_irq, iqe_handle_irq, IRQF_TRIGGER_HIGH, "umac-lb", priv);
    if (ret) {
        printk("can not request the iqe irq, return %d!\n", ret);
        return -1;
    }
    //bit3 or bit2
    REG_PL_WR8(0xB9E03C90, 0xC);
    //9, config iq length
    REG_PL_WR8(0xB9E03C2C, iqlength & 0xFF);
    REG_PL_WR8(0xB9E03C30, (iqlength & 0xFF00) >> 8);
    REG_PL_WR8(0xB9E03C34, (iqlength & 0xFF0000) >> 16);
    REG_PL_WR8(0xB9E03C38, (iqlength & 0xFF000000) >> 24);
    //10, config IQ_CONF register
    // 3'b0XX bb<=>RF
    // 3'b100 player<=>RF
    // 3'b101 bb<=rf\ recorder<=rf
    // 3'b110 bb<=player
    // 3'b111 bb=>recorder
    if (mode == IQE_BB_PLAYER) {
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C440 : 0xB9E08040, 0x6);
    } else if (mode == IQE_RF_PLAYER) {
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C440 : 0xB9E08040, 0x4);
    } else if (mode == IQE_BB_RECORDER) {
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C440 : 0xB9E08040, 0x7);
    } else if (mode == IQE_RF_RECORDER) {
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C440 : 0xB9E08040, 0x5);
        //config IQ_ENGINE_BB_RECEIVE_DATA
        REG_PL_WR8(0xB9E03C60, priv->mod_params->is_hb ? 0x10 : 0x01);
    } else {
        printk("invalid iqe mode, mode = 0x%x\n", mode);
        return -1;
    }
    if (mode == IQE_BB_PLAYER || mode == IQE_RF_PLAYER) {
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C454 : 0xB9E08054, 0x2);
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C458 : 0xB9E08058, 0xD);
    } else {
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C45C : 0xB9E0805C, 0x2);
        REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C460 : 0xB9E08060, 0xD);
    }
    iqe->init = true;
    return 0;
}
void iqe_enable(struct siwifi_hw *priv, int enable)
{
    struct siwifi_iqe *iqe = (struct siwifi_iqe *)&priv->iqe;
    printk("enable : %d, iqe mode:%d, length: %d, addr:%x , error:%d\n",
            enable, iqe->mode, iqe->iq_buffer_len, iqe->iq_buffer_phy, iqe->error);
    if (enable) {
        REG_PL_WR8(0xB9E03C28, 0x1);
        return ;
    }
    // mark the flag as invalid
    iqe->init = false;
    //stop iqe
    REG_PL_WR8(0xB9E03C28, 0x0);
    REG_PL_WR8(priv->mod_params->is_hb ? 0xB9E0C440 : 0xB9E08040, 0x0);
    // free irq
    free_irq(iqe->iq_irq, priv);
    dma_unmap_single(priv->dev, iqe->iq_buffer_phy, iqe->iq_buffer_len, DMA_TO_DEVICE);
    // if we are recorder, write the iq to file system
    if (iqe->mode == IQE_BB_RECORDER ||
            iqe->mode == IQE_RF_RECORDER)
    {
        mm_segment_t old_fs;
        loff_t pos = 0;
        uint32_t iq_pairs = 0;
        char wbuf[32] = "\0";
        uint32_t *iq_buf = NULL;
        struct file *file = filp_open(SIWIFI_IQ_OUTPUT_FILE, O_RDWR | O_CREAT, 0777);
        if (IS_ERR(file)) {
            printk("can not open file %s\n", SIWIFI_IQ_OUTPUT_FILE);
            goto RELEASE;
        }
        old_fs = get_fs();
        set_fs(KERNEL_DS);
        // write the IQ file line by line to output file
        iq_buf = (uint32_t *)iqe->iq_buffer;
        do {
            sprintf(wbuf, "0x%08x\n", iq_buf[iq_pairs]);
            kernel_write(file, wbuf, strlen(wbuf), &pos);
            iq_pairs += 1;
#ifdef SIWIFI_IQ_DEBUG
            if (iq_pairs < 50)
                printk("[%d] : %s", iq_pairs, wbuf);
#endif
            memset(wbuf, 0, 32);
        } while(iq_pairs < (iqe->iq_buffer_len >> 2));
        printk("write %lld bytes into file\n", pos);
        set_fs(old_fs);
        filp_close(file, NULL);
    }
RELEASE:
    //free IQ memory
    if (iqe->iq_buffer)
    {
        iqe->iq_buffer = NULL;
        iqe->iq_buffer_phy = 0;
    }
}
