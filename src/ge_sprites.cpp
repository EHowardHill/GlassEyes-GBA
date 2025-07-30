#include "ge_sprites.h"

using namespace bn;

// Static member definitions
vector<v_sprite_ptr *, 12> v_sprite_ptr::manager;
vector_2 v_sprite_ptr::camera = {0, 0};

bool within_bounds(bound me, bound you)
{
    int me_x = me.position.x.integer();
    int me_y = me.position.y.integer();
    int you_x = you.position.x.integer();
    int you_y = you.position.y.integer();
    int width = you.width.integer() / 2;
    int height = you.height.integer() / 2;

    return (me_x > you_x - width && me_x < you_x + width) && (me_y > you_y - height && me_y < you_y + height);
}

v_sprite_ptr::v_sprite_ptr(const sprite_item *sprite_item_ptr_, vector_2 position, int width, int height, int frame_)
    : sprite_item_ptr(sprite_item_ptr_), frame(frame_)
{
    bounds.position = position;
    bounds.width = width;
    bounds.height = height;
    manager.push_back(this);
}

v_sprite_ptr::~v_sprite_ptr()
{
    // Remove this instance from the manager
    for (auto it = manager.begin(); it != manager.end(); ++it)
    {
        if (*it == this)
        {
            manager.erase(it);
            break;
        }
    }
}

void v_sprite_ptr::move(vector_2 direction)
{
    bounds.position.x = bounds.position.x - direction.x;
    bounds.position.y = bounds.position.y - direction.y;
}

bound v_sprite_ptr::real_position() const
{
    return {
        {bounds.position.x - camera.x, bounds.position.y - camera.y},
        bounds.width,
        bounds.height};
}

void v_sprite_ptr::update()
{
    for (auto *item : manager)
    {
        bound bounds = item->real_position();

        if (item->sprite_ptr_raw.has_value())
        {
            if (!within_bounds(bounds, screen))
            {
                item->sprite_ptr_raw.reset();
            }
            else
            {
                item->sprite_ptr_raw.value().set_position(bounds.position.x, bounds.position.y);
                item->sprite_ptr_raw.value().set_tiles(item->sprite_item_ptr->tiles_item(), item->frame);
            }
        }
        else if (within_bounds(bounds, screen))
        {
            item->sprite_ptr_raw = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y, item->frame);
        }
    }
}