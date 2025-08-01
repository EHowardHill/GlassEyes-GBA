#include "bn_log.h"
#include "bn_sprites.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

// Body Sprites
#include "bn_sprite_items_spr_vista_01.h"

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

animation::animation(array<int, 64> frames_, int speed_, bool loop_) : frames(frames_), speed(speed_), loop(loop_) {

                                                                       };

v_sprite_ptr::v_sprite_ptr()
    : sprite_item_ptr(nullptr), frame(0), tall(false)
{
    bounds.position = {0, 0};
    bounds.width = 0;
    bounds.height = 0;
    manager.push_back(this);
}

v_sprite_ptr::v_sprite_ptr(const sprite_item *sprite_item_ptr_, vector_2 position, int width, int height, int frame_, bool tall_)
    : sprite_item_ptr(sprite_item_ptr_), frame(frame_), tall(tall_)
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

void v_sprite_ptr::set_frame(int frame_)
{
    frame = frame_;

    if (sprite_ptr_raw.has_value())
    {
        if (tall)
        {
            // For tall sprites, top sprite uses frame * 2
            sprite_ptr_raw.value().set_tiles(sprite_item_ptr->tiles_item(), frame * 2);

            // Bottom sprite uses (frame * 2) + 1
            if (sprite_ptr_bottom.has_value())
            {
                sprite_ptr_bottom.value().set_tiles(sprite_item_ptr->tiles_item(), (frame * 2) + 1);
            }
        }
        else
        {
            // Regular sprites use frame directly
            sprite_ptr_raw.value().set_tiles(sprite_item_ptr->tiles_item(), frame);
        }
    }
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
                item->sprite_ptr_bottom.reset();
            }
            else
            {
                item->sprite_ptr_raw.value().set_position(bounds.position.x, bounds.position.y);

                if (item->tall)
                {
                    item->sprite_ptr_raw.value().set_tiles(item->sprite_item_ptr->tiles_item(), item->frame * 2);

                    if (item->sprite_ptr_bottom.has_value())
                    {
                        item->sprite_ptr_raw.value().set_position(bounds.position.x, bounds.position.y - 32);
                        item->sprite_ptr_bottom.value().set_position(bounds.position.x, bounds.position.y);
                        item->sprite_ptr_bottom.value().set_tiles(item->sprite_item_ptr->tiles_item(), (item->frame * 2) + 1);
                    }
                }
                else
                {
                    item->sprite_ptr_raw.value().set_tiles(item->sprite_item_ptr->tiles_item(), item->frame);
                }
            }
        }
        else if (within_bounds(bounds, screen))
        {
            if (item->tall)
            {
                // Create top sprite
                item->sprite_ptr_raw = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y - 32, item->frame * 2);
                // Create bottom sprite
                item->sprite_ptr_bottom = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y, (item->frame * 2) + 1);
            }
            else
            {
                item->sprite_ptr_raw = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y, item->frame);
            }
        }
    }
}

character::character(int index_, vector_2 start_, bool npc_) : index(index_), start(start_), npc(npc_)
{
    update_sprite_item(index_);
    current_animation = &anim_stand;
};

bool character::is_tall() const
{
    switch (index)
    {
    case CHAR_VISTA:
        return true; // Vista is a tall character
    default:
        return false;
    }
}

void character::update_sprite_item(int index_)
{
    index = index_;
    bool tall = is_tall();

    switch (index)
    {
    case CHAR_VISTA:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_vista_01;
        v_sprite.tall = tall;
        break;
    }

    default:
    {
        break;
    }
    }

    if (v_sprite.sprite_ptr_raw.has_value())
    {
        start.x = v_sprite.sprite_ptr_raw.value().x();
        start.y = v_sprite.sprite_ptr_raw.value().y();
    }

    // Reset sprites
    v_sprite.sprite_ptr_raw.reset();
    v_sprite.sprite_ptr_bottom.reset();

    // Create new sprites based on whether character is tall
    if (tall)
    {
        v_sprite.sprite_ptr_raw = v_sprite.sprite_item_ptr->create_sprite(start.x, start.y - 32, 0);
        v_sprite.sprite_ptr_bottom = v_sprite.sprite_item_ptr->create_sprite(start.x, start.y, 1);
    }
    else
    {
        v_sprite.sprite_ptr_raw = v_sprite.sprite_item_ptr->create_sprite(start.x, start.y);
    }
}

void character::update()
{
    // Move
    if (!npc)
    {
        bool moving = false;

        if (v_sprite.bounds.position.y > -32 - 16 && v_sprite.bounds.position.y < 32 + 16)
        {
            v_sprite_ptr::camera.y = v_sprite.bounds.position.y;
        }

        if (v_sprite.bounds.position.x > 0 && v_sprite.bounds.position.x < 0)
        {
            v_sprite_ptr::camera.x = v_sprite.bounds.position.x;
        }

        if (bn::keypad::up_held())
        {
            v_sprite.move({0, 1});
            moving = true;
            face = DIR_UP;
        }

        if (bn::keypad::down_held())
        {
            v_sprite.move({0, -1});
            moving = true;
            face = DIR_DOWN;
        }

        if (bn::keypad::left_held())
        {
            v_sprite.move({1, 0});
            moving = true;
            face = DIR_LEFT;
        }

        if (bn::keypad::right_held())
        {
            v_sprite.move({-1, 0});
            moving = true;
            face = DIR_RIGHT;
        }

        // Set animation based on movement
        if (moving && current_animation != &anim_walk)
        {
            current_animation = &anim_walk;
        }
        else if (!moving && current_animation != &anim_stand)
        {
            current_animation = &anim_stand;
        }
    }

    // Draw
    if (v_sprite.sprite_ptr_raw.has_value() && v_sprite.sprite_ptr_bottom.has_value())
    {
        if (ticker % 6 == 0)
        {
            frame = (frame + 1) % 4;
        }

        // Update frame from animation
        int new_frame = current_animation->frames[frame];
        v_sprite.sprite_ptr_bottom.value().set_horizontal_flip(false);
        v_sprite.sprite_ptr_raw.value().set_horizontal_flip(false);

        switch (face)
        {
        case DIR_UP:
        {
            v_sprite.set_frame(new_frame);
            break;
        }
        case DIR_DOWN:
        {
            v_sprite.set_frame(new_frame);
            break;
        }
        case DIR_LEFT:
        {
            v_sprite.set_frame(new_frame + 3);
            v_sprite.sprite_ptr_bottom.value().set_horizontal_flip(true);
            v_sprite.sprite_ptr_raw.value().set_horizontal_flip(true);
            break;
        }
        case DIR_RIGHT:
        {
            v_sprite.set_frame(new_frame + 3);
        }
        default:
        {
            break;
        }
        }
    }

    ticker++;
}