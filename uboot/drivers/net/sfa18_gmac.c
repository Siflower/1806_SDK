#include <common.h>
#include <malloc.h>
#include <net.h>
#include <config.h>

#include <errno.h>
#include <phy.h>
#include <miiphy.h>

#include <asm/io.h>
#include <netdev.h>
#include <dm/uclass.h>
#include <spi_flash.h>
#include "sfa18_gmac.h"
#include "sf_intel_api.h"
#include "sf_rtk_api.h"
#include "sf_yt_api.h"
#include "sf_nf_api.h"

int sf_gmac_vlan_all_enable = 0;
int sf_gmac_first_recv_vlan = 0;

struct sgmac_priv *g_priv = NULL;

static int sgmac_invalidate_cache(struct sgmac_priv *priv, uint is_tx, uint is_tbl, uint buf_index){
#ifndef NO_CACHE
	if(is_tx){
		if(is_tbl)
			invalidate_dcache_range((uint)(priv->dma_tx + buf_index), (uint)((uint)(priv->dma_tx + buf_index) + sizeof(struct sgmac_dma_desc ) ));
		else
			invalidate_dcache_range((uint)priv->tx_buf + priv->dma_buf_sz* buf_index, (uint)(priv->tx_buf + priv->dma_buf_sz* (buf_index + 1) ));
	}
	else{
		if(is_tbl)
			invalidate_dcache_range((uint)(priv->dma_rx+ buf_index), (uint)((uint)(priv->dma_rx+ buf_index) + sizeof(struct sgmac_dma_desc)));
		else
			invalidate_dcache_range((uint)priv->rx_buf+ priv->dma_buf_sz*buf_index, (uint)(priv->rx_buf+ priv->dma_buf_sz* (buf_index + 1) ));
	}
#endif
	return 0;
}

static int sgmac_flush_cache(struct sgmac_priv *priv, uint is_tx, uint is_tbl, uint buf_index){
#ifndef NO_CACHE
	if(is_tx){
		if(is_tbl)
			flush_dcache_range((uint)(priv->dma_tx + buf_index), (uint)((uint)(priv->dma_tx+ buf_index) + sizeof(struct sgmac_dma_desc) ));
		else
			flush_dcache_range((uint)priv->tx_buf+ priv->dma_buf_sz* buf_index, (uint)(priv->tx_buf+ priv->dma_buf_sz* (buf_index + 1) ));
	}
	else{
		if(is_tbl)
			flush_dcache_range((uint)(priv->dma_rx+ buf_index), (uint)((uint)(priv->dma_rx+ buf_index) + sizeof(struct sgmac_dma_desc ) ));
		else
			flush_dcache_range((uint)priv->rx_buf+ priv->dma_buf_sz* buf_index, (uint)(priv->rx_buf+ priv->dma_buf_sz* (buf_index + 1) ));
	}
#endif
	return 0;
}

static inline void sgmac_mac_enable(struct sgmac_priv *priv)
{
	u32 value = readl(priv->base + GMAC_CONTROL);
	value |= GMAC_CONTROL_RE | GMAC_CONTROL_TE;
	writel(value, priv->base + GMAC_CONTROL);

	value = readl(priv->base + GMAC_DMA_OPERATION);
	value |= DMA_OPERATION_ST | DMA_OPERATION_SR;
	writel(value, priv->base + GMAC_DMA_OPERATION);
}

static inline void sgmac_mac_disable(struct sgmac_priv *priv)
{
	u32 value = readl(priv->base + GMAC_DMA_OPERATION);
	value &= ~(DMA_OPERATION_ST | DMA_OPERATION_SR);
	writel(value, priv->base + GMAC_DMA_OPERATION);

	value = readl(priv->base + GMAC_CONTROL);
	value &= ~(GMAC_CONTROL_TE | GMAC_CONTROL_RE);
	writel(value, priv->base + GMAC_CONTROL);
}

