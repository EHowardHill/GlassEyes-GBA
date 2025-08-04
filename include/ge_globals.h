// ge_globals.h

#ifndef GE_GLOBALS_H
#define GE_GLOBALS_H

#include "bn_random.h"

#include "ge_maps.h"

enum action_list
{
	ACT_NONE,
	CONVO_SCENE_1_INIT,
	ACTIONS_SIZE
};

enum response_list
{
	NONE,
	RESPONSES_SIZE
};

struct global_data
{
	bn::random bn_random;
	int action_iterations[ACTIONS_SIZE] = {0};

	global_data()
	{
	}
};

// Declaration only - use 'extern' keyword
extern global_data *global_data_ptr;

#endif // GE_GLOBALS_H