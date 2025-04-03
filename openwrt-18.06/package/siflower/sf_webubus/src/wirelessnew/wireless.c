/*
 * rpcd - UBUS RPC server
 *
 *   Copyright (C) 2013 Jo-Philipp Wich <jow@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _GNU_SOURCE /* crypt() */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <signal.h>
#include <glob.h>
#include <libubox/blobmsg_json.h>
#include <libubox/avl-cmp.h>
#include <libubus.h>
#include <uci.h>


#include <rpcd/plugin.h>
#include <stdio.h>

static const struct rpc_daemon_ops *ops;

static struct blob_buf buf;
static struct uci_context *cursor;

static void get_cmd_result(const char *cmd, char *tmp, int len) {
	FILE *p_file = NULL;
	memset(tmp, 0, len);
	p_file = popen(cmd, "r");
	if (p_file) {
		while (fgets(tmp, len, p_file) != NULL) {}
		pclose(p_file);
	}
}

enum {
	RPC_D_DATA,
	__RPC_D_MAX
};

static const struct blobmsg_policy rpc_data_policy[__RPC_D_MAX] = {
	[RPC_D_DATA]   = { .name = "data",  .type = BLOBMSG_TYPE_STRING },
};

static int
rpc_web_get_test(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	char lan_ip[16] = "";
	struct uci_package *p;
	struct uci_ptr ptr = {
		.package = "network",
		.section = "lan",
		.option  = "ipaddr"
	};

	if (uci_load(cursor, ptr.package, &p) || !p)
		goto out;

	uci_lookup_ptr(cursor, &ptr, NULL, true);

	if (ptr.o && ptr.o->type == UCI_TYPE_STRING)
		strcpy(lan_ip, ptr.o->v.string);

	uci_unload(cursor, p);

out:
	blob_buf_init(&buf, 0);
	blobmsg_add_string(&buf, "ipaddr", lan_ip);
	ubus_send_reply(ctx, req, buf.head);
	return 0;
}

static int
rpc_web_set_test(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	struct blob_attr *tb[__RPC_D_MAX];

	struct uci_package *p;
	struct uci_ptr ptr = {
		.package = "network",
		.section = "lan",
		.option  = "ipaddr",
		.value  = "192.168.6.1"
	};

	printf("====value1===%s\n", ptr.value);
	blobmsg_parse(rpc_data_policy, __RPC_D_MAX, tb,
	              blob_data(msg), blob_len(msg));

	if (!tb[RPC_D_DATA] || blobmsg_data_len(tb[RPC_D_DATA]) >= 128)
		return UBUS_STATUS_INVALID_ARGUMENT;

	ptr.value = blobmsg_data(tb[RPC_D_DATA]);
	printf("====value2===%s\n", ptr.value);

	uci_load(cursor, ptr.package, &p);
	uci_set(cursor, &ptr);
	uci_save(cursor, p);
	uci_commit(cursor, &p, true);
	uci_unload(cursor, p);

	return 0;
}

enum {
	RPC_S_DEVICE,
	RPC_S_SSID,
	RPC_S_BSSID,
	RPC_S_ENCRYPTION,
	RPC_S_KEY,
	__RPC_S_MAX
};

static const struct blobmsg_policy rpc_wificonn_policy[__RPC_S_MAX] = {
	[RPC_S_DEVICE]     = {.name = "device",     .type = BLOBMSG_TYPE_STRING},
	[RPC_S_SSID]       = {.name = "ssid",       .type = BLOBMSG_TYPE_STRING},
	[RPC_S_BSSID]      = {.name = "bssid",      .type = BLOBMSG_TYPE_STRING},
	[RPC_S_ENCRYPTION] = {.name = "encryption", .type = BLOBMSG_TYPE_STRING},
	[RPC_S_KEY]        = {.name = "key",        .type = BLOBMSG_TYPE_STRING},
};

