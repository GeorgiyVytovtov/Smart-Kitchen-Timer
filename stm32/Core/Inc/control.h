#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include <buzzer.h>
#include "main.h"
#include "filter.h"
#include "mq2.h"
#include "display.h"
#include "timer.h"
#include "sht31.h"
#include "ds3231.h"

void display_update();
void alarm_control();
void timer_control();
void get_sensor_data();
void read_time();

#endif
