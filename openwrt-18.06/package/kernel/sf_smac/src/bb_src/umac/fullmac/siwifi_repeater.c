/*
 * Three address repeater
 */
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/if_vlan.h>
#include <linux/list.h>
#include "siwifi_repeater.h"
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
#include <linux/ipv6.h>
#include <net/ipv6.h>
#endif
#ifdef CONFIG_REPEATER_DEBUG
#define repeater_dbg(fmt, args...) printk("[%s:%d] DEBUG: " fmt, __func__, __LINE__, ##args)
#else
#define repeater_dbg(fmt, args...)
#endif
#define repeater_err(fmt, args...) printk("[%s:%d] ERR: " fmt, __func__, __LINE__, ##args)
#define ETHER_PROTO_LEN     2   /* ether proto len */
#define ARP_OPC_OFFSET      6   /* option code offset */
#define ARP_SRC_ETH_OFFSET  8   /* src h/w address offset */
#define ARP_SRC_IP_OFFSET   14  /* src IP address offset */
#define ARP_TGT_ETH_OFFSET  18  /* target h/w address offset */
#define ARP_TGT_IP_OFFSET   24  /* target IP address offset */
#define ARP_OPC_REQUEST     1   /* ARP request */
#define ARP_OPC_REPLY       2   /* ARP reply */
#define ARP_DATA_LEN        28  /* ARP data length */
#define DHCP_PORT_SERVER    67  /* DHCP server UDP port */
#define DHCP_PORT_CLIENT    68  /* DHCP client UDP port */
/* DHCP params */
#define DHCP_TYPE_OFFSET    0   /* DHCP type (request|reply) offset */
#define DHCP_TID_OFFSET     4   /* DHCP transition id offset */
#define DHCP_FLAGS_OFFSET   10  /* DHCP flags offset */
#define DHCP_CIADDR_OFFSET  12  /* DHCP client IP address offset */
#define DHCP_YIADDR_OFFSET  16  /* DHCP your IP address offset */
#define DHCP_GIADDR_OFFSET  24  /* DHCP relay agent IP address offset */
#define DHCP_CHADDR_OFFSET  28  /* DHCP client h/w address offset */
#define DHCP_OPT_OFFSET     236 /* DHCP options offset */
#define DHCP_FLAGS_LEN      2   /* DHCP flags field length */
#define DHCP_FLAG_BCAST 0x8000  /* DHCP broadcast flag */
#define REPEATER_STA_AGING_TIME (5 * 60)  // 300s
#define SNAP_HDR_LEN    6   /* 802.3 SNAP header length */
#define IPV4_MCAST_TO_ETHER_MCAST(ipv4, ether) \
{ \
    ether[0] = 0x01; \
    ether[1] = 0x00; \
    ether[2] = 0x5E; \
    ether[3] = (ipv4 & 0x7f0000) >> 16; \
    ether[4] = (ipv4 & 0xff00) >> 8; \
    ether[5] = (ipv4 & 0xff); \
}
#define REPEATER_STA_HASH_IP(ip, ipver, len) ((ip)[g_repeater_ip_len[ipver] - 1] & (len - 1))


static DEFINE_SPINLOCK(g_repeater_lock);
static struct proc_dir_entry *g_repeater_proc = NULL;
struct list_head g_rp_info_list = LIST_HEAD_INIT(g_rp_info_list);
static const u8 g_repeater_ip_len[IPTYPE] = {
    4,
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
    16,
    16
#endif
};
static u8 g_ipv4_null[] = {0x00, 0x00, 0x00, 0x00};
/* 802.3 llc/snap header */
static u8 g_llc_snap_hdr[SNAP_HDR_LEN] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00};
static u8 g_llc_hdr_stp[SNAP_HDR_LEN] = {0x42, 0x42, 0x03, 0x00, 0x00, 0x00};

/* L3 */
typedef struct prot_hdlr prot_hdlr_t;
struct prot_hdlr {
    u16 type;   /* ether type */
    prot_proc_t prot_proc;
};
static struct prot_hdlr repeater_prot_handle[] = {
    {ETH_P_IP, repeater_ip_proc}, /* 0x0800 */
    {ETH_P_ARP, repeater_arp_proc},   /* 0x0806 */
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
    {ETH_P_IPV6, repeater_ipv6_proc}, /* 0x86DD */
#endif
};
typedef struct ipv4_hdlr ipv4_hdlr_t;
struct ipv4_hdlr {
    u8 type;     /* protocol type */
    ipv4_proc_t ipv4_proc;
};
/* L4 */
static ipv4_hdlr_t repeater_ipv4_handle[] = {
    {IPPROTO_UDP, repeater_udp_proc},  /* 0x11 */
};
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
typedef struct ipv6_hdlr ipv6_hdlr_t;
struct ipv6_hdlr {
    u8 type;    /* protocol type */
    ipv6_proc_t ipv6_proc;
};
static ipv6_hdlr_t repeater_ipv6_handle[] = {
    {NEXTHDR_ICMP, repeater_ipv6_icmp_proc}, /* 58 */
    {NEXTHDR_UDP, repeater_ipv6_udp_proc},
};
#endif
typedef struct udp_hdlr udp_hdlr_t;
struct udp_hdlr {
    u16 port;    /* udp dest. port */
    udp_proc_t udp_proc;
};

/* L7 */
static udp_hdlr_t repeater_udp_handle[] = {
    {DHCP_PORT_SERVER, repeater_dhcpc_proc}, /* 0x43 */
    {DHCP_PORT_CLIENT, repeater_dhcps_proc}, /* 0x44 */
};
/*
 * Adjust 16 bit checksum - taken from RFC 3022.
 *
 */
