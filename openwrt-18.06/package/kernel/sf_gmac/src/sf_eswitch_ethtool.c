/*
* Description
*
*
* Siflower software
*/

#include <linux/mii.h>
#include "sf_gmac.h"
#include "sf_eswitch_ethtool.h"

extern unsigned long g_ethtool_phyad;

static struct {
    const char str[ETH_GSTRING_LEN];
} ethtool_stats_keys[] = {
    { "tx_jabber" },
    { "tx_frame_flushed" },
    { "tx_payload_error" },
    { "tx_ip_header_error" },
    { "tx_local_fault" },
    { "tx_remote_fault" },
    { "rx_watchdog" },
    { "rx_da_filter_fail" },
    { "rx_payload_error" },
    { "rx_ip_header_error" },
    { "tx_process_stopped" },
    { "rx_buf_unav" },
    { "rx_process_stopped" },
    { "tx_early" },
    { "fatal_bus_error" },
};

static int gsw_read_phy_reg(struct sgmac_priv *priv, int phyNo, int phyReg) {
	int phydata = 0;
	phydata = priv->pesw_priv->read_phy(priv->pesw_priv,phyNo, phyReg);
	return phydata;
}

static int gsw_write_phy_reg(struct sgmac_priv *priv, int phyNo, int phyReg, int phydata) {
	priv->pesw_priv->write_phy(priv->pesw_priv,phyNo, phyReg, phydata);
	return 0;
}



static void
gsw_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	struct sgmac_priv *priv = netdev_priv(dev);
	strlcpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_MODULE_VERSION, sizeof(info->version));
	strlcpy(info->bus_info, dev_name(priv->dev), sizeof(info->bus_info));
}

static int
gsw_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct sgmac_priv *priv = netdev_priv(dev);
	int phy_ctrl, phy_status, ctrl1000, stat1000, nego, advert;

	cmd->phy_address = g_ethtool_phyad;
	cmd->supported = (SUPPORTED_10baseT_Half | SUPPORTED_10baseT_Full |
			SUPPORTED_100baseT_Half | SUPPORTED_100baseT_Full |
			SUPPORTED_1000baseT_Full | SUPPORTED_Autoneg |
			SUPPORTED_TP | SUPPORTED_MII);

	cmd->port = PORT_MII;
	cmd->transceiver = XCVR_INTERNAL;
	cmd->mdio_support = ETH_MDIO_SUPPORTS_C22;
	cmd->advertising = ADVERTISED_TP | ADVERTISED_MII;
	phy_ctrl = gsw_read_phy_reg(priv, cmd->phy_address,
			PHY_BASE_CONTROL_REG);
	phy_status = gsw_read_phy_reg(priv, cmd->phy_address,
			PHY_BASE_STATUS_REG);
	ctrl1000 = gsw_read_phy_reg(priv, cmd->phy_address,
			PHY_1000BASE_CTRL_REG);
	stat1000 = gsw_read_phy_reg(priv, cmd->phy_address,
			PHY_1000BASE_STAT_REG);

	if (phy_ctrl & GSW_ANENABLE) {
		cmd->advertising |= ADVERTISED_Autoneg;
		cmd->autoneg = AUTONEG_ENABLE;
		advert = gsw_read_phy_reg(priv, cmd->phy_address,
				PHY_AUTONEG_ADVERTISE_REG);
		cmd->advertising |= mii_lpa_to_ethtool_lpa_t(advert);
		cmd->advertising |= mii_ctrl1000_to_ethtool_adv_t(ctrl1000);

		if (phy_status & GSW_ANEGCOMPLETE) {
			advert = gsw_read_phy_reg(priv, cmd->phy_address,
					PHY_AUTONEG_LINK_PARTNER_REG);
			cmd->lp_advertising |= mii_lpa_to_ethtool_lpa_t(advert);
			cmd->lp_advertising |=
				mii_stat1000_to_ethtool_lpa_t(stat1000);
		} else
			cmd->lp_advertising = 0;

		nego = cmd->advertising & cmd->lp_advertising;
		if (nego & (ADVERTISED_1000baseT_Full |
			ADVERTISED_1000baseT_Half))
		{
			ethtool_cmd_speed_set(cmd, SPEED_1000);
			cmd->duplex = !!(nego & ADVERTISED_1000baseT_Full);
		} else if (nego & (ADVERTISED_100baseT_Full |
			ADVERTISED_100baseT_Half))
		{
			ethtool_cmd_speed_set(cmd, SPEED_100);
			cmd->duplex = !!(nego & ADVERTISED_100baseT_Full);
		} else {
			ethtool_cmd_speed_set(cmd, SPEED_10);
			cmd->duplex = !!(nego & ADVERTISED_10baseT_Full);
		}
	} else {
		cmd->autoneg = AUTONEG_DISABLE;
		ethtool_cmd_speed_set(cmd,
				((phy_ctrl & GSW_SPEED1000 &&
				  (phy_ctrl & GSW_SPEED100) == 0) ?
				SPEED_1000 :
				((phy_ctrl & GSW_SPEED100) ?
				SPEED_100 : SPEED_10)));
		cmd->duplex = (phy_ctrl & GSW_FULLDPLX) ?
			DUPLEX_FULL : DUPLEX_HALF;
	}
	//priv->full_duplex = cmd->duplex;

	return 0;
}

