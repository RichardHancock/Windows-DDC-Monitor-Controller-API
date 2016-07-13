#pragma once

struct ContinuousSetting
{
	unsigned long min;
	unsigned long max;
	unsigned long current;

	//Whether this data is valid (The Monitor get function was successful)
	bool valid;
};