static int sf_gmac_write_hwaddr(struct eth_device *dev)
{
	u32 data;
	int num = 0;
	unsigned char *addr = dev->enetaddr;
	struct sgmac_priv* priv = (struct sgmac_priv *)dev->priv;
	if (addr) {
		data = (addr[5] << 8) | addr[4] | (num ? GMAC_ADDR_AE : 0);
		writel(data, priv->base + GMAC_ADDR_HIGH(num));
		data = (addr[3] << 24) | (addr[2] << 16) | (addr[1] << 8) |
		       addr[0];
		writel(data, priv->base + GMAC_ADDR_LOW(num));
	} else {
		writel(0, priv->base + GMAC_ADDR_HIGH(num));
		writel(0, priv->base + GMAC_ADDR_LOW(num));
	}
	return 0;
}

static void sgmac_set_mdc_clk_div(struct sgmac_priv *priv)
{
	int value = readl(priv->base + GMAC_GMII_ADDR);
	value &= ~GMAC_GMII_ADDR_CR_MASK;
	value |= GMAC_GMII_ADDR_CR;
	writel(value, priv->base + GMAC_GMII_ADDR);
}

static int sgmac_hw_init(struct eth_device *dev, struct sgmac_priv *priv)
{
	int i = 0;
	u32 value, ctrl;
	/* Save the ctrl register value */
	ctrl = readl(priv->base + GMAC_CONTROL) & GMAC_CONTROL_SPD_MASK;

	/* SW reset */
	writel(DMA_BUS_MODE_SFT_RESET, priv->base + GMAC_DMA_BUS_MODE);
	while (readl(priv->base + GMAC_DMA_BUS_MODE) & DMA_BUS_MODE_SFT_RESET) {
		mdelay(10);
		i++;
		if (i > 10) {
			printf("DMA reset timeout\n");
			return -ETIMEDOUT;
		}
	}

	sf_gmac_write_hwaddr(dev);
	value = (0x10 << DMA_BUS_MODE_PBL_SHIFT) |
		(0x10 << DMA_BUS_MODE_RPBL_SHIFT) | DMA_BUS_MODE_FB |
		DMA_BUS_MODE_ATDS | DMA_BUS_MODE_AAL;
	writel(value, priv->base + GMAC_DMA_BUS_MODE);

	writel(0, priv->base + GMAC_DMA_INTR_ENA);

	/* Mask power mgt interrupt */
	writel(GMAC_INT_MASK_PMTIM, priv->base + GMAC_INT_STAT);

	/* GMAC requires AXI bus init. This is a 'magic number' for now */
	writel(0x0077000E, priv->base + GMAC_DMA_AXI_BUS);
	// austin:set link, speed and duplex here
	// no link
	priv->link = 0;
	priv->duplex = DUPLEX_FULL;
#ifdef CONFIG_SFAX8_RGMII_GMAC
	priv->speed = SPEED_1000;
	ctrl = GMAC_CONTROL_CST | GMAC_CONTROL_JE | GMAC_CONTROL_ACS |
	       GMAC_SPEED_1000M | GMAC_CONTROL_DM;
#else
	priv->speed = SPEED_100;
	ctrl = GMAC_CONTROL_CST | GMAC_CONTROL_JE | GMAC_CONTROL_ACS |
	       GMAC_SPEED_100M | GMAC_CONTROL_DM;
#endif

	ctrl |= GMAC_CONTROL_IPC;
	writel(ctrl, priv->base + GMAC_CONTROL);
	/* Set the HW DMA mode and the COE */
	writel(DMA_OPERATION_TSF | DMA_OPERATION_RFD | DMA_OPERATION_RFA |
					DMA_OPERATION_RTC_256 |
					DMA_OPERATION_OSF,
			priv->base + GMAC_DMA_OPERATION);

	return 0;
}

/* GMAC Descriptor Access Helpers */
static inline void desc_set_buf_len(struct sgmac_dma_desc *p, u32 buf_sz)
{
	if (buf_sz > MAX_DESC_BUF_SZ)
		p->buf_size = cpu_to_le32(
				MAX_DESC_BUF_SZ |
				(buf_sz - MAX_DESC_BUF_SZ)
						<< DESC_BUFFER2_SZ_OFFSET);
	else
		p->buf_size = cpu_to_le32(buf_sz);
}

static inline void
desc_init_rx_desc(struct sgmac_dma_desc *p, int ring_size, int buf_sz)
{
	struct sgmac_dma_desc *end = p + ring_size - 1;

	memset(p, 0, sizeof(*p) * ring_size);

	for (; p <= end; p++){
		desc_set_buf_len(p, buf_sz);
	}

	end->buf_size |= cpu_to_le32(RXDESC1_END_RING);
}

