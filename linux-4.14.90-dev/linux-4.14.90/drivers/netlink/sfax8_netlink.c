#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <asm/types.h>
#include <linux/inet.h>
#include <net/sock.h>
#include <linux/socket.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <net/genetlink.h>

#include <linux/sfax8_netlink.h>

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>


int sfax8_genl_debug_show(struct seq_file *m, void *v)
{
	seq_printf(m,"generic netlink debug interface\n");
	return 0;
}

int sfax8_genl_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, sfax8_genl_debug_show, NULL);
}

ssize_t sfax8_genl_debug_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	int ret = 0;
	ssize_t read;
	char recvbuf[256] = {0};

	ret = sprintf(recvbuf, "Welcome to sfax8 generic netlink module!\n");
	read = simple_read_from_buffer(user_buf, count, ppos, recvbuf, ret);
	return ret;
}

ssize_t sfax8_genl_debug_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	return count;
}

static struct file_operations genl_debug_ops = {
	.owner = THIS_MODULE,
	.open  = sfax8_genl_debug_open,
	.read  = sfax8_genl_debug_read,
	.write  = sfax8_genl_debug_write,
	.llseek  = default_llseek,
};
#endif


// global variable for sfax8
struct sfax8_netlink *g_sfnl = NULL;




/*-------------------------------------------------------
 * FUCTION AREA
 * Differnt functions for differnt operation
 *------------------------------------------------------*/
static int sf_work_thread(void *msg)
{
	unsigned long i = 0, index= 0, last_i = 0, interval = 0, ret = 0;
	char arg[4][16] = {'\0'};
	char *data = (char *)msg;
	int len = strlen(data);

	if(!len){
		printk("receive data empty\n");
		return -1;
	}

	for(;i < len;i++){
		if(data[i] == ' '){
			memcpy(arg[index], data + last_i, i - last_i);
			last_i = i + 1;
			index++;
		}
	}
	memcpy(arg[index], data + last_i, len - last_i);

	ret = kstrtoul(arg[0], 0, &interval);
	if(ret == -EINVAL)
	  goto err_parsing;
	else if(ret == -ERANGE)
	  goto err_overflow;

	while (!kthread_should_stop()) {
		printk("In work thread, execute interval %ld ms\n", interval);
		msleep(interval);
	}
	return 0;

err_parsing:
	printk("parsing Error, please check the message!\n");
err_overflow:
	printk("convert overflow, please check the message!\n");
	return ret;
}

static int cmd_attr_create_kthread(struct genl_info *info)
{
	struct task_struct	*work_kthread;
	struct nlattr *na;
	struct sk_buff *rep_skb;
	size_t size;
	void *reply;
	char *msg;
	int ret;

	na = info->attrs[SF_CMD_ATTR_KTHREAD_RUN];
	if (!na)
	  return -EINVAL;

	/* message from user space */
	msg = nla_data(na);
	printk("Kernel : receive message, %s\n", msg);

	work_kthread = kthread_run(sf_work_thread, (void *)msg, "work_kthread");
	if (IS_ERR(work_kthread)){
		printk("creat work thread fail\n");
		return -EINVAL;
	}

	/* make work_kthread pointer value as a replay message */
	size = nla_total_size(sizeof(void *));

	rep_skb = genlmsg_new(size, GFP_KERNEL);
	if (!rep_skb)
	  return -ENOMEM;

	if (!info)
	  return -EINVAL;

	/* prepare mesage head */
	reply = genlmsg_put_reply(rep_skb, info, &(g_sfnl->generic_family.family), 0, SF_CMD_GENERIC);
	if (!reply)
	  goto err;

	ret = nla_put(rep_skb, SF_CMD_ATTR_KTHREAD_RUN, size, (void *)&work_kthread);
	if (ret < 0)
	  goto err;
	printk("Kernel : send replay message, 0x%p\n", work_kthread);

	genlmsg_end(rep_skb, reply);
	return genlmsg_reply(rep_skb, info);

err:
	nlmsg_free(rep_skb);
	return ret;
}

static int cmd_attr_stop_kthread(struct genl_info *info)
{
	struct task_struct	*work_kthread;
	struct nlattr *na;
	int *msg;

	na = info->attrs[SF_CMD_ATTR_KTHREAD_STOP];
	if (!na)
	  return -EINVAL;

	/* message from user space */
	msg = nla_data(na);
	printk("Kernel : receive message, 0x%2x\n", *msg);

	work_kthread = (struct task_struct *)*msg;
	kthread_stop(work_kthread);

	return 0;
}

