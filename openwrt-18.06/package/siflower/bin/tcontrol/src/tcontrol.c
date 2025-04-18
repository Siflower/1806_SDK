/*
* =====================================================================================
*
*       Filename:  tcontrol/src/tcontrol.c
*
*    Description:  Monitor the current chip temperature and take corresponding cooling
*       measures.
*
*        Version:  1.0
*        Created:  12/20/2021 10:18:20 AM
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  Ming.Guang <ming.guang@siflower.com.cn>
*        Company:  Siflower Communication Tenology Co.,Ltd
*
* =====================================================================================
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <dirent.h>
#include "tcontrol.h"

struct siwifi_temp_ctrl * hb_ctrl;
struct siwifi_temp_ctrl * lb_ctrl;

int first_warn_flag = 0;

/**
 * ************************************************************************
* Function that judge the rationality of configuration file and fill info.
* @0:  Temperature control switch
* @1:  High temperature threshold
* @2:  Temperature low threshold
* @3:  Listening interval (s)
* @4:  Jump interval
* @5:  Cooling interval
* @6-10:  Different cooling strategies
*
* @return  Only 0 is returned for success
* *************************************************************************
*/
static int siwifi_fill_info(struct siwifi_temp_ctrl * temp_ctrl, int value, int i)
{
    switch(i) {
        case 0:
            if(value == 0)
                temp_ctrl->enable_temp_ctrl = false;
            else if (value == 1)
                temp_ctrl->enable_temp_ctrl = true;
            else
                goto error;
            break;
        case 1:
            temp_ctrl->temp_ctrl_level[0] = value;
            break;
        case 2:
            if (value < temp_ctrl->temp_ctrl_level[0])
                temp_ctrl->temp_ctrl_level[1] = value;
            else
                goto error;
            break;
        case 3:
            temp_ctrl->sleep_time = value;
            break;
        case 4:
            temp_ctrl->time_ctrl_limit[0] = value;
            break;
        case 5:
            temp_ctrl->time_ctrl_limit[1] = value;
            break;
        case 6:
            if(value < 0 || value > 100)
                goto error;
            else
                temp_ctrl->tx_ctrl_limit[i-6] = value;
            break;
        default:
            if(value < 0 || value > 100)
                goto error;
            if (value < temp_ctrl->tx_ctrl_limit[i-7])
                temp_ctrl->tx_ctrl_limit[i-6] = value;
            else
                temp_ctrl->tx_ctrl_limit[i-6] = temp_ctrl->tx_ctrl_limit[i-7] - 5;
            if (temp_ctrl->tx_ctrl_limit[i-6] < 0)
                temp_ctrl->tx_ctrl_limit[i-6] = 0;
            break;
    }

    return 0;
error:
    LOG("Error in configuration file!\n");

    return -1;
}

/**
* Functions that read the configuration file and initialize the temperature control parameters.
*/
static int siwifi_temp_config_init(struct siwifi_temp_ctrl * temp_ctrl, int flag)
{
    FILE * file;
    char temp_info[32], * str;
    int i, value, ret;

    // open the file
    file = fopen(flag ? TEMP_CTRL_HB_FILE_PTAH : TEMP_CTRL_LB_FILE_PTAH, "r");
    if (file == NULL) {
        LOG("%s: Failed to open the file.\n", __func__);
        return -1;
    }

    // fill the temperature control info
    for (i = 0; i < SIZE_OF_FILE_NAME && !feof(file); i++) {
        fgets(temp_info, 32, file);
        if (!strstr(temp_info, info_name[i])) {
            LOG("Error in configuration file!\n");
            ret = -1;
            goto end;
        }
        strtok(temp_info, "=");
        str = strtok(NULL, "=");
        value = strtol(str, NULL, 10);
        ret = siwifi_fill_info(temp_ctrl, value, i);
        if (ret)
            goto end;
    }
    if(flag)
        temp_ctrl->band_flag = 1;
    else
        temp_ctrl->band_flag = 0;
    temp_ctrl->enable_cool_ctrl = false;
    temp_ctrl->time_ctrl_count[0] = 0;
    temp_ctrl->time_ctrl_count[1] = 0;
    temp_ctrl->jump_count = 0;

end:
    // close the file
    if(fclose(file))
        LOG("%s: Failed to close the file.\n", __func__);

    return ret;
}

