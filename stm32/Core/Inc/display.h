#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"
#include "lcd1602_i2c.h"
#include "filter.h"
#include "timer.h"


void view_temperature(int temp, uint8_t wear);
void view_current_time(uint8_t hr, uint8_t mn);
void view_timer_time(uint32_t time);
void view_blink_timer_time(uint8_t sigment, uint16_t time);
void view_blink_current_time(uint8_t sigment, uint8_t hr, uint8_t mn);

#endif