enum {
	RPC_SN_SSID,
	RPC_SN_ENCRYPTION,
	RPC_SN_KEY,
    RPC_SN_FLAG,
	__RPC_SN_MAX
};

static const struct blobmsg_policy rpc_wificonnnew_policy[__RPC_SN_MAX] = {
	[RPC_SN_SSID]       = {.name = "ssid",       .type = BLOBMSG_TYPE_STRING},
	[RPC_SN_ENCRYPTION] = {.name = "encryption", .type = BLOBMSG_TYPE_STRING},
	[RPC_SN_KEY]        = {.name = "key",        .type = BLOBMSG_TYPE_STRING},
    [RPC_SN_FLAG]        = {.name = "flag",        .type = BLOBMSG_TYPE_STRING},
};

enum {
	RPC_I_DEVICE,
	RPC_I_SSID,
	RPC_I_ENCRYPTION,
	RPC_I_KEY,
	RPC_I_STATUS,
	__RPC_I_MAX
};

static const struct blobmsg_policy rpc_wifiself_policy[__RPC_I_MAX] = {
	[RPC_I_DEVICE]     = {.name = "device",     .type = BLOBMSG_TYPE_STRING},
	[RPC_I_SSID]       = {.name = "ssid",       .type = BLOBMSG_TYPE_STRING},
	[RPC_I_ENCRYPTION] = {.name = "encryption", .type = BLOBMSG_TYPE_STRING},
	[RPC_I_KEY]        = {.name = "key",        .type = BLOBMSG_TYPE_STRING},
	[RPC_I_STATUS]     = {.name = "status",     .type = BLOBMSG_TYPE_STRING}
};
enum {
	RPC_T_RADIO,
	__RPC_T_MAX
};

static const struct blobmsg_policy rpc_status_policy[__RPC_T_MAX] = {
	[RPC_T_RADIO] = {.name = "radio", .type = BLOBMSG_TYPE_STRING},
};


enum {
	RPC_W_IFNAME,
	RPC_W_IP,
	__RPC_W_MAX
};

static const struct blobmsg_policy rpc_wds_policy[__RPC_W_MAX] = {
	[RPC_W_IFNAME] = {.name = "ifname", .type = BLOBMSG_TYPE_STRING},
	[RPC_W_IP] = {.name = "ip", .type = BLOBMSG_TYPE_STRING},
};

enum {
	RPC_L_RADIO,
	RPC_L_TXPOWER_LVL,
	__RPC_L_MAX
};

static const struct blobmsg_policy rpc_set_txpower_lvl_policy[__RPC_L_MAX] = {
	[RPC_L_RADIO] = {.name = "radio", .type = BLOBMSG_TYPE_STRING},
	[RPC_L_TXPOWER_LVL] = {.name = "txpower_lvl", .type = BLOBMSG_TYPE_STRING},
};

static int get(char *package, char *section, char *option, char *buf) {
	struct uci_context *ctx = uci_alloc_context();
	struct uci_package *p = NULL;
	int ret = -1;
	uci_set_confdir(ctx, "/etc/config");
	if(uci_load(ctx, package, &p) == UCI_OK)
	{
		struct uci_section *val = uci_lookup_section(ctx, p, section);
		if(val != NULL){
			const char *value = uci_lookup_option_string(ctx, val, option);
			if(value != NULL){
				ret = 0;
				sprintf(buf,"%s",value);
			}
		}
		uci_unload(ctx,p);
	}
	uci_free_context(ctx);
	return ret;
}


/*static int set_ano(char* package,char* option,char* value,int pos){
	struct uci_context *now = uci_alloc_context();
	struct uci_package *pack = NULL;
	struct uci_element *e;
	struct uci_ptr ptr = {
		.package = package,
		.option = option,
		.value = value
	};
	uci_load(now, ptr.package, &pack);
	int i = 1;
	uci_foreach_element(&pack->sections, e) {
		struct uci_section *s = uci_to_section(e);
		ptr.section = s->e.name;
		if (i == pos)
			break;
		else
			i++;
	}
	uci_set(now, &ptr);
	uci_save(now, pack);
	uci_commit(now, &pack, true);
	uci_unload(now, pack);
	uci_free_context(now);
	now = NULL;
	return 0;
}*/