static void repeater_fix_csum(u8 *chksum, u8 *optr, int olen, u8 *nptr, int nlen)
{
    long x, old, new;
    x = (chksum[0] << 8) + chksum[1];
    if (!x) {
        return;
    }
    x = ~x & 0xFFFF;
    while (olen) {
        old = (optr[0]<< 8) + optr[1];
        optr += 2;
        x -= old & 0xffff;
        if (x <= 0) {
            x--;
            x &= 0xffff;
        }
        olen -= 2;
    }
    while (nlen) {
        new = (nptr[0] << 8) + nptr[1];
        nptr += 2;
        x += new & 0xffff;
        if (x & 0x10000) {
            x++;
            x &= 0xffff;
        }
        nlen -= 2;
    }
    x = ~x & 0xFFFF;
    chksum[0] = (u8)(x >> 8);
    chksum[1] = (u8)x;
}
static int inline repeater_hash_by_mac(unsigned char *mac, int max_len)
{
    return (mac[3] ^ mac[4] ^ mac[5]) / (max_len - 1);
}
static struct repeater_sta *repeater_find_sta_mac(struct repeater_sta *sta, unsigned char *mac)
{
    struct repeater_sta *tmp = NULL;
    for (tmp = sta; tmp; tmp = tmp->mac_link) {
        if (ether_addr_equal(tmp->mac, mac)) {
            tmp->last_use = jiffies;
            return tmp;
        }
    }
    return NULL;
}

static struct repeater_sta *repeater_alloc_sta(void)
{
    struct repeater_sta *sta = NULL;
    sta = kzalloc(sizeof(struct repeater_sta), GFP_ATOMIC);
    return sta;
}
static int repeater_update_sta_mac(struct repeater_info *rp_info, unsigned char *mac, struct repeater_sta **out)
{
    int hash = repeater_hash_by_mac(mac, rp_info->arr_len);
    struct repeater_sta *sta = NULL;
    sta = repeater_find_sta_mac(rp_info->mac_head[hash], mac);
    if (sta) {
        *out = sta;
        return 0;
    }
    if (rp_info->sta_num >= rp_info->max_sta) {
        return -1;
    }
    sta = repeater_alloc_sta();
    if (!sta) {
        return -1;
    }
    sta->last_use = jiffies;
    rp_info->sta_num++;
    memcpy(sta->mac, mac, ETH_ALEN);
    sta->mac_link = rp_info->mac_head[hash];
    rp_info->mac_head[hash] = sta;
    *out = sta;
    return 0;
}
static void repeater_del_ip_link(struct repeater_info *rp_info, struct repeater_sta *sta, int ip_ver)
{
    int ip_hash = REPEATER_STA_HASH_IP(sta->ip[ip_ver], ip_ver, rp_info->arr_len);
    struct repeater_sta **next = &rp_info->ip_head[ip_ver][ip_hash];
    if (NULL == rp_info->ip_head[ip_ver][ip_hash] ||
        (IPVER_4 == ip_ver && !memcmp(sta->ip[ip_ver], g_ipv4_null, g_repeater_ip_len[ip_ver]))) {
        return;
    }
    while (*next != sta && *next != NULL) {
        next = &((*next)->ip_link[ip_ver]);
    }
    *next = sta->ip_link[ip_ver];
    sta->ip_link[ip_ver] = NULL;
}

static int repeater_update_sta_all(struct repeater_info *rp_info,  unsigned char *mac, u8 *ip, int ip_ver)
{
    int mac_hash = 0, ip_hash = 0;
    struct repeater_sta *sta = NULL;
    int update_ip = 0;
    struct repeater_sta *prev = NULL, *sta2 = NULL;
    if (ip_ver == IPVER_4 && *(u32 *)ip == 0 ) {
        repeater_dbg("ipv4 is null, dont add MAC/IP entry.\n");
        return 0;
    }
    mac_hash = repeater_hash_by_mac(mac, rp_info->arr_len);
    sta = repeater_find_sta_mac(rp_info->mac_head[mac_hash], mac);
    if (sta) {
        if (!memcmp(sta->ip[ip_ver], ip, g_repeater_ip_len[ip_ver])) {
            goto next;
        }
        repeater_del_ip_link(rp_info, sta, ip_ver);
        update_ip = 1;
    } else if (rp_info->sta_num < rp_info->max_sta) {
        sta = repeater_alloc_sta();
        if (sta) {
            repeater_dbg("Alloc sta %pM \n", &mac);
            memcpy(sta->mac, mac, ETH_ALEN);
            sta->mac_link = rp_info->mac_head[mac_hash];
            rp_info->mac_head[mac_hash] = sta;
            update_ip = 1;
            sta->last_use = jiffies;
            rp_info->sta_num++;
        }
    } else {
        repeater_dbg("Entry.\n");
        return -1;
    }
    if (!update_ip) {
        goto next;
    }
    ip_hash = REPEATER_STA_HASH_IP(ip, ip_ver, rp_info->arr_len);
    if (ip_ver == IPVER_4)
        repeater_dbg("mac %pM New IP %pI4 ip_hash %d \n", mac, ip, ip_hash);
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
    if (ip_ver == IPVER_6_LLA || ip_ver == IPVER_6) {
        repeater_dbg("ipv6(%d) mac %pM New IPv6 %pI6 ip_hash %d \n", ip_ver, mac, ip, ip_hash);
    }
#endif
    memcpy(sta->ip[ip_ver], ip, g_repeater_ip_len[ip_ver]);
    sta->ip_link[ip_ver] = rp_info->ip_head[ip_ver][ip_hash];
    rp_info->ip_head[ip_ver][ip_hash] = sta;
    prev = sta;
    for (sta2 = sta->ip_link[ip_ver]; sta2; sta2 = sta2->ip_link[ip_ver]) {
        if (!memcmp(sta->ip[ip_ver], sta2->ip[ip_ver], g_repeater_ip_len[ip_ver])) {
            prev->ip_link[ip_ver] = sta2->ip_link[ip_ver];
            sta2->ip_link[ip_ver] = NULL;
            memset(sta2->ip[ip_ver], 0, g_repeater_ip_len[ip_ver]);
        }
        prev = sta2;
    }
next:
    return 0;
}

#ifdef CONFIG_SIWIFI_REPEATER_IPV6
static int repeater_ipv6_update_sta_all(struct repeater_info *rp_info,  unsigned char *mac, u8 *ip) {
    int ip_ver = IPVER_6;
    u16 ip_prefix = *((u16 *)ip);

    if (ntohs(ip_prefix) == 0xfe80) {
        ip_ver = IPVER_6_LLA;
    }
    return repeater_update_sta_all(rp_info, mac, ip, ip_ver);
}
#endif

