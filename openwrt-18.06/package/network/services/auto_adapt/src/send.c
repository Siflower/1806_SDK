#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <linux/udp.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER) 
#define CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS 80
#define ENABLE_UDHCPC_SLACK_FOR_BUGGY_SERVERS 1
#define MAC_BCAST_ADDR  (uint8_t *) "\xff\xff\xff\xff\xff\xff"
#define BB_LITTLE_ENDIAN 0
#define DHCP_OPTIONS_BUFSIZE    308
#define BOOTREQUEST             1
#define DHCPOFFER               2
#define DHCP_MAGIC              0x63825363
#define DHCP_END                0xff
#define IF_FEATURE_UDHCP_PORT(...)
#define OPT_CODE                0
#define OPT_LEN                 1
#define OPT_DATA                2
#define DHCP_PADDING            0x00
#define DHCP_CLIENT_ID          0x3d
#define DHCP_OPTION_OVERLOAD    0x34
#define DHCP_MESSAGE_TYPE       0x35
#define DHCP_END                0xff
#define LISTEN_RAW    2
#define PACKET_AUXDATA 8
#define FILE_FIELD              1
#define SNAME_FIELD             2


struct dhcp_packet {
	uint8_t op;      /* BOOTREQUEST or BOOTREPLY */
	uint8_t htype;   /* hardware address type. 1 = 10mb ethernet */
	uint8_t hlen;    /* hardware address length */
	uint8_t hops;    /* used by relay agents only */
	uint32_t xid;    /* unique id */
	uint16_t secs;   /* elapsed since client began acquisition/renewal */
	uint16_t flags;  /* only one flag so far: */
#define BROADCAST_FLAG 0x8000 /* "I need broadcast replies" */
	uint32_t ciaddr; /* client IP (if client is in BOUND, RENEW or REBINDING state) */
	uint32_t yiaddr; /* 'your' (client) IP address */
	/* IP address of next server to use in bootstrap, returned in DHCPOFFER, DHCPACK by server */
	uint32_t siaddr_nip;
	uint32_t gateway_nip; /* relay agent IP address */
	uint8_t chaddr[16];   /* link-layer client hardware address (MAC) */
	uint8_t sname[64];    /* server host name (ASCIZ) */
	uint8_t file[128];    /* boot file name (ASCIZ) */
	uint32_t cookie;      /* fixed first four option bytes (99,130,83,99 dec) */
	uint8_t options[388];
} ;

struct ip_udp_dhcp_packet {
	struct iphdr ip;
	struct udphdr udp;
	struct dhcp_packet data;
} ;

struct udp_dhcp_packet {
	struct udphdr udp;
	struct dhcp_packet data;
} ;

enum {
	IP_UDP_DHCP_SIZE = sizeof(struct ip_udp_dhcp_packet) - CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS,
	UDP_DHCP_SIZE    = sizeof(struct udp_dhcp_packet) - CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS,
	DHCP_SIZE        = sizeof(struct dhcp_packet) - CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS,
};

uint16_t inet_cksum(uint16_t *addr, int nleft)
{
	unsigned sum = 0;
	while (nleft > 1) {
		sum += *addr++;
		nleft -= 2;
	}

	if (nleft == 1) {
		if (BB_LITTLE_ENDIAN)
			sum += *(uint8_t*)addr;
		else
			sum += *(uint8_t*)addr << 8;
	}

	sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
	sum += (sum >> 16);                     /* add carry */

	return (uint16_t)~sum;
}

int udhcp_end_option(uint8_t *optionptr)
{
	int i = 0;

	while (optionptr[i] != DHCP_END) {
		if (optionptr[i] != DHCP_PADDING)
			i += optionptr[i + OPT_LEN] + OPT_DATA-1;
		i++;
	}
	return i;
}

int udhcp_read_interface(const char *interface, int *ifindex, uint8_t *mac)
{
	/* char buffer instead of bona-fide struct avoids aliasing warning */
	char ifr_buf[sizeof(struct ifreq)];
	struct ifreq *const ifr = (void *)ifr_buf;

	int fd;
	memset(ifr, 0, sizeof(*ifr));
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

	ifr->ifr_addr.sa_family = AF_INET;
	strncpy(ifr->ifr_name, interface,16);

	if (ifindex) {
		if (ioctl(fd, SIOCGIFINDEX, ifr) != 0) {
			close(fd);
			return -1;
		}
		*ifindex = ifr->ifr_ifindex;
	}

	if (mac) {
		if (ioctl(fd, SIOCGIFHWADDR, ifr) != 0) {
			close(fd);
			return -1;
		}
		memcpy(mac, ifr->ifr_hwaddr.sa_data, 6);
	}

	close(fd);
	return 0;
}

