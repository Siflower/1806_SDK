#ifndef SF_API_H
#define SF_API_H
#include "sf_common.h"

int set_global_snoop_special_mac(int idx ,unsigned long long *mac,unsigned long long *mask,int enable);

void set_mac_loopback(int port_list);
void set_mac_no_loopback(int port);

int set_tmu_scheduler_config(tmu_schd_params_t *psch);

int tmu_shaper_config_parameters_pps(uint rate_units, uint rate, uint clk_frequency, uint *intwt, uint *frcwt, uint *clkd);
int tmu_shaper_config_parameters(uint rate_units, uint rate, uint clk_frequency, uint *intwt, uint *frcwt, uint *clkd);
int set_tmu_shaper_config(tmu_shpr_params_t *pshp,  uint dynamic);

void write_fasw_context_memory(uint data, uint que_no, uint phy_no, uint reg_no);
int tmu_add_queue2scheduler(tmu_classq_params_t *que);
int set_tmu_classq_add(tmu_classq_params_t *que);

void set_tmu_tdq_default_enable(void);
int set_tmu_tdq_enable(int dev_no, int enable);

int set_tmu_various_cfg(tmu_schd_params_t* tsch_in,tmu_shpr_params_t* tshp_in, tmu_classq_params_t* tque_in,int port_list);

int get_gpi_addr_base( int dev_no);
int npu_gpi_ingress_config(tmu_shpr_params_t * pshp, int dev_no, int effect);

int set_cutthru(int port_list, int cutthru_type);

int set_dos_value(int item, int value);

int set_tc_sel(int port, int map);
int set_tc_sel_one_to_one(int port, int type);
int set_pid2tc_map(int map);
int set_pcp2tc_map(int port,int map);

int set_global_mgmt_port(int port, int mgmt_type);
int set_port_enable(int port_num,int enable);
int set_port_vlan_attr(int port_num, int attr);

int set_global_snoop_ethertype(int idx, unsigned long long* snoop_etype, int enable);

int set_port_untag(int port_num);
int set_port_vid_prefix(int port_num);

int set_bmu_watermark(int bmu_low_watermark, int bmu_high_watermark);
int init_gpi_fw_control(int dev_no, int phy_low_watermark, int phy_high_watermark);

int set_ptp_enable(int dev_no);
int set_ptp_mac_filter(int *mac);

int gmac_set_mac_addr1(unsigned long long *mac);
void set_SA_DA_model(int value);
#endif /* ifndef SF_API_H */
