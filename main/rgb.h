#ifndef		_RGB_H
#define		_RGB_H

#include <stdint.h>
#include "hal/ledc_types.h"

typedef enum rgb {
	RGB_RED,
	RGB_BLUE,
	RGB_GREEN,
	RGB_MAX,
} rgb_t;

typedef struct rgb_led {
	int				gpio;
	ledc_channel_t	chan;
	ledc_mode_t		speed_mode;
	uint32_t		duty;
} rgb_led_t;

typedef struct color {
	uint32_t	red;
	uint32_t	green;
	uint32_t	blue;
} color_t;

void rgb_init();
void rgb_set_color(color_t);
int rgb_set(rgb_t, unsigned int);
int rgb_set_raw(rgb_t, uint32_t);

#endif
