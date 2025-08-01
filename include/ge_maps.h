// ge_maps.h

#ifndef GE_MAPS_H
#define GE_MAPS_H

#include "bn_regular_bg_item.h"
#include "bn_sound_item.h"
#include "bn_array.h"
#include "ge_structs.h"

// Using bn namespace types
using bn::regular_bg_item;
using bn::sound_item;
using bn::array;

struct map
{
    vector_2 size;
    array<int, 2048> collisions;
    array<array<int, 2048>, 2048> actions;  // 2D array properly declared
    const regular_bg_item* bg_item_ptr;     // Should be const for constexpr

    // Constexpr constructor for compile-time initialization
    constexpr map(vector_2 size_,
                  const array<int, 2048>& collisions_,
                  const array<array<int, 2048>, 2048>& actions_,
                  const regular_bg_item* bg_item_ptr_)
        : size(size_), 
          collisions(collisions_), 
          actions(actions_), 
          bg_item_ptr(bg_item_ptr_) {}
};

#endif