#ifndef INC_MQ2_H_
#define INC_MQ2_H_

#include "main.h"
#include "filter.h"

void mq2_init(ADC_HandleTypeDef *hadc1);
void mq2_start_measurement();
bool is_ready_data();
uint32_t mq2_get_data();
bool mq2_is_alarm(SensorValue val);

#endif
