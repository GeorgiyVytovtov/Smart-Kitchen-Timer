#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"

typedef enum {
	NONE = 0,
	SINGL,
	TRIGGER
} TypeSignal;

void buzzer_init(TIM_HandleTypeDef* htim4);
void buzzer_start(uint32_t time_signal, TypeSignal typeSignal);
void buzzer_update();
void buzzer_reset();

#endif
