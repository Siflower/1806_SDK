#ifndef _SFAX8_NETLINK_H
#define _SFAX8_NETLINK_H

#include <net/genetlink.h>


#define SF_GENL_VERSION              0x1
#define MAX_MSG_SIZE                 (nla_total_size(MSGLEN) + NLMSG_LENGTH(GENL_HDRLEN))
#define MSGLEN                       64


/*
 * CMD is used to classify differnt ops.
 * Differnt family share the same ops, so that cmd for ops should be unique
 */
enum {
    SF_CMD_UNSPEC = 0,         /* Reserved */
    SF_CMD_GENERIC,            /* generic cmd for sfax8 */
    __SF_CMD_MAX,
};
#define SF_CMD_MAX (__SF_CMD_MAX - 1)


/*
 * Differnt attr type for differnt function.
 * New attrs should only be inserted at the enum's end
 * prior to __SF_CMD_ATTR_MAX
 */
enum {
    SF_CMD_ATTR_UNSPEC = 0,
    SF_CMD_ATTR_DPS,             /* use for detect port status change */
    SF_CMD_ATTR_ECHO,            /* use for echo message */
    SF_CMD_ATTR_KTHREAD_RUN,     /* use for create kthread */
    SF_CMD_ATTR_KTHREAD_STOP,    /* use for stop kthread */
    SF_CMD_ATTR_WIFI,            /* reserve for wifi */
    SF_CMD_ATTR_APP,             /* reserve for app */
    SF_CMD_ATTR_ERR,
    __SF_CMD_ATTR_MAX,
};
#define SF_CMD_ATTR_MAX (__SF_CMD_ATTR_MAX - 1)

enum {
	SF_ETH_CMD_ATTR_UNSPEC = 0,
	SF_ETH_CMD_ATTR_DPS_PORT,            /* eth phy port*/
	SF_ETH_CMD_ATTR_DPS_LINK,            /* 0---link down  1---link up */
	SF_ETH_CMD_ATTR_DPS_IS_WAN,          /* 0---lan     1---wan */
	SF_ETH_CMD_ATTR_DPS_VLANID,
	SF_ETH_CMD_ATTR_DPS_IFNAME,
	__SF_ETH_CMD_ATTR_MAX,
};

typedef struct nlfamily_template {
	struct genl_family family;
	struct genl_multicast_group group[1];
}sf_nlfamily;


struct sfax8_netlink {
	sf_nlfamily generic_family;
#ifdef CONFIG_DEBUG_FS
	struct dentry     *genl_debug;
#endif
};

#endif