static inline void desc_init_tx_desc(struct sgmac_dma_desc *p, u32 ring_size)
{
	memset(p, 0, sizeof(*p) * ring_size);
	p[ring_size - 1].flags = cpu_to_le32(TXDESC_END_RING);
}

/**
 * init_sgmac_dma_desc_rings - init the RX/TX descriptor rings
 * @dev: net device structure
 * Description:  this function initializes the DMA RX/TX descriptors
 * and allocates the socket buffers.
 */
static inline void
desc_set_buf_addr(struct sgmac_dma_desc *p, u32 paddr, int len)
{
	p->buf1_addr = cpu_to_le32(paddr);
	if (len > MAX_DESC_BUF_SZ)
		p->buf2_addr = cpu_to_le32(paddr + MAX_DESC_BUF_SZ);
}

static inline void desc_set_rx_owner(struct sgmac_dma_desc *p)
{
	/* Clear all fields and set the owner */
	p->flags = cpu_to_le32(DESC_OWN);
}

static void sgmac_rx_fill(struct sgmac_priv *priv)
{
	struct sgmac_dma_desc *p;
	int bufsz = priv->dma_buf_sz;
	unsigned char i = 0;
	char * paddr_start = (char * )virt_to_phys(priv->rx_buf);
	char * paddr = NULL;
	for(; i < DMA_RX_RING_SZ; i++){
		paddr = paddr_start  + bufsz * i;
		p = priv->dma_rx + i;
		desc_set_buf_addr(p, (u32) paddr, priv->dma_buf_sz);
		desc_set_rx_owner(p);
	}
}

static inline void desc_set_tx_owner(struct sgmac_dma_desc *p)
{
	u32 tmpflags = le32_to_cpu(p->flags);
	tmpflags &= TXDESC_END_RING;
	tmpflags |=  TXDESC_FIRST_SEG| TXDESC_LAST_SEG |DESC_OWN;
	p->flags = cpu_to_le32(tmpflags);
}

static void sgmac_tx_fill(struct sgmac_priv *priv) {
	struct sgmac_dma_desc *p;
	char *paddr = (char *)virt_to_phys(priv->tx_buf);
	unsigned char i = 0;
	for(;i < DMA_TX_RING_SZ; i++){
		p = priv->dma_tx + i;
		desc_set_buf_addr(p, (u32)paddr, priv->dma_buf_sz);
		paddr += priv->dma_buf_sz;
	}
}

static int sgmac_dma_desc_rings_init(struct sgmac_priv *priv) {
	unsigned int bfsize;

	int i = 0;
	/* Set the Buffer size according to the MTU;
	 * The total buffer size including any IP offset must be a multiple
	 * of 8 bytes.
	 */
	bfsize = ALIGN(MAX_FRAME_SIZE + ETH_HLEN + ETH_FCS_LEN + 2, BUF_ALIGN);

	priv->dma_rx_alloc = malloc( DMA_RX_RING_SZ * sizeof(struct sgmac_dma_desc) + BUF_ALIGN);
	priv->dma_rx = (struct sgmac_dma_desc *)(priv->dma_rx_alloc+ BUF_ALIGN - ((uint)priv->dma_rx_alloc % BUF_ALIGN));
	priv->dma_rx_phy = (u32 )virt_to_phys(priv->dma_rx);

	priv->dma_tx_alloc = malloc( DMA_TX_RING_SZ * sizeof(struct sgmac_dma_desc) + BUF_ALIGN);
	priv->dma_tx = (struct sgmac_dma_desc *)(priv->dma_tx_alloc+ BUF_ALIGN - ((uint)priv->dma_tx_alloc % BUF_ALIGN));
	priv->dma_tx_phy = (u32 )virt_to_phys(priv->dma_tx);

	priv->rx_index= 0;
	priv->dma_buf_sz = bfsize;
	desc_init_rx_desc(priv->dma_rx, DMA_RX_RING_SZ, priv->dma_buf_sz);
	priv->rx_buf_alloc = malloc(bfsize* DMA_RX_RING_SZ + BUF_ALIGN);
	priv->rx_buf= (priv->rx_buf_alloc + BUF_ALIGN - ((uint)priv->rx_buf_alloc % BUF_ALIGN));

	sgmac_rx_fill(priv);

	desc_init_tx_desc(priv->dma_tx, DMA_TX_RING_SZ);
	priv->tx_buf_alloc = malloc(bfsize* DMA_TX_RING_SZ + BUF_ALIGN);
	priv->tx_buf= (priv->tx_buf_alloc + BUF_ALIGN - ((uint)priv->tx_buf_alloc % BUF_ALIGN));
	sgmac_tx_fill(priv);

	writel(priv->dma_tx_phy, priv->base + GMAC_DMA_TX_BASE_ADDR);
	writel(priv->dma_rx_phy, priv->base + GMAC_DMA_RX_BASE_ADDR);

	for (; i < DMA_TX_RING_SZ; i++) {
		sgmac_invalidate_cache(priv, 1, 1, i);
		sgmac_invalidate_cache(priv, 1, 0, i);
	}
	for (i = 0; i < DMA_RX_RING_SZ; i++) {
		sgmac_invalidate_cache(priv, 0, 1, i);
		sgmac_invalidate_cache(priv, 0, 0, i);
	}
	return 0;

}

