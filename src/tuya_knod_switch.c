#include "tuya_knod_switch.h"
#include "tuya_app_light.h"
#include "uni_log.h"

typedef enum 
{
    SW_LED = 20,
    BRIGHT_VALUE = 22,
    TEMP_VALUE = 23,
}DP_ID_E;

/***********************************************************
*   Function:  updata_led_temperature_status
*   Input:     none
*   Output:    none
*   Return:    none
*   Notice:    updata dp of led temperature value
***********************************************************/
VOID updata_led_temperature_status(void)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 0;
    dp_cnt = 1;

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = TEMP_VALUE;
    dp_arr[0].type = PROP_VALUE;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_value = led_sta.led_temperature;

    //上报 dp 点数据到云平台
    op_ret = dev_report_dp_json_async(NULL ,dp_arr,dp_cnt);
    Free(dp_arr);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;
}

/***********************************************************
*   Function:  updata_led_bright_status
*   Input:     none
*   Output:    none
*   Return:    none
*   Notice:    updata dp of led bright value
***********************************************************/
VOID updata_led_bright_status(void)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 0;
    dp_cnt = 1;

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = BRIGHT_VALUE;
    dp_arr[0].type = PROP_VALUE;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_value = led_sta.led_bright;

    op_ret = dev_report_dp_json_async(NULL ,dp_arr,dp_cnt);
    Free(dp_arr);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;
}

/***********************************************************
*   Function:  updata_led_switch_status
*   Input:     none
*   Output:    none
*   Return:    none
*   Notice:    updata dp of led switch status
***********************************************************/
VOID updata_led_switch_status(void)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 0;
    dp_cnt = 1;

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = SW_LED;
    dp_arr[0].type = PROP_BOOL;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_bool = led_sta.on_off;

    op_ret = dev_report_dp_json_async(NULL ,dp_arr,dp_cnt);
    Free(dp_arr);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;
}

/***********************************************************
*   Function:  hw_report_all_dp_status
*   Input:     none
*   Output:    none
*   Return:    none
*   Notice:    updata dp of led all value
***********************************************************/
VOID hw_report_all_dp_status(VOID)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 0;
    dp_cnt = 3;

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = SW_LED;
    dp_arr[0].type = PROP_BOOL;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_bool = led_sta.on_off;


    dp_arr[1].dpid = BRIGHT_VALUE;
    dp_arr[1].time_stamp = 0;
    dp_arr[1].type = PROP_VALUE;
    dp_arr[1].value.dp_value = led_sta.led_bright;

    dp_arr[2].dpid = TEMP_VALUE;
    dp_arr[2].time_stamp = 0;
    dp_arr[2].type = PROP_VALUE;
    dp_arr[2].value.dp_value = led_sta.led_temperature;

    op_ret = dev_report_dp_json_async(NULL ,dp_arr,dp_cnt);
    Free(dp_arr);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    PR_DEBUG("dp_query report_all_dp_data");
    
    return;
}

/***********************************************************
*   Function:  deal_dp_proc
*   Input:     root : 涂鸦云平台下发的数据点
*   Output:    none
*   Return:    none
*   Notice:    根据涂鸦云平台下发的数据，设置设备的状态
***********************************************************/
VOID deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    UCHAR_T dpid;

    dpid = root->dpid;
    PR_DEBUG("dpid:%d",dpid);

    switch(dpid) {
        case SW_LED:
            if (root->value.dp_bool == TRUE) {
                led_on();
            } else if (root->value.dp_bool == FALSE) {
                led_off();
            }
        break;
            
        case BRIGHT_VALUE:
            led_bright_set(root->value.dp_value);
        break;
        
        case TEMP_VALUE:
            led_temperature_set(root->value.dp_value);
        break;

        default:
        break;
    }
    
    return;
}
