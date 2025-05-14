#include <buzzer.h>

#define BUZZER_PWM_OFF 				0
#define SINGL_FRIQUANSY				600
#define TRIG_START_SINGL_FRIQUANSY	300
#define TRIG_TIME_PAUSE				100

static bool is_start_timer_time;
static uint32_t buzzer_start_time = 0;
static uint32_t buzzer_trigg_time = 0;
static uint32_t buzzer_work_time = 0;
static TypeSignal currentSignal;
static TIM_HandleTypeDef* htim1;

void buzzer_init(TIM_HandleTypeDef* htim)
{
	htim1=htim;
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, BUZZER_PWM_OFF);
}

void buzzer_start(uint32_t time_signal, TypeSignal typeSignal) {
	currentSignal = typeSignal;
	is_start_timer_time = true;
	buzzer_work_time = time_signal;
	buzzer_start_time = HAL_GetTick();
	buzzer_trigg_time = HAL_GetTick();
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, SINGL_FRIQUANSY);
}

void buzzer_update() {
	switch(currentSignal)
	{
	case SINGL:
		if (is_start_timer_time
				&& HAL_GetTick() - buzzer_start_time > buzzer_work_time) {
			buzzer_reset();
		}
		break;
	case TRIGGER:
		static uint8_t i = 1;
		if(HAL_GetTick()-buzzer_trigg_time>TRIG_TIME_PAUSE)
		{
			__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, TRIG_START_SINGL_FRIQUANSY*i);
			i = (i==3) ? 0 : i+1;
			buzzer_trigg_time= HAL_GetTick();
		}
		break;
	case NONE:
		break;
	}
}

void buzzer_reset() {
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, BUZZER_PWM_OFF);
	is_start_timer_time = false;
	currentSignal = NONE;
}

