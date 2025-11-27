/*
 * =====================================================================================
 *
 *       Filename:  siwifi_iqengine.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/29/2022 01:57:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */

#ifndef _SIWIFI_IQENGINE_H_
#define _SIWIFI_IQENGINE_H_
#include "siwifi_defs.h"

#define SIWIFI_IQ_USE_RESERVED_ADDR 1
/* define iq engine as recorder mode output data file */
#define  SIWIFI_IQ_OUTPUT_FILE "/tmp/iq_recorder.txt"
/* define iq engine as player mode input data file */
#define SIWIFI_IQ_INPUT_FILE "/tmp/iq_player.txt"
/* define iq engine buffer ddr base address */
#define SIWIFI_IQ_BUFFER_BASE_ADDR 0x02100000
#define SIWIFI_IQ_DEBUG 1

enum {
    IQE_MODE_NONE,
    IQE_BB_PLAYER,
    IQE_RF_PLAYER,
    IQE_BB_RECORDER,
    IQE_RF_RECORDER,
};
int iqe_init(struct siwifi_hw *priv, uint32_t iqlength, int mode);
void iqe_enable(struct siwifi_hw *priv, int enable);
#endif
