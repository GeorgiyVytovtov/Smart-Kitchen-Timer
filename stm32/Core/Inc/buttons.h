#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "main.h"

typedef enum
{
	NONE_CLICK=0,
	CLICK,
	DOUBLE_CLICK,
	HOLD
}StateStartButton;


uint8_t read_start_stop_button();
bool read_reset_button();
bool read_up_button();
bool read_down_button();
bool read_left_button();
bool read_right_button();

#endif
