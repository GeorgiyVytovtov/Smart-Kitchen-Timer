#include <buzzer.h>
#include <timer.h>
#include "buttons.h"
#include "ds3231.h"
#include "fee.h"
#include "display.h"

#define MAX_TIMER_TIME	43200

static bool is_started;
static bool is_service_mode;
static bool is_SETTINGS_CURRENT_TIME;
static uint32_t timer_time;
static uint32_t start_time;
static TIM_HandleTypeDef* htim2;
static ModeTimer current_mode = STOP;

void timer_init(TIM_HandleTypeDef* htim)
{
	htim2=htim;
	fee_read_data(FEE_START_ADDRESS, &timer_time, sizeof(timer_time));
	if (timer_time == 0xFFFFFFFF) {
		timer_time = 600;
	}
	start_time=timer_time;
	set_timer_time(timer_time);
}

ModeTimer timer_start_stop() {
	StateStartButton start_stop = read_start_stop_button();

	if (start_stop == CLICK) {
		if (!is_started && !is_service_mode && !is_SETTINGS_CURRENT_TIME) {
			start_time = timer_time;
			HAL_TIM_Base_Start_IT(htim2);
			is_started = true;
			current_mode = START;
		} else {
			HAL_TIM_Base_Stop_IT(htim2);
			is_started = false;

			if (is_service_mode) {
				is_service_mode = false;
				start_time = timer_time;
				fee_write_data(FEE_START_ADDRESS, &timer_time,
						sizeof(timer_time));
			} else if (is_SETTINGS_CURRENT_TIME) {
				is_SETTINGS_CURRENT_TIME = false;
			}
			current_mode = STOP;
		}
	}

	else if (start_stop == HOLD && is_started == false) {
		is_SETTINGS_CURRENT_TIME = true;
		current_mode = SETTINGS_CURRENT_TIME;
	}

	else if (start_stop == DOUBLE_CLICK && is_started == false) {
		start_time = timer_time;
		is_service_mode = true;

		current_mode = SETTINGS_TIMER;
	}

	return current_mode;
}


void timer_reset() {
	if (read_reset_button() && !is_started) {
		HAL_TIM_Base_Stop_IT(htim2);
		timer_time = start_time;
		buzzer_reset();
	}
}

void timer_settings(uint8_t *pos) {
	uint32_t increments[] = {1, 60, 3600};
	if (read_up_button() && timer_time + increments[*pos]<=MAX_TIMER_TIME) {
		timer_time = timer_time + increments[*pos];
	} else if (read_down_button() && increments[*pos]<=timer_time) {
		timer_time = timer_time - increments[*pos];
	} else if (read_left_button()) {
		*pos = (*pos == 2) ? 0 : *pos + 1;
	} else if (read_right_button()) {
		*pos = (*pos == 0) ? 2 : *pos - 1;
	}
}

void settings_current_time(uint8_t *cr_pos, uint8_t *hr, uint8_t *mn) {
	if (read_up_button()) {
		if (*cr_pos) {
			*hr = (*hr + 1) % 24;
		} else {
			*mn = (*mn + 1) % 60;
		}
	} else if (read_down_button()) {
		if (*cr_pos && *hr - 1 >= 0) {
			*hr = *hr - 1;
		} else if (!(*cr_pos) && *mn - 1 >= 0) {
			*mn = *mn - 1;
		}
	} else if (read_left_button()) {
		*cr_pos = (*cr_pos == 1) ? 0 : *cr_pos + 1;
	} else if (read_right_button()) {
		*cr_pos = (*cr_pos == 0) ? 1 : *cr_pos - 1;
	}
}

uint32_t get_timer_time()
{
	return timer_time;
}

void set_timer_time(uint32_t time)
{
	timer_time=time;
}

void TIM2_IRQHandler(void) {
	/* USER CODE BEGIN TIM2_IRQn 0 */

	/* USER CODE END TIM2_IRQn 0 */
	HAL_TIM_IRQHandler(htim2);
	if (timer_time > 0) {
		timer_time--;
	} else {
		HAL_TIM_Base_Stop_IT(htim2);
		buzzer_start(3000, SINGL);
		is_started = false;
		timer_time=start_time;
	}
	/* USER CODE BEGIN TIM2_IRQn 1 */

	/* USER CODE END TIM2_IRQn 1 */
}



