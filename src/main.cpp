// main.cpp
#include "bn_core.h"
#include "bn_log.h"
#include "bn_regular_bg_item.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_list.h"

// Face Sprites
#include "bn_sprite_items_db_ch_jeremy.h"
#include "bn_sprite_items_db_ch_vista.h"

#include "bn_sprite_items_spr_vista_01.h"

#include "bn_regular_bg_items_floor_wood01.h"

#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_text.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_actions.h"

#include "ge_map_data.cpp"

using namespace bn;

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
    while (loop_value == 0)
    {
        char_mgr.update(&current_map);
        loop_value = action_listener(&current_map, &char_mgr);
        current_map.update();
        bool dialogue_is_active = char_mgr.db.has_value() && !char_mgr.db.value().is_ended();
        v_sprite_ptr::update(!dialogue_is_active);
        if (handle_frame) {
            handle_frame = false;
        } else {
            core::update();
        }
    }
    v_sprite_ptr::clear_all();
    return loop_value;
}

int main()
{
    core::init();

    // Initial map
    global_data_ptr = new global_data();
    global_data_ptr->entry_map = &map_garbage_01;
    global_data_ptr->entry_position = {9, 5};

    while (true)
    {
        BN_LOG("NEW MAP: ");
        BN_LOG(global_data_ptr->entry_position.x, " - ", global_data_ptr->entry_position.y);
        navigate_map();
        core::update();
    }
}