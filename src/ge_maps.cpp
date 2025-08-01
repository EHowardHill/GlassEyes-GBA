// ge_maps.cpp

#include "ge_maps.h"
#include "bn_regular_bg_items_room_cabin_01.h"

// Create empty actions array (all zeros)
constexpr array<array<int, 2048>, 2048> empty_actions = {};

// Define the map with proper initialization
constexpr map map_room_01 = {
    {8, 8},
    {0, 0, 0, 0, 0, 0, 0, 0},
    empty_actions,
    &bn::regular_bg_items::room_cabin_01};