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
#include "ge_battle.h"

#include "ge_map_data.cpp"

using namespace bn;

int navigate_map()
{
    map_manager current_map(global_data_ptr->entry_map);
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

    if (global_data_ptr->entry_map != &map_room01)
    {
        char_mgr.add_character(CHAR_JEREMY, global_data_ptr->entry_position, true);
    }

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

int main()
{
    core::init();

    // Set for debug
    global_data_ptr = new global_data();
    global_data_ptr->process_stage = CUTSCENE_TO_GARBAGE;

    int value = NEW_CHAPTER;

    while (true)
    {
        switch (value)
        {
        case BATTLE:
        {
            value = battle_map();
            break;
        }
        case NEW_CHAPTER:
        {
            switch (global_data_ptr->process_stage)
            {
            case CUTSCENE_TO_GARBAGE:
            {
                global_data_ptr->entry_map = &map_garbage_01;
                global_data_ptr->entry_position = {9, 5};
                global_data_ptr->bg_track = &music_items::bg_garbage;
                break;
            }
            case GARBAGE_TO_BLACK:
            {
                music::stop();
                sound_items::sfx_knock.play();
                for (int t = 0; t < 96; t++)
                {
                    core::update();
                }
                global_data_ptr->entry_map = &map_lab_01;
                global_data_ptr->entry_position = {4, 5};
                global_data_ptr->bg_track = &music_items::bg_office;
                break;
            }
            case BLACK_TO_LAB:
            {
                global_data_ptr->entry_map = &map_lab_01;
                global_data_ptr->entry_position = {4, 5};
                global_data_ptr->bg_track = &music_items::bg_office;
                break;
            }
            default:
            {
                global_data_ptr->entry_map = &map_garbage_01;
                global_data_ptr->entry_position = {9, 5};
                global_data_ptr->bg_track = &music_items::bg_garbage;
                break;
            }
            }

            value = CONTINUE;
            break;
        }
        default:
        {
            if (!music::playing())
            {
                global_data_ptr->bg_track->play();
            }

            value = navigate_map();
            break;
        }
        }

        core::update();
    }
}