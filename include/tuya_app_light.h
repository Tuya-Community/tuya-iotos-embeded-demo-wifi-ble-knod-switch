#ifndef __TUYA_APP_LIGHT_H__
#define __TUYA_APP_LIGHT_H__

#include "tuya_gpio.h"
#include "tuya_cloud_types.h"

//LED 亮度最大最小宏定义
#define LED_BRIGHT_MIN        10
#define LED_BRIGHT_MAX        1000

#define LED_TEMPERATURE_MIN   0
#define LED_TEMPERATURE_MAX   1000

#define LED1    TY_GPIOA_9
#define LED2    TY_GPIOA_0
#define LED3    TY_GPIOA_7

//控制 LED 的 PWM 编号
#define LED_COOL    0  //GPIO7
#define LED_WARM    1  //GPIO9

#define LED_ON(LED)     tuya_gpio_write(LED, FALSE);
#define LED_OFF(LED)    tuya_gpio_write(LED, TRUE);

//切换 调整亮度，温度 的枚举
typedef enum {
    bright = 0,
    temperature
}LED_REGULATE_MODE_E;

typedef struct {
    BOOL_T on_off;
    LED_REGULATE_MODE_E regulate_mode;
    uint8 LED_Cool;
    uint8 LED_Warm;

    UINT32 led_bright;
    UINT32 led_temperature;

}LED_STA_T;

extern LED_STA_T led_sta;


void app_light_init(void);

void led_bright_set(INT32 bright);
void led_temperature_set(INT32 temperature);
void led_off(void);
void led_on(void);


#endif