/**
* Functions that sets the maximum length of the MSDU aggregate.
*/
static int siwifi_set_amsdu_maxnb(int band_flag, int num)
{
    FILE * file;
    char path[64], str[3];

    // convert integer to string
    if (sprintf(str, "%d", num) < 0) {
        LOG("Failed to convert integer to string.\n");
        return -1;
    }
    // open the file
    //snprintf(path, sizeof(path), "/sys/module/%s/parameters/amsdu_maxnb",
    //        band_flag ? "sf16a18_hb_fmac" : "sf16a18_hb_fmac");
    snprintf(path, sizeof(path), "/sys/module/sf16a18_fmac/parameters/%s",
            band_flag ? "amsdu_maxnb_hb" : "amsdu_maxnb_lb");
    file = fopen(path, "r+");
    if (file == NULL) {
        LOG("%s: Failed to open the file.(path: %s)\n", __func__, path);
        return -1;
    }

    // set amsdu len
    if (fputs(str, file) < 0) {
        LOG("%s: Failed to set amsdu len.(path: %s)\n", __func__, path);
        return -1;
    }
    // close the file
    if(fclose(file)) {
        LOG("%s: Failed to close the file.\n", __func__);
        return -1;
    }

    return 0;
}

/**
* Functions that get the HWQ quantity that needs to be limited.
*/
static int siwifi_get_hwq_num(struct siwifi_temp_ctrl * temp_ctrl, int rate_percent)
{
    int hwq_num;

    // judge whether the rate_percent is in range
    if (rate_percent < 1 || rate_percent > 100) {
        LOG("%s:Rate_percent error!\n", __func__);
        return -1;
    }

    // 2.4G (Maximum theoretical rate)
    if (!temp_ctrl->band_flag) {
        // 75% ~ rate ~ 100%
        if (100 >= rate_percent && rate_percent >= 75 ) {
            // set amsdu max num to 6 (Preserve aggregation of MSDU frames)
            if (siwifi_set_amsdu_maxnb(temp_ctrl->band_flag, 6))
                return -1;
            hwq_num = lb_rate2hwq_table[rate_percent - 1];
        // 0% ~ rate ~ 74%
        } else if (75 > rate_percent && rate_percent >= 0) {
            // set amsdu max num to 1 (Turn off aggregation of MSDU frames)
            if (siwifi_set_amsdu_maxnb(temp_ctrl->band_flag, 1))
                return -1;
            hwq_num = lb_rate2hwq_table[rate_percent - 1];
        } else {
            LOG("%s:rate_percent is wrong!\n", __func__);
            return -1;
        }
    // 5G (Maximum theoretical rate)
    } else {
        // 50% ~ rate ~ 100%
        if ( 100 >= rate_percent && rate_percent >= 50) {
            // set amsdu max num to 6 (Preserve aggregation of MSDU frames)
            if (siwifi_set_amsdu_maxnb(temp_ctrl->band_flag, 6))
                return -1;
            hwq_num = hb_rate2hwq_table[rate_percent - 1];
        // 0% ~ rate ~ 49%
        } else if (50 > rate_percent && rate_percent >= 0) {
            // set amsdu max num to 1 (Turn off aggregation of MSDU frames)
            if (siwifi_set_amsdu_maxnb(temp_ctrl->band_flag, 1))
                return -1;
            hwq_num = hb_rate2hwq_table[rate_percent - 1];
        } else {
            LOG("%s:rate_percent is wrong!\n", __func__);
            return -1;
        }
    }

    return hwq_num;
}

