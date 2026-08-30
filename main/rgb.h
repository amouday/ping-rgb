#ifndef		_RGB_H
#define		_RGB_H

#include <stdint.h>
#include "hal/ledc_types.h"

enum {
	RGB_RED,
	RGB_BLUE,
	RGB_GREEN,
	RGB_MAX,
};

typedef struct rgb_led {
	int				gpio;
	ledc_channel_t	chan;
	ledc_mode_t		speed_mode;
	uint32_t		duty;
} rgb_led_t;

void rgb_init();

#endif
