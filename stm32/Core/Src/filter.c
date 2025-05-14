#include "filter.h"


int filter_ema(SensorValue* val)
{
	const float k=0.3;
	if(!val->is_first_value)
	{
		val->filt_value+=(val->current_value-val->filt_value)*k;
	}
	else
	{
		val->filt_value=val->current_value;
		val->is_first_value=false;
	}
	return val->filt_value;
}