static void sf_gmac_halt(struct eth_device *dev)
{
	sgmac_mac_disable((struct sgmac_priv *)dev->priv);
	return ;
}

static inline void
desc_set_buf_addr_and_size(struct sgmac_dma_desc *p, u32 paddr, int len)
{
	desc_set_buf_len(p, len);
	desc_set_buf_addr(p, (u32)paddr, len);
}

static inline int desc_get_owner(struct sgmac_dma_desc *p)
{
	return le32_to_cpu(p->flags) & DESC_OWN;
}

unsigned char vlan_tag[4] = {0x81,0x00,0x00,0x01};
int sf_gmac_send(struct eth_device *dev, void *packet, int length)
{
	struct sgmac_priv *priv = dev->priv;
	struct sgmac_dma_desc *desc;
	char *l_pkt = malloc(length + 4);

	desc = priv->dma_tx + priv->tx_index;
	sgmac_invalidate_cache(priv, 1, 1, priv->tx_index);
	if (desc_get_owner(desc)){
		error(" desc owned by dma");
		return -1;
	}

	if(sf_gmac_vlan_all_enable){
		memcpy( (void *)l_pkt, packet,12);
		memcpy( (void *)(l_pkt+12), (void *)vlan_tag ,4);
		memcpy( (void *)(l_pkt+16), (packet + 12) ,(length - 12) );
		length += 4;
  		memcpy(priv->tx_buf + priv->dma_buf_sz * priv->tx_index, (void *)l_pkt, length);
	}else
  		memcpy(priv->tx_buf + priv->dma_buf_sz * priv->tx_index, packet, length);

	// printf(" tx idx %d\n", priv->tx_index);
	//memcpy(priv->tx_buf + priv->dma_buf_sz * priv->tx_index, packet, length);
	sgmac_flush_cache(priv, 1, 0, priv->tx_index);

	desc_set_buf_addr_and_size(desc, virt_to_phys(priv->tx_buf + priv->dma_buf_sz * priv->tx_index), length);

	desc_set_tx_owner(desc);

	sgmac_flush_cache(priv, 1, 1, priv->tx_index);
	writel(1, (void *)priv->base + GMAC_DMA_TX_POLL);

	priv->tx_index= dma_ring_incr(priv->tx_index, DMA_TX_RING_SZ);

	free(l_pkt);

	return 0;
}

static inline int desc_get_rx_frame_len(struct sgmac_dma_desc *p)
{
	u32 data, len;
	data = le32_to_cpu(p->flags);
	len = (data & RXDESC_FRAME_LEN_MASK) >> RXDESC_FRAME_LEN_OFFSET;
	//	if (data & RXDESC_FRAME_TYPE)
	//		len -= ETH_FCS_LEN;

	return len;
}

int sf_gmac_rx_pop_vlan(unsigned char * buf,int len){
	memcpy((void*)(buf + 12),(void *)(buf + 16), (len - 16));
	return 0;
}