/**
* Functions that get node value.
*/
static int siwifi_get_node_value(char * buff)
{
    FILE * file;
    char value[32];
    int ret = -1;

    // open the file
    file = fopen(buff, "r+");
    if (file == NULL) {
        LOG("%s:Open file failed!\n", __func__);
        return ret;
    }

    // get the value
    fgets(value, 32, file);
    LOG("buff %s value %s\n", buff, value);
    if (!strncmp(value, "lb", 2))
        ret = 0;
    if (!strncmp(value, "hb", 2))
        ret = 1;

    // close the file
    fclose(file);

    return ret;
}

/**
* Functions that get PHY name.
*/
static void siwifi_get_phy_name(char * phy, int band_flag)
{
    DIR *dir;
    struct dirent *ent;
    char buff[128];
    int ret;

    dir = opendir("/sys/kernel/debug/ieee80211");
    while((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, "phy", 3))
            continue;
        snprintf(buff, sizeof(buff), "/sys/kernel/debug/ieee80211/%s/siwifi/band_type", ent->d_name);
        ret = siwifi_get_node_value(buff);
        if(band_flag && ret == 1) {
            snprintf(phy, 10, "%s", ent->d_name);
            return;
        } else if(!band_flag && ret == 0){
            snprintf(phy, 10, "%s", ent->d_name);
            return;
        } else {
            continue;
        }
    }

    phy = "error";
    LOG("Get phy name failed !\n");

    return;
}

/**
* Functions that limit TX throughput.
* When the current limit is higher than 99%,
* an antenna will be turned off. (total 2)
*/
static int siwifi_set_tx_limit(struct siwifi_temp_ctrl * temp_ctrl, int limit)
{
    FILE * file;
    char path[128], str[3], phy[8];
    int hwq_num;

    // get HWQ num
    hwq_num = siwifi_get_hwq_num(temp_ctrl, limit);
    if (hwq_num < 0) {
        LOG("Get HWQ number failed!\n");
        return -1;
    }

    // convert integer to string
    if (sprintf(str, "%d", hwq_num) < 0) {
        LOG("Failed to convert integer to string.\n");
        return -1;
    }

    // get phy name
    siwifi_get_phy_name(phy, temp_ctrl->band_flag);

    // first path
    // open the file
    snprintf(path, sizeof(path), "/sys/kernel/debug/ieee80211/%s/siwifi/throughput_ctrl/tx_ctrl", phy);
    file = fopen(path, "r+");
    if (file == NULL) {
        LOG("%s: Failed to open the file.(path: %s)\n", __func__, path);
        goto next;
    }
    // set tx limit
    if (fputs(str, file) < 0) {
        LOG("%s: Failed to set tx limit.(path: %s)\n", __func__, path);
        goto end;
    }
    // close the file
    if(fclose(file)) {
        LOG("%s: Failed to close the file.\n", __func__);
        return -1;
    }

    return 0;

next:
    // second path
    // open the file
    snprintf(path, sizeof(path), "/proc/%s/siwifi/throughput_ctrl/tx_ctrl",
            temp_ctrl->band_flag ? "hb" : "lb");
    file = fopen(path, "r+");
    if (file == NULL) {
        LOG("%s: Failed to open the file.(path: %s)\n", __func__, path);
        return -1;
    }
    // set tx limit
    if (fputs(str, file) < 0) {
        LOG("%s: Failed to set tx limit.(path: %s)\n", __func__, path);
        goto end;
    }
    // close the file
    if(fclose(file)) {
        LOG("%s: Failed to close the file.\n", __func__);
        return -1;
    }

    return 0;
end:
    // close the file
    if(fclose(file)) {
        LOG("%s: Failed to close the file.\n", __func__);
        return -1;
    }

    return -1;
}