int send_packet(struct dhcp_packet *dhcp_pkt,int ifindex)
{
	int fd;
	struct sockaddr_ll dest_sll;
	struct ip_udp_dhcp_packet packet;
	int result = -1;
	unsigned padding;

	fd=socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
	if(fd==-1){
		printf("socket failed\n");
	}
	memset(&dest_sll, 0, sizeof(dest_sll));
	memset(&packet, 0, offsetof(struct ip_udp_dhcp_packet, data));

	packet.data = *dhcp_pkt;

	dest_sll.sll_family = AF_PACKET;
	dest_sll.sll_protocol = htons(ETH_P_IP);
	dest_sll.sll_ifindex = ifindex;

	dest_sll.sll_halen = 6;
	const uint8_t *dest_arp = MAC_BCAST_ADDR;
	memcpy(dest_sll.sll_addr, dest_arp, 6);

	bind(fd, (struct sockaddr *)&dest_sll, sizeof(dest_sll));

	padding = DHCP_OPTIONS_BUFSIZE - 1 - udhcp_end_option(packet.data.options);
	if (padding > DHCP_SIZE - 300)
		padding = DHCP_SIZE - 300;

	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = INADDR_ANY;
	packet.ip.daddr = INADDR_BROADCAST;
	packet.udp.source = htons(68);
	packet.udp.dest = htons(67);

	packet.udp.len = htons(UDP_DHCP_SIZE - padding);
	packet.ip.tot_len = packet.udp.len;
	packet.udp.check = inet_cksum((uint16_t *)&packet,IP_UDP_DHCP_SIZE - padding);
	packet.ip.tot_len = htons(IP_UDP_DHCP_SIZE - padding);
	packet.ip.ihl = sizeof(packet.ip) >> 2;
	packet.ip.version = IPVERSION;
	packet.ip.ttl = IPDEFTTL;
	packet.ip.check = inet_cksum((uint16_t *)&packet.ip, sizeof(packet.ip));

	result = sendto(fd, &packet, IP_UDP_DHCP_SIZE - padding, /*flags:*/ 0,(struct sockaddr *) &dest_sll, sizeof(dest_sll));
//	close(fd);
	return result;
}

void udhcp_init_header(struct dhcp_packet *packet)
{
	memset(packet, 0, sizeof(*packet));
	packet->op = BOOTREQUEST;
	packet->htype = 1; /* ethernet */
	packet->hlen = 6;
	packet->cookie = htonl(DHCP_MAGIC);
	uint8_t const_options[388] = {0x35,0x01,0x01,0x39,0x02,0x02,0x40,0x37,0x08,0x01,0x03,0x06,0x0c,0x0f,0x1c,0x2a,
								0x79,0x3c,0x0c,0x75,0x64,0x68,0x63,0x70,0x20,0x31,0x2e,0x32,0x39,0x2e,0x33,0x0c,0x07,
								0x4f,0x70,0x65,0x6e,0x57,0x72,0x74,0xff};
	memcpy(packet->options, const_options, 41);
}

static void init_packet(struct dhcp_packet *packet, uint8_t *mac)
{
	udhcp_init_header(packet);
	packet->xid = rand();
	packet->secs = htons(0);
	memcpy(packet->chaddr, mac, 6);
}

static int sockfd = -1;

int setsockopt_int(int fd, int level, int optname, int optval)
{
	return setsockopt(fd, level, optname, &optval, sizeof(int));
}

static int udhcp_raw_socket(int ifindex)
{
	int fd;
	struct sockaddr_ll sock;
	fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
	memset(&sock, 0, sizeof(sock)); /* let's be deterministic */
	sock.sll_family = AF_PACKET;
	sock.sll_protocol = htons(ETH_P_IP);
	sock.sll_ifindex = ifindex;
	bind(fd, (struct sockaddr *) &sock, sizeof(sock));
	setsockopt_int(fd, SOL_PACKET, PACKET_AUXDATA,1);

	return fd;
}

