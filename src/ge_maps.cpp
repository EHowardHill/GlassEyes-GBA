// ge_maps.cpp

#include "ge_maps.h"
#include "ge_sprites.h"

#include "bn_regular_bg_ptr.h"

map_manager::map_manager(const regular_bg_item *item_, const map *current_map_)
    : current_map(current_map_)
{
    floor_ptr = item_->create_bg(0, 0);
    collider_ptr = current_map_->bg_item_ptr->create_bg(0, 0);
}

void map_manager::update()
{
    vector_2 * cam = &v_sprite_ptr::camera;

    floor_ptr.value().set_position(-cam->x, -cam->y);
    collider_ptr.value().set_position(-cam->x, -cam->y);
}