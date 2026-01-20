#include <linux/inet.h>
#include "../../sf_eswitch.h"
#include "gsw_sw_init.h"

#define RULE_INDEX 1
#define pr_fmt(fmt) "sf_bridge_redirect_hw: " fmt

extern ethsw_api_dev_t *pedev0[GSW_DEV_MAX];

static GSW_return_t intel7084_bridge_redirect_hw_apply(u32 daddr)
{
	GSW_return_t ret;

	GSW_PCE_rule_t rule = {
		.pattern = {
			.nIndex		= RULE_INDEX,
			.bEnable	= LTQ_TRUE,
			.eDstIP_Select	= GSW_PCE_IP_V4,
			.nDstIP.nIPv4	= daddr,
			.nDstIP_Mask	= 0xff00,
		},

		.action = {
			.ePortMapAction	= GSW_PCE_ACTION_PORTMAP_ALTERNATIVE,
			.nForwardPortMap	= BIT(5), /* CPU port */
		},
	};

	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();

	if (ret == GSW_statusOk)
		pr_info("enabled addr %08x\n", daddr);

	return ret;
}

static GSW_return_t intel7084_bridge_redirect_hw_remove(void)
{
	GSW_return_t ret;

	GSW_PCE_ruleDelete_t del = {
		.nIndex	= RULE_INDEX,
	};

	SF_MDIO_LOCK();
	ret = GSW_PceRuleDelete(pedev0[0], &del);
	SF_MDIO_UNLOCK();

	if (ret == GSW_statusOk)
		pr_info("disabled\n");

	return ret;
}

int intel7084_bridge_redirect_ip(const char *ip)
{
	__be32 daddr;

	if (!ip || !ip[0]) {
		pr_err("please specify parameter ip!\n");
		return -EINVAL;
	}

	if (!in4_pton(ip, -1, (u8 *)&daddr, -1, NULL)) {
		pr_err("invalid IP address \"%s\"\n", ip);
		return -EINVAL;
	}

	return intel7084_bridge_redirect_hw_apply(ntohl(daddr));
}

int intel7084_bridge_redirect_disable(void)
{
	return intel7084_bridge_redirect_hw_remove();
}

#ifdef CONFIG_SFAX8_ESWITCH_REDIRECT
// rule 8/9 for port2/3 to upload arp request packet
int intel7084_bridge_redirect_arp(void)
{
    GSW_return_t ret;
    GSW_PCE_rule_t rule;
    memset(&rule, 0, sizeof(rule));

    // Set Port 2 to upload ARP packets（Rule 8）
	rule.pattern.bPortIdEnable      = 1;
	rule.pattern.nPortId            = 2;  // Port 2
	rule.pattern.bEnable            = 1;
	rule.pattern.nIndex             = 8;  // Rule index number 8

	// Match ARP protocol (Ethertype 0x0806)
    rule.pattern.bEtherTypeEnable = 1;
    rule.pattern.nEtherType = 0x0806;
    rule.pattern.nEtherTypeMask = 0xFFFF;

    // Forward only to CPU, do not set other ports
    rule.action.ePortMapAction      = GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
    rule.action.nForwardPortMap     = BIT(RGMII_PORT0);  // CPU port

    SF_MDIO_LOCK();
    ret = GSW_PceRuleWrite(pedev0[0], &rule);
    SF_MDIO_UNLOCK();

    if (ret == GSW_statusOk)
        pr_info("enable to upload arp req index 8\n");
    else
        pr_err("failed to enable to upload arp req index 8\n");

    // Set Port 3 to upload ARP packets（Rule 9）
    rule.pattern.nIndex             = 9;
    rule.pattern.nPortId            = 3;

    SF_MDIO_LOCK();
    ret = GSW_PceRuleWrite(pedev0[0], &rule);
    SF_MDIO_UNLOCK();

    if (ret == GSW_statusOk)
        pr_info("enable to upload arp req index 9\n");

    return ret;
}

int intel7084_bridge_redirect_arp_disabled(void)
{
    GSW_return_t ret;
	GSW_PCE_ruleDelete_t del;
	int i;

	memset(&del, 0, sizeof(del));

	for(i = 8; i < 10; i++) {
		del.nIndex	= i;

		SF_MDIO_LOCK();
		ret = GSW_PceRuleDelete(pedev0[0], &del);
		SF_MDIO_UNLOCK();

		if (ret == GSW_statusOk) {
			pr_info("disable to upload arp req index %u\n", i);
		} else {
			pr_err("failed to disable upload arp index %u with ret: %d\n", i, ret);
		}
	}

	return ret;
}

