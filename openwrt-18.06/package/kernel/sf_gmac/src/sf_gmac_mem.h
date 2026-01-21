/*
 * =====================================================================================
 *
 *       Filename:  sgmac_mem.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/06/2021 07:29:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#ifndef _SIGMAC_MEM_H_
#define _SIGMAC_MEM_H_
#include <linux/skbuff.h>


// allocate rx memory skb
struct sk_buff *sgmac_dev_alloc_rxskb(int size);

// init rx skb pools
// num: how many skbs we allocate
// size : the skb size of each skb
int sgmac_init_private_rxskbs(struct net_device *dev, uint32_t num, uint32_t size);

void sgmac_deinit_private_rxskbs(void);
void sgmac_print_mem_info(void);
void sgmac_mem_init(void);
#endif