static struct repeater_sta *repeater_find_sta_ip(struct repeater_info *rp_info, u8 *ip, int ip_ver)
{
    int hash = REPEATER_STA_HASH_IP(ip, ip_ver, rp_info->arr_len);
    struct repeater_sta *tmp = NULL;
    for (tmp = rp_info->ip_head[ip_ver][hash]; tmp; tmp = tmp->ip_link[ip_ver]) {
        if (!memcmp(&tmp->ip[ip_ver], ip, g_repeater_ip_len[ip_ver])) {
            tmp->last_use = jiffies;
            return tmp;
        }
    }
    return NULL;
}

#ifdef CONFIG_SIWIFI_REPEATER_IPV6
// search mac by ip in MAC-IPv6 or MAC-IPv6_LLA list
static struct repeater_sta *repeater_ipv6_find_sta_ip(struct repeater_info *rp_info, u8 *ip)
{
    int ip_ver = IPVER_6;
    u16 ip_prefix = *((u16 *)ip);

    // The prefix of the link local address is 0xfe80
    if (ntohs(ip_prefix) == 0xfe80) {
        ip_ver = IPVER_6_LLA;
    }

    return repeater_find_sta_ip(rp_info, ip, ip_ver);
}
#endif

static int repeater_dhcpc_proc(struct repeater_info *rp_info, u8 *eh, u8 *iph, u8 *udph, u8 *data, int data_len, int send)
{
    struct repeater_sta *sta = NULL;
    u16 dhcp_flag = 0;
    if (data_len < DHCP_OPT_OFFSET) {
        repeater_err("DHCP body is small.\n");
        return 0;
    }
    repeater_dbg("Entry.\n");
    if (send) {
        if (repeater_update_sta_mac(rp_info, data + DHCP_CHADDR_OFFSET, &sta) < 0) {
            repeater_err("unable to add entry  \n");
            return -1;
        }
        memcpy(&dhcp_flag, data + DHCP_FLAGS_OFFSET, DHCP_FLAGS_LEN);
        dhcp_flag |= htons(DHCP_FLAG_BCAST);
        memcpy(&sta->dhcp_flag, data + DHCP_FLAGS_OFFSET, DHCP_FLAGS_LEN);
        if (dhcp_flag != sta->dhcp_flag) {
            repeater_dbg("Entry. sta->dhcp_flag %x dhcp_flag %x\n", sta->dhcp_flag, dhcp_flag);
            repeater_fix_csum(udph + offsetof(struct udphdr, check), data + DHCP_FLAGS_OFFSET,
                    DHCP_FLAGS_LEN, (u8 *)&dhcp_flag, DHCP_FLAGS_LEN);
            memcpy(data + DHCP_FLAGS_OFFSET, (u8 *)&dhcp_flag, DHCP_FLAGS_LEN);
            repeater_dbg("Entry. sta->dhcp_flag %x dhcp_flag %x\n", sta->dhcp_flag, *(u16 *)(data + DHCP_FLAGS_OFFSET));
        }
        ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
    } else {
        repeater_dbg("SRC %pM DST %pM %pM should be not here.\n", eh, eh + ETH_ALEN, data + DHCP_CHADDR_OFFSET);
        return -1;
    }
    /* no further processing! */
    return 1;
}

static int repeater_dhcps_proc(struct repeater_info *rp_info, u8 *eh, u8 *iph, u8 *udph, u8 *data, int data_len, int send)
{
    struct repeater_sta *sta = NULL;
    const u8 ether_bcast[ETH_ALEN] = {255, 255, 255, 255, 255, 255};
    int mac_hash = 0;
    if (data_len < DHCP_OPT_OFFSET) {
        repeater_err("DHCP body is small.\n");
        return 0;
    }
    repeater_dbg("Entry.\n");
    mac_hash = repeater_hash_by_mac(data + DHCP_CHADDR_OFFSET, rp_info->arr_len);
    sta = repeater_find_sta_mac(rp_info->mac_head[mac_hash], data + DHCP_CHADDR_OFFSET);
    if (!sta) {
        repeater_dbg("cant find entry \n");
        return -1;
    }
    if (memcmp(data + DHCP_FLAGS_OFFSET, (u8 *)&sta->dhcp_flag, DHCP_FLAGS_LEN)) {
        repeater_fix_csum(udph + offsetof(struct udphdr, check), data + DHCP_FLAGS_OFFSET,
                    DHCP_FLAGS_LEN, (u8 *)&sta->dhcp_flag, DHCP_FLAGS_LEN);
        memcpy(data + DHCP_FLAGS_OFFSET, &sta->dhcp_flag, DHCP_FLAGS_LEN);
        repeater_dbg("sta->dhcp_flag %x dhcp_flag %x\n", sta->dhcp_flag, *(u16 *)(data + DHCP_FLAGS_OFFSET));
    }
    /* force BROADCAST */
    ether_addr_copy(eh, ether_bcast);
    /* no further processing! */
    return 1;
}

static int repeater_udp_proc(struct repeater_info *rp_info, u8 *eh, u8 *iph, u8 *data, int data_len, int send)
{
    struct udphdr *udph = (struct udphdr *)data;
    u16 dport = ntohs(udph->dest);
    int i = 0;
    if (data_len < sizeof(struct udphdr)) {
        repeater_err("Udp header is small.\n");
        return -1;
    }
    data_len -= sizeof(struct udphdr);
    for (i = 0; i < ARRAY_SIZE(repeater_udp_handle); i++) {
        if (dport != repeater_udp_handle[i].port) {
            continue;
        }
        return repeater_udp_handle[i].udp_proc(rp_info, eh, iph, data, data + sizeof(struct udphdr), data_len, send);
    }
    return 0;
}

