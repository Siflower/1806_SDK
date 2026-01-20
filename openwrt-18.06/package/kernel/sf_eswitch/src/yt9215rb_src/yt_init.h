#ifndef __YT_INIT_H__
#define __YT_INIT_H__

/*
 * Include Files
 */
 #include "yt_types.h"
 
/*
 * Symbol Definition
 */

/*
 * Macro Definition
 */


/*
 * Data Type Declaration
 */


/*
 * Function Declaration
 */
extern yt_ret_t  yt_basic_init(void);
extern yt_ret_t  yt_device_init(uint8_t unit, uint8_t *dev);
extern yt_ret_t  yt_device_close(uint8_t unit);
extern yt_ret_t yt_patch_init(void);
extern yt_ret_t yt_modules_init(void);
extern yt_ret_t  yt_init(void);
extern yt_ret_t  yt_drv_init(void);
extern yt_ret_t  yt_port_default_cfg_init(void);
extern yt_ret_t  yt_phy_drv_load(void);

#endif

