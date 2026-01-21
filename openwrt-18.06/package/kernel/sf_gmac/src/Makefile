sgmac-y += sf_gmac.o

sgmac-y += yt8521.o

sgmac-$(CONFIG_SFAX8_ESWITCH_DRIVER) += sf_eswitch_ethtool.o
sgmac-$(CONFIG_SFAX8_GMAC_RX_BUFFER_POOL) += sf_gmac_mem.o

obj-$(CONFIG_SFAX8_GMAC) += sgmac.o