int sf_gmac_recv(struct eth_device *dev)
{
	struct sgmac_dma_desc *p;
	int frame_len;
	unsigned char * buf;
	struct sgmac_priv *priv = dev->priv;

	do {
		sgmac_invalidate_cache(priv, 0, 1, priv->rx_index);
		p = priv->dma_rx +  priv->rx_index;
		if (desc_get_owner(p)) {
			return -1;
		}

		frame_len = desc_get_rx_frame_len(p);

		buf = net_rx_packets[priv->rx_index % PKTBUFSRX];

		char * rx_pkt = (char *)phys_to_virt(p->buf1_addr);
		sgmac_invalidate_cache(priv, 0, 0, priv->rx_index);
		memcpy(buf, rx_pkt, frame_len);
		// printf("recv pkt to buf len %d \n",frame_len);

		//remove vlan tag
		if(sf_gmac_vlan_all_enable){
			if(sf_gmac_first_recv_vlan == 0){
				sf_gmac_first_recv_vlan = 1;
				memcpy((void *)&vlan_tag[3], (void *)(buf + 15), 1);
				printf("now vlan tag is %d\n", vlan_tag[3]);
			}
			sf_gmac_rx_pop_vlan(buf,frame_len);
			frame_len -= 4;
		}

		net_process_received_packet(buf , frame_len );
		desc_set_rx_owner(p);
		sgmac_flush_cache(priv, 0, 1, priv->rx_index);
		priv->rx_index = dma_ring_incr(priv->rx_index, DMA_RX_RING_SZ);
	} while (1);

	return 0;
}

static int32_t wait_phy_rw_not_busy(struct sgmac_priv *priv)
{
	int32_t reg = readl(priv->base + GMAC_GMII_ADDR);
	// wait for GMII Register GB is not busy
	while (reg & GMII_ADDR_MASK_GB) {
		reg = readl(priv->base + GMAC_GMII_ADDR);
	}
	return reg;
}

int sgmac_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct sgmac_priv *priv = (struct sgmac_priv *)bus->priv;
	int value = wait_phy_rw_not_busy(priv);
	// clear the data first
	writel(0xffffffff, priv->base + GMAC_GMII_DATA);
	// set address flag
	value = (value & (~GMII_ADDR_MASK_GR)) |
		((reg << 6) & GMII_ADDR_MASK_GR);
	// set phy addr
	value = (value & (~GMII_ADDR_MASK_PA)) |
		((addr << 11) & GMII_ADDR_MASK_PA);
	// set read flag
	value = value & (~GMII_ADDR_VALUE_GW_WRITE);
	// set GB flag to indica is busy now
	value = value | GMII_ADDR_MASK_GB;
	writel(value, priv->base + GMAC_GMII_ADDR);
	// wait for complete
	wait_phy_rw_not_busy(priv);
	return readl(priv->base + GMAC_GMII_DATA) & GMII_DATA_MASK;
}

// set phy reg with mac register
int sgmac_mdio_write(struct mii_dev *bus, int addr, int devad, int reg, u16 val)
{
	struct sgmac_priv *priv = (struct sgmac_priv *)bus->priv;
	int addr_value = wait_phy_rw_not_busy(priv);
	// prepare data for register 5
	writel(val & GMII_DATA_MASK, priv->base + GMAC_GMII_DATA);
	// set address flag
	addr_value = (addr_value & (~GMII_ADDR_MASK_GR)) |
		     ((reg << 6) & GMII_ADDR_MASK_GR);
	// set phy addr
	addr_value = (addr_value & (~GMII_ADDR_MASK_PA)) |
		     ((addr << 11) & GMII_ADDR_MASK_PA);
	// set write flag
	addr_value = addr_value | GMII_ADDR_VALUE_GW_WRITE;
	// set GB flag to indica is busy now
	addr_value = addr_value | GMII_ADDR_MASK_GB;
	writel(addr_value, priv->base + GMAC_GMII_ADDR);
	// wait for finish
	wait_phy_rw_not_busy(priv);
	return 0;
}

static void sgmac_adjust_link(struct sgmac_priv *priv,
		struct phy_device *phydev)
{
	int reg = readl(priv->base + GMAC_CONTROL);
	if (!phydev->link) {
		printf("%s: No link.\n", phydev->dev->name);
		return;
	}
	reg &= ~GMAC_CONTROL_SPD_MASK;
	if (phydev->speed == 10)
		reg |= GMAC_SPEED_10M;
	else if (phydev->speed == 100)
		reg |= GMAC_SPEED_100M;
	else if (phydev->speed == 1000)
		reg |= GMAC_SPEED_1000M;

