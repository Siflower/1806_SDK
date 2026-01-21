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
    cmm_err_t ret = CMM_ERR_OK;

    if(TRUE == gcal_inited)
    {
        return CMM_ERR_OK;
    }

    CMM_ERR_CHK(cal_board_profile_init(), ret);

    gcal_inited = TRUE;
   
    return CMM_ERR_OK;
}

void cal_mgm_exit(void)
{
    // cal_board_profile_clear();
    gcal_inited = (yt_bool_t)FALSE;

    return;
}