static int repeater_arp_proc(struct repeater_info *rp_info, unsigned char *eh, unsigned char *data, int data_len, int send)
{
    u8 *ip = NULL;
    u8 *mac = NULL;
    struct repeater_sta *sta = NULL;
    repeater_dbg("Entry. send %d\n", send);
    if (data_len < ARP_DATA_LEN) {
        repeater_err("arp header is small.\n");
        return -1;
    }
    if (send) {
        mac = data + ARP_SRC_ETH_OFFSET;
        ip = data + ARP_SRC_IP_OFFSET;
        repeater_dbg("MAC %pM IP %pI4\n", mac, ip);
        if (repeater_update_sta_all(rp_info, mac, ip, IPVER_4) < 0) {
            return -1;
        }
        ether_addr_copy(data + ARP_SRC_ETH_OFFSET, rp_info->if_mac);
        ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
        mac = data + ARP_SRC_ETH_OFFSET;
        ip = data + ARP_SRC_IP_OFFSET;
        repeater_dbg("After: MAC %pM IP %pI4\n", mac, ip);
    } else if ((*(u16 *)(data + ARP_OPC_OFFSET) == htons(ARPOP_REPLY)) ||
        ((*(u16 *)(data + ARP_OPC_OFFSET) == htons(ARPOP_REQUEST)) &&
        (!is_multicast_ether_addr(eh)))) {
        ip = data + ARP_TGT_IP_OFFSET;
        sta = repeater_find_sta_ip(rp_info, ip, IPVER_4);
        if (!sta) {
            repeater_dbg("not find arp ip %pI4\n", ip);
            return -1;
        }
        repeater_dbg("MAC %pM IP %pI4 --- %pM -> %pM\n", &sta->mac, ip, eh, eh + ETH_ALEN);
        ether_addr_copy(data + ARP_TGT_ETH_OFFSET, sta->mac);
        ether_addr_copy(eh, sta->mac);
    }
    return 0;
}

static int repeater_ip_proc(struct repeater_info *rp_info, unsigned char *eh, unsigned char *data, int data_len, int send)
{
    u8 mac[ETH_ALEN] = {0};
    u8 *mac_ref = NULL;
    u32 dst_ip = 0;
    int i = 0;
    struct iphdr *iph = (struct iphdr *)data;
    u8 protocol = iph->protocol;
    int l4_offset = 0;
    struct repeater_sta *sta = NULL;
    int ret = 0;
    if (data_len < sizeof(struct iphdr)) {
        repeater_err("ipv4 header is small.\n");
        return -1;
    }
    for (i = 0; i < ARRAY_SIZE(repeater_ipv4_handle); i++) {
        if (repeater_ipv4_handle[i].type != protocol) {
            continue;
        }
        l4_offset = iph->ihl << 2;
        ret = repeater_ipv4_handle[i].ipv4_proc(rp_info, eh, data, data + l4_offset, data_len - l4_offset, send);
        if (ret != 0) {
            return ret;
        }
        break;
    }
    if (send) {
        int update = 1;
        if (ipv4_is_multicast(iph->daddr) && iph->saddr == 0) {
            repeater_err("sip %pI4 dip %pI4\n", &iph->saddr, &iph->daddr);
            update = 0;
        }
        if (update && repeater_update_sta_all(rp_info, eh + ETH_ALEN,
                                                (u8 *)&iph->saddr, IPVER_4) < 0) {
            repeater_dbg("Update sta err.!\n");
            return -1;
        }
        ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
    } else if (!is_multicast_ether_addr(eh)) {
        //dst_ip = ntohl(iph->daddr);
        dst_ip = iph->daddr;
        if (ipv4_is_multicast(dst_ip)) {
            IPV4_MCAST_TO_ETHER_MCAST(dst_ip, mac);
            mac_ref = (u8 *)&mac;
        } else {
            sta = repeater_find_sta_ip(rp_info, (u8 *)&iph->daddr, IPVER_4);
            if (!sta) {
                repeater_dbg("not find sta %pI4\n", &iph->daddr);
                return -1;
            }
            mac_ref = (u8 *)&sta->mac;
        }
        ether_addr_copy(eh, mac_ref);
    }
    return 0;
}

#ifdef CONFIG_SIWIFI_REPEATER_IPV6
#define IPv6_UDP_DHCPC_PORT 546
#define IPv6_UDP_DHCPS_PORT 547

#define IPV6_DHCP_TYPE_SOL 1           /* Message type: Solicit (1)          */
#define IPV6_DHCP_TYPE_ADV 2           /* Message type: Advertise (2)        */
#define IPV6_DHCP_TYPE_REQ 3           /* Message type: Request (3)          */
#define IPV6_DHCP_TYPE_REP 7           /* Message type: Reply (7)            */

