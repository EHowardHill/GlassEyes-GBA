// ge_maps.h

#ifndef GE_MAPS_H
#define GE_MAPS_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_sound_item.h"
#include "bn_array.h"
#include "bn_optional.h"

#include "ge_structs.h"

// Floors
#include "bn_regular_bg_items_floor_wood01.h"

// Using bn namespace types
using bn::array;
using bn::regular_bg_item;
using bn::sound_item;

struct map
{
  vector_2 size;
  vector_2 raw_size;
  array<int, 2048> collisions;
  array<array<int, 2048>, 2048> actions; // 2D array properly declared
  array<int, 2048> characters;
  const regular_bg_item *bg_item_ptr; // Should be const for constexpr

  // Constexpr constructor for compile-time initialization
  constexpr map(vector_2 size_,
                vector_2 raw_size_,
                const array<int, 2048> &collisions_,
                const array<array<int, 2048>, 2048> &actions_,
                const array<int, 2048> &characters_,
                const regular_bg_item *bg_item_ptr_)
      : size(size_),
        raw_size(raw_size_),
        collisions(collisions_),
        actions(actions_),
        characters(characters_),
        bg_item_ptr(bg_item_ptr_) {}
};

struct map_manager
{
  optional<regular_bg_ptr> floor_ptr;
  optional<regular_bg_ptr> collider_ptr;
  const map *current_map;

  map_manager(const regular_bg_item *item_, const map *current_map_);

  void update();
  int collision(vector_2 location);
  bool check_box_collision(bound box);
  int action(vector_2 location);
};

#endif