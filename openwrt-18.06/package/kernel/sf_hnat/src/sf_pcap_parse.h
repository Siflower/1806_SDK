#ifndef _SF_CAP_PARSE_H_
#define _SF_CAP_PARSE_H_
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/if_pppox.h>
#include <linux/if_vlan.h>
#include <linux/ppp_defs.h>

// structpcap_file_hdr {
// u32magic;/*0xa1b2c3d4*/
// u16version_major;/*magjorVersion2*/
// u16version_minor;/*magjorVersion4*/
// int thiszone;/*gmttolocalcorrection*/
// u32sigfigs;/*accuracyoftimestamps*/
// u32snaplen;/*maxlengthsavedportionofeachpkt*/
// u32linktype;/*datalinktype(LINKTYPE_*)*/
// };
// structpcap_pkt_hdr {
// structtime_valts;/*timestamp*/
// u32caplen;/*lengthofportionpresent*/
// u32len;/*lengththispacket(offwire)*/
// };
//
//
struct pppoe_ses_hdr{
	struct pppoe_hdr hdr;
	__be16 proto;
};

struct sf_cap_file_data {
	// structpcap_file_hdr pcap_fhdr;
	// structpcap_pkt_hdr pcap_phdr;
	struct ethhdr teth_hdr;
	u8 is_vlan;
	struct vlan_hdr tvlan_hdr;
	u8 is_pppoe;
	struct pppoe_ses_hdr tpppoe_hdr;
	struct iphdr tip_hdr;
	u8 is_udp;
	struct tcphdr ttcp_hdr;
	struct udphdr tudp_hdr;
	char * pmalloc_buf;
	u32    buf_read_len;
	// pointer to data after L3 header
	char * pdata_buf;
	u8 cap_offset;
};

int sf_parse_cap_file(char * file_name, u8 is_jumbo, struct sf_cap_file_data * pcap_data);
#endif // _SF_CAP_PARSE_H_
