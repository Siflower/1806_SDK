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

static void get_cmd_result(const char *cmd, char *tmp, int len) {
	FILE *p_file = NULL;
	memset(tmp, 0, len);
	p_file = popen(cmd, "r");
	if (p_file) {
		while (fgets(tmp, len, p_file) != NULL) {}
		pclose(p_file);

		int str_len = strlen(tmp);
		if (str_len > 0 && tmp[str_len - 1] == '\n') {
			tmp[str_len - 1] = '\0';
		}
	}
}

static const struct rpc_daemon_ops *ops;

static struct blob_buf buf;
static struct uci_context *cursor;
enum {
	RPC_D_DATA,
	__RPC_D_MAX
};

static const struct blobmsg_policy rpc_data_policy[__RPC_D_MAX] = {
	[RPC_D_DATA]   = { .name = "data",  .type = BLOBMSG_TYPE_STRING },
};

enum {
	RPC_F_NAME,
	__RPC_F_MAX
};

static const struct blobmsg_policy rpc_get_factory_info_policy[__RPC_F_MAX] = {
	[RPC_F_NAME] = {.name = "name", .type = BLOBMSG_TYPE_STRING},
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

static char* get(char* package, char* section, char* option) {
	char *value = (char*)malloc(100*sizeof(char));
	struct uci_context *now = uci_alloc_context();
	struct uci_package *pack = NULL;
	struct uci_ptr ptr = {
		.package = package,
		.section = section,
		.option = option
	};
	uci_load(now, ptr.package, &pack);
	uci_lookup_ptr(cursor, &ptr, NULL, true);
	if (ptr.o && ptr.o->type == UCI_TYPE_STRING)
		strcpy(value, ptr.o->v.string);
	uci_unload(now, pack);
	uci_free_context(now);
	now = NULL;
	return value;
}

static int
rpc_web_get_server_addr(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	char* ip = get("sicloud","addr","ip");
	char* port = get("sicloud","addr","port");
	char* version = get("sicloud","addr","version");
	char* tmp1 = strcat(ip,":");
	char* tmp2 = strcat(port,"/");
	char* tmp3 = strcat(tmp1,tmp2);
	char* server_addr = strcat(tmp3,version);
	char* cloudtype = get("sicloud","addr","cloudtype");
	char* chip = "fullmask";
	char* sn = "fffffffff";
	char* imagetype = "0";
	char* env = get("basic_setting","ota","env");
	blob_buf_init(&buf, 0);
	blobmsg_add_string(&buf, "server_addr", server_addr);
	blobmsg_add_string(&buf, "cloudtype", cloudtype);
	blobmsg_add_string(&buf, "chip", chip);
	blobmsg_add_string(&buf, "sn", sn);
	blobmsg_add_string(&buf, "imagetype", imagetype);
	blobmsg_add_string(&buf, "env", env);
	ubus_send_reply(ctx, req, buf.head);
	return 0;

}

static int
rpc_web_get_factory_info(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
	struct blob_attr *tb[__RPC_F_MAX];
	blobmsg_parse(rpc_get_factory_info_policy, __RPC_F_MAX, tb, blob_data(msg), blob_len(msg));

	char *name = blobmsg_data(tb[RPC_F_NAME]);
	char cmd[128];
	char result[128];
	int i;

	blob_buf_init(&buf, 0);
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "cat sys/devices/platform/factory-read/%s", name);
	get_cmd_result(cmd, result, sizeof(result));

    for (i = 0; result[i]!= '\0'; i++) {
        if ((unsigned char)result[i] < 32 || (unsigned char)result[i] >= 127) {
            result[i] = ' ';
        }
    }

	blobmsg_add_string(&buf, "result", result);

	ubus_send_reply(ctx, req, buf.head);

	return 0;
}

static int
rpc_web_api_init(const struct rpc_daemon_ops *o, struct ubus_context *ctx)
{
	int rv = 0;

	static const struct ubus_method web_system_methods[] = {
		UBUS_METHOD_NOARG("get_test",           rpc_web_get_test),
		UBUS_METHOD_NOARG("get_server_addr",    rpc_web_get_server_addr),
		UBUS_METHOD("get_factory_info",         rpc_web_get_factory_info, rpc_get_factory_info_policy),
		UBUS_METHOD("set_test",                 rpc_web_set_test, rpc_data_policy),
	};

	static struct ubus_object_type web_system_type =
		UBUS_OBJECT_TYPE("siflower-rpc-web-system", web_system_methods);

	static struct ubus_object system_obj = {
		.name = "web.system",
		.type = &web_system_type,
		.methods = web_system_methods,
		.n_methods = ARRAY_SIZE(web_system_methods),
	};

	cursor = uci_alloc_context();

	if (!cursor)
		return UBUS_STATUS_UNKNOWN_ERROR;

	ops = o;

	rv |= ubus_add_object(ctx, &system_obj);

	return rv;
}

struct rpc_plugin rpc_plugin = {
	.init = rpc_web_api_init
};
