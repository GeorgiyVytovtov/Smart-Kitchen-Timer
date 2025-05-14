#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include "main.h"

#define DS3231_ADDR 0x68<<1

void ds3231_init(I2C_HandleTypeDef* hi2c);
void ds3231_set_time(uint8_t hr, uint8_t mn);
void ds3231_read_time(uint8_t* hr, uint8_t* mn);

#endif