static int cmd_attr_echo_message(struct genl_info *info)
{
	struct nlattr *na;
	struct sk_buff *rep_skb;
	size_t size;
	char *msg;
	void *reply;
	int ret;

	na = info->attrs[SF_CMD_ATTR_ECHO];
	if (!na)
	  return -EINVAL;

	/* message from user space */
	msg = nla_data(na);
	printk("Kernel : receive message, %s\n", msg);

	/* replay message */
	size = nla_total_size(strlen(msg)+1);

	rep_skb = genlmsg_new(size, GFP_KERNEL);
	if (!rep_skb)
	  return -ENOMEM;

	if (!info)
	  return -EINVAL;

	/* prepare mesage head */
	reply = genlmsg_put_reply(rep_skb, info, &(g_sfnl->generic_family.family), 0, SF_CMD_GENERIC);
	if (!reply)
	  goto err;

	ret = nla_put(rep_skb, SF_CMD_ATTR_ECHO, size, msg);
	if (ret < 0)
	  goto err;

	genlmsg_end(rep_skb, reply);
	return genlmsg_reply(rep_skb, info);

err:
	nlmsg_free(rep_skb);
	return ret;
}

static int cmd_attr_wifi(struct genl_info *info)
{
	//Todo: wait wifi provide API
	return 0;
}

static int cmd_attr_app(struct genl_info *info)
{
	//Todo: wait app provide API
	return 0;
}

/*
 * Func  general api for send message to user space
 * @cmd  generic netlink command to classify message
 * @attr generic netlink attribute type
 * @data attribute payload in message
 * @len  attribute payload len
 */
int genl_sendmsg(struct genl_family *family, int attr, void *data, int len){
	struct sk_buff *skb;
	int ret = 0;
	void *msg_head;

	skb = genlmsg_new(MAX_MSG_SIZE, GFP_KERNEL);
	if (!skb)
	  return -ENOMEM;

	msg_head = genlmsg_put(skb, 0, 0, family, 0, SF_CMD_GENERIC);
	if (!msg_head){
		printk("%s : add genlmsg header error!\n", __func__);
		ret = -ENOMEM;
		goto err;
	}

	ret = nla_put(skb, attr, len, data);
	if (ret < 0)
	  goto err;

	genlmsg_end(skb, msg_head);

	ret = genlmsg_multicast(family, skb, 0, 0, GFP_KERNEL);
	return 0;

err:
	nlmsg_free(skb);
	return ret;
}
EXPORT_SYMBOL(genl_sendmsg);




/*-------------------------------------------------------
 * BASIC SOCKET AREA
 * Provide BSD socket communication between kernel subsystems
 *------------------------------------------------------*/

/*
 * func: creat kernel server socket and lisen message from client
 * @recvbuf pointer to receive buffer, and that should never hold a userland pointer
 * @len receive buffer size
 *
 */
int sf_init_server_socket(void *recvbuf, int len)
{
	struct socket *server_sock, *client_sock;
	struct sockaddr_in s_addr;
	struct kvec vec;
	struct msghdr msg;
	unsigned short portnum = 0x8870;
	int ret = 0 ,val = 1;

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(portnum);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//s_addr.sin_port = 0

	server_sock = (struct socket *)kmalloc(sizeof(struct socket), GFP_KERNEL);
	client_sock = (struct socket *)kmalloc(sizeof(struct socket), GFP_KERNEL);

	/*create a socket*/
	ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, 0, &server_sock);
	if(ret){
		printk("server: socket_create error!\n");
	}
	//printk("server: socket_create ok!\n");

	/*set the socket can be reused*/
	ret = kernel_setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(val));
	if(ret){
		printk("server: kernel_setsockopt error!!!\n");
	}

	/*bind the socket*/
	ret = server_sock->ops->bind(server_sock,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in));
	if(ret < 0){
		printk("server: bind error\n");
		return ret;
	}
	//printk("server: bind ok!\n");

	/*listen*/
	ret = server_sock->ops->listen(server_sock, 10);
	if(ret < 0){
		printk("server: listen error\n");
		return ret;
	}
	printk("server: listen ok!\n");

	ret = kernel_accept(server_sock, &client_sock, 10);
	if(ret < 0){
		printk("server: accept error!\n");
		return ret;
	}
	//printk("server: accept ok, Connection Established\n");

	/*receive message from client*/
	memset(&vec, 0, sizeof(vec));
	memset(&msg, 0, sizeof(msg));
	vec.iov_base = recvbuf;
	vec.iov_len = len;
	ret = kernel_recvmsg(client_sock, &msg, &vec, 1, len, 0); /*receive message*/
	printk("receive message:%s ,size %d\n", (char *)recvbuf, ret);

	/*release socket*/
	sock_release(server_sock);
	sock_release(client_sock);

	return ret;
}
EXPORT_SYMBOL(sf_init_server_socket);

