#include "bn_log.h"
#include "bn_sprites.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

// Body Sprites
#include "bn_sprite_items_spr_vista_01.h"

#include "ge_structs.h"
#include "ge_sprites.h"
#include "ge_maps.h"

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
    // Now we add the direction instead of subtracting
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

void v_sprite_ptr::update(bool dialogue_box_ended)
{
    for (auto *item : manager)
    {
        bound bounds = item->real_position();
        bound acceptable = screen;

        if (!dialogue_box_ended)
        {
            acceptable.position.y = 0 - (screen_height / 2) + 12;
        }

        if (item->sprite_ptr_raw.has_value())
        {
            // Sprite exists - check if it should be removed or updated
            if (!within_bounds(bounds, acceptable))
            {
                // Out of bounds - remove sprites
                item->sprite_ptr_raw.reset();
                item->sprite_ptr_bottom.reset();
            }
            else
            {
                item->sprite_ptr_raw.value().set_z_order(10 - item->sprite_ptr_raw.value().y().integer() / 16);

                // In bounds - update position and tiles
                if (item->tall)
                {
                    item->sprite_ptr_bottom.value().set_z_order(10 - item->sprite_ptr_raw.value().y().integer() / 16);

                    // For tall sprites, update both sprites
                    item->sprite_ptr_raw.value().set_position(bounds.position.x, bounds.position.y - 32);
                    item->sprite_ptr_raw.value().set_tiles(item->sprite_item_ptr->tiles_item(), item->frame * 2);

                    if (item->sprite_ptr_bottom.has_value())
                    {
                        item->sprite_ptr_bottom.value().set_position(bounds.position.x, bounds.position.y);
                        item->sprite_ptr_bottom.value().set_tiles(item->sprite_item_ptr->tiles_item(), (item->frame * 2) + 1);
                    }
                }
                else
                {
                    // Regular sprite - update position and tiles
                    item->sprite_ptr_raw.value().set_position(bounds.position.x, bounds.position.y);
                    item->sprite_ptr_raw.value().set_tiles(item->sprite_item_ptr->tiles_item(), item->frame);
                }
            }
        }
        else if (within_bounds(bounds, acceptable))
        {
            // Sprite doesn't exist but is in bounds - create it
            if (item->sprite_item_ptr != nullptr) // Safety check
            {
                if (item->tall)
                {
                    // Create both sprites for tall characters
                    item->sprite_ptr_raw = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y - 32, item->frame * 2);
                    item->sprite_ptr_bottom = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y, (item->frame * 2) + 1);
                }
                else
                {
                    // Create single sprite for regular characters
                    item->sprite_ptr_raw = item->sprite_item_ptr->create_sprite(bounds.position.x, bounds.position.y, item->frame);
                }
            }
        }
    }
}

character::character(int index_, vector_2 start_, bool npc_) : index(index_), npc(npc_)
{
    start.x = (start_.x * 32) + 16;
    start.y = (start_.y * 32) + 16;

    // Initialize v_sprite bounds position
    // v_sprite_ptr::manager.push_back(&v_sprite);
    v_sprite.bounds.position = start;
    v_sprite.bounds.width = 32;
    v_sprite.bounds.height = 32;

    update_sprite_item(index_);
    current_animation = &anim_stand;

    if (!npc_ || (v_sprite_ptr::camera.x == 0 && v_sprite_ptr::camera.y == 0))
    {
        v_sprite_ptr::camera.x = start.x;
        v_sprite_ptr::camera.y = start.y;
    }
}

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

    // Reset sprites
    v_sprite.sprite_ptr_raw.reset();
    v_sprite.sprite_ptr_bottom.reset();
}

void character::update(map_manager *current_map, bool db_inactive)
{
    vector_2 delta = {0, 0};

    if (!npc)
    {
        if (db_inactive) {
            move_to.x = 0;
            move_to.y = 0;
        }

        vector_2 bound_1 = {
            (screen_width / 2),
            (screen_height / 2)};

        vector_2 bound_2 = {
            (current_map->current_map->size.x * 32) - bound_1.x,
            (current_map->current_map->size.y * 32) - bound_1.y};

        if (!db_inactive)
        {
            bound_2.y = bound_2.y + 96;
        }

        v_sprite_ptr::camera.x = v_sprite.bounds.position.x;
        v_sprite_ptr::camera.y = v_sprite.bounds.position.y;

        if (v_sprite_ptr::camera.x < bound_1.x)
            v_sprite_ptr::camera.x = bound_1.x;
        if (v_sprite_ptr::camera.y < bound_1.y)
            v_sprite_ptr::camera.y = bound_1.y;
        if (v_sprite_ptr::camera.x > bound_2.x)
            v_sprite_ptr::camera.x = bound_2.x;
        if (v_sprite_ptr::camera.y > bound_2.y)
            v_sprite_ptr::camera.y = bound_2.y;

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

    // Animation and sprite update code remains the same...
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
    if (v_sprite.sprite_ptr_raw.has_value())
    {
        if (ticker % 6 == 0)
        {
            frame = (frame + 1) % current_animation->size;
        }

        int new_frame = current_animation->frames[frame];

        v_sprite.sprite_ptr_raw.value().set_horizontal_flip(false);
        if (v_sprite.sprite_ptr_bottom.has_value())
        {
            v_sprite.sprite_ptr_bottom.value().set_horizontal_flip(false);
        }

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
                v_sprite.sprite_ptr_raw.value().set_horizontal_flip(true);
                if (v_sprite.sprite_ptr_bottom.has_value())
                {
                    v_sprite.sprite_ptr_bottom.value().set_horizontal_flip(true);
                }
                break;
            case DIR_RIGHT:
                v_sprite.set_frame(new_frame + 3);
                break;
            }
        }
        else
        {
            v_sprite.set_frame(new_frame);
        }
    }

    ticker++;
}

static character *find(list<character, 64> &characters, int index)
{
    for (character &ch : characters)
    {
        if (ch.index == index)
        {
            return &ch;
        }
    }

    // Return first character if list isn't empty
    if (!characters.empty())
    {
        return &(*characters.begin());
    }

    return nullptr; // List is empty
}

void character::add(list<character, 64> *characters, int character_id, vector_2 location, bool npc)
{
    characters->emplace_back(character_id, location, npc);
}