#ifndef __TUYA_APP_KEY_H__
#define __TUYA_APP_KEY_H__

#include "tuya_gpio.h"

#ifndef TRUE
#define TRUE                      1
#endif

#ifndef FALSE
#define FALSE                     0
#endif

/*  */
#define ROTARY_N_PORT   TY_GPIOA_26
#define ROTARY_A_PORT   TY_GPIOA_24
#define ROTARY_B_PORT   TY_GPIOA_6

#define ROTARY_A_LEVEL tuya_gpio_read(ROTARY_A_PORT)
#define ROTARY_B_LEVEL tuya_gpio_read(ROTARY_B_PORT)

void app_key_init(void);


#endif
