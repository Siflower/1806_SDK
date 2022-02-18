#include <mach/pcba_test.h>
#include <common.h>
#include <asm/io.h>
#include <net.h>

#define GMAC_ADDR_BASE 0xB0800000

/* GMAC Register list */
#define GMAC_CONTROL                    0x00000000      /* Register 0 (MAC Configuration Register)*/

/* MAC Configuration Register */
#define GMAC_CONTROL_SARC_MASK		0xc0000000	/* bit [30:28] Source Address Insertion or Replacement Control */
#define GMAC_CONTROL_TWOKPE		0x08000000	/* bit [27] IEEE 802.3as Support for 2K Packets */
#define GMAC_CONTROL_CST		0x02000000	/* bit [25] CRC Stripping for Type Frames */
#define GMAC_CONTROL_WD			0x00800000	/* bit [23] Watchdog Disable */
#define GMAC_CONTROL_JD			0x00400000	/* bit [22] Jabber Disable */
#define GMAC_CONTROL_JE			0x00100000	/* bit [20] Jumbo Frame Enable */
#define GMAC_CONTROL_SPD_MASK		0x0000c000	/* bit [15:14] Speed Mask(10M/100M/1000M) */
#define GMAC_CONTROL_PS			0x00008000	/* bit [15] Port Select */
#define GMAC_CONTROL_FES		0x00004000	/* bit [14] speed (0: 10 Mbps, 1: 100 Mbps) */
#define GMAC_CONTROL_LM			0x00001000	/* bit [12] Loopback Mode */
#define GMAC_CONTROL_DM			0x00000800	/* bit [11] Duplex Mode */
#define GMAC_CONTROL_IPC		0x00000400	/* bit [10] Checksum Offload */
#define GMAC_CONTROL_ACS		0x00000080	/* bit [7] Automatic Pad/FCS Strip */
#define GMAC_CONTROL_TE			0x00000008	/* bit [3] Transmitter Enable */
#define GMAC_CONTROL_RE			0x00000004	/* bit [2] Receiver Enable */

#define GMAC_SPEED_1000M		0
#define GMAC_SPEED_100M			GMAC_CONTROL_SPD_MASK
#define GMAC_SPEED_10M			GMAC_CONTROL_PS

enum gmac_test_type {
	GMAC_1000M_TEST = 0,
};

struct gmac_test_param {
	enum gmac_test_type test_type;
};

static int gmac_1000m_test(void)
{
	void __iomem *base;
	int value = 0;
	base = (void __iomem *)GMAC_ADDR_BASE;
	value = readl(base + GMAC_CONTROL);
	value &= GMAC_CONTROL_SPD_MASK;
	if (value == GMAC_SPEED_1000M)
		return CMD_SUCCESS;
	return CMD_FAIL;
}

int gmac_test(void *d) {
	int ret = CMD_FAIL;
	struct gmac_test_param *gmac_test_param =
			(struct gmac_test_param *)d;

	switch (gmac_test_param->test_type) {
	case GMAC_1000M_TEST:
		ret = gmac_1000m_test();
		break;
	default:
		ret = CMD_FAIL;
		break;
	}
	return ret;
}
