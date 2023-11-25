/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    10.11.2014
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libleddrv.h>

#include "libleddrv_ll.h"

#define LED_FILE_NAME "conf/leds.txt"

void leddrv_ll_init(void) {
}

void leddrv_ll_update(unsigned char leds_state[LEDDRV_LED_N]) {
	int i;
	int idx;
	char buff[LEDDRV_LED_N * 2];

	for (i = 0; i < LEDDRV_LED_N; i++) {
		char state = !!leds_state[i];
		fprintf(stderr, "led(%03d)=%d\n", i, state);
		buff[i * 2] = state + '0';
		buff[i * 2 + 1] = ',';
	}
	idx = open(LED_FILE_NAME, O_RDWR);
	if (idx < 0) {
		return;
	}

	write(idx, buff, (LEDDRV_LED_N * 2) - 1);

	close(idx);
}

int leddrv_ll_error(int n) {
	return 0;
}

void leddrv_load_state(unsigned char leds_state[]) {
	int i;
	int idx;
	char buff[LEDDRV_LED_N * 2];

	idx = open(LED_FILE_NAME, O_RDWR);
	if (idx < 0) {
		return;
	}
	read(idx, buff, (LEDDRV_LED_N * 2));
	close(idx);

	for (i = 0; i < LEDDRV_LED_N; i++) {
		leds_state[i] = buff[i * 2] - '0';
	}
}

