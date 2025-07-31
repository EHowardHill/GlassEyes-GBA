// main.cpp
#include "bn_core.h"
#include "bn_log.h"
#include "bn_regular_bg_item.h"
#include "bn_string.h"
#include "bn_keypad.h"

// Sprites
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_items_db_ch_jeremy.h"
#include "bn_sprite_items_db_ch_vista.h"

#include "bn_regular_bg_items_floor_wood01.h"
#include "bn_regular_bg_items_room_cabin_01.h"

#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_text.h"

using namespace bn;

int main()
{
    core::init();
    global_data_ptr = new global_data();

    auto floor = regular_bg_items::floor_wood01.create_bg(0, 0);
    auto items = regular_bg_items::room_cabin_01.create_bg(0, 48);

    conversation test_convo = {
        {&sprite_items::db_ch_vista, EM_DEFAULT, ACT_SPEAK, "Hello?", "Is this working?"},
        {&sprite_items::db_ch_jeremy, EM_LAUGH, ACT_SPEAK, "", "HA... HA HA...", "", true, SIZE_LARGE},
        {&sprite_items::db_ch_jeremy, EM_EMBARRASSED, ACT_SPEAK, "Ope", "Sorry bout that", "", false, SIZE_SMALL},
        dialogue_line::end_marker()};

    optional<dialogue_box> db;
    {
        dialogue_box db_test;
        db = db_test;
    }

    db.value().load(&test_convo);
    db.value().init();

    while (true)
    {
        if (db.has_value())
        {
            db.value().update();

            if (keypad::a_pressed())
            {
                db.value().index++;
                db.value().init();
            }

            if (db.value().is_ended())
            {
                BN_LOG("RESET!");
                db.reset();
            }
        }

        v_sprite_ptr::update();
        core::update();
    }
}