#include "tuya_app_light.h"
#include "soc_pwm.h"
#include "light_printf.h"


#define PWM_Frequency       1000
#define PWM_OFF_DUTY_CYCLE  1000

LED_STA_T led_sta = {
    .on_off = FALSE,
    .regulate_mode = bright,
    .LED_Cool = LED_COOL,   //GPIOA_7 LED3
    .LED_Warm = LED_WARM,   //GPIOA_9 LED1
    .led_bright = LED_BRIGHT_MAX,
    .led_temperature = LED_TEMPERATURE_MIN    //0：全冷光
};


void app_light_init(void) 
{
    UCHAR_T pwm_gpio_list[] = {7, 9};
    UCHAR_T pwm_num = (SIZEOF(pwm_gpio_list) / SIZEOF(pwm_gpio_list[0]));
    
    tuya_gpio_inout_set(LED2, FALSE);
    LED_OFF(LED2);

    opSocPwmInit(PWM_Frequency, PWM_OFF_DUTY_CYCLE, pwm_num, pwm_gpio_list, TRUE);
}


void led_bright_set(INT32 bright) 
{
    UINT32 warm_bright;

    if (bright > LED_BRIGHT_MAX) {
        led_sta.led_bright = LED_BRIGHT_MAX;
    } else if (bright < LED_BRIGHT_MIN) {
        led_sta.led_bright = LED_BRIGHT_MIN;
    } else {
        led_sta.led_bright = bright;
    }

    
    if (led_sta.on_off == FALSE) {
        PR_NOTICE("light is off, return.");
        updata_led_bright_status();
        return;
    }

    warm_bright = (led_sta.led_bright * led_sta.led_temperature / LED_TEMPERATURE_MAX);
    
    vSocPwmSetDuty(led_sta.LED_Cool, (PWM_Frequency - (led_sta.led_bright - warm_bright)));
    vSocPwmSetDuty(led_sta.LED_Warm, (PWM_Frequency - warm_bright));
    updata_led_bright_status();
}

void led_temperature_set(INT32 temperature) 
{
    if (temperature > LED_TEMPERATURE_MAX) {
        led_sta.led_temperature = LED_TEMPERATURE_MAX;
    } else if (temperature < LED_TEMPERATURE_MIN) {
        led_sta.led_temperature = LED_TEMPERATURE_MIN;
    } else {
        led_sta.led_temperature = temperature;
    }

    if (led_sta.on_off == FALSE) {
        PR_NOTICE("light is off, return.");
        updata_led_temperature_status();
        return;
    }
    
    led_bright_set(led_sta.led_bright);
    updata_led_temperature_status();
}

void led_off(void) 
{
    led_sta.on_off = FALSE;
    vSocPwmSetDuty(led_sta.LED_Cool, PWM_OFF_DUTY_CYCLE);
    vSocPwmSetDuty(led_sta.LED_Warm, PWM_OFF_DUTY_CYCLE);
    
    updata_led_switch_status();
}

void led_on(void) 
{
    led_sta.on_off = TRUE;
    led_temperature_set(led_sta.led_temperature);
    
    updata_led_switch_status();
}

