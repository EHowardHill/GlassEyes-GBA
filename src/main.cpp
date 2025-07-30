// main.cpp
#include "bn_core.h"
#include "bn_log.h"
#include "bn_regular_bg_item.h"
#include "bn_string.h"
#include "bn_keypad.h"

// Sprites
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_items_db_ch_jeremy.h"

#include "ge_sprites.h"
#include "ge_text.h"

using namespace bn;

int main()
{
    core::init();

    conversation test_convo = {
        {&sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_SPEAK, "Hello?", "Is this working?", ""},
        {&sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_SPEAK, "YES!", "It's working!!", ""},
        dialogue_line::end_marker()};

    optional<dialogue_box> db;
    {
        dialogue_box db_test;
        db = db_test;
    }

    db.value().load(&test_convo);
    db.value().init();

    int angle = 90;
    auto wizard = sprite_items::wizard.create_sprite(0, 0);

    while (true)
    {
        angle++;
        angle = angle % 360;
        wizard.set_rotation_angle(angle);

        if (db.has_value())
        {
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