	if (phydev->duplex)
		reg |= GMAC_CONTROL_DM;
	else
		reg &= ~GMAC_CONTROL_DM;

	writel(reg, priv->base + GMAC_CONTROL);
	printf("Speed: %d, %s duplex%s\n", phydev->speed,
			(phydev->duplex) ? "full" : "half",
			(phydev->port == PORT_FIBRE) ? ", fiber mode" : "");
	return;
}

#ifndef CONFIG_GMAC_USE_GPIO_MDIO
static int sgmac_mdio_init(const char *name, void *priv)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -ENOMEM;
	}

	bus->read = sgmac_mdio_read;
	bus->write = sgmac_mdio_write;
	snprintf(bus->name, sizeof(bus->name), "%s", name);

	bus->priv = priv;

	return mdio_register(bus);
}
#endif

#ifdef CONFIG_SFA18_GMAC_PHY
static int sgmac_phy_init(struct sgmac_priv *priv, void *dev)
{
	struct phy_device *phydev;
	int mask = 0xffffffff, ret;

#ifdef CONFIG_SFA18_RGMII_GMAC
	phydev = phy_find_by_mask(priv->bus, mask, PHY_INTERFACE_MODE_RGMII);
#else
	phydev = phy_find_by_mask(priv->bus, mask, PHY_INTERFACE_MODE_RMII);
#endif
	if (!phydev)
		return -ENODEV;

	phy_connect_dev(phydev, dev);

	phydev->supported &= PHY_GBIT_FEATURES;
	if (priv->max_speed) {
		ret = phy_set_supported(phydev, priv->max_speed);
		if (ret)
			return ret;
	}
	phydev->advertising = phydev->supported;

	priv->phydev = phydev;
	phy_config(phydev);
	// enable phy rx clk here, or gmac dma will reset fail
	if (phydev->phy_id == 0x10a)
		yt8521_config_init(phydev);

	return 0;
}
#endif

static int sf_gmac_init(struct eth_device *dev, bd_t *bt)
{
    struct sgmac_priv *priv = (struct sgmac_priv *)dev->priv;

	sgmac_mac_enable(priv);
	return 0;
}

#ifndef CONFIG_GMAC_USE_GPIO_MDIO
static void sf_trigger_eswitch_hwReset(void)
{
	int gpio_pin = CONFIG_SFA18_ESWITCH_RST_GPIO;
	if(gpio_pin != 0xffff){
		gpio_request(gpio_pin, "gswHWRst");
		gpio_direction_output(gpio_pin, 1);
		gpio_set_value(gpio_pin, 0);
		// should more than 1ms
		mdelay(250);
		gpio_set_value(gpio_pin, 1);
		gpio_free(gpio_pin);
		mdelay(1000);
		printf("trigger eswitch hw reset, gpio:%d\n", gpio_pin);
	}
}
#endif

int sf_get_gmac_delay_from_factory(unsigned char *buf)
{
	struct udevice *udev;
	int ret;

	ret = uclass_get_device(UCLASS_SPI_FLASH, 0, &udev);
	if (ret != 0) {
		return -1;
	}

	return spi_flash_read_dm(udev, (SF_FACTORY_OFFSET + GMAC_DELAY_READ_OFFSET),
			GMAC_DELAY_READ_SIZE, buf);
}


