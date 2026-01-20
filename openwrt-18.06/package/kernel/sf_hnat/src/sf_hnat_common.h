#ifndef _SF_HNAT_COMMON_H_
#define _SF_HNAT_COMMON_H_
//hnat_mode
#define BASIC_MODE                  (0 << 25)
#define SYMMETRIC_MODE              (1 << 25)
#define FULL_CONE_MODE              (2 << 25)
#define RESTRICT_CONE_MODE          (3 << 25)
#define PORT_RESTRICT_CONE_MODE     (1 << 27)

struct sf_hashkey {
// not modify
	unsigned int sip;//private ip
	unsigned int dip;//public ip
	unsigned short sport;//private port
	unsigned short dport;//public port
	unsigned int router_pub_ip;
	unsigned char src_mac[6];
	unsigned short src_vlan;
	unsigned short dest_vlan;
	unsigned char dest_mac[6];
	unsigned char router_src_mac[6]; //wan mac
	unsigned char router_dest_mac[6];// lan mac
	unsigned short router_port;
	unsigned char proto; // tcp or udp 1
	unsigned short ppp_sid;//till here 49 bytes
// not modify
	unsigned char valid;
	unsigned char cur_pppoe_en;
	unsigned char src_vlan_index;
	unsigned char dest_vlan_index;//we should save the info for dip entry config and debug
	unsigned char rt_pub_net_index;
	unsigned char dest_dip_index;
	unsigned char src_dip_index;
	//TODO hash value
	unsigned char ppphd_index;
	unsigned char dnat_to_host;
	unsigned char is_dip_rt_ip_same_subnet;
	struct flow_offload *pkey_flow;
	unsigned char lan_subnet_index;
	unsigned char wan_subnet_index;
	// unsigned char keep_flag;
}__packed;//packed struct for memcmp



int sf_hnat_init(struct platform_device *pdev, void __iomem *reg_base, struct net_device *pdriver_ndev);
int sf_hnat_deinit(struct platform_device *pdev);
unsigned char sf_hnat_search(struct platform_device *pdev, struct sk_buff *skb);
int sf_hnat_ndo_flow_offload(struct platform_device *pdev,
							 enum flow_offload_type type,
							 struct flow_offload *flow,
							 struct flow_offload_hw_path *src,
							 struct flow_offload_hw_path *dest);
int sf_hnat_xmit(struct platform_device *pdev, struct sk_buff *skb);

int sf_hnat_print(enum flow_offload_type type,struct flow_offload *flow, struct flow_offload_hw_path *src, struct flow_offload_hw_path *dest);

int sf_dump_napt_table_content_by_index(struct sf_hnat_priv *phnat_priv, int index);

char sf_hnat_add_entry(struct sf_hnat_priv* phnat_priv, struct sf_hashkey *sf_key, unsigned int * napt_index);
int sf_hnat_delete_entry(struct sf_hnat_priv* phnat_priv, struct sf_hashkey *sf_key);
int sf_hnat_delete_entry_by_flow(struct sf_hnat_priv* phnat_priv, struct flow_offload *pkey_flow);
char sf_hnat_delete_entry_by_index_unlock(struct sf_hnat_priv* phnat_priv, unsigned int napt_index);
int sf_hnat_delete_entry_by_index(struct sf_hnat_priv* phnat_priv, unsigned int napt_index);
/*dump some information for debug*/
int sf_hnat_change_nat_mode(struct sf_hnat_priv* phnat_priv, unsigned int hnat_mode);

int sf_dump_dip_table_content_by_index(struct sf_hnat_priv *phnat_priv, int index);

void sf_hnat_dump_napt_key(struct sf_hashkey * sf_key);
void sf_hnat_dump_napt_key_by_index(unsigned short napt_index);
void sf_hnat_dump_napt_hash_by_index(unsigned short napt_index);

int sf_hnat_table_wr(struct sf_hnat_priv * phnat_priv, int tab_no,int depth,int *data);
int sf_hnat_speed_read(struct sf_hnat_priv *phnat_priv);

int sf_hnat_table_rd(struct sf_hnat_priv* phnat_priv, int tab_no,int depth,int *data);
void sf_hnat_read_count_registers(struct sf_hnat_priv *phnat_priv);
int int_to_array(void* value, unsigned char* v_array, char len);
int sf_hnat_dump_ppphd(struct sf_hnat_priv *phnat_priv);
void sf_hnat_napt_vld_clean(void);
void sf_hnat_dump_flow(struct sf_hnat_priv * phnat_priv);
int sf_hnat_update_lan_subnet(struct sf_hnat_priv *phnat_priv, unsigned char index);
int sf_hnat_del_lan_subnet(struct sf_hnat_priv *phnat_priv, unsigned int index);
int sf_hnat_del_wan_subnet(struct sf_hnat_priv *phnat_priv, unsigned int index);
int sf_hnat_dump_dip_hash_by_index(unsigned short index);
int sf_hnat_dump_dip_by_index(struct sf_hnat_priv * phnat_priv, unsigned short index);
#endif
