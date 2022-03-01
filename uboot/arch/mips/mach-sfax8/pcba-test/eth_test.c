#include "../drivers/net/sfa18_mac.h"
#include <mach/pcba_test.h>
#include <common.h>
#include "net.h"
#include <netdev.h>
#include <asm/io.h>

enum eth_test_type {
	 ETH_1_PORT_TEST = 0,
	 ETH_2_PORT_TEST,
	 ETH_3_PORT_TEST,
	 ETH_CONNECT_TEST,
};

struct eth_test_param {
	int test_type;
	int reset;
	int port[SF_MAC_PORTS];
};

extern struct sf_priv    *g_priv;
extern int add_default_mac_entry(unsigned char *mac, int port);

unsigned char eth_test_pkt[PCBA_PKT_SIZE] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x84, 0xc2, 0xe4, 0x14, 0x55, 0xfb,
#ifdef CONFIG_TARGET_SFA18_86V
	0x81, 0x00, 0x00, 0x02,
#endif
	0x08, 0x06, 0x00, 0x01,
	0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x84, 0xc2,
	0xe4, 0x14, 0x55, 0xfb, 0xc0, 0xa8, 0x04, 0x05,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8,
	0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

int port_test(struct eth_test_param *param){
	int i = 0, ret = 0;

	g_priv->test_result = 0;
	ret = sf_eth_send(g_priv->dev, (void *)eth_test_pkt, sizeof(eth_test_pkt));
	if(ret == 0)
	  printf("npu test: send packet from tx port_map 0x%x\n", g_priv->txport);
	else
	  return -1;

	while(i < 50){
		udelay(10*1000);
		ret = sf_eth_recv(g_priv->dev);
		if(g_priv->test_result == 1){
			printf("npu test: receive from rx port number %d success\n", g_priv->rxport);
			return 0;
		}
		i++;
	}

	return -1;
}

int npu_test(void *d){
	int ret = 0, result = 0;
#ifdef CONFIG_TARGET_SFA18_86V
	char mac[6] = {0};
#endif
	struct eth_test_param *param = (struct eth_test_param *)d;

	printf("======== Start npu_test: test type %d ===========\n", param->test_type);
	g_priv->pcba_npu_test = 1;
	if (param->reset){
		ret = sf_phy_reset_for_low_power();
		if (ret < 0)
			return CMD_FAIL;
	}
	sf_eth_init(NULL,NULL);

	switch (param->test_type){
		case ETH_1_PORT_TEST:
			// test for port0 --> port0
			g_priv->txport = 1 << param->port[0];
			g_priv->rxport = param->port[0];
			ret = port_test(param);
			if(ret < 0)
			  result = -1;
			break;

		case ETH_2_PORT_TEST:
			// test for port0 --> port1
			g_priv->txport = 1 << param->port[0];
			g_priv->rxport = param->port[1];
#ifdef CONFIG_TARGET_SFA18_86V
			// wait phy reset completely
			if (param->reset)
				mdelay(500);
			eth_test_pkt[7] = (unsigned char)0x10; //send arp pkt with differnt SA
			strncpy(mac, (const char *)(eth_test_pkt + 6), 6);
			add_default_mac_entry((unsigned char *)mac, param->port[0]);
#endif

			ret = port_test(param);
			if(ret < 0){
				result = -1;
				break;
			}

			// test for port1 --> port0
			g_priv->txport = 1 << param->port[1];
			g_priv->rxport = param->port[0];
#ifdef CONFIG_TARGET_SFA18_86V
			eth_test_pkt[7] = (unsigned char)0x11; //send arp pkt with differnt SA
			strncpy(mac, (const char *)(eth_test_pkt + 6), 6);
			add_default_mac_entry((unsigned char *)mac, param->port[1]);
#endif
			ret = port_test(param);
			if(ret < 0)
			  result = -1;
			break;

		case ETH_3_PORT_TEST:
			// test for port0 --> port1
			g_priv->txport = 1 << param->port[0];
			g_priv->rxport = param->port[1];
			eth_test_pkt[0] = (unsigned char)0x10;
			// wait phy reset completely
			if (param->reset)
				mdelay(500);
			ret = port_test(param);
			if(ret < 0){
				result = -1;
				break;
			}

			// test for port0 --> port2
			g_priv->txport = 1 << param->port[0];
			g_priv->rxport = param->port[2];
			eth_test_pkt[0] = (unsigned char)0x11;
			ret = port_test(param);
			if(ret < 0)
			  result = -1;
			break;

		case ETH_CONNECT_TEST:
			result = -1;
			break;

		default:
			result = -1;
			break;
	}

#ifdef CONFIG_SFAX8_DEFAULT_TX_PORT
	g_priv->txport = 1 << CONFIG_SFAX8_DEFAULT_TX_PORT;
#endif
	g_priv->pcba_npu_test = 0;

	if (result == 0){
		printf("======== End npu test, test success =========\n");
		return CMD_SUCCESS;
	}else{
		printf("======== End npu test, test fail =========\n");
		return CMD_FAIL;
	}
}
