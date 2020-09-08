sfax8eth-y += sf_eth.o
sfax8eth-y += sf_debug.o
sfax8eth-y += sf_switch.o
sfax8eth-y += sf_switch_hash_table.o
sfax8eth-y += sf_switch_init.o
sfax8eth-y += sf_mdio.o
sfax8eth-y += sf_ethtool.o

sfax8eth-$(CONFIG_SFAX8_PTP) += sf_ptp.o
sfax8eth-$(CONFIG_SFAX8_SWITCH_API) += sf_api.o

obj-$(CONFIG_SFAX8_SWITCH)			  += sfax8eth.o
