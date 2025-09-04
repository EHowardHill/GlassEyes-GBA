#include "bn_log.h"
#include "bn_sprites.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

// Body Sprites
#include "bn_sprite_items_spr_vista_01.h"
#include "bn_sprite_items_spr_jeremy_01.h"
#include "bn_sprite_items_spr_visker_01.h"
#include "bn_sprite_items_spr_visker_wife_01.h"
#include "bn_sprite_items_spr_ginger_01.h"
#include "bn_sprite_items_spr_croke_01.h"
#include "bn_sprite_items_spr_elements.h"

#include "ge_structs.h"
#include "ge_text.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_animations.h"

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

v_sprite_ptr::v_sprite_ptr()
    : sprite_item_ptr(nullptr), frame(0)
{
    bounds.position = {0, 0};
    bounds.width = 0;
    bounds.height = 0;
    manager.push_back(this);
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
    // Clear sprites before removing from manager
    sprite_ptr_raw[0].reset();
    sprite_ptr_raw[1].reset();

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
    bounds.position.x = bounds.position.x + direction.x;
    bounds.position.y = bounds.position.y + direction.y;
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

    if (sprite_ptr_raw[0].has_value())
    {
        // Always use tall sprite logic
        // Top sprite uses frame * 2
        sprite_ptr_raw[0].value().set_tiles(sprite_item_ptr->tiles_item(), frame * 2);

        // Bottom sprite uses (frame * 2) + 1
        if (sprite_ptr_raw[1].has_value())
        {
            sprite_ptr_raw[1].value().set_tiles(sprite_item_ptr->tiles_item(), (frame * 2) + 1);
        }
    }
}

void v_sprite_ptr::update(bool dialogue_box_ended)
{
    for (auto *item : manager)
    {
        bound bounds = item->real_position();
        bound acceptable = {screen.position, screen.width + 32, screen.height + 32};

        if (!dialogue_box_ended)
        {
            acceptable.position.y = 0 - (screen_height / 2) + 12;
        }

        if (item->sprite_ptr_raw[0].has_value())
        {
            // Sprite exists - check if it should be removed or updated
            if (!within_bounds(bounds, acceptable))
            {
                // Out of bounds - remove sprites
                item->sprite_ptr_raw[0].reset();
                item->sprite_ptr_raw[1].reset();
            }
            else
            {
                // Update z-order for both sprites
                if (item->sprite_item_ptr != &sprite_items::spr_elements)
                {
                    item->sprite_ptr_raw[0].value().set_z_order(10 - item->sprite_ptr_raw[1].value().y().integer() / 16);
                    item->sprite_ptr_raw[1].value().set_z_order(10 - item->sprite_ptr_raw[1].value().y().integer() / 16);
                } else {
                    item->sprite_ptr_raw[1].value().set_z_order(999);
                }

                // Always update both sprites (tall sprites)
                item->sprite_ptr_raw[0].value().set_position(bounds.position.x, bounds.position.y - 32);
                item->sprite_ptr_raw[0].value().set_tiles(item->sprite_item_ptr->tiles_item(), item->frame * 2);

                if (item->sprite_ptr_raw[1].has_value())
                {
                    item->sprite_ptr_raw[1].value().set_position(bounds.position.x, bounds.position.y);
                    item->sprite_ptr_raw[1].value().set_tiles(item->sprite_item_ptr->tiles_item(), (item->frame * 2) + 1);
                }
            }
        }
        else if (within_bounds(bounds, acceptable))
        {
            // Sprite doesn't exist but is in bounds - create it
            if (item->sprite_item_ptr != nullptr) // Safety check
            {
                // Always create both sprites for tall characters
                item->sprite_ptr_raw[0] = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y - 32, item->frame * 2);
                item->sprite_ptr_raw[1] = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y, (item->frame * 2) + 1);
            }
        }
    }
}

