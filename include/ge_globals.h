// ge_globals.h

#ifndef GE_GLOBALS_H
#define GE_GLOBALS_H

#include "bn_music_item.h"
#include "bn_random.h"
#include "ge_maps.h"
#include "ge_sprites.h"

using namespace bn;

enum action_list
{
	ACT_NONE,
	CONVO_SCENE_1,
	CONVO_GARBAGE_1,
	CONVO_GARBAGE_2,
	CONVO_GARBAGE_3,
	TO_GARBAGE_02,
	CONVO_GARBAGE_SIGN01,
	TO_VISKERS_HOUSE,
	TO_GARBAGE_03,
	TO_GARBAGE_02b,
	CONVO_VISKER_SIGN,
	CONVO_VISKER_BED,
	CONVO_VISKER_NIGHTSTAND,
	CONVO_VISKER_FOOD,
	CONVO_GARBAGE_03_SIGN,
	CONVO_VISKER_FIGHT,
	CONVO_VISKER_AFTER_FIGHT,
	TO_GARBAGE_04,
	GARBAGE_04_EXIT_01,
	GARBAGE_04_EXIT_02,
	GARBAGE_04_EXIT_03,
	GARBAGE_04_EXIT_04,
	GARBAGE_04_EXIT_05,
	CONVO_GARBAGE_04_SIGN,
	EXIT_GARBAGE_01,
	CONVO_GARBAGE_05_SIGN,
	CONVO_GARBAGE_FINAL_01,
	ACTIONS_SIZE
};

enum response_list
{
	NONE,
	RESPONSES_SIZE
};

struct global_data
{
	random bn_random;
	int action_iterations[ACTIONS_SIZE] = {0};
	const map *entry_map = nullptr;
	vector_2 entry_position = {0, 0};
	vector_2 entry_position_raw = {0, 0};
	int entry_direction = DIR_RIGHT;
	int battle_foe = 0;
	int hp[4] = {20, 20, 20, 20};
	int max_hp[4] = {20, 20, 20, 20};
	int enemy_hp[4] = {20, 20, 20, 20};
	int enemy_max_hp[4] = {20, 20, 20, 20};
	bool progress_cue = false;
	bool process_stage = 0;

	const music_item *bg_track;
};

// Declaration only - use 'extern' keyword
extern global_data *global_data_ptr;

#endif // GE_GLOBALS_H