static int set(char* package, char* section, char* option, char* value) {
	struct uci_context *now = uci_alloc_context();
	struct uci_package *pack = NULL;
	struct uci_ptr ptr = {
		.package = package,
		.section = section,
		.option = option,
		.value = value
	};
	uci_load(now, ptr.package, &pack);
	uci_set(now, &ptr);
	uci_save(now, pack);
	uci_commit(now, &pack, true);
	uci_unload(now, pack);
	uci_free_context(now);
	now = NULL;
	return 0;
}

static int del(char* package, char* section, char* option) {
	struct uci_context *now = uci_alloc_context();
	struct uci_package *pack = NULL;
	struct uci_ptr ptr = {
		.package = package,
		.section = section,
		.option = option
	};
	uci_load(now, ptr.package, &pack);
	uci_delete(now, &ptr);
	uci_save(now, pack);
	uci_commit(now, &pack, false);
	uci_unload(now, pack);
	uci_free_context(now);
	now = NULL;
	return 0;
}

static int rpc_web_wireless_wds_wifi_self(struct ubus_context *ctx,
										  struct ubus_object *obj,
										  struct ubus_request_data *req,
										  const char *method,
										  struct blob_attr *msg) {
	struct blob_attr *tb[__RPC_I_MAX];
	blobmsg_parse(rpc_wifiself_policy, __RPC_I_MAX, tb, blob_data(msg),
				  blob_len(msg));

	char wl[] = "wireless", nw[] = "network", s[32];

	if (!strcmp(blobmsg_data(tb[RPC_I_STATUS]), "true")) {
		if (!strcmp(blobmsg_data(tb[RPC_I_DEVICE]), "wlan0-1")) {
			strcpy(s, "default_radio0");
		} else {
			strcpy(s, "default_radio1");
		}
		set(wl, s, "ssid", blobmsg_data(tb[RPC_I_SSID]));
		set(wl, s, "encryption", blobmsg_data(tb[RPC_I_ENCRYPTION]));
		if (strcmp(blobmsg_data(tb[RPC_I_ENCRYPTION]), "none")) {
			set(wl, s, "key", blobmsg_data(tb[RPC_I_KEY]));
		}
		system("wifi reload");
	} else if (!strcmp(blobmsg_data(tb[RPC_I_STATUS]), "false")) {
		del(nw, "wwan", NULL);
		del(wl, "wds", NULL);
		system("/etc/init.d/network restart");
	}
	return 0;
}