// Currently, it is only used as the debug function, and no information has been changed
static int repeater_ipv6_udp_proc(struct repeater_info *rp_info, u8 *eh, u8 *iph, u8 *data, int data_len, int send)
{
#ifdef CONFIG_REPEATER_DEBUG
    struct udphdr *udphdr = (struct udphdr *)data;
    struct ethhdr *v6_eh = (struct ethhdr *)eh;
    struct ipv6hdr *v6_hdr = (struct ipv6hdr *)iph;
    u8 *ipv6_hdr = NULL;
    u16 udp_sport = ntohs(udphdr->source);

    if (data_len < sizeof(struct udphdr)) {
        repeater_err("ipv6 udp header is small\n");
        return -1;
    }

    if (udp_sport != IPv6_UDP_DHCPC_PORT && udp_sport != IPv6_UDP_DHCPS_PORT)
        return -1;

    ipv6_hdr = (u8 *)udphdr + sizeof(struct udphdr);

    switch (*ipv6_hdr) {
        case IPV6_DHCP_TYPE_SOL:
            repeater_dbg("DHCP SOL(%d) eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                            send, v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            break;
        case IPV6_DHCP_TYPE_ADV:
            repeater_dbg("DHCP ADV(%d) eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                            send, v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            break;
        case IPV6_DHCP_TYPE_REQ:
            repeater_dbg("DHCP REQ(%d) eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                            send, v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            break;
        case IPV6_DHCP_TYPE_REP:
            repeater_dbg("DHCP REP(%d) eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                            send, v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            break;
    }
#endif

    return 0;
}

/*
    1. ICMPv6 message

        1.1 RS (Router Solicitation)
            type(1) = IPV6_ICMP_TYPE_RS
            code(1) = 0
            ------------------------------------------------
            |  type(1)  | code(1)  |     checksum(2)       |
            |                reserved(4)                   |
            |                option                        |
            ------------------------------------------------
        1.2 RA (Router Advertisement)
            type(1) = IPV6_ICMP_TYPE_RA
            code(1) = 0
            ------------------------------------------------
            |  type(1)  | code(1)  |     checksum(2)       |
            |   chl(1)  | flags(1) | router lifetime(2)    |  // chl : Cur hop limit, reference value : 64
            |            reachable time(4)                 |
            |              retrans time(4)                 |
            |                option                        |
            ------------------------------------------------
        1.3 NS (Neighbor Solicitation)
            type(1) = IPV6_ICMP_TYPE_NS
            code(1) = 0
            ------------------------------------------------
            |  type(1)  | code(1)  |     checksum(2)       |
            |                reserved(4)                   |
            |           target address(16)                 |
            |                option                        |
            ------------------------------------------------
        1.4 NA (Neighbor Advertisement)
            type(1) = IPV6_ICMP_TYPE_NA
            code(1) = 0
            ------------------------------------------------
            |  type(1)  | code(1)  |     checksum(2)       |
            |                flags(4)                      |
            |           target address(16)                 |
            |                option                        |
            ------------------------------------------------

    2. ICMPv6 option message

        2.1 SLLA (Source link-layer address)
            type(1) = IPV6_ICMP_OPTION_TYPE_SLLA
            length(1) = 1
            ------------------------------------------------
            |  type(1)  | length(1)|     macaddr(2)        |
            |                macaddr(4)                    |
            ------------------------------------------------
        2.2 TLLA (Target link-layer address)
            type(1) = IPV6_ICMP_OPTION_TYPE_TLLA
            length(1) = 1
            ------------------------------------------------
            |  type(1)  | length(1)|     macaddr(2)        |
            |                macaddr(4)                    |
            ------------------------------------------------
        2.3 MTU
            type(1) = 5
            length(1) = 1
            ------------------------------------------------
            |  type(1)  | length(1)|    reserved(2)        |
            |                  mtu(4)                      |  // mtu reference value : 1500
            ------------------------------------------------
        2.4 PI (prefix infomation)
            type(1) = 3
            length(1) = 4
            ------------------------------------------------
            |  type(1)  | length(1)|  pl(1)   |  flags(1)  |  // pl: prefix length, reference value : 64
            |             valid lifetime(4)                |
            |         preferred lifetime(4)                |
            |                reserved(4)                   |
            |                 prefix(16)                   |
            ------------------------------------------------
        2.5 RDNSS (Recursive DNS Server)
            type(1) = 25
            length(1) = 3
            ------------------------------------------------
            |  type(1)  | length(1)|    reserved(2)        |
            |             lifetime(4)                      |
            |       recursive DNS servers(16)              |
            ------------------------------------------------
        2.6 AI (Advertisement Interval)
            type(1) = 7
            length(1) = 1
            ------------------------------------------------
            |  type(1)  | length(1)|    reserved(2)        |
            |             reserved(4)                      |
            |         advertisement interval(4)            |
            ------------------------------------------------
 */

//#define IPV6_HEADER_LENGTH 40
#define IPV6_ICMP_TYPE_NONE     1
#define IPV6_ICMP_TYPE_ECHO_REQ 128          /* Type: Echo request (128)           */
#define IPV6_ICMP_TYPE_ECHO_RSP 129          /* Type: Echo reply (129)             */
#define IPV6_ICMP_TYPE_RS       133          /* Type: Router Solicitation (133)    */
#define IPV6_ICMP_TYPE_RA       134          /* Type: Router Advertisement (134)   */
#define IPV6_ICMP_TYPE_NS       135          /* Type: Neighbor Solicitation (135)  */
#define IPV6_ICMP_TYPE_NA       136          /* Type: Neighbor Advertisement (136) */

#define IPV6_ICMP_CHECKSUM_OFFSET   2

#define IPV6_ICMP_TYPE_RS_LENGTH    8
#define IPV6_ICMP_TYPE_NS_LENGTH    24
#define IPV6_ICMP_TYPE_NA_LENGTH    24

#define IPV6_ICMP_OPTION_TYPE_SLLA     1  /* Type: Source link-layer address (1) */
#define IPV6_ICMP_SLLA_LENGTH_OFFSET   1
#define IPV6_ICMP_SLLA_MACADDR_OFFSET  2
#define IPV6_ICMP_OPTION_TYPE_TLLA     2  /* Type: Target link-layer address (2) */
#define IPV6_ICMP_TLLA_LENGTH_OFFSET   1
#define IPV6_ICMP_TLLA_MACADDR_OFFSET  2

/*
    Processing ICMPv6 message of tx
    The rx message is processed in repeater_ipv6_proc
 */
static int repeater_ipv6_icmp_proc(struct repeater_info *rp_info, u8 *eh, u8 *iph, u8 *data, int data_len, int send)
{
    u8 *ipv6_icmp_type = data;
    struct ethhdr *v6_eh = (struct ethhdr *)eh;
    struct ipv6hdr *v6_hdr = (struct ipv6hdr *)iph;
    u16 v6_payload_len = ntohs(v6_hdr->payload_len);

    switch (*ipv6_icmp_type) {
        case IPV6_ICMP_TYPE_NONE:
            if (*(ipv6_icmp_type + 1) == 5) {
                repeater_dbg("NO(%d) destination unreachable! eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                            send, v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            }
            break;

        // In general, the destination address of the RS frame is the broadcast address, which does not need to be replaced
        case IPV6_ICMP_TYPE_RS:
            repeater_dbg("RS(%d) len %d, hl %d, eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                            send, v6_payload_len, v6_hdr->hop_limit,
                            v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            if (send) {
                if (v6_payload_len > IPV6_ICMP_TYPE_RS_LENGTH) {
                    u8 *ipv6_icmp_option_type = data + IPV6_ICMP_TYPE_RS_LENGTH;
                    if (*ipv6_icmp_option_type == IPV6_ICMP_OPTION_TYPE_SLLA) {
                        repeater_dbg("RS(%d) option_slla (length %d) (%pM replace %pM)\n",
                                send, *(ipv6_icmp_option_type + IPV6_ICMP_SLLA_LENGTH_OFFSET), rp_info->if_mac,
                                        ipv6_icmp_option_type + IPV6_ICMP_SLLA_MACADDR_OFFSET);
                        repeater_fix_csum(data + IPV6_ICMP_CHECKSUM_OFFSET, ipv6_icmp_option_type + IPV6_ICMP_SLLA_MACADDR_OFFSET,
                                            ETH_ALEN, rp_info->if_mac, ETH_ALEN);
                        ether_addr_copy(ipv6_icmp_option_type + IPV6_ICMP_SLLA_MACADDR_OFFSET, rp_info->if_mac);
                    }
                }
                repeater_ipv6_update_sta_all(rp_info, v6_eh->h_source, (u8 *)&v6_hdr->saddr);
                ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
                /* no further processing! */
                return 1;
            }
            break;

        // Since there is no IPv6 service, it is generally unnecessary to send IPv6 RA message
        case IPV6_ICMP_TYPE_RA:
            break;

        case IPV6_ICMP_TYPE_NS:
            if (send) {
                repeater_dbg("NS(%d) eth(%pM replace %pM, dst %pM) ipv6(%pI6 to %pI6)\n",
                                send, rp_info->if_mac, eh + ETH_ALEN, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
                if (v6_payload_len > IPV6_ICMP_TYPE_NS_LENGTH) {
                    u8 *ipv6_icmp_option_type = data + IPV6_ICMP_TYPE_NS_LENGTH;
                    if (*ipv6_icmp_option_type == IPV6_ICMP_OPTION_TYPE_SLLA) {
                        repeater_dbg("NS(%d) option_slla (length %d) (%pM replace %pM)\n",
                                send, *(ipv6_icmp_option_type + IPV6_ICMP_SLLA_LENGTH_OFFSET), rp_info->if_mac,
                                        ipv6_icmp_option_type + IPV6_ICMP_SLLA_MACADDR_OFFSET);
                        repeater_fix_csum(data + IPV6_ICMP_CHECKSUM_OFFSET, ipv6_icmp_option_type + IPV6_ICMP_SLLA_MACADDR_OFFSET,
                                            ETH_ALEN, rp_info->if_mac, ETH_ALEN);
                        repeater_ipv6_update_sta_all(rp_info, v6_eh->h_source, (u8 *)&v6_hdr->saddr);
                        ether_addr_copy(ipv6_icmp_option_type + IPV6_ICMP_SLLA_MACADDR_OFFSET, rp_info->if_mac);
                    }
                }
                ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
                /* no further processing! */
                return 1;
            }
            break;

        case IPV6_ICMP_TYPE_NA:
            if (send) {
                repeater_dbg("NA(%d) eth(%pM replace %pM  dst %pM) ipv6(%pI6 to %pI6)\n",
                                send, rp_info->if_mac, eh + ETH_ALEN, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
                if (v6_payload_len > IPV6_ICMP_TYPE_NA_LENGTH) {
                    u8 *ipv6_icmp_option_type = data + IPV6_ICMP_TYPE_NA_LENGTH;
                    if (*ipv6_icmp_option_type == IPV6_ICMP_OPTION_TYPE_TLLA) {
                        repeater_fix_csum(data + IPV6_ICMP_CHECKSUM_OFFSET, ipv6_icmp_option_type + IPV6_ICMP_TLLA_MACADDR_OFFSET,
                                            ETH_ALEN, rp_info->if_mac, ETH_ALEN);
                        ether_addr_copy(ipv6_icmp_option_type + IPV6_ICMP_TLLA_MACADDR_OFFSET, rp_info->if_mac);
                    }
                }
                ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
                /* no further processing! */
                return 1;
            }
            break;

#ifdef CONFIG_REPEATER_DEBUG
        case IPV6_ICMP_TYPE_ECHO_REQ:
            {
                u16 ipv6_ping_seq = *((u16*)(ipv6_icmp_type + 6));
                repeater_dbg("ECHO REQ(%d) eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                                send, v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            }
            break;

        case IPV6_ICMP_TYPE_ECHO_RSP:
            {
                u16 ipv6_ping_seq = *((u16*)(ipv6_icmp_type + 6));
                repeater_dbg("ECHO REP(%d) seq(%d) eth(%pM to %pM) ipv6(%pI6 to %pI6)\n",
                                send, ntohs(ipv6_ping_seq), v6_eh->h_source, v6_eh->h_dest, &v6_hdr->saddr, &v6_hdr->daddr);
            }
            break;
#endif
    }

    return 0;
}

static int repeater_ipv6_proc(struct repeater_info *rp_info, unsigned char *eh, unsigned char *data, int data_len, int send)
{
    int i = 0;
    int l4_offset = 0;
    int ret = 0;
    struct ipv6hdr *ip6h = (struct ipv6hdr *)data;
    u8 nexthdr = ip6h->nexthdr;
    struct repeater_sta *sta = NULL;

    if (data_len < sizeof(struct ipv6hdr)) {
        repeater_err("ipv6 header is small.\n");
        return -1;
    }

    for (i = 0; i < ARRAY_SIZE(repeater_ipv6_handle); i++) {
        if (repeater_ipv6_handle[i].type != nexthdr) {
            continue;
        }
        l4_offset = sizeof(struct ipv6hdr);
        ret = repeater_ipv6_handle[i].ipv6_proc(rp_info, eh, data, data + l4_offset, data_len - l4_offset, send);
        if (ret != 0) {
            return ret;
        }
        break;
    }

    if (send) {
        if (repeater_ipv6_update_sta_all(rp_info, eh + ETH_ALEN, (u8 *)&ip6h->saddr) < 0) {
            repeater_err("Update ipv6 sta err.!\n");
            return -1;
        }
        ether_addr_copy(eh + ETH_ALEN, rp_info->if_mac);
    } else {
        u8 *ip6h_da = (u8 *)&ip6h->daddr;
        u16 ip_prefix = *((u16 *)ip6h_da);
        u8 mul_dst[6] = { 0x33, 0x33, 0, 0, 0, 0 };

        /*  IPv6_multicast_address   ......:**:**:**:**
                                           |  |  |  |
             eth_multicast_address    33:33:**:**:**:**   */
        if (ntohs(ip_prefix) == 0xff02) {
            memcpy(&mul_dst[2], (ip6h_da + 12), 4);
            ether_addr_copy(eh, mul_dst);
            return 0;
        }

        sta = repeater_ipv6_find_sta_ip(rp_info, ip6h_da);
        if (sta)
            ether_addr_copy(eh, sta->mac);
    }
    return 0;
}
#endif /* CONFIG_SIWIFI_REPEATER_IPV6 */

int debug_open = 0;
module_param(debug_open, int, 0644);
/* Proccess L2 Data */
static int repeater_l2_proc(struct repeater_info *rp_info, struct sk_buff **skb, int send)
{
    struct ethhdr *eh = NULL;
    u8 *l3_data = NULL;
    int type = 0;
    int i = 0;
    int ret = -1;
    u8 *l2_data = NULL;
    u8 *ptype = NULL;
    int data_len = 0;
    struct sk_buff *new_skb = NULL;
    if (debug_open) {
        return 0;
    }
    if ((*skb)->len < sizeof(struct ethhdr)) {
        return -1;
    }
    eh = (struct ethhdr *)((*skb)->data);
    type = ntohs(eh->h_proto);
    spin_lock_bh(&g_repeater_lock);
    /* only handle bridge forward */
    if (!(rp_info->vif_dev->priv_flags & IFF_BRIDGE_PORT)) {
        spin_unlock_bh(&g_repeater_lock);
        return 0;
    }
    if (ether_addr_equal(eh->h_source, rp_info->if_mac)) {
        repeater_dbg("%s smac %pM -> dmac %pM proto 0x%X\n", send ? "send" : "recv", &eh->h_source, &eh->h_dest, type);
        spin_unlock_bh(&g_repeater_lock);
        return send ? 0 : -2;
    }
    spin_unlock_bh(&g_repeater_lock);
    if (send && ((skb_shared(*skb) || (skb_cloned(*skb))) && is_multicast_ether_addr(eh->h_dest))) {
        new_skb = skb_copy(*skb, GFP_ATOMIC);
        if (unlikely(!new_skb)) {
            repeater_err("mem is not enough.\n");
            return -1;
        }
        kfree_skb(*skb);
        *skb = new_skb;
    }
    l2_data = (*skb)->data;
    ptype = (*skb)->data + offsetof(struct ethhdr, h_proto);
    /* 802.2 Handle */
    if (type <= ETH_DATA_LEN && data_len >= ETH_HLEN + SNAP_HDR_LEN) {
        u8 *llc = ptype;
        repeater_dbg("type 0x%X\n", type);
        if (memcmp(g_llc_snap_hdr, llc, SNAP_HDR_LEN)) {
            repeater_dbg("llc packet\n");
            if (send && !memcmp(g_llc_hdr_stp, llc, SNAP_HDR_LEN)) {
                repeater_dbg("stp packet\n");
                ether_addr_copy(l2_data + ETH_ALEN, rp_info->if_mac);
            }
            return 0;
        }
        ptype = llc + SNAP_HDR_LEN;
    }
    if (htons(ETH_P_8021Q) == (*(u16 *)ptype)) {
        ptype += sizeof(struct vlan_hdr);
    }
    l3_data = ptype + ETHER_PROTO_LEN;
    data_len = (*skb)->len - (l3_data - l2_data);
    type = ntohs(*(u16 *)ptype);
    for (i = 0; i < ARRAY_SIZE(repeater_prot_handle); i++) {
        if (repeater_prot_handle[i].type != type) {
            continue;
        }
        spin_lock_bh(&g_repeater_lock);
        ret = (repeater_prot_handle[i].prot_proc)(rp_info, l2_data, l3_data, data_len, send);
        spin_unlock_bh(&g_repeater_lock);
    }
    if (ret < 0) {
        repeater_dbg("%s proto 0x%X can not proccess.\n", send ? "send" : "recv", type);
    }
    return ret;
}

int repeater_rev_proc(struct repeater_info *rp_info, struct sk_buff **skb)
{
    return repeater_l2_proc(rp_info, skb, 0) == -2 ? -1 : 0;
}
int repeater_send_proc(struct repeater_info *rp_info, struct sk_buff **skb)
{
    return repeater_l2_proc(rp_info, skb, 1) == -2 ? -1 : 0;
}
static void repeater_sta_aging(unsigned long ptr)
{
    struct repeater_info *rp_info = (struct repeater_info *)ptr;
    struct repeater_sta *sta = NULL, **next = NULL;
    int i = 0;
    spin_lock_bh(&g_repeater_lock);
    for (i = 0; i < rp_info->arr_len; i++) {
        next = &rp_info->mac_head[i];
        while (*next) {
            sta = *next;
            if (time_after_eq(jiffies, sta->last_use + REPEATER_STA_AGING_TIME * HZ)) {
                repeater_dbg("sta %pM aging cur %lu last_use %lu\n", &sta->mac, jiffies, sta->last_use);
                repeater_del_ip_link(rp_info, sta, IPVER_4);
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
                repeater_del_ip_link(rp_info, sta, IPVER_6);
                repeater_del_ip_link(rp_info, sta, IPVER_6_LLA);
#endif
                *next = sta->mac_link;
                rp_info->sta_num--;
                kfree(sta);
                continue;
            }
            next = &((*next)->mac_link);
        }
    }
    spin_unlock_bh(&g_repeater_lock);
    mod_timer(&(rp_info->expires), jiffies + REPEATER_STA_AGING_TIME * HZ);
}
int repeater_register(struct repeater_info **rp_info, struct net_device *vif_dev, int max_sta)
{
    struct repeater_info *tmp = NULL;
    int ret = 0;
    int i = 0;
    repeater_dbg("repeater init in.\n");
    tmp = kzalloc(sizeof(struct repeater_info), GFP_ATOMIC);
    if (!tmp) {
        ret = -ENOMEM;
        goto out;
    }
    tmp->vif_dev  = vif_dev;
    tmp->sta_num  = 0;
    tmp->max_sta  = max_sta;
    tmp->arr_len  = max_sta / 2;
    for (i = 0; i < IPTYPE; i++) {
        tmp->ip_head[i]  = kzalloc(tmp->arr_len * sizeof(struct repeater_sta *), GFP_ATOMIC);
        if (!tmp->ip_head) {
            ret = -ENOMEM;
            goto out1;
        }
    }
    tmp->mac_head = kzalloc(tmp->arr_len * sizeof(struct repeater_sta *), GFP_ATOMIC);
    if (!tmp->mac_head) {
        ret = -ENOMEM;
        goto out2;
    }
    INIT_LIST_HEAD(&tmp->list);
    setup_timer(&(tmp->expires), repeater_sta_aging, (unsigned long)tmp);
    mod_timer(&(tmp->expires), jiffies + REPEATER_STA_AGING_TIME * HZ);
    *rp_info = tmp;
    spin_lock_bh(&g_repeater_lock);
    list_add(&tmp->list, &g_rp_info_list);
    spin_unlock_bh(&g_repeater_lock);
    repeater_dbg("Repeater init ok. dev->name %s\n", vif_dev->name);
    return ret;
out2:
    for (i = 0; i < IPTYPE; i++) {
        if (tmp->ip_head[i])
            kfree(tmp->ip_head[i]);
    }
out1:
    kfree(tmp);
out:
    return ret;
}

static void repeater_clear_sta(struct repeater_info *rp_info)
{
    int i = 0;
    struct repeater_sta *sta = NULL, *next = NULL;
    for (i = 0; i < rp_info->arr_len; i++) {
        sta = rp_info->mac_head[i];
        while (sta) {
            next = sta->mac_link;
            kfree(sta);
            sta = next;
        }
    }
}
int repeater_unregister(struct repeater_info *rp_info)
{
    struct repeater_info *pos = NULL, *n = NULL;
    int i = 0;
    spin_lock_bh(&g_repeater_lock);
    list_for_each_entry_safe(pos, n, &g_rp_info_list, list) {
        if (pos == rp_info) {
            repeater_clear_sta(rp_info);
            for (i = 0; i < IPTYPE; i++) {
                if (pos->ip_head[i])
                    kfree(pos->ip_head[i]);
            }
            kfree(pos->mac_head);
            list_del(&pos->list);
            del_timer(&pos->expires);
            kfree(pos);
            break;
        }
    }
    spin_unlock_bh(&g_repeater_lock);
    return 0;
}
static int repeater_netdev_event_handle(struct notifier_block *this, unsigned long event, void *ptr)
{
    struct net_device *dev = netdev_notifier_info_to_dev(ptr);
    struct repeater_info *rp_info = NULL, *rp_info_tmp = NULL;
    repeater_dbg("Entry.\n");
    spin_lock_bh(&g_repeater_lock);
    list_for_each_entry(rp_info_tmp, &g_rp_info_list, list) {
        if (rp_info_tmp->vif_dev == dev) {
            repeater_dbg("VIF_dev %s, MAC %pM\n", dev->name, dev->dev_addr);
            rp_info = rp_info_tmp;
            break;
        }
    }
    if (rp_info && (NETDEV_UP == event || NETDEV_CHANGEADDR == event)) {
        ether_addr_copy(rp_info->if_mac, dev->dev_addr);
    }
    spin_unlock_bh(&g_repeater_lock);

    if (rp_info && NETDEV_UNREGISTER == event) {
        repeater_unregister(rp_info);
    }
    return NOTIFY_DONE;
}

static int repeater_debug_read(struct seq_file *s, void *data)
{
    struct repeater_info *rp_info = NULL;
    struct repeater_sta *sta = NULL;
    int i = 0;
    seq_printf(s, "Repeater Debug Info: \n");
    spin_lock_bh(&g_repeater_lock);
    list_for_each_entry(rp_info, &g_rp_info_list, list) {
        seq_printf(s, "vif_dev %s if_name %pM sta_num %d max_sta %d cur_time %lu ms\n", rp_info->vif_dev->name,
                                                   rp_info->if_mac, rp_info->sta_num, rp_info->max_sta, jiffies);
        seq_printf(s, "-----------------  MAC HASH TABLE  -----------------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->mac_head[i]; sta; sta = sta->mac_link) {
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
                    seq_printf(s, "hash_idx %2d mac %pM ip %pi4 ipv6 %pI6 ipv6_lla %pI6 last_use %lu\n",
                                i, &sta->mac, &sta->ip[IPVER_4], &sta->ip[IPVER_6], &sta->ip[IPVER_6_LLA], sta->last_use);
#else
                    seq_printf(s, "hash_idx %2d mac %pM ip %pI4 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_4], sta->last_use);
#endif
            }
        }

        seq_printf(s, "-----------------   IP HASH TABLE  -----------------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->ip_head[IPVER_4][i]; sta; sta = sta->ip_link[IPVER_4]) {
                if (sta->ip[IPVER_4][0] != 0)
                    seq_printf(s, "hash_idx %2d mac %pM ip %pI4 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_4], sta->last_use);
            }
        }
#ifdef CONFIG_SIWIFI_REPEATER_IPV6
        seq_printf(s, "----------------- IPv6 HASH TABLE  -----------------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->ip_head[IPVER_6][i]; sta; sta = sta->ip_link[IPVER_6]) {
                if (sta->ip[IPVER_6][0] != 0)
                    seq_printf(s, "hash_idx %2d mac %pM ipv6 %pI6 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_6], sta->last_use);
            }
        }
        seq_printf(s, "----------------- IPv6 HASH TABLE (LLA)  -----------------\n");
        for (i = 0; i < rp_info->arr_len; i++) {
            for (sta = rp_info->ip_head[IPVER_6_LLA][i]; sta; sta = sta->ip_link[IPVER_6_LLA]) {
                if (sta->ip[IPVER_6_LLA][0] != 0)
                    seq_printf(s, "hash_idx %2d mac %pM ipv6 %pI6 last_use %lu\n", i, &sta->mac, &sta->ip[IPVER_6_LLA], sta->last_use);
            }
        }
#endif
        seq_printf(s, "----------------------------------------------------------\n");
    }
    spin_unlock_bh(&g_repeater_lock);
    return 0;
}
static int repeater_debug_open(struct inode *inode, struct file *file)
{
    return single_open(file, repeater_debug_read, NULL);
}
static struct file_operations repeater_fops = {
    .open = repeater_debug_open,
    .read = seq_read,
    .release = single_release,
};
static struct notifier_block repeater_netdev_event = {
    .notifier_call = repeater_netdev_event_handle,
};
int repeater_init(void)
{
    int ret;
    g_repeater_proc = proc_create("repeater_debug", 0644, NULL, &repeater_fops);
    ret = register_netdevice_notifier(&repeater_netdev_event);
    return ret;
}

void repeater_exit(void)
{
    remove_proc_entry("repeater_debug", NULL);
    unregister_netdevice_notifier(&repeater_netdev_event);
}
