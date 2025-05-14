#include "mq2.h"

#define ALARM_VALUE	1200

static ADC_HandleTypeDef *hadc1;
static uint32_t adc_val;
static volatile bool mq2_data_ready = false;

void mq2_init(ADC_HandleTypeDef *hadc) {
	hadc1 = hadc;
	HAL_ADCEx_Calibration_Start(hadc1);
}

void mq2_start_measurement() {
	if ((HAL_ADC_GetState(hadc1) & HAL_ADC_STATE_BUSY_REG) != HAL_ADC_STATE_BUSY_REG) {
		HAL_ADC_Start_DMA(hadc1, &adc_val, 1);
	}
}

bool is_ready_data()
{
	return mq2_data_ready;
}

uint32_t mq2_get_data() {
	mq2_data_ready = false;
	return adc_val;
}

bool mq2_is_alarm(SensorValue val) {
	if (val.filt_value < ALARM_VALUE) {
		return true;
	}
	return false;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc == hadc1) {
        mq2_data_ready = true;
    }
}

