// main.cpp
#include "bn_core.h"
#include "bn_log.h"
#include "bn_regular_bg_item.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_list.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_music_items_info.h"
#include "bn_sound_items.h"
#include "bn_math.h"

// Face Sprites
#include "bn_sprite_items_db_ch_jeremy.h"
#include "bn_sprite_items_db_ch_vista.h"

#include "bn_sprite_items_spr_vista_01.h"

#include "bn_regular_bg_items_floor_wood01.h"

#include "main.h"
#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_text.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_actions.h"

#include "ge_map_data.cpp"

#include "bn_sprite_items_spr_jeremy_01.h"
#include "bn_sprite_items_hearts.h"

using namespace bn;

vector_2 moveTowards(vector_2 from, vector_2 towards, fixed_t<4> speed)
{
    fixed_t<4> dx = towards.x - from.x;
    fixed_t<4> dy = towards.y - from.y;

    fixed_t<4> distance = sqrt(dx * dx + dy * dy);

    fixed_t<4> moveX = (dx / distance) * speed;
    fixed_t<4> moveY = (dy / distance) * speed;

    if (distance <= speed)
    {
        from.x = towards.x;
        from.y = towards.y;
    }
    else
    {
        // Update position
        from.x += moveX;
        from.y += moveY;
    }

    return from;
}

int navigate_map()
{
    const regular_bg_item *floor_texture;

    // Set floor
    /*
    if (global_data_ptr->entry_map == &map_room01) {
        floor_texture = &bn::regular_bg_items::floor_wood01;
    }
    */

    map_manager current_map(floor_texture, global_data_ptr->entry_map);
    character_manager char_mgr;

    for (int y = 0; y < current_map.current_map->size.y.integer(); y++)
    {
        for (int x = 0; x < current_map.current_map->size.x.integer(); x++)
        {
            int tile = x + (y * current_map.current_map->size.x.integer());

            if (current_map.current_map->characters[tile] != 0)
            {
                char_mgr.add_character(current_map.current_map->characters[tile] - 1, {x, y}, false);
            }
        }
    }

    char_mgr.add_character(CHAR_JEREMY, global_data_ptr->entry_position, true);

    int loop_value = 0;
    bool handle_frame = true;
    while (loop_value == CONTINUE && char_mgr.status == CONTINUE)
    {
        char_mgr.update(&current_map);
        loop_value = action_listener(&current_map, &char_mgr);
        current_map.update();
        bool dialogue_is_active = char_mgr.db.has_value() && !char_mgr.db.value().is_ended();
        v_sprite_ptr::update(!dialogue_is_active);
        if (handle_frame)
        {
            handle_frame = false;
        }
        else
        {
            core::update();
        }
    }

    v_sprite_ptr::clear_all();
    return loop_value + char_mgr.status;
}

enum BATTLE_STAGE
{
    stage_heart_moving,
    stage_main
};

int battle_map()
{
    int stage = stage_heart_moving;
    sound_items::snd_fight_start.play();
    sprite_ptr heart = sprite_items::hearts.create_sprite(
        global_data_ptr->entry_position_raw.x,
        global_data_ptr->entry_position_raw.y - 32,
        1);
    heart.set_z_order(-1);

    {
        sprite_ptr character[2] = {
            sprite_items::spr_jeremy_01.create_sprite(
                global_data_ptr->entry_position_raw.x,
                global_data_ptr->entry_position_raw.y - 32,
                global_data_ptr->entry_direction * 2),
            sprite_items::spr_jeremy_01.create_sprite(
                global_data_ptr->entry_position_raw.x,
                global_data_ptr->entry_position_raw.y,
                (global_data_ptr->entry_direction * 2) + 1)};

        for (int t = 0; t < 12; t++)
        {
            heart.set_visible(t % 4 >= 2);
            core::update();
        }
    }

    while (true)
    {
        if (stage == stage_heart_moving)
        {
            if (bn::abs(heart.x()) <= 1 && bn::abs(heart.y()) <= 1)
            {
                heart.set_position(0, 0);
                stage = stage_main;
            }
            else
            {
                vector_2 new_pos = moveTowards({heart.position().x().integer(), heart.position().y().integer()}, {0, 0}, 2);
                heart.set_position(new_pos.x, new_pos.y);
            }
        }

        core::update();
    }
}

int main()
{
    core::init();

    // Initial map
    global_data_ptr = new global_data();
    global_data_ptr->entry_map = &map_garbage_01;
    global_data_ptr->entry_position = {9, 5};
    global_data_ptr->bg_track = &music_items::bg_garbage;

    while (true)
    {
        int value = navigate_map();
        core::update();

        if (value == BATTLE)
        {
            value = battle_map();
            core::update();
        }
    }
}