static int rpc_web_wireless_wds_wifi_connect(struct ubus_context *ctx,
											 struct ubus_object *obj,
											 struct ubus_request_data *req,
											 const char *method,
											 struct blob_attr *msg) {
	struct blob_attr *tb[__RPC_S_MAX];
	blobmsg_parse(rpc_wificonn_policy, __RPC_S_MAX, tb, blob_data(msg),
				  blob_len(msg));

	char wl[] = "wireless", s[32] = "wds", d0[] ="default_radio0",d1[] ="default_radio1",v[256] = "";
	set(wl, s, NULL, "wifi-iface");
	if (!strcmp(blobmsg_data(tb[RPC_S_DEVICE]), "radio0")) {
		set(wl, s, "device", "radio0");
		set(wl, s, "ifname", "wlan0-1");

	} else if (!strcmp(blobmsg_data(tb[RPC_S_DEVICE]), "radio1")) {
		set(wl, s, "device", "radio1");
		set(wl, s, "ifname", "wlan1-1");
	}
	set(wl, s, "ssid", blobmsg_data(tb[RPC_S_SSID]));
	set(wl, s, "bssid", blobmsg_data(tb[RPC_S_BSSID]));
	if (!strcmp(blobmsg_data(tb[RPC_S_ENCRYPTION]), "WPA2(PSK)")) {
		strcpy(v, "psk2+ccmp");
	} else if (!strcmp(blobmsg_data(tb[RPC_S_ENCRYPTION]), "Open")) {
		strcpy(v, "none");
	} else if (!strcmp(blobmsg_data(tb[RPC_S_ENCRYPTION]), "WPA(PSK)")) {
		strcpy(v, "psk");
	} else if (!strcmp(blobmsg_data(tb[RPC_S_ENCRYPTION]), "WPA/WPA2(PSK) MIXED")) {
		strcpy(v, "psk-mixed");
	}
	set(wl, s, "encryption", v);
	set(wl, d0, "encryption", v);
	set(wl, d1, "encryption", v);
	if (strcmp(v, "none")) {
		set(wl, s, "key", blobmsg_data(tb[RPC_S_KEY]));
		set(wl, d0, "key", blobmsg_data(tb[RPC_S_KEY]));
		set(wl, d1, "key", blobmsg_data(tb[RPC_S_KEY]));
	}
	set(wl, s, "mode", "sta");

	system("wifi reload");
	system("usr/sbin/sf_wpad start_wpa &");

	return 0;
}

static int rpc_web_wireless_wds_wifi_connect_new(struct ubus_context *ctx,
											 struct ubus_object *obj,
											 struct ubus_request_data *req,
											 const char *method,
											 struct blob_attr *msg) {
	struct blob_attr *tb[__RPC_SN_MAX];
	blobmsg_parse(rpc_wificonnnew_policy, __RPC_SN_MAX, tb, blob_data(msg),
				  blob_len(msg));

	char wl[] = "wireless", s[32] = "wds",d0[] ="default_radio0",d1[] ="default_radio1", v[256] = "";
	set(wl, s, NULL, "wifi-iface");
	if (!strcmp(blobmsg_data(tb[RPC_SN_FLAG]), "2.4G")) {
		set(wl, s, "device", "radio0");
		set(wl, s, "ifname", "wlan0-1");

	} else if (!strcmp(blobmsg_data(tb[RPC_SN_FLAG]), "5G")) {
		set(wl, s, "device", "radio1");
		set(wl, s, "ifname", "wlan1-1");
	}
	set(wl, s, "ssid", blobmsg_data(tb[RPC_SN_SSID]));

	if (!strcmp(blobmsg_data(tb[RPC_SN_ENCRYPTION]), "WPA2(PSK)")) {
		strcpy(v, "psk2+ccmp");
	} else if (!strcmp(blobmsg_data(tb[RPC_SN_ENCRYPTION]), "Open")) {
		strcpy(v, "none");
	} else if (!strcmp(blobmsg_data(tb[RPC_SN_ENCRYPTION]), "WPA(PSK)")) {
		strcpy(v, "psk");
	} else if (!strcmp(blobmsg_data(tb[RPC_SN_ENCRYPTION]), "WPA/WPA2(PSK) MIXED")) {
		strcpy(v, "psk-mixed");
	}
	set(wl, s, "encryption", v);
	set(wl, d0, "encryption", v);
	set(wl, d1, "encryption", v);
	if (strcmp(v, "none")) {
		set(wl, s, "key", blobmsg_data(tb[RPC_SN_KEY]));
		set(wl, d0, "key", blobmsg_data(tb[RPC_SN_KEY]));
		set(wl, d1, "key", blobmsg_data(tb[RPC_SN_KEY]));
	}
	set(wl, s, "mode", "sta");

	system("wifi reload");
	system("usr/sbin/sf_wpad start_wpa &");

	return 0;
}

