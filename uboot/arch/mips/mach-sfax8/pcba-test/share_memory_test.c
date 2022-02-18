#include <common.h>
#include <mach/pcba_test.h>
#include <asm/io.h>

#define SHARE_MEMORY_SIZE          (96 * 1024)
#define SHARE_MEMORY_24G_BASE_ADDR 0xB1000000
#define SHARE_MEMORY_5G_BASE_ADDR  0xB1400000

#define SF_IRAM_SOFT_RESET         0x0C00
#define SIFLOWER_SYSCTL_BASE       0xB9E00000
#define IRAM_CLK1_EN_OFFSET        0x20
#define BOE_OFFSET		   0x0C
#define IRAM_SOFT_RESET1_OFFSET    0x1C
#define REG_SYSM_SHARE_RAM_SEL     (volatile void *)(0xB9E00000 + 0x242C)

/* SF16A18 has 96K(2.4G) + 96K(5G) share memory and we use host cpu to do w/r
 * test. Firstly, we write 0 and read it, then we write 0xFFFFFFFF and read it.
 */
int share_memory_test(void *d)
{
	int i = 0;
	unsigned int write_value = 0, read_value = 0;
	unsigned int tmp = 0;

	writel(0xFF, (void *)(SIFLOWER_SYSCTL_BASE + SF_IRAM_SOFT_RESET +
				     IRAM_CLK1_EN_OFFSET));
	writel(0x0, (void *)(SIFLOWER_SYSCTL_BASE + SF_IRAM_SOFT_RESET +
				    IRAM_SOFT_RESET1_OFFSET));
	writel(0xF, (void *)(SIFLOWER_SYSCTL_BASE + SF_IRAM_SOFT_RESET +
				    BOE_OFFSET));

	tmp = readb(REG_SYSM_SHARE_RAM_SEL);
	tmp = tmp | (1 << 0) | (1 << 1);
	writeb(tmp, REG_SYSM_SHARE_RAM_SEL);

	for (i = 0; i < SHARE_MEMORY_SIZE / sizeof(unsigned int); i++) {
		write_value = 0;
		writel(write_value, (void *)(SHARE_MEMORY_24G_BASE_ADDR +
						    i * sizeof(unsigned int)));
		read_value = readl((void *)(SHARE_MEMORY_24G_BASE_ADDR +
				   i * sizeof(unsigned int)));
		if (write_value != read_value) {
			printf("2.4G share memory r/w test fail, addr 0x%x, "
			       "write value 0x%x, read value 0x%x\n",
					SHARE_MEMORY_24G_BASE_ADDR +
							i * sizeof(unsigned int),
					write_value, read_value);
			return CMD_FAIL;
		}

		writel(write_value, (void *)(SHARE_MEMORY_5G_BASE_ADDR +
						    i * sizeof(unsigned int)));
		read_value = readl((void *)(SHARE_MEMORY_5G_BASE_ADDR +
				   i * sizeof(unsigned int)));
		if (write_value != read_value) {
			printf("5G share memory r/w test fail, addr 0x%x, "
			       "write value 0x%x, read value 0x%x\n",
					SHARE_MEMORY_5G_BASE_ADDR +
							i * sizeof(unsigned int),
					write_value, read_value);
			return CMD_FAIL;
		}

		write_value = 0xFFFFFFFF;
		writel(write_value, (void *)(SHARE_MEMORY_24G_BASE_ADDR +
						    i * sizeof(unsigned int)));
		read_value = readl((void *)(SHARE_MEMORY_24G_BASE_ADDR +
				   i * sizeof(unsigned int)));
		if (write_value != read_value) {
			printf("2.4G share memory r/w test fail, addr 0x%x, "
			       "write value 0x%x, read value 0x%x\n",
					SHARE_MEMORY_24G_BASE_ADDR +
							i * sizeof(unsigned int),
					write_value, read_value);
			return CMD_FAIL;
		}
		writel(write_value, (void *)(SHARE_MEMORY_5G_BASE_ADDR +
						    i * sizeof(unsigned int)));
		read_value = readl((void *)(SHARE_MEMORY_5G_BASE_ADDR +
				   i * sizeof(unsigned int)));
		if (write_value != read_value) {
			printf("5G share memory r/w test fail, addr 0x%x, "
			       "write value 0x%x, read value 0x%x\n",
					SHARE_MEMORY_5G_BASE_ADDR +
							i * sizeof(unsigned int),
					write_value, read_value);
			return CMD_FAIL;
		}
	}

	return CMD_SUCCESS;
}
