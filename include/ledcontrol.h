#ifndef LEDCONTROL_H
#define LEDCONTROL_H

extern void init_leds_task();
extern void set_led_state(int led, char* led_state, int delay);

#endif