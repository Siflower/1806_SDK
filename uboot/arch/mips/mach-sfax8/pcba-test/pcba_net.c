#include <mach/pcba_test.h>
#include <mach/pcba_net.h>
#include <common.h>
#include "net.h"
#include <netdev.h>

static int pcba_port = 0;
static int pcba_remote_port = 0;
int rxmsg_ok = 0;
int rxmsg_len = 0;
uchar rxbuf[1024];
struct in_addr pcba_remote_ip;

void pcba_rx_handler(uchar *pkt, unsigned dest, struct in_addr sip,
		unsigned src, unsigned len)
{
	pcba_port = dest;
	pcba_remote_port = src;

	if (len <= 0){
		rxmsg_ok = 0;
		return;
	}

	memcpy(rxbuf, pkt, len);
	rxmsg_len = len;
	rxmsg_ok = 1;
	return;
}

int pcba_send(uchar *msg, int len){
	uchar *pkt;

	pkt = net_tx_packet + net_eth_hdr_size() + IP_UDP_HDR_SIZE;

	memcpy(pkt, msg, len);
	return net_send_udp_packet(net_server_ethaddr, pcba_remote_ip,
				    pcba_remote_port, pcba_port, len);
}

int pcba_recv(unsigned char *buf, int buf_len){
	int len, i=0;

	while(1){
		eth_rx();
		if(rxmsg_ok == 1){
			rxmsg_ok = 0;
			len = buf_len < rxmsg_len ? buf_len : rxmsg_len;
			memcpy(buf, rxbuf, len);
			return len;
		}
		if(i < 20000){
			udelay(25);
			i++;
		}else{
			return 0;
		}
	}
}

int pcba_net_init(void *v){
	struct in_addr lip;
	struct in_addr rip;
	int ret;

	printf("start init pcba net\n");
	lip = string_to_ip("0.0.0.0");
	net_copy_ip(&net_ip, &lip);
	rip = string_to_ip("0.0.0.0");
	net_copy_ip(&pcba_remote_ip, &rip);

	bootstage_mark_name(BOOTSTAGE_ID_ETH_START, "eth_start");
	net_init();
	if (eth_is_on_demand_init()) {
		eth_halt();
		eth_set_current();
		ret = eth_init();
		if (ret < 0) {
			eth_halt();
			return ret;
		}
	} else {
		eth_init_state_only();
	}

	if (eth_get_dev())
		memcpy(net_ethaddr, eth_get_ethaddr(), 6);

	net_set_udp_handler(pcba_rx_handler);

	return 0;
}

void pcba_net_register(void){
	struct connect_interface eth = {
		.connect = pcba_net_init,
		.recv = pcba_recv,
		.send = pcba_send,
		.type = SF_NPU,
		.priority = 1,
		.data = NULL,
	};

	int ret = register_interface(&eth);
	if(ret < 0){
		printf("register eth inferface error\n");
	}
	return;
}
