// ge_maps.cpp

#include "bn_log.h"
#include "bn_regular_bg_ptr.h"

#include "ge_maps.h"
#include "ge_sprites.h"
#include "ge_character_manager.h"

map_manager::map_manager(const regular_bg_item *item_, const map *current_map_)
    : current_map(current_map_)
{
    if (item_ != nullptr)
        floor_ptr = item_->create_bg(0, 0);
    collider_ptr = current_map_->bg_item_ptr->create_bg(0, 0);
}

void map_manager::update()
{
    vector_2 *cam = &v_sprite_ptr::camera;

    vector_2 offset = {
        current_map->raw_size.x / 2,
        current_map->raw_size.y / 2};

    if (floor_ptr.has_value())
        floor_ptr.value().set_position(-cam->x, -cam->y);
    collider_ptr.value().set_position(-cam->x + offset.x, -cam->y + offset.y);
}

int map_manager::collision(vector_2 location)
{
    // Get the tile coordinates - no arbitrary offsets
    // We need to check multiple points for the character's bounding box
    int tile_x = location.x.integer() / 32;
    int tile_y = location.y.integer() / 32;

    // Clamp to map bounds
    if (tile_x < 0 || tile_x >= current_map->size.x.integer() ||
        tile_y < 0 || tile_y >= current_map->size.y.integer())
    {
        return 1; // Out of bounds is considered a collision
    }

    int tile_index = tile_x + (tile_y * current_map->size.x.integer());
    return current_map->collisions[tile_index];
}

int map_manager::action(vector_2 location)
{
    // Get the tile coordinates
    int tile_x = location.x.integer() / 32;
    int tile_y = location.y.integer() / 32;

    // Clamp to map bounds
    if (tile_x < 0 || tile_x >= current_map->size.x.integer() ||
        tile_y < 0 || tile_y >= current_map->size.y.integer())
    {
        return 0; // Out of bounds has no action
    }

    int tile_index = tile_x + (tile_y * current_map->size.x.integer());
    return current_map->actions[tile_index];
}

bool map_manager::check_box_collision(bound box)
{
    // Check all four corners of the bounding box
    int left = (box.position.x.integer() - box.width.integer() / 2);
    int right = (box.position.x.integer() + box.width.integer() / 2 - 1);
    int top = (box.position.y.integer() - box.height.integer() / 2);
    int bottom = (box.position.y.integer() + box.height.integer() / 2 - 1);

    // Check corners
    if (collision({left, top}) > 0)
        return true;
    if (collision({right, top}) > 0)
        return true;
    if (collision({left, bottom}) > 0)
        return true;
    if (collision({right, bottom}) > 0)
        return true;

    // For wider sprites, also check middle points
    if (box.width.integer() > 32)
    {
        int mid_x = box.position.x.integer();
        if (collision({mid_x, top}) > 0)
            return true;
        if (collision({mid_x, bottom}) > 0)
            return true;
    }

    if (box.height.integer() > 32)
    {
        int mid_y = box.position.y.integer();
        if (collision({left, mid_y}) > 0)
            return true;
        if (collision({right, mid_y}) > 0)
            return true;
    }

    return false;
}