#include "rgb_config.h"
#include "rgb.h"

rgb_led_t rgb_led[RGB_MAX];

ledc_timer_config_t timer_config = {
	.speed_mode 		= RGB_SPEED_MODE,
	.duty_resolution 	= RGB_DUTY_RESOLUTION,
	.timer_num 			= RGB_TIMER,
	.freq_hz 			= RGB_PWM_FREQ, 
	.clk_cfg 			= RGB_CLK_CONFIG,
};

ledc_channel_config_t chan_config = {
	.speed_mode		= RGB_SPEED_MODE,
	.timer_sel		= RGB_TIMER,
};

void 
rgb_init()
{
	ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

	rgb_led[RGB_RED].gpio = RGB_GPIO_RED;
	rgb_led[RGB_RED].chan = RGB_PWM_CHAN_RED;
	rgb_led[RGB_RED].speed_mode = RGB_SPEED_MODE;

	rgb_led[RGB_GREEN].gpio = RGB_GPIO_GREEN;
	rgb_led[RGB_GREEN].chan = RGB_PWM_CHAN_GREEN;
	rgb_led[RGB_GREEN].speed_mode = RGB_SPEED_MODE;

	rgb_led[RGB_BLUE].gpio = RGB_GPIO_BLUE;
	rgb_led[RGB_BLUE].chan = RGB_PWM_CHAN_BLUE;
	rgb_led[RGB_BLUE].speed_mode = RGB_SPEED_MODE;

	for (int i = 0; i < RGB_MAX; i++) {
		chan_config.gpio_num = rgb_led[i].gpio;
		chan_config.channel = rgb_led[i].chan;
		ESP_ERROR_CHECK(ledc_channel_config(&chan_config));
	}
}

