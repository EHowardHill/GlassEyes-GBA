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

#include "ge_map_data.cpp"

using namespace bn;

int main()
{
    core::init();
    global_data_ptr = new global_data();

    map_manager current_map(&bn::regular_bg_items::floor_wood01, &map_room01);
    character_manager char_mgr;

    conversation test_convo = {
        {nullptr, EM_DEFAULT, ACT_SPEAK, "Most days were the", "same for Vista,"},
        dialogue_line::end_marker()};

    optional<dialogue_box> db;
    {
        dialogue_box db_test;
        db = db_test;
    }

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

    db.value().load(&test_convo);
    db.value().init(&char_mgr);

    while (true)
    {
        if (db.has_value())
        {
            db.value().update();

            if (keypad::a_pressed())
            {
                db.value().index++;
                db.value().init(&char_mgr);
            }

            if (db.value().is_ended())
            {
                db.reset();
            }
        }

        // Update all characters
        char_mgr.update(&current_map);
        current_map.update();
        v_sprite_ptr::update();
        core::update();
    }
}