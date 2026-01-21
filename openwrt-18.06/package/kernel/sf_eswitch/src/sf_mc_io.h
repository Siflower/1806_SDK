#define SF_MAX_VIF 10
#define IF_NAMESIZE 16
#define SF_DEFAULT_PORT_NO 4

enum sf_ioctl_type{
	SF_INIT_MC_ROUTE,
	SF_ADD_MC_ROUTE,
	SF_DEL_MC_ROUTE,
	SF_CLEAR_MC_ROUTE
};

struct sf_vif_desc{
    char	Name[IF_NAMESIZE];
	int		s_addr;
};

struct sf_mc_route {
	uint32_t	mcast_addr;
	struct sf_vif_desc	ovifs[SF_MAX_VIF];
};

//map devname to phy port
struct sf_port_devname{
    char	Name[IF_NAMESIZE];
	u8	port;
};

//0-br-lan, 1-br-lan2, 2-br-lan3, 3-eth0.2
struct sf_port_devname default_p2d_match[SF_DEFAULT_PORT_NO] = {
	{ "br-lan", 0},
	{ "br-lan2", 1},
	{ "br-lan3", 2},
	{ "eth0.2", 3},
};