character::character(int index_, vector_2 start_) : index(index_)
{
    v_sprite.bounds.position = {
        (start_.x * 32) + 16,
        (start_.y * 32) + 16};
    v_sprite.bounds.width = 28;
    v_sprite.bounds.height = 28;

    switch (index_)
    {
    case CHAR_VISTA:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_vista_01;
        break;
    }
    case CHAR_JEREMY:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_jeremy_01;
        break;
    }
    case CHAR_VISKER:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_visker_01;
        break;
    }
    case CHAR_VISKER_WIFE:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_visker_wife_01;
        break;
    }
    case CHAR_GINGER:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_ginger_01;
        break;
    }
    case CHAR_CROKE:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_croke_01;
        break;
    }
    case ITEM_BUTTON:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_elements;
        idle_animation = &elem_button_up;
        break;
    }
    case ITEM_SPIKE:
    {
        v_sprite.sprite_item_ptr = &bn::sprite_items::spr_elements;
        idle_animation = &elem_spike_up;
        break;
    }

    default:
    {
        break;
    }
    }

    // Reset sprites
    v_sprite.sprite_ptr_raw[0].reset();
    v_sprite.sprite_ptr_raw[1].reset();
    current_animation = &anim_stand;

    if (type() == CH_TYPE_PLAYER || (v_sprite_ptr::camera.x == 0 && v_sprite_ptr::camera.y == 0))
    {
        v_sprite_ptr::camera.x = start_.x;
        v_sprite_ptr::camera.y = start_.y;
    }
}