static int udhcp_recv_raw_packet(struct dhcp_packet *dhcp_pkt, int fd)
{
	int bytes;
	struct ip_udp_dhcp_packet packet;
	uint16_t check;
	unsigned char cmsgbuf[CMSG_LEN(sizeof(struct tpacket_auxdata))];
	struct iovec iov;
	struct msghdr msg;
	struct cmsghdr *cmsg;

	iov.iov_base = &packet;
	iov.iov_len = sizeof(packet);
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	for (;;) {
		bytes = recvmsg(fd, &msg, 0);
		if (bytes < 0) {
			if (errno == EINTR)
				continue;
			printf("packet read error, ignoring\n");
			return bytes; /* returns -1 */
		}
		break;
	}

	if (bytes < (int) (sizeof(packet.ip) + sizeof(packet.udp))) {
		printf("packet is too short, ignoring\n");
		return -2;
	}

	if (bytes < ntohs(packet.ip.tot_len)) {
		printf("oversized packet, ignoring\n");
		return -2;
	}

	bytes = ntohs(packet.ip.tot_len);

	if (packet.ip.protocol != IPPROTO_UDP
	 || packet.ip.version != IPVERSION
	 || packet.ip.ihl != (sizeof(packet.ip) >> 2)
	 || packet.udp.dest != htons(68)
	 || ntohs(packet.udp.len) != (uint16_t)(bytes - sizeof(packet.ip))
	) {
		printf("unrelated/bogus packet, ignoring\n");
		return -2;
	}

	/* verify IP checksum */
	check = packet.ip.check;
	packet.ip.check = 0;
	if (check != inet_cksum((uint16_t *)&packet.ip, sizeof(packet.ip))) {
		printf("bad IP header checksum, ignoring\n");
		return -2;
	}

	for (cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
		if (cmsg->cmsg_level == SOL_PACKET
		 && cmsg->cmsg_type == PACKET_AUXDATA
		) {
			struct tpacket_auxdata *aux = (void *)CMSG_DATA(cmsg);
			if (aux->tp_status & TP_STATUS_CSUMNOTREADY)
				goto skip_udp_sum_check;
		}
	}

	memset(&packet.ip, 0, offsetof(struct iphdr, protocol));
	packet.ip.tot_len = packet.udp.len; /* yes, this is needed */
	check = packet.udp.check;
	packet.udp.check = 0;
	if (check && check != inet_cksum((uint16_t *)&packet, bytes)) {
		printf("packet with bad UDP checksum received, ignoring\n");
		return -2;
	}
 skip_udp_sum_check:

	if (packet.data.cookie != htonl(DHCP_MAGIC)) {
		printf("packet with bad magic, ignoring\n");
		return -2;
	}

	bytes -= sizeof(packet.ip) + sizeof(packet.udp);
	memcpy(dhcp_pkt, &packet.data, bytes);
	return bytes;
}

uint8_t* udhcp_get_option(struct dhcp_packet *packet, int code)
{
	uint8_t *optionptr;
	int len;
	int rem;
	int overload = 0;
	enum {
		FILE_FIELD101  = FILE_FIELD  * 0x101,
		SNAME_FIELD101 = SNAME_FIELD * 0x101,
	};

	optionptr = packet->options;
	rem = sizeof(packet->options);
	while (1) {
		if (rem <= 0) {
 complain:
			printf("bad packet, malformed option field\n");
			return NULL;
		}
		if (optionptr[OPT_CODE] == DHCP_PADDING) {
			rem--;
			optionptr++;
			continue;
		}
		if (optionptr[OPT_CODE] == DHCP_END) {
			if ((overload & FILE_FIELD101) == FILE_FIELD) {
				overload |= FILE_FIELD101; /* "we looked at it" */
				optionptr = packet->file;
				rem = sizeof(packet->file);
				continue;
			}
			if ((overload & SNAME_FIELD101) == SNAME_FIELD) {
				overload |= SNAME_FIELD101; /* "we looked at it" */
				optionptr = packet->sname;
				rem = sizeof(packet->sname);
				continue;
			}
			break;
		}

		if (rem <= OPT_LEN)
			goto complain; /* complain and return NULL */
		len = 2 + optionptr[OPT_LEN];
		rem -= len;
		if (rem < 0)
			goto complain; /* complain and return NULL */

		if (optionptr[OPT_CODE] == code) {
			return optionptr + OPT_DATA;
		}

		if (optionptr[OPT_CODE] == DHCP_OPTION_OVERLOAD) {
			if (len >= 3)
				overload |= optionptr[OPT_DATA];
		}
		optionptr += len;
	}
	return NULL;
}

int main(int argc, char* argv[]){
	int result;
	char *iface;
	uint8_t client_mac[6];
	iface = (char *)malloc(sizeof(char));
	iface = argv[1];
	int ifindex;
	udhcp_read_interface(iface,&ifindex,client_mac);
	if(ifindex)
		printf("ifindex : %d\n",ifindex);
	else {
		return 0;
	}
	sockfd = udhcp_raw_socket(ifindex);
	struct dhcp_packet packet;
	init_packet(&packet, client_mac);

	result = send_packet(&packet,ifindex);

	udhcp_recv_raw_packet(&packet, sockfd);
	uint8_t *message;
	message = udhcp_get_option(&packet, DHCP_MESSAGE_TYPE);
	if (*message == DHCPOFFER) {
		char cmd[128];
		sprintf(cmd,"echo '%s' > /tmp/dhcp_iface",iface);
		system(cmd);
		printf("receive dhcp offer in ifindex %d, start setting\n",ifindex);
	} else
		printf("no dhcp offer received\n");
	return result;
}
