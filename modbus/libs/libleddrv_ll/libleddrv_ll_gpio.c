/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    10.11.2014
 */

#include "libleddrv_ll.h"

#include <assert.h>
#include <stdbool.h>
#include <util/array.h>

#include <drivers/gpio/gpio_driver.h>

#include <libleddrv.h>

struct leddrv_pin_desc {
	int gpio; /**< port */
	int pin; /**< pin mask */
};

static const struct leddrv_pin_desc leds[] = {
	#include <leds_config.inc>
};


void leddrv_ll_init(void) {
	int i;
	for (i = 0; i < LEDDRV_LED_N; i++) {
		gpio_setup_mode(leds[i].gpio, leds[i].pin, GPIO_MODE_OUT);
	}
}

void leddrv_ll_update(unsigned char leds_state[]) {
	int i;

	for (i = 0; i < LEDDRV_LED_N; i++) {
		gpio_set(leds[i].gpio, leds[i].pin,
				leds_state[i] ? GPIO_PIN_HIGH : GPIO_PIN_LOW);
	}
}

int leddrv_ll_error(int n) {
	return 0;
}

void leddrv_load_state(unsigned char leds_state[]) {
}
