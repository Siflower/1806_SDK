#ifndef _SF_AN8855_OPS_H_
#define _SF_AN8855_OPS_H_

int an8855_phy_write(unsigned int phyNo, unsigned int phyAddr, unsigned int pRegData);
int an8855_phy_read(unsigned int phyNo, unsigned int phyAddr, unsigned int *pRegData);

#ifdef CONFIG_SFAX8_ESWITCH_REDIRECT
void air_an8855_redirect_dhcp(void);
void air_an8855_redirect_dhcp_disabled(void);
void air_an8855_redirect_dns(void);
void air_an8855_redirect_dns_disabled(void);
#endif

#endif /* _SF_AN8855_OPS_H_ */