static int rpc_web_wireless_wds_wifi_connect_status(struct ubus_context *ctx,
									   struct ubus_object *obj,
									   struct ubus_request_data *req,
									   const char *method,
									   struct blob_attr *msg) {
	struct blob_attr *tb[__RPC_T_MAX];
	char *ifname;
	char encryption[64] = {};
	char tmp[64] = {};
	char cmd[200] = {};
	int radio = 0;

	blobmsg_parse(rpc_status_policy, __RPC_T_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[RPC_T_RADIO] || blobmsg_data_len(tb[RPC_T_RADIO]) >= 128)
		return UBUS_STATUS_INVALID_ARGUMENT;


	ifname = blobmsg_data(tb[RPC_T_RADIO]);

	get("wireless", "wds", "encryption", encryption);
	if (!strcmp(ifname, "wlan1-1"))
		radio = 1;
	sprintf(cmd, "cat /sys/kernel/debug/ieee80211/phy%d/siwifi/repeater_status | tr -d '\n'", radio);
	get_cmd_result(cmd, tmp, 64);

	blob_buf_init(&buf, 0);

	if ((!strcmp(encryption, "none")) && !strcmp(tmp, "repeater assoc!"))
		blobmsg_add_string(&buf, "status", "ok");
	else if(!strcmp(tmp, "repeater eapol!"))
		blobmsg_add_string(&buf, "status", "ok");
	else
		blobmsg_add_string(&buf, "status", "failed");

	ubus_send_reply(ctx, req, buf.head);

	return 0;
}

static int rpc_web_wireless_wds_enable(struct ubus_context *ctx,
									   struct ubus_object *obj,
									   struct ubus_request_data *req,
									   const char *method,
									   struct blob_attr *msg) {
	struct blob_attr *tb[__RPC_W_MAX];
	blobmsg_parse(rpc_wds_policy, __RPC_W_MAX, tb, blob_data(msg), blob_len(msg));

	char *ifname = blobmsg_data(tb[RPC_W_IFNAME]);
	char *ip = blobmsg_data(tb[RPC_W_IP]);
	char cmd[128];
	char iface[16];

	set("network", "lan", "proto", "dhcp");
	set("network", "lan", "oip", ip);
	del("network", "lan", "ipaddr");
	del("network", "lan", "netmask");
	del("network", "lan","ip6assign");

	set("wireless", "guest_radio0", "disabled", "1");
	set("wireless", "guest_radio1", "disabled", "1");

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "uci show wireless | grep \"'%s'\" | awk -F . '{print $2}' | tr -d '\n'", ifname);
	get_cmd_result(cmd, iface, sizeof(iface));

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "uci set wireless.%s.network=lan", iface);
	system(cmd);
	system("uci commit wireless 2>/dev/console");

	system("/etc/init.d/network restart");

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "sh /www/luci2/scripts/wds_status.sh %s", ifname);
	system(cmd);

	return 0;
}

static int rpc_web_wireless_wds_disable(struct ubus_context *ctx,
										struct ubus_object *obj,
										struct ubus_request_data *req,
										const char *method,
										struct blob_attr *msg) {

	//make sure htmode without "+-"
	char htmode[6] = {};
	char oip[20] = {};
	char hosts[32] = {};
	int ret = 0;
#ifdef WDS_SSID_FOLLOW
	char ossid[32];
	char iface[8];
#endif
	char command[100];

	get("wireless", "radio0", "htmode", htmode);
	ret = get("network", "lan", "oip", oip);
	htmode[5] = '\0';
	set("wireless", "radio0", "htmode", htmode);

#ifdef WDS_SSID_FOLLOW
	get("wireless", "wds", "device", iface);
	iface[6] = '\0';
	if (!strcmp(iface, "radio0")) {
		ret = get("wireless", "default_radio0", "ossid", ossid);
		if(ret == 0) {
			set("wireless", "default_radio0", "ssid", ossid);
			del("wireless", "default_radio0", "ossid");
		}
	} else if(!strcmp(iface, "radio1")) {
		ret = get("wireless", "default_radio1", "ossid", ossid);
		if(ret == 0) {
			set("wireless", "default_radio1", "ssid", ossid);
			del("wireless", "default_radio1", "ossid");
		}
	}
#endif
	del("wireless", "wds", NULL);
	if (ret == 0) {
		set("network", "lan", "proto", "static");
		set("network", "lan", "ipaddr", oip);
		set("network", "lan", "netmask", "255.255.255.0");
		set("network", "lan", " ip6assign", "60");
		get("basic_setting", "vendor", "hosts", hosts);
		sprintf(command, "/usr/sbin/dns_redirect.sh %s enable %s", hosts, oip);
        system(command);
		system("sh /sbin/internet_detect.sh 4");
		system("/etc/init.d/network restart");
	    return 0;
	}
	system("sh /sbin/internet_detect.sh 4");
	system("/etc/init.d/dnsmasq start");
	system("/etc/init.d/network restart");

	return 0;
}

