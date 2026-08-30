#ifndef		__RGB_CONFIG_H
#define		__RGB_CONFIG_H

#include "driver/ledc.h"
#include "hal/ledc_types.h"

#define RGB_GPIO_RED			9
#define RGB_GPIO_GREEN			8
#define RGB_GPIO_BLUE			7

#define RGB_PWM_CHAN_RED	0
#define RGB_PWM_CHAN_GREEN	1
#define RGB_PWM_CHAN_BLUE	2

#define RGB_SPEED_MODE			LEDC_LOW_SPEED_MODE
#define RGB_DUTY_RESOLUTION		LEDC_TIMER_14_BIT
#define RGB_TIMER				LEDC_TIMER_0
#define RGB_PWM_FREQ			1000
#define RGB_CLK_CONFIG			LEDC_USE_RC_FAST_CLK

#endif