// rule 6/7 for port2/3 to upload dhcp request packet
int intel7084_bridge_redirect_dhcp(void)
{
	GSW_return_t ret;
	GSW_PCE_rule_t rule;
	int i;

	memset(&rule, 0, sizeof(rule));
	// 设置port2上传 dhcp 请求
	rule.pattern.bEnable	= 1;
	rule.pattern.nIndex		= 6;  // rule 6
	rule.pattern.bProtocolEnable = 1;
	rule.pattern.nProtocol = 17; //udp协议
	rule.pattern.nProtocolMask = 0;
	rule.pattern.bAppDataLSB_Enable = 1;
	rule.pattern.nAppDataLSB = 67; // 目的端口, dhcp 请求
	rule.pattern.bPortIdEnable = 1;
	rule.pattern.nPortId = 2;   // port2

	rule.action.ePortMapAction	= GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
	rule.action.nForwardPortMap	= BIT(RGMII_PORT0); /* CPU port */

	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();

	if (ret == GSW_statusOk)
		pr_info("enable to upload dhcp req index 6\n");
	else
		pr_err("failed to enable to upload dhcp req index 6\n");

	// 设置port3上传 dhcp 请求
	rule.pattern.nIndex		= 7;  // rule 7
	rule.pattern.nPortId = 3;     // port3

	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();

	if (ret == GSW_statusOk)
		pr_info("enable to upload dhcp req index 7\n");

	return ret;
}

int intel7084_bridge_redirect_dhcp_disabled(void)
{
	GSW_return_t ret;
	GSW_PCE_ruleDelete_t del;
	int i;

	memset(&del, 0, sizeof(del));

	for(i = 6; i < 8; i++) {
		del.nIndex	= i;

		SF_MDIO_LOCK();
		ret = GSW_PceRuleDelete(pedev0[0], &del);
		SF_MDIO_UNLOCK();

		if (ret == GSW_statusOk) {
			pr_info("disabled to upload udp dhcp req index %u\n", i);
		} else {
			pr_err("failed to disable upload udp dhcp req index %u with ret: %d\n", i, ret);
		}
	}

	return ret;

}

// rule 2/3/4/5 for port2/3 to upload dns request packet
int intel7084_bridge_redirect_dns(void)
{
	GSW_return_t ret;
	GSW_PCE_rule_t rule;
	int i;

	// 设置port2上传 udp dns请求
	memset(&rule, 0, sizeof(rule));
	rule.pattern.nIndex = 2;  // rule 2
	rule.pattern.bEnable = 1;
	rule.pattern.bProtocolEnable = 1;
	rule.pattern.nProtocol = 17; //udp协议
	rule.pattern.nProtocolMask = 0;
	rule.pattern.bAppDataLSB_Enable = 1;
	rule.pattern.nAppDataLSB = 53; // 目的端口, dns请求
	rule.pattern.bPortIdEnable = 1;
	rule.pattern.nPortId = 2; // port2

	rule.action.ePortMapAction	= GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
	rule.action.nForwardPortMap	= BIT(RGMII_PORT0); /* CPU port */

	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();
	if (ret == GSW_statusOk)
		pr_info("enable to upload udp dns req index 2\n");
	else
		pr_err("failed to enable upload udp dns req index 2\n");

	// 设置port3上传 udp dns请求
	rule.pattern.nIndex = 3; // rule 3
	rule.pattern.nPortId = 3; // port3
	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();
	if (ret == GSW_statusOk)
		pr_info("enable to upload udp dns req index 3\n");

	// 设置port2上传 tcp dns请求
	memset(&rule, 0, sizeof(rule));
	rule.pattern.bEnable	= 1;
	rule.pattern.nIndex		= 4;  // rule 4
	rule.pattern.bProtocolEnable = 1;
	rule.pattern.nProtocol = 6; //tcp 协议
	rule.pattern.nProtocolMask = 0;
	rule.pattern.bAppDataLSB_Enable = 1;
	rule.pattern.nAppDataLSB = 53; // 目的端口, dns请求
	rule.pattern.bPortIdEnable = 1;
	rule.pattern.nPortId = 2;  // port2

	rule.action.ePortMapAction	= GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
	rule.action.nForwardPortMap	= BIT(RGMII_PORT0); /* CPU port */

	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();

	if (ret == GSW_statusOk)
		pr_info("enable to upload tcp dns req index 4\n");

	// 设置port3上传 tcp dns请求
	rule.pattern.nIndex = 5;   // rule 5
	rule.pattern.nPortId = 3;  // port3

	SF_MDIO_LOCK();
	ret = GSW_PceRuleWrite(pedev0[0], &rule);
	SF_MDIO_UNLOCK();

	if (ret == GSW_statusOk)
		pr_info("enable to upload tcp dns req index 5\n");

	return ret;
}

int intel7084_bridge_redirect_dns_disabled(void)
{
	GSW_return_t ret;
	GSW_PCE_ruleDelete_t del;
	int i;

	memset(&del, 0, sizeof(del));

	for(i = 2; i < 6; i++) {
		del.nIndex	= i;

		SF_MDIO_LOCK();
		ret = GSW_PceRuleDelete(pedev0[0], &del);
		SF_MDIO_UNLOCK();

		if (ret == GSW_statusOk) {
			pr_info("disabled to upload udp dns/dhcp req index %u\n", i);
		} else {
			pr_err("failed to disable upload udp dns/dhcp req index %u with ret: %d\n", i, ret);
		}
	}

	return ret;

}
#endif