static int
rpc_web_get_channel_info(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	void *table;
	char channel_24g[10];
	char channel_5g[10];
	blob_buf_init(&buf, 0);

	table = blobmsg_open_table(&buf, "info");
	memset(channel_24g, 0, sizeof(channel_24g));
	memset(channel_5g, 0, sizeof(channel_5g));

	get("wireless", "radio0", "channel", channel_24g);
	if(strcmp(channel_24g, "auto")) {
		memset(channel_24g, 0, sizeof(channel_24g));
		get_cmd_result(" iwinfo | grep -A 6 -w 'wlan0 ' | grep Mode | awk '{print $4}' | tr -d '\n' ", channel_24g, 10);
	}
	blobmsg_add_string(&buf, "channel_24g", channel_24g);

	get("wireless", "radio1", "channel", channel_5g);
	if(strcmp(channel_5g, "auto")) {
		memset(channel_5g, 0, sizeof(channel_5g));
		get_cmd_result(" iwinfo | grep -A 6 -w 'wlan1 ' | grep Mode | awk '{print $4}' | tr -d '\n'", channel_5g, 10);
	}
	blobmsg_add_string(&buf, "channel_5g", channel_5g);

	blobmsg_close_table(&buf, table);
	ubus_send_reply(ctx, req, buf.head);

	return 0;
}

static int
rpc_web_get_htmodelist(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	void *table;
	char htmodelist_24g[15];
	char htmodelist_5g[15];
	blob_buf_init(&buf, 0);

	table = blobmsg_open_table(&buf, "htmodelist");
	memset(htmodelist_24g, 0, sizeof(htmodelist_24g));
	memset(htmodelist_5g, 0, sizeof(htmodelist_5g));

	get_cmd_result(" iwinfo | grep -A 10 -w 'wlan0 ' | grep Type | awk '{print $5}' | tr -d '\n' ", htmodelist_24g, 15);
	blobmsg_add_string(&buf, "htmodelist_24g", htmodelist_24g);

	get_cmd_result(" iwinfo | grep -A 10 -w 'wlan1 ' | grep Type | awk '{print $5}' | tr -d '\n'", htmodelist_5g, 15);
	blobmsg_add_string(&buf, "htmodelist_5g", htmodelist_5g);

	blobmsg_close_table(&buf, table);
	ubus_send_reply(ctx, req, buf.head);

	return 0;
}

