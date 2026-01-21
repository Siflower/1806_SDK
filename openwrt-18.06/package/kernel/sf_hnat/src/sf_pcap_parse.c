#include "sf_pcap_parse.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/in.h>

#define BUFSIZE 2048
#define JUMBO_BUFSIZE 10240

#define STRSIZE  32
#define PATH_LEN  128
#define PATH_PREFIX "/etc/pkt/"
// #definePCAP_FILE_HDR_SIZE24
// #definePCAP_PKT_HDR_SIZE16
#define PCAP_OFFSET    0x28 //40
#define FIVE_VW_OFFSET 0x3c //

int sf_parse_cap_file(char * file_name, u8 is_jumbo, struct sf_cap_file_data * pcap_data){
	// structpcap_file_hdr*file_hdr;
	// structpcap_pkt_hdr*ptk_hdr;
	struct ethhdr * peth_hdr;
	struct vlan_hdr * pvlan_hdr;
	struct pppoe_ses_hdr *p_pppoe_hdr;
	struct iphdr* pip_hdr;
	struct tcphdr* ptcp_hdr;
	struct udphdr* pudp_hdr;
	struct file *fp_read;
	// mm_segment_t old_fs;
	int ret = 0;
	u32 buf_size = 0, read_len = 0;
	u16 proto = 0;
	u8  l4_proto = 0;
	char pkt_path[PATH_LEN] = {0};
	char *pkt_buf = NULL;
	char *seek_buf = NULL;

	if(is_jumbo){
		buf_size = BUFSIZE;
	}else{
		buf_size = JUMBO_BUFSIZE;
	}

	sprintf(pkt_path,"%s%s", PATH_PREFIX, file_name);
	fp_read = filp_open(pkt_path, O_RDONLY , 0);
	if (IS_ERR(fp_read)) {
		ret = -1;
		printk("%s open failed,err = %ld\n",pkt_path, PTR_ERR(fp_read));
		goto error_open;
	}

	pkt_buf = vmalloc(buf_size);
	memset(pkt_buf,0,sizeof(buf_size));
	if(!pkt_buf){
		ret = -2;
		printk("malloc buf fail\n");
		goto error_malloc;
	}
	// old_fs = get_fs();
	// set_fs(KERNEL_DS);
	read_len = kernel_read(fp_read, pkt_buf, buf_size, &fp_read->f_pos);
	if (read_len <= 0) {
		ret = -3;
		printk("read pkt file fail\n");
		goto error_read;
	}

	printk("read pkt file length %d\n", read_len);

	// file_hdr	=&pcap_data->pcap_fhdr;
	// ptk_hdr	=&pcap_data->pcap_phdr;
	peth_hdr	= &(pcap_data->teth_hdr);
	pvlan_hdr	= &(pcap_data->tvlan_hdr);
	p_pppoe_hdr = &(pcap_data->tpppoe_hdr);
	pip_hdr		= &(pcap_data->tip_hdr);
	ptcp_hdr	= &(pcap_data->ttcp_hdr);
	pudp_hdr	= &(pcap_data->tudp_hdr);

	// memcpy(file_hdr, pkt_buf, PCAP_FILE_HDR_SIZE);
	// seek_buf += PCAP_FILE_HDR_SIZE;

	// memcpy(pkt_hdr, seek_buf, PCAP_PKT_HDR_SIZE);
	// seek_buf += PCAP_PKT_HDR_SIZE;
	//
	seek_buf = pkt_buf;
	if( strstr(file_name,"5vw") == NULL ){
		seek_buf += PCAP_OFFSET;
		pcap_data->cap_offset = PCAP_OFFSET;
	}
	else{
		seek_buf += FIVE_VW_OFFSET;
		pcap_data->cap_offset = FIVE_VW_OFFSET;
	}
	printk("cap offset %d\n", pcap_data->cap_offset);

	memcpy(peth_hdr, seek_buf, ETH_HLEN);
	seek_buf += ETH_HLEN;
	proto = peth_hdr->h_proto;

	printk("mac %pM", peth_hdr->h_dest);
	printk("mac %pM", peth_hdr->h_source);
	// printk("proto 0x%x", proto);
	if(proto == htons(ETH_P_8021Q)){
		printk("proto vlan  0x%x", proto);
		memcpy(pvlan_hdr, seek_buf, VLAN_HLEN);
		seek_buf += VLAN_HLEN;
		proto =  pvlan_hdr->h_vlan_encapsulated_proto;
		pcap_data->is_vlan = 1;
	}


	if(proto == htons(ETH_P_PPP_SES)){
		printk("proto ppp 0x%x", proto);
		memcpy(p_pppoe_hdr, seek_buf,PPPOE_SES_HLEN);
		if(p_pppoe_hdr->proto == htons(PPP_IP)){
			proto = htons(ETH_P_IP);
			pcap_data->is_pppoe = 1;
		}
		else {
			ret = -4;
			printk("not pppoe session data ip packet error\n");
			goto error_read;
		}
		seek_buf += PPPOE_SES_HLEN;
	}

	if(proto == htons(ETH_P_IP)){
		printk("proto ip 0x%x", proto);
		memcpy(pip_hdr, seek_buf,sizeof(struct iphdr));
		seek_buf += sizeof(struct iphdr);
		l4_proto = pip_hdr->protocol;
	}else{
		printk("not ip packet\n");
		goto error_read;
	}


	printk("proto l3 0x%x %x\n", l4_proto, IPPROTO_TCP);
	if(l4_proto == IPPROTO_UDP){
		memcpy(pudp_hdr, seek_buf,sizeof(struct udphdr));
		seek_buf += sizeof(struct tcphdr);
		pcap_data->is_udp = 1;
	}else if (l4_proto == IPPROTO_TCP){
		memcpy(ptcp_hdr, seek_buf,sizeof(struct tcphdr));
		seek_buf += sizeof(struct udphdr);
	}else{
		ret = -5;
		printk("not tcp or udp packet\n");
		goto error_read;
	}

	pcap_data->pmalloc_buf = pkt_buf;
	pcap_data->buf_read_len = read_len;
	pcap_data->pdata_buf = seek_buf;

	// set_fs(old_fs);
	filp_close(fp_read, NULL);
	return ret;

error_read:
	// set_fs(old_fs);
	vfree(pkt_buf);
error_malloc:
	filp_close(fp_read, NULL);
error_open:

	return ret;
}
