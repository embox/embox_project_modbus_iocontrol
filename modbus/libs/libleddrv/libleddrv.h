/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    05.11.2014
 */

#ifndef LIBLEDDRV_H_
#define LIBLEDDRV_H_

#ifdef __EMBOX__
#include <framework/mod/options.h>
#include <module/iocontrol/modbus/lib/libleddrv.h>
#define LEDDRV_LED_N OPTION_MODULE_GET(iocontrol__modbus__lib__libleddrv,NUMBER,leds_quantity)
#else
#define LEDDRV_LED_N 80
#endif

extern int leddrv_init(void);

extern int leddrv_set(unsigned int led_n);
extern int leddrv_clr(unsigned int led_n);

extern int leddrv_getstates(unsigned char leds_state[LEDDRV_LED_N]);
extern int leddrv_updatestates(unsigned char new_leds_state[LEDDRV_LED_N]);

extern int leddrv_geterrors(unsigned char leds_error[LEDDRV_LED_N]);

#endif /* LIBLEDDRV_H_ */