/**
* Functions that get current chip temperature.
*/
static int siwifi_get_temperature()
{
    FILE * file;
    char temp[32];
    int cur_temp;

    // open the file
    file = fopen(GET_TEMP_PTAH, "r");
    if (file == NULL) {
        LOG("%s: Failed to open the file.\n", __func__);
        return -1;
    }

    // determine whether there is data in the file
    if (feof(file)) {
        LOG("%s: Data is empty.\n", __func__);
        return -1;
    }

    // get the temp
    fgets(temp, 32, file);
    cur_temp = strtol(temp, NULL, 10);

    // close the file
    if(fclose(file)) {
        LOG("%s: Failed to close the file.\n", __func__);
        return -1;
    }

    return cur_temp;
}

/**
* Monitor the current chip temperature and take corresponding cooling measures.
*/
static int siwifi_take_cooling(struct siwifi_temp_ctrl * temp_ctrl, int cur_temp)
{
    int set_flag = 0;
    int warn_temp = 105;
    FILE *pp = NULL;
    //if cur_temp >= warning_temp clz one ant
    if ((cur_temp >= warn_temp) && (first_warn_flag == 0) && (temp_ctrl->band_flag == 0)) {
        first_warn_flag = 1;

        pp = popen("echo 0 > sys/kernel/debug/ieee80211/phy1/siwifi/rc_set_no_ss", "r");
        if (pp == NULL)
        {
            LOG("Temp ctrl : rc_set_no_ss 0 fail\n");
            return 0;
        }
        pclose(pp);

        pp = popen("echo property_id=53 property_value=2 > /sys/kernel/debug/aetnensis/property", "r");
        if (pp == NULL)
        {
            LOG("Temp ctrl : set property 53 fail\n");
            return 0;
        }
        pclose(pp);
    } else if ((cur_temp <= (warn_temp - 25)) && (first_warn_flag == 1) && (temp_ctrl->band_flag == 0)) {
        //Todo restore two ant
        first_warn_flag = 0;

        pp = popen("echo property_id=53 property_value=0 > /sys/kernel/debug/aetnensis/property", "r");
        if (pp == NULL)
        {
            LOG("Temp ctrl : set property 53 fail\n");
            return 0;
        }
        pclose(pp);

        pp = popen("echo 1 > sys/kernel/debug/ieee80211/phy1/siwifi/rc_set_no_ss", "r");
        if (pp == NULL)
        {
            LOG("Temp ctrl : rc_set_no_ss 1 fail\n");
            return 0;
        }
        pclose(pp);
    }

    // judge whether cooling measures are being taken, and current temp is lower than high temp level
    if (cur_temp < temp_ctrl->temp_ctrl_level[0] && !temp_ctrl->enable_cool_ctrl)
        return 0;

    // take cooling measures
    /* 2,continue cooling measures */
    if (temp_ctrl->enable_cool_ctrl) {
        // temp A > B
        // when cur_temp >= A
        if (cur_temp >= temp_ctrl->temp_ctrl_level[0]) {
            temp_ctrl->time_ctrl_count[1] = 0;
            goto tag2;
        // when cur_temp <= B
        } else if (cur_temp <= temp_ctrl->temp_ctrl_level[1]) {
tag1:
            /* 3,end cooling measures */
            temp_ctrl->enable_cool_ctrl = false;
            temp_ctrl->time_ctrl_count[0] = 0;
            temp_ctrl->time_ctrl_count[1] = 0;
            temp_ctrl->jump_count = 0;
            set_flag = siwifi_set_tx_limit(temp_ctrl, 100);
        // when B < cur_temp < A
        } else {
            if (temp_ctrl->time_ctrl_count[0] == temp_ctrl->time_ctrl_limit[0]) {
tag2:
                if (temp_ctrl->jump_count < TEMP_CTRL_DEFAULT_JUMP_LIMIT) {
                    set_flag = siwifi_set_tx_limit(temp_ctrl, temp_ctrl->tx_ctrl_limit[temp_ctrl->jump_count]);
                    temp_ctrl->jump_count++;
                }
                temp_ctrl->time_ctrl_count[0] = 0;
            }
            temp_ctrl->time_ctrl_count[0]++;
            temp_ctrl->time_ctrl_count[1]++;
        }

        // judge whether the maximum cooling time is reached
        if (temp_ctrl->time_ctrl_count[1] == temp_ctrl->time_ctrl_limit[1])
            goto tag1;
    /* 1,start cooling measures */
    } else {
        temp_ctrl->enable_cool_ctrl = true;
        set_flag = siwifi_set_tx_limit(temp_ctrl, temp_ctrl->tx_ctrl_limit[temp_ctrl->jump_count]);
        temp_ctrl->time_ctrl_count[0]++;
        temp_ctrl->time_ctrl_count[1]++;
        temp_ctrl->jump_count++;
    }

    if (set_flag) {
        LOG("Set tx limit failed!\n");
        temp_ctrl->enable_temp_ctrl = false;
        return -1;
    }

    return 0;
}

