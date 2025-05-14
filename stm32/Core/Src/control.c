#include "control.h"

static SensorValue temperature = { true, 0, 0 }, humidity = { true, 0, 0 },
		mq2_val = { true, 0, 0 };
static int (*gaz_filter)(SensorValue *val) = filter_ema;
static int (*temperature_filter)(SensorValue *val) = filter_ema;
static int (*humidity_filter)(SensorValue *val) = filter_ema;
static uint8_t cr_pos, pos, hr, mn;
static ModeTimer current_mode_timer, previos_mode_timer;

void read_time() {
	static uint32_t last_read_time = 0;
	if ((HAL_GetTick() - last_read_time > 10000)
			&& current_mode_timer != SETTINGS_CURRENT_TIME) {
		ds3231_read_time(&hr, &mn);
		last_read_time = HAL_GetTick();
	}
}

void get_sensor_data() {
	sht31_start_mesurement();
	if (SHT31_read_temp_humidity(&temperature.current_value,
			&humidity.current_value)) {
		temperature_filter(&temperature);
		humidity_filter(&humidity);
	}
}

void timer_control() {
	current_mode_timer = timer_start_stop();
	if(previos_mode_timer!=current_mode_timer && previos_mode_timer==SETTINGS_CURRENT_TIME)
	{
		ds3231_set_time(hr, mn);
	}
	previos_mode_timer=current_mode_timer;
	if (current_mode_timer == SETTINGS_TIMER) {
		timer_settings(&pos);
	} else if (current_mode_timer == SETTINGS_CURRENT_TIME) {
		settings_current_time(&cr_pos, &hr, &mn);
	}
	timer_reset();
	buzzer_update();
}

void alarm_control() {
	mq2_start_measurement();
	if (is_ready_data()) {
		mq2_val.current_value = mq2_get_data();
		gaz_filter(&mq2_val);
		if (mq2_is_alarm(mq2_val)) {
			buzzer_start(10000, TRIGGER);
		}
	}
}

void display_update() {
	static uint32_t last_display_update = 0;
	if (HAL_GetTick() - last_display_update > 100) {
		uint32_t time = get_timer_time();
		if (current_mode_timer == SETTINGS_TIMER) {
			view_blink_timer_time(pos, time);
		} else {
			view_timer_time(time);
		}
		if (current_mode_timer == SETTINGS_CURRENT_TIME) {
			view_blink_current_time(cr_pos, hr, mn);
		} else {
			view_current_time(hr, mn);
		}
		view_temperature(temperature.filt_value, humidity.filt_value);
		last_display_update = HAL_GetTick();
	}
}
