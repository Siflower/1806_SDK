#ifndef SF_ETH_H
#define SF_ETH_H
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/ethtool.h>
#include "sf_common.h"

#define SF_VERSION              "0.1"

#define SF_DEFAULT_MSG_ENABLE    \
	(NETIF_MSG_DRV      | \
	NETIF_MSG_PROBE    | \
	NETIF_MSG_LINK     | \
	NETIF_MSG_TIMER    | \
	NETIF_MSG_IFDOWN   | \
	NETIF_MSG_IFUP     | \
	NETIF_MSG_RX_ERR   | \
	NETIF_MSG_TX_ERR)

/* NPU_Soft_Clkgate[7] */
#define NPU_ETH_BYPREF_CLK		(1 << 5) //supply for mac gmii_refclk, not used
#define NPU_ETH_TSU_CLK			(1 << 6) //supply for timestamp unit

#define SF_TX_PKT_INJECT_EN     (1 << 0)
#define SF_TX_LAUNCH_TIME_VALID (1 << 1)
#define SF_TX_PKT_PTP_EN        (1 << 2)
#define SF_EGR_TSR_VALID        (1 << 4)
#define SF_PUNT_VALID           (1 << 5)
#define SF_RX_TS_VALID          (1 << 6)

#define HIF_INT        (1 << 0)
#define HIF_RXBD_INT   (1 << 1)
#define HIF_RXPKT_INT  (1 << 2)
#define HIF_TXBD_INT   (1 << 3)
#define HIF_TXPKT_INT  (1 << 4)

#define HIF_INT_EN  (1 << 0)
#define HIF_RXBD_INT_EN   (1 << 1)
#define HIF_RXPKT_INT_EN  (1 << 2)
#define HIF_TXBD_INT_EN   (1 << 3)
#define HIF_TXPKT_INT_EN  (1 << 4)

#define BMU_INT  (1 << 0)
#define BMU_EMPTY_INT  (1 << 1)
#define BMU_FULL_INT  (1 << 2)
#define BMU_THRES_INT  (1 << 3)

#define BMU_INT_EN  (1 << 0)
#define BMU_EMPTY_INT_EN  (1 << 1)
#define BMU_FULL_INT_EN  (1 << 2)
#define BMU_THRES_INT_EN  (1 << 3)

#define SF_MAX_TX_QUEUES     1

#define SF_TX_HDR_LEN 16
#define SF_RX_HDR_LEN 16

#define CB_INJ_TX_FLAG  41
#define CB_INJ_TX_PORT  42
#define CB_TX_SA_MISS   0x40
// for reset thread
#define RESET_SLEEP_TIME  1000
struct tx_header
{

	uchar rsvd0;
	uchar queue;
	uchar txport_map;
	uchar ctrl;
	ushort ref_num;
	ushort rsvd1;
	uint launch_time_nsec;
	uint launch_time_sec;
};

struct rx_header
{
	ushort punt_reason;
	uchar rxport_num;
	uchar ctrl;
	uint rsvd;
	uint rx_timestamp_nsec;
	uint rx_timestamp_sec;
};
int sf_txbd_update(struct sk_buff *skb, struct sf_priv *priv, int nfrags);
int sf_eth_xmit(struct sk_buff *skb, struct net_device *dev);
void sf_writel(uint value,uint offset);
void sf_writew(uint value,uint offset);
int sf_readl(uint offset);
int sf_eth_stop(struct net_device *dev);
// int sf_log_info();

#endif /* ifndef SF_ETH_H */
