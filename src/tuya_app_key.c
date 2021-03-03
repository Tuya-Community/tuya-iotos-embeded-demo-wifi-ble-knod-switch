#include "tuya_key.h"
#include "tuya_app_key.h"
#include "tuya_app_light.h"
#include "BkDriverGpio.h"
#include "uni_log.h"

INT_T KEY_TIMER_MS;

KEY_USER_DEF_S KEY_DEF_T;

static void app_key_ab_proc_cb(void)
{
    //去使能 旋钮开关外部中断
    int bk_ret;
    bk_ret = BkGpioFinalize(ROTARY_A_PORT);
    if (kGeneralErr == bk_ret) {
        PR_ERR("BkGpioFinalize error!");
        return;
    }

    
    if(ROTARY_A_LEVEL != ROTARY_B_LEVEL) {
        //PR_NOTICE("A != B"); //顺时针方向
        if (led_sta.regulate_mode == temperature) {
            led_temperature_set(led_sta.led_temperature + 100);
        } else {
            led_bright_set(led_sta.led_bright + 100);
        }
    } else {
        //PR_NOTICE("A == B"); //逆时针方向
        if (led_sta.regulate_mode == temperature) {
            led_temperature_set(led_sta.led_temperature - 100);
        } else {
            led_bright_set(led_sta.led_bright - 100);
        }
    }

    //使能旋钮开关外部中断
    BkGpioEnableIRQ(ROTARY_A_PORT, IRQ_TRIGGER_FALLING_EDGE, app_key_ab_proc_cb, NULL);
}

static void app_key_n_proc_cb(TY_GPIO_PORT_E port,PUSH_KEY_TYPE_E type,INT_T cnt)
{
    OPERATE_RET op_ret;
    
    if (port == ROTARY_N_PORT) {
        switch (type) {
            case NORMAL_KEY:
                if (led_sta.on_off ==  FALSE) {
                    led_on();
                    PR_NOTICE("led on");
                } else if (led_sta.on_off ==  TRUE) {
                    led_off();
                    PR_NOTICE("led off");
                }
                break;
            case LONG_KEY:
                PR_NOTICE("ROTARY_N_PORT long press.");
                tuya_iot_wf_gw_unactive();
                break;
            case SEQ_KEY:
                //PR_NOTICE("ROTARY_N_PORT SEQ press, the count is %d.", cnt);
                if (cnt == 2) {
                    if (led_sta.regulate_mode == temperature) {
                        led_sta.regulate_mode = bright;
                    } else if (led_sta.regulate_mode == bright) {
                        led_sta.regulate_mode = temperature;
                    }
                }
                
                break;
            default:
                break;
        }
    }
    
}

static void key_devic_init(void)
{
    OPERATE_RET opRet;
        
    /* 旋钮正反转检测初始化 */
    BkGpioEnableIRQ(ROTARY_A_PORT, IRQ_TRIGGER_FALLING_EDGE, app_key_ab_proc_cb, NULL);
    
    /* 旋钮开关按下检测初始化 */
    opRet = key_init(NULL, 0, KEY_TIMER_MS);
    if (opRet != OPRT_OK) {
        PR_ERR("key_init err:%d", opRet);
        return opRet;
    }

    memset(&KEY_DEF_T, 0, SIZEOF(KEY_DEF_T));
    KEY_DEF_T.port = ROTARY_N_PORT;
    KEY_DEF_T.long_key_time = 3000;
    KEY_DEF_T.low_level_detect = TRUE;
    KEY_DEF_T.lp_tp = LP_ONCE_TRIG;
    KEY_DEF_T.call_back = app_key_n_proc_cb;
    KEY_DEF_T.seq_key_detect_time = 400;
    
    opRet = reg_proc_key(&KEY_DEF_T);
    if (opRet != OPRT_OK) {
        PR_ERR("reg_proc_key err:%d", opRet);
        return opRet;
    }
}

void app_key_init(void)
{
    /* 配置按键 IO口 */
    tuya_gpio_inout_set(ROTARY_N_PORT, TRUE);
    tuya_gpio_inout_set(ROTARY_A_PORT, TRUE);
    tuya_gpio_inout_set(ROTARY_B_PORT, TRUE);

    key_devic_init();
}
