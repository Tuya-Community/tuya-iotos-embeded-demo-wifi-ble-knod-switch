#ifndef _tuya_KNOD_SWITCH_H_
#define _tuya_KNOD_SWITCH_H_

#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"


VOID deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

//单项功能 dp点 的上报
VOID updata_led_temperature_status(void);
VOID updata_led_bright_status(void);
VOID updata_led_switch_status(void);

//上报所有 dp点 的数据状态
VOID hw_report_all_dp_status(VOID);

#endif