/**
* Thread response function.
*/
static void * siwifi_thread_handler(void * arg)
{
    int cur_temp;
    bool flag = true;

    // if cooling enable,take cooling measure
    while (lb_ctrl->enable_temp_ctrl || hb_ctrl->enable_temp_ctrl) {
        // Determine whether the temperature is controlled by 2.4G or 5g,or both
        if (flag && lb_ctrl->enable_temp_ctrl) {
                LOG("Control 2.4G TX throughput to control temperature.\n");
        }
        if (flag && hb_ctrl->enable_temp_ctrl) {
                LOG("Control 5G TX throughput to control temperature.\n");
        }

        // get chip temperature
        cur_temp = siwifi_get_temperature();
        if (cur_temp < 0)
            return arg;

        // take cooling
        if (lb_ctrl->enable_temp_ctrl) {
            if (siwifi_take_cooling(lb_ctrl, cur_temp))
                return arg;
        }
        if (hb_ctrl->enable_temp_ctrl) {
            if (siwifi_take_cooling(hb_ctrl, cur_temp))
                return arg;
        }

        // next cycle (The sleep time must be consistent 2.4g & 5g)
        // lb_ctrl->sleep_time == hb_ctrl->sleep_time
        // sleep(hb_ctrl->sleep_time);
        sleep(lb_ctrl->sleep_time);
        flag = false;
    }

    if (flag)
        LOG("The temperature control switch is not turned on!\n");

    return arg;
}

int main()
{
    pthread_t thread;
    int ret;

    // Request space for temperature control pointer
    lb_ctrl = (struct siwifi_temp_ctrl * )malloc(sizeof(struct siwifi_temp_ctrl));
    if (!lb_ctrl){
        LOG("Malloc buffer for siwifi_temp_ctrl failed.\n");
        return -1;
    }
    hb_ctrl = (struct siwifi_temp_ctrl * )malloc(sizeof(struct siwifi_temp_ctrl));
    if (!hb_ctrl){
        LOG("Malloc buffer for siwifi_temp_ctrl failed.\n");
        free(lb_ctrl);
        return -1;
    }

    // Initialize temperature control parameters
    if (siwifi_temp_config_init(lb_ctrl, 0) || siwifi_temp_config_init(hb_ctrl, 1))
        return -1;

    // create a thread for temperature control
    ret = pthread_create(&thread, NULL, siwifi_thread_handler, NULL);
    if(ret != 0) {
        LOG( "Create thread for temperature control failed!\n");
        free(lb_ctrl);
        free(hb_ctrl);
        return -1;
    }

    // Wait for thread to finish and close the main thread
    pthread_join(thread, NULL);
    free(lb_ctrl);
    free(hb_ctrl);
    LOG("Turn off temperature control.\n");

    return 0;
}
