#include <linux/ethtool.h>
#include "sf_common.h"

static uint sf_get_msg_type(struct net_device *dev){
	struct sf_priv *priv = netdev_priv(dev);
	return priv->msg_enable;
}
static void sf_set_msg_type(struct net_device *dev, u32 value) {
	struct sf_priv *priv = netdev_priv(dev);
	priv->msg_enable = value;
}
struct ethtool_ops sf_ethtool_ops = {
	.begin			= NULL,
	.complete		= NULL, /* fp_ethtool_complete */
	.get_settings	= NULL, /* fp_get_settings */
	.set_settings	= NULL, /* fp_set_settings */
	.get_msglevel	= sf_get_msg_type,
	.set_msglevel	= sf_set_msg_type,
};
