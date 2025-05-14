#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include "main.h"

typedef struct
{
	bool is_first_value;
	int current_value;
	int filt_value;
} SensorValue;


int filter_ema(SensorValue* val);

#endif
