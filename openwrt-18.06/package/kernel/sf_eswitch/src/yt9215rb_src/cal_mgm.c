/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_types.h"
#include "yt_error.h"
#include "cal_mgm.h"
#include "cal_bprofile.h"

/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

/*
 * Data Declaration
 */
uint8_t gcal_inited = FALSE;

/*
 * Function Declaration
 */


/*
 [Function Name]
     
 [Input/Output]
 
 [Description]

 [Return]
 
 [Note]

 */
uint32_t cal_mgm_init(void)
{
    CMM_PARAM_CHK((TRUE == gcal_inited), CMM_ERR_OK);

    cal_board_profile_init();

    gcal_inited = TRUE;
   
    return CMM_ERR_OK;
}
