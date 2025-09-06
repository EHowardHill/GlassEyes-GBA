#include "ge_globals.h"
#include "ge_maps.h"
#include "ge_character_manager.h"

int action_listener(map_manager *man, character_manager *ch_man);

constexpr int is_interactive_array[999] = {
    CONVO_GARBAGE_SIGN01,
    CONVO_VISKER_SIGN,
    CONVO_GARBAGE_03_SIGN,
    CONVO_VISKER_AFTER_FIGHT,
    CONVO_GARBAGE_04_SIGN,
    CONVO_GARBAGE_05_SIGN,
    LAB_COMPUTER,
    CONVO_FOREST_01_SIGN,
    CONVO_FOREST_02_SIGN,
    CONVO_FOREST_03_SIGN,
    CONVO_COWBEMMIE_01_SIGN,
    TEMMIE_01,
    TEMMIE_02,
    TEMMIE_03,
    TEMMIE_04,
    TEMMIE_05,
    TEMMIE_06,
    TEMMIE_07,
    CONVO_CAVE_02_SIGN,
    CAVE_04_SIGN,
    -1};