void character::update(map_manager *current_map, bool db_inactive)
{
    vector_2 delta = {0, 0};

    if (type() == CH_TYPE_PLAYER)
    {
        if (db_inactive)
        {
            move_to.x = 0;
            move_to.y = 0;
            idle_animation = nullptr;
        }

        vector_2 bound_1 = {
            (screen_width / 2),
            (screen_height / 2)};

        vector_2 bound_2 = {
            (current_map->current_map->size.x * 32) - bound_1.x,
            (current_map->current_map->size.y * 32) - bound_1.y};

        if (db_inactive)
        {
            if (bn::keypad::up_held())
            {
                delta.y = -1;
            }

            if (bn::keypad::down_held())
            {
                delta.y = 1;
            }

            if (bn::keypad::left_held())
            {
                delta.x = -1;
            }

            if (bn::keypad::right_held())
            {
                delta.x = 1;
            }
        }

        v_sprite_ptr::camera.x = v_sprite.bounds.position.x;

        if (!db_inactive && index != CHAR_VISTA)
        {
            if (v_sprite.bounds.position.y > v_sprite_ptr::camera.y)
            {
                v_sprite_ptr::camera.y = v_sprite.bounds.position.y;
            }
        }
        else
        {
            v_sprite_ptr::camera.y = v_sprite.bounds.position.y;
        }

        if (v_sprite_ptr::camera.x < bound_1.x)
            v_sprite_ptr::camera.x = bound_1.x;

        if (v_sprite_ptr::camera.x > bound_2.x)
            v_sprite_ptr::camera.x = bound_2.x;
    }
    else
    {
        if (!db_inactive)
        {
            if (v_sprite.sprite_ptr_raw[0].has_value())
            {
                // Calculate where this sprite appears on screen
                bound screen_pos = v_sprite.real_position();

                // Keep sprites at y=0 or above for ~80 pixels of buffer from dialogue box
                if (screen_pos.position.y > 0)
                {
                    // Move camera down just enough to keep sprite above this threshold
                    fixed adjustment = screen_pos.position.y - 0;
                    v_sprite_ptr::camera.y += adjustment;
                }
            }
        }
    }

    // Fixed move_to logic with proper directions
    if (move_to.x != 0 && move_to.y != 0)
    {
        vector_2 move_to_exp = {
            (move_to.x * 32) + 16,
            (move_to.y * 32) + 16};

        if (move_to_exp.x > v_sprite.bounds.position.x)
        {
            delta.x = 1;
        }
        else if (move_to_exp.x < v_sprite.bounds.position.x)
        {
            delta.x = -1;
        }

        if (move_to_exp.y > v_sprite.bounds.position.y)
        {
            delta.y = 1;
        }
        else if (move_to_exp.y < v_sprite.bounds.position.y)
        {
            delta.y = -1;
        }
    }

    // Fixed facing direction
    if (delta.y > 0)
    {
        face = DIR_DOWN;
    }
    else if (delta.y < 0)
    {
        face = DIR_UP;
    }

    if (delta.x > 0)
    {
        face = DIR_RIGHT;
    }
    else if (delta.x < 0)
    {
        face = DIR_LEFT;
    }

    if (keypad::b_held())
    {
        delta.x = delta.x * 2;
        delta.y = delta.y * 2;
    }

    // Pixel-perfect collision detection
    // Check future position for each axis separately
    bound future_bounds_x = v_sprite.bounds;
    future_bounds_x.position.x = future_bounds_x.position.x + delta.x;

    bound future_bounds_y = v_sprite.bounds;
    future_bounds_y.position.y = future_bounds_y.position.y + delta.y;

    // Check X movement
    if (current_map->check_box_collision(future_bounds_x))
    {
        delta.x = 0;
    }

    // Check Y movement
    if (current_map->check_box_collision(future_bounds_y))
    {
        delta.y = 0;
    }

    // For diagonal movement, also check the combined movement
    if (delta.x != 0 && delta.y != 0)
    {
        bound future_bounds_both = v_sprite.bounds;
        future_bounds_both.position.x = future_bounds_both.position.x + delta.x;
        future_bounds_both.position.y = future_bounds_both.position.y + delta.y;

        if (current_map->check_box_collision(future_bounds_both))
        {
            // Try to slide along walls
            // If diagonal fails but individual axes might work, keep the working axis
            if (delta.x != 0 && !current_map->check_box_collision(future_bounds_x))
            {
                delta.y = 0; // Can move X but not Y
            }
            else if (delta.y != 0 && !current_map->check_box_collision(future_bounds_y))
            {
                delta.x = 0; // Can move Y but not X
            }
            else
            {
                // Can't move in either direction
                delta.x = 0;
                delta.y = 0;
            }
        }
    }

    // Apply movement with fixed direction
    v_sprite.bounds.position.x = v_sprite.bounds.position.x + delta.x;
    v_sprite.bounds.position.y = v_sprite.bounds.position.y + delta.y;

    bool moving = (delta.x != 0 || delta.y != 0);

    // Animation and sprite update code
    bool custom_anim = false;
    if (moving)
    {
        current_animation = &anim_walk;
    }
    else if (idle_animation == nullptr)
    {
        current_animation = &anim_stand;
    }
    else
    {
        if (current_animation != idle_animation)
        {
            frame = 0;
        }
        current_animation = idle_animation;
        custom_anim = true;
    }

    // Rest of the update code...
    if (v_sprite.sprite_ptr_raw[0].has_value())
    {
        int ticker_speed;

        switch (current_animation->speed)
        {
        case SP_SLOW:
        {
            ticker_speed = 12;
            break;
        }
        case SP_FAST:
        {
            ticker_speed = 3;
            break;
        }
        default:
        {
            ticker_speed = 6;
            break;
        }
        }

        if (current_animation->loop)
        {
            if (ticker % ticker_speed == 0)
            {
                frame = (frame + 1) % current_animation->size;
            }
        }
        else
        {
            if (ticker % ticker_speed == 0 && frame < current_animation->size - 1)
            {
                frame++;
            }
        }

        int new_frame = current_animation->frames[frame];

        // Always set horizontal flip for both sprites
        v_sprite.sprite_ptr_raw[0].value().set_horizontal_flip(false);
        v_sprite.sprite_ptr_raw[1].value().set_horizontal_flip(false);

        if (!custom_anim)
        {
            switch (face)
            {
            case DIR_UP:
                v_sprite.set_frame(new_frame + 6);
                break;
            case DIR_DOWN:
                v_sprite.set_frame(new_frame);
                break;
            case DIR_LEFT:
                v_sprite.set_frame(new_frame + 3);
                v_sprite.sprite_ptr_raw[0].value().set_horizontal_flip(true);
                v_sprite.sprite_ptr_raw[1].value().set_horizontal_flip(true);
                break;
            case DIR_RIGHT:
                v_sprite.set_frame(new_frame + 3);
                break;
            default:
            {
                break;
            }
            }
        }
        else
        {
            v_sprite.set_frame(new_frame);
        }
    }

    ticker++;
}

void character::add(list<character, 32> *characters, int character_id, vector_2 location)
{
    characters->emplace_back(character_id, location);
};