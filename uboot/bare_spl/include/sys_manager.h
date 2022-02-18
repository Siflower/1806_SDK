#ifndef __SYS_MANAGER_H__
#define __SYS_MANAGER_H__

extern int sys_init(void);
extern int memory_soft_reset(void);
extern int bus_soft_reset(void);
extern int io_soft_reset(void);
extern int sys_apb_modules_reset(void);
extern int sys_dma_reset(void);
extern int sys_npu_reset(void);
extern int sys_wifi_reset(void);
extern int sys_usb_reset(void);
extern int sys_gmac_reset(void);
extern int sys_emmc_reset(void);
extern int sys_sdio_reset(void);
extern int sys_gdu_reset(void);
extern int sys_gpio_reset(void);
extern int sys_reboot(void);

extern int sys_apb_reset_pcm(void);
extern int sys_apb_reset_spi(void);
extern int sys_apb_reset_uart(void);
extern int sys_apb_reset_i2c(void);
extern int sys_apb_reset_pwm(void);
extern int sys_apb_reset_i2s(void);
extern int sys_apb_reset_timer(void);
extern int sys_apb_reset_rtc(void);
extern int sys_apb_reset_watchdog(void);
extern int sys_apb_reset_spdif(void);
extern int sys_apb_bus_reset(void);

#endif