/*
 * func: creat kernel client socket and send message to server
 * @data input data
 * @len  data len
 *
 */
int sf_init_client_socket(void *data, int len)
{
	struct socket *sock;
	struct sockaddr_in s_addr;
	struct kvec vec;
	struct msghdr msg;
	unsigned short portnum = 0x8870;
	int ret = 0;

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(portnum);

	s_addr.sin_addr.s_addr = in_aton("127.0.0.1"); //server ip
	sock = (struct socket *)kmalloc(sizeof(struct socket), GFP_KERNEL);

	/*create a socket*/
	ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, 0, &sock);
	if(ret < 0){
		printk("client: socket create error!\n");
		return ret;
	}
	//printk("client: socket create ok!\n");

	/*connect server*/
	ret = sock->ops->connect(sock, (struct sockaddr *)&s_addr, sizeof(s_addr), 0);
	if(ret != 0){
		printk("client: connect error!\n");
		return ret;
	}
	//printk("client: connect ok!\n");


	/*send message*/
	vec.iov_base = data;
	vec.iov_len = len;

	memset(&msg, 0, sizeof(msg));

	ret = kernel_sendmsg(sock, &msg, &vec, 1, len);
	if(ret < 0){
		printk("client: kernel_sendmsg error!\n");
		return ret;
	}else if(ret != len){
		printk("client: ret!=len\n");
	}
	printk("client: message send ok!\n");

	sock_release(sock);
	return ret;
}
EXPORT_SYMBOL(sf_init_client_socket);

#ifdef CONFIG_SEND_ERR
#define MAX_KERR_MSG_LEN  256
int ker_err_send(char *type, int module, int code, char *text, char *path, int flag)
{
    struct sk_buff *skb = NULL;
    void *msg_head = NULL;
    int size;
    int rc;
    char msg[MAX_KERR_MSG_LEN];
    if(!type || !text || !path) {
        return -ENXIO;
    }
    if((strlen(type) + strlen(text) + strlen(path)) > (MAX_KERR_MSG_LEN - 20)){
        return -E2BIG;
    }
    sprintf(msg,"%s %s %s %d %d %02x", type, text, path, module, code, flag);

    skb = genlmsg_new(MAX_MSG_SIZE, GFP_KERNEL);
    if(!skb){
        return -ENOMEM;
    }

    msg_head = genlmsg_put(skb, 0, 0, &(g_sfnl->generic_family.family), 0, SF_CMD_GENERIC);

    if(!msg_head)
    {
        rc = -ENOMEM;
        goto err_out;
    }

    rc = nla_put_string(skb, SF_CMD_ATTR_ERR, msg);
    if(rc != 0){
        goto err_out;
    }
    genlmsg_end(skb, msg_head);

    rc = genlmsg_multicast(&(g_sfnl->generic_family.family), skb, 0, 0, GFP_KERNEL);
    if(rc != 0&& rc != -ESRCH)
    {
        printk("genlmsg_multicast to user failed, return :%d\n",rc);
        return rc;
    }
    printk("genl_multicast to user success\n");
    return rc;

err_out:
    if(skb)
        nlmsg_free(skb);
    return rc;
}

EXPORT_SYMBOL(ker_err_send);
#endif

/*-------------------------------------------------------
 * INIT AREA
 * Init family and ops
 *------------------------------------------------------*/