int sf_gmac_register(void)
{
	struct eth_device *dev;
	struct sgmac_priv *priv;
#ifdef CONFIG_GMAC_USE_GPIO_MDIO
	uint regValue = 0;
#else
	int chip_id = 0, i = 0;
	rtk_portmask_t portmask;
#endif
	int ret = 0;

	dev = (struct eth_device *)malloc(sizeof(struct eth_device));
	if (dev == NULL) {
		error("%s: Not enough memory!\n", __func__);
		return -1;
	}
	priv = (struct sgmac_priv *)malloc(sizeof(struct sgmac_priv));
	if (priv == NULL) {
		error("%s: Not enough memory!\n", __func__);
		return -1;
	}
	memset(dev, 0, sizeof(struct eth_device));
	memset(priv, 0, sizeof(struct sgmac_priv));

	g_priv = priv;
	sprintf(dev->name, "sf_eth1");
	dev->priv = (void *)priv;
	dev->iobase = GMAC_ADDR_BASE;
	dev->init = sf_gmac_init;
	dev->halt = sf_gmac_halt;
	dev->send = sf_gmac_send;
	dev->recv = sf_gmac_recv;
	dev->write_hwaddr = sf_gmac_write_hwaddr;
	priv->base = (void *)GMAC_ADDR_BASE;
	eth_register(dev);
	printf("read version 0x%x\n", readl(priv->base + GMAC_VERSION));
	sgmac_set_mdc_clk_div(priv);


#ifdef CONFIG_GMAC_USE_GPIO_MDIO
	sf_nf_mdio_init();
	//read it twice to prevent it from getting value
	l2sw_getAsicReg(0x1300, &regValue);
	l2sw_getAsicReg(0x1300, &regValue);
	printf("read 0x1300 reg value :0x%x\n", regValue);
	if (regValue == 0x1619){
		priv->gswitch = 1;
		nf_switch_init();
	}
#else
	sgmac_mdio_init(dev->name, priv);
	priv->bus = miiphy_get_dev_by_name(dev->name);

#ifndef CONFIG_SFA18_GMAC_PHY
	// trigger switch hw reset first
	sf_trigger_eswitch_hwReset();

	// chip id to read realtek
	rtl8367c_setAsicReg(priv, 0x13C2, 0x0249);
	rtl8367c_getAsicReg(priv, 0x1300, &chip_id);

	if (!chip_id) {
		gsw_reg_rd(priv, 0xFA11, 0, 16, (unsigned int*)&chip_id);
	}

	if (chip_id == 0x6367) {
		// realtek giga switch
		priv->gswitch = 1;
		rtk_extPort_rgmii_init(priv, 16);
#if defined(CONFIG_SOC_SFA28_MPW0)
		rtk_port_rgmiiDelayExt_set(priv, 16, 1, 1);
#endif

		// RM#10001 rtk isolation vlan to fix ip error
		for(; i < 5; i++){
			RTK_PORTMASK_CLEAR(portmask);
			RTK_PORTMASK_PORT_SET(portmask, 6);
			RTK_PORTMASK_PORT_SET(portmask, i);
			rtk_port_isolation_set(priv, i, &portmask);
		}

	} else if (chip_id == 0x2003) {
		// intel giga switch
		priv->gswitch = 1;
		intel_rgmii_init(priv, 5);
#if defined(CONFIG_SOC_SFA28_MPW0)
		gsw_reg_wr(priv, PCDU_5_TXDLY_OFFSET,
					PCDU_5_TXDLY_SHIFT,
					PCDU_5_TXDLY_SIZE,
					2);
#endif

		// RM#10001 intel enable vlan to fix ip error
		sf_gmac_vlan_all_enable = 1;
		intel7084_vlan_set();

	}else
#else
	{
		sgmac_phy_init(priv, dev);
	}
#endif /* CONFIG_SFA18_GMAC_PHY */
#endif /* CONFIG_GMAC_USE_GPIO_MDIO */


	writew(CONFIG_SFA18_GMAC_TX_DELAY, (void *)EMAC_CLK_TX_I_DLY);
	writew(CONFIG_SFA18_GMAC_RX_DELAY, (void *)EMAC_CLK_PHY_RX_I_DLY);
	writew(0x1, (void *)EMAC_CLK_PHY_RX_I_DLY_EN);

	sgmac_hw_init(dev, priv);
	sgmac_dma_desc_rings_init(priv);
	/* Start up the PHY */
	if (priv->gswitch) {
		int reg = readl(priv->base + GMAC_CONTROL);
		reg &= ~GMAC_CONTROL_SPD_MASK;
		reg |= GMAC_SPEED_1000M | GMAC_CONTROL_DM;
		writel(reg, priv->base + GMAC_CONTROL);
	} else {
		ret = phy_startup(priv->phydev);
		if (ret) {
			printf("Could not initialize PHY %s\n",
					priv->phydev->dev->name);
			return ret;
		}

		/* Enable Emac Registers */
		sgmac_adjust_link(priv, priv->phydev);
	}

	return 0;
}
