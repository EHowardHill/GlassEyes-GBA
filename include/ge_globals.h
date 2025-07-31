// ge_globals.h

#ifndef GE_GLOBALS_H
#define GE_GLOBALS_H

#include "bn_random.h"

class global_data
{
public:
	bn::random bn_random;

	global_data()
	{
	}
};

// Declaration only - use 'extern' keyword
extern global_data *global_data_ptr;

#endif // GE_GLOBALS_H