static int
gsw_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct sgmac_priv *priv = netdev_priv(dev);
	int speed = ethtool_cmd_speed(cmd);

	if (speed != SPEED_10 &&
		speed != SPEED_100 &&
		speed != SPEED_1000)
		return -EINVAL;
	if (cmd->duplex != DUPLEX_HALF && cmd->duplex != DUPLEX_FULL)
		return -EINVAL;
	if (cmd->port != PORT_MII)
		return -EINVAL;
	if (cmd->phy_address < 0 || cmd->phy_address > GSW_MAX_PORTS)
		return -EINVAL;
	if (cmd->autoneg != AUTONEG_DISABLE && cmd->autoneg != AUTONEG_ENABLE)
		return -EINVAL;

	if (cmd->autoneg == AUTONEG_ENABLE) {
		uint advert, tmp, phy_val;
		uint advert2 = 0, tmp2 = 0;
		if ((cmd->advertising & (ADVERTISED_10baseT_Half |
					 ADVERTISED_10baseT_Full |
					 ADVERTISED_100baseT_Half |
					 ADVERTISED_100baseT_Full |
					 ADVERTISED_1000baseT_Full)) == 0)
			return -EINVAL;

		advert = gsw_read_phy_reg(priv, cmd->phy_address,
				PHY_AUTONEG_ADVERTISE_REG);
		advert2 = gsw_read_phy_reg(priv, cmd->phy_address,
				PHY_1000BASE_CTRL_REG);
		tmp = advert & ~(ADVERTISE_ALL | ADVERTISE_100BASE4);
		tmp |= ethtool_adv_to_mii_adv_t(cmd->advertising);
		tmp2 |= ethtool_adv_to_mii_ctrl1000_t(cmd->advertising);
		if (advert != tmp) {
			gsw_write_phy_reg(priv, cmd->phy_address,
					PHY_AUTONEG_ADVERTISE_REG, tmp);
			//priv->advertising = tmp;
		}
		if (advert2 != tmp2)
			gsw_write_phy_reg(priv, cmd->phy_address,
					PHY_1000BASE_CTRL_REG, tmp2);

		phy_val = gsw_read_phy_reg(priv, cmd->phy_address,
				PHY_BASE_CONTROL_REG);
		phy_val |= (GSW_ANENABLE | GSW_ANRESTART);
		gsw_write_phy_reg(priv, cmd->phy_address,
				PHY_BASE_CONTROL_REG, phy_val);
		//priv->force_media = 0;
	} else {
		uint tmp, phy_val;
		phy_val = gsw_read_phy_reg(priv, cmd->phy_address,
				PHY_BASE_CONTROL_REG);
		tmp = phy_val & ~(GSW_ANENABLE | GSW_SPEED100 |
				  GSW_SPEED1000 | GSW_FULLDPLX);
		if (speed == SPEED_1000)
			tmp |= GSW_SPEED1000;
		else if (speed == SPEED_100)
			tmp |= GSW_SPEED100;
		if (cmd->duplex == DUPLEX_FULL) {
			tmp |= GSW_FULLDPLX;
			//priv->full_duplex = 1;
		} //else
			//priv->full_duplex = 0;
		if (phy_val != tmp)
			gsw_write_phy_reg(priv, cmd->phy_address,
					PHY_BASE_CONTROL_REG, tmp);

		//priv->force_media = 1;
	}
	return 0;
}

static int gsw_get_sset_count (struct net_device *dev, int sset)
{
    switch (sset) {
    case ETH_SS_STATS:
        return GMAC_NUM_STATS;
    default:
        return -EOPNOTSUPP;
    }
}

static void gsw_get_strings (struct net_device *dev, u32 stringset, u8 *buf)
{
    switch (stringset) {
    case ETH_SS_STATS:
        memcpy(buf, &ethtool_stats_keys, sizeof(ethtool_stats_keys));
        break;
    default:
        BUG();
        break;
    }
}

static void gsw_get_ethtool_stats (struct net_device *dev,
		struct ethtool_stats *estats, u64 *tmp_stats)
{
	struct sgmac_priv *priv = netdev_priv(dev);
	int i = 0;

	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_jabber);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_frame_flushed);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_payload_error);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_ip_header_error);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_local_fault);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_remote_fault);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.rx_watchdog);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.rx_da_filter_fail);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.rx_payload_error);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.rx_ip_header_error);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_process_stopped);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.rx_buf_unav);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.rx_process_stopped);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.tx_early);
	tmp_stats[i++] = le64_to_cpu(priv->xstats.fatal_bus_error);
}

static int gsw_nway_reset(struct net_device *dev)
{
	struct sgmac_priv *priv = netdev_priv(dev);
	int phy_ctrl = 0, ret = -EINVAL;

	phy_ctrl = gsw_read_phy_reg(priv, g_ethtool_phyad,
			PHY_BASE_CONTROL_REG);
	if (phy_ctrl & GSW_ANENABLE) {
		phy_ctrl |= GSW_ANRESTART;
		gsw_write_phy_reg(priv, g_ethtool_phyad,
				PHY_BASE_CONTROL_REG, phy_ctrl);
		ret = 0;
	}

	return ret;
}

static void gsw_get_ringparam(struct net_device *dev,
		struct ethtool_ringparam *ring)
{
    ring->rx_max_pending = DMA_RX_RING_SZ;
    ring->tx_max_pending = DMA_TX_RING_SZ;
    ring->rx_pending = DMA_RX_RING_SZ;
    ring->tx_pending = DMA_TX_RING_SZ;
}

struct ethtool_ops eswitch_ethtool_ops = {
	.get_settings		= gsw_get_settings,
	.set_settings		= gsw_set_settings,
	.get_drvinfo		= gsw_get_drvinfo,
	.get_sset_count		= gsw_get_sset_count,
	.get_ethtool_stats	= gsw_get_ethtool_stats,
	.get_strings		= gsw_get_strings,
	.get_link		= ethtool_op_get_link,
	.nway_reset		= gsw_nway_reset,
	.get_ringparam		= gsw_get_ringparam,
};