static int
rpc_web_wireless_set_txpower_lvl(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	struct blob_attr *tb[__RPC_L_MAX];
	blobmsg_parse(rpc_set_txpower_lvl_policy, __RPC_L_MAX, tb, blob_data(msg), blob_len(msg));

	void *table;
	char *radio = blobmsg_data(tb[RPC_L_RADIO]);
	char *txpower_lvl = blobmsg_data(tb[RPC_L_TXPOWER_LVL]);
	char cmd[128];
	char log[128];
	char phy_idx[4];
	blob_buf_init(&buf, 0);
	table = blobmsg_open_table(&buf, "txpower_lvl");

	memset(log, 0, sizeof(log));
	sprintf(log, "rpc_web_wireless_set_txpower_lvl %s %s", radio, txpower_lvl);
	blobmsg_add_string(&buf, "log1", log);
	memset(cmd, 0, sizeof(cmd));
	if(!strcmp(radio, "radio0")) {
		sprintf(cmd, "iw dev | grep '^phy#' | sed -n '2s/phy#//p' | tr -d '\n'");
		get_cmd_result(cmd, phy_idx, sizeof(phy_idx));

		memset(log, 0, sizeof(log));
		sprintf(log, "radio0 %s ", phy_idx);
		blobmsg_add_string(&buf, "log2", log);

		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "echo %s > /sys/kernel/debug/ieee80211/phy%s/siwifi/txpower_lvl", txpower_lvl, phy_idx);
	} else if(!strcmp(radio, "radio1")) {
		sprintf(cmd, "iw dev | grep '^phy#' | sed -n 's/phy#//p' | head -n 1 | tr -d '\n'");
		get_cmd_result(cmd, phy_idx, sizeof(phy_idx));

		memset(log, 0, sizeof(log));
		sprintf(log, "radio1 %s ", phy_idx);
		blobmsg_add_string(&buf, "log2", log);

		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "echo %s > /sys/kernel/debug/ieee80211/phy%s/siwifi/txpower_lvl", txpower_lvl, phy_idx);
	}
	system(cmd);

	blobmsg_close_table(&buf, table);
	ubus_send_reply(ctx, req, buf.head);

	return 0;
}

static int
rpc_web_api_init(const struct rpc_daemon_ops *o, struct ubus_context *ctx)
{
	int rv = 0;

	static const struct ubus_method web_wireless_methods[] = {
		UBUS_METHOD_NOARG("get_test",         rpc_web_get_test),
		UBUS_METHOD_NOARG("get_channel_info", 		rpc_web_get_channel_info),
		UBUS_METHOD_NOARG("get_htmodelist", 		rpc_web_get_htmodelist),
		UBUS_METHOD_NOARG("wds_disable",            rpc_web_wireless_wds_disable),
		UBUS_METHOD("set_test",               rpc_web_set_test, rpc_data_policy),
		UBUS_METHOD("wds_wifi_self",          rpc_web_wireless_wds_wifi_self, rpc_wifiself_policy),
		UBUS_METHOD("wds_wifi_connect",       rpc_web_wireless_wds_wifi_connect, rpc_wificonn_policy),
		UBUS_METHOD("wds_wifi_connect_new",       rpc_web_wireless_wds_wifi_connect_new, rpc_wificonnnew_policy),
		UBUS_METHOD("wds_wifi_connect_status",       rpc_web_wireless_wds_wifi_connect_status, rpc_status_policy),
		UBUS_METHOD("wds_enable",             rpc_web_wireless_wds_enable, rpc_wds_policy),
		UBUS_METHOD("set_txpower_lvl",             rpc_web_wireless_set_txpower_lvl, rpc_set_txpower_lvl_policy)
	};

	static struct ubus_object_type web_wireless_type =
		UBUS_OBJECT_TYPE("siflower-rpc-web-wireless", web_wireless_methods);

	static struct ubus_object wireless_obj = {
		.name = "web.wireless",
		.type = &web_wireless_type,
		.methods = web_wireless_methods,
		.n_methods = ARRAY_SIZE(web_wireless_methods),
	};

	cursor = uci_alloc_context();

	if (!cursor)
		return UBUS_STATUS_UNKNOWN_ERROR;

	ops = o;

	rv |= ubus_add_object(ctx, &wireless_obj);

	return rv;
}

struct rpc_plugin rpc_plugin = {
	.init = rpc_web_api_init
};
