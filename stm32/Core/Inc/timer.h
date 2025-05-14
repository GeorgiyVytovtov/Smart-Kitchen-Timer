#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

typedef enum
{
	START=0,
	STOP,
	SETTINGS_TIMER,
	SETTINGS_CURRENT_TIME
}ModeTimer;

void timer_init(TIM_HandleTypeDef* htim2);
void timer_settings(uint8_t *pos);
ModeTimer timer_start_stop();
void timer_reset();
uint32_t get_timer_time();
void set_timer_time(uint32_t time);
void settings_current_time(uint8_t *cr_pos, uint8_t* hr, uint8_t* mn);

#endif
