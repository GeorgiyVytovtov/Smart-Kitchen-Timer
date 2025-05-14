/*
 * buttons.c
 *
 *  Created on: Apr 20, 2025
 *      Author: Komp
 */

#include "buttons.h"

#define PRESS_TIME			50
#define DOUBLE_CLICK_TIME	400
#define HOLD_TIME			5000

static bool flag_up = false;
static uint32_t T_up = 0;
static bool flag_down = false;
static uint32_t T_down = 0;
static bool flag_left = false;
static uint32_t T_left = 0;
static bool flag_right = false;
static uint32_t T_right = 0;
static bool flag_start_stop = false;
static uint32_t T_start_stop = 0;
static bool flag_reset = false;
bool hold_triggered = false;
static uint32_t T_reset = 0;

static uint8_t click_count=0;
static uint32_t last_click_time=0;
static StateStartButton pending_state;

StateStartButton read_start_stop_button() {
	uint8_t button_state = HAL_GPIO_ReadPin(GPIOB, START_STOP_Pin);

	if (button_state == GPIO_PIN_RESET && !flag_start_stop && !hold_triggered) {
		flag_start_stop = true;
		T_start_stop = HAL_GetTick();
	}

	else if (button_state == GPIO_PIN_SET && flag_start_stop) {
		uint32_t time_press = HAL_GetTick() - T_start_stop;
		flag_start_stop = false;
		hold_triggered = false;

		if (time_press > PRESS_TIME && time_press < HOLD_TIME) {
			click_count++;
			last_click_time = HAL_GetTick();
			if (click_count >= 2) {
				click_count = 0;
				pending_state = NONE_CLICK;
				return DOUBLE_CLICK;
			}

			pending_state = CLICK;
		}
	}

	else if (button_state == GPIO_PIN_RESET && flag_start_stop
			&& !hold_triggered) {
		uint32_t time_press = HAL_GetTick() - T_start_stop;
		if (time_press >= HOLD_TIME) {
			hold_triggered = true;
			pending_state = NONE_CLICK;
			return HOLD;
		}
	}

	if (pending_state == CLICK
			&& HAL_GetTick() - last_click_time > DOUBLE_CLICK_TIME) {
		pending_state = NONE_CLICK;
		click_count = 0;
		return CLICK;
	}

	return NONE_CLICK;
}

bool read_reset_button() {
	if ((HAL_GPIO_ReadPin(GPIOB, RESET_Pin) == 0) && !flag_reset
			&& (HAL_GetTick() - T_reset) > PRESS_TIME) {
		flag_reset = true;
		T_reset = HAL_GetTick();
		return true;
	} else if ((HAL_GPIO_ReadPin(GPIOB, RESET_Pin) == 1) && flag_reset
			&& (HAL_GetTick() - T_reset) > PRESS_TIME) {
		flag_reset = false;
		T_reset = HAL_GetTick();
		return false;
	}
	return false;
}

bool read_up_button() {
	if ((HAL_GPIO_ReadPin(GPIOB, UP_Pin) == 0) && !flag_up
			&& (HAL_GetTick() - T_up) > PRESS_TIME) {
		flag_up = true;
		T_up = HAL_GetTick();
		return true;
	} else if ((HAL_GPIO_ReadPin(GPIOB, UP_Pin) == 1) && flag_up
			&& (HAL_GetTick() - T_up) > PRESS_TIME) {
		flag_up = false;
		T_up = HAL_GetTick();
		return false;
	}
	return false;
}

bool read_down_button() {
	if ((HAL_GPIO_ReadPin(GPIOB, DOWN_Pin) == 0) && !flag_down
			&& (HAL_GetTick() - T_down) > PRESS_TIME) {
		flag_down = true;
		T_down = HAL_GetTick();
		return true;
	} else if ((HAL_GPIO_ReadPin(GPIOB, DOWN_Pin) == 1) && flag_down
			&& (HAL_GetTick() - T_down) > PRESS_TIME) {
		flag_down = false;
		T_down = HAL_GetTick();
		return false;
	}
	return false;
}

bool read_left_button() {
	if ((HAL_GPIO_ReadPin(GPIOB, LEFT_Pin) == 0) && !flag_left
			&& (HAL_GetTick() - T_left) > PRESS_TIME) {
		flag_left = true;
		T_left = HAL_GetTick();
		return true;
	} else if ((HAL_GPIO_ReadPin(GPIOB, LEFT_Pin) == 1) && flag_left
			&& (HAL_GetTick() - T_left) > PRESS_TIME) {
		flag_left = false;
		T_left = HAL_GetTick();
		return false;
	}
	return false;
}

bool read_right_button() {
	if ((HAL_GPIO_ReadPin(GPIOB, RIGHT_Pin) == 0) && !flag_right
			&& (HAL_GetTick() - T_right) > PRESS_TIME) {
		flag_right = true;
		T_right = HAL_GetTick();
		return true;
	} else if ((HAL_GPIO_ReadPin(GPIOB, RIGHT_Pin) == 1) && flag_right
			&& (HAL_GetTick() - T_right) > PRESS_TIME) {
		flag_right = false;
		T_right = HAL_GetTick();
		return false;
	}
	return false;
}