int sf_genl_recv(struct sk_buff *skb, struct genl_info *info){
	/* echo message to user space */
	if (info->attrs[SF_CMD_ATTR_ECHO])
	  return cmd_attr_echo_message(info);
	if (info->attrs[SF_CMD_ATTR_KTHREAD_RUN])
	  return cmd_attr_create_kthread(info);
	if (info->attrs[SF_CMD_ATTR_KTHREAD_STOP])
	  return cmd_attr_stop_kthread(info);
	if (info->attrs[SF_CMD_ATTR_WIFI])
	  return cmd_attr_wifi(info);
	if (info->attrs[SF_CMD_ATTR_APP])
	  return cmd_attr_app(info);
	else
	  return -EINVAL;
}

static struct nla_policy sf_genl_policy[SF_CMD_ATTR_MAX + 1] = {
	[SF_CMD_ATTR_ECHO]			= {.type = NLA_STRING, .len  = MSGLEN},
	[SF_CMD_ATTR_KTHREAD_RUN]	= {.type = NLA_STRING, .len  = MSGLEN},
	[SF_CMD_ATTR_WIFI]			= {.type = NLA_STRING, .len  = MSGLEN},
	[SF_CMD_ATTR_APP]			= {.type = NLA_STRING, .len  = MSGLEN},
    [SF_CMD_ATTR_ERR]           = {.type = NLA_STRING, .len  = MSGLEN},
};

static const struct genl_ops sf_genl_ops[] = {
	{
		.cmd		= SF_CMD_GENERIC,
		.doit		= sf_genl_recv,
		.policy		= sf_genl_policy,
		.flags		= (GENL_CMD_CAP_DO|GENL_CMD_CAP_HASPOL),
		.dumpit		= NULL,
	},
};

int sf_genl_family_init(sf_nlfamily *sf_family, char *family_name, char *group_name){
	int ret;

	//sf_family->family.id = GENL_ID_GENERATE;
	sf_family->family.hdrsize = 0;
	sf_family->family.version = SF_GENL_VERSION;
	sf_family->family.maxattr = SF_CMD_ATTR_MAX;
	memcpy(sf_family->family.name, family_name, min_t(size_t, strlen(family_name)+1, GENL_NAMSIZ));
	sf_family->family.ops = sf_genl_ops;
	sf_family->family.n_ops = ARRAY_SIZE(sf_genl_ops);
	printk("register new generic netlink family name %s!\n",sf_family->family.name);

	if (group_name){
		/* muticast message must register a group */
		memcpy(sf_family->group[0].name, group_name, min_t(size_t, strlen(group_name), GENL_NAMSIZ));
		sf_family->family.mcgrps = sf_family->group;
		sf_family->family.n_mcgrps = ARRAY_SIZE(sf_family->group);
	}
	ret= genl_register_family(&(sf_family->family));

	if (ret){
		printk("register generic netlink family error!\n");
		return ret;
	}

	return 0;
}
EXPORT_SYMBOL(sf_genl_family_init);

int sf_genl_family_exit(struct genl_family *family){
	int ret;

	ret = genl_unregister_family(family);
	if (ret){
		printk("unregister generic netlink family error!\n");
	}

	return 0;
}
EXPORT_SYMBOL(sf_genl_family_exit);


static int init_sf_genl(void)
{
	printk("sf generic netlink module init\n");

	g_sfnl = kmalloc(sizeof(struct sfax8_netlink), GFP_KERNEL);
#ifdef CONFIG_DEBUG_FS
	g_sfnl->genl_debug = debugfs_create_file("genl_debug", 0777, NULL, NULL, &genl_debug_ops);
#endif

	sf_genl_family_init(&(g_sfnl->generic_family), "COMMON_NL", "common_nl");

	printk("sf generic netlink module init success,get family id %d!\n",g_sfnl->generic_family.family.id);
	return 0;
}

static void exit_sf_genl(void)
{
#ifdef CONFIG_DEBUG_FS
	debugfs_remove(g_sfnl->genl_debug);
#endif

	sf_genl_family_exit(&(g_sfnl->generic_family.family));
	kfree(g_sfnl);

	printk("sf generic netlink module exit success\n");
}

module_init(init_sf_genl);
module_exit(exit_sf_genl);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qin xia <qin.xia@siflower.com.cn>");
MODULE_DESCRIPTION("GeNetlink driver for sfax8");
MODULE_VERSION(SF_GENL_VERSION);
