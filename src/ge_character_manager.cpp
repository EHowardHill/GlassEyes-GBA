#include "bn_log.h"
#include "bn_list.h"
#include "bn_unique_ptr.h"
#include "bn_optional.h"
#include "bn_music.h"
#include "bn_sound_items.h"

#include "ge_sprites.h"
#include "ge_text.h"
#include "ge_character_manager.h"
#include "ge_maps.h"
#include "ge_globals.h"

#include "bn_sprite_items_hearts.h"

using namespace bn;

// Implementation in ge_character_manager.cpp
character_manager::character_manager() : player_ptr(nullptr)
{
}

character *character_manager::add_character(int index, vector_2 position, bool is_npc)
{
    if (characters.size() >= 64)
        return nullptr;

    is_npc = true;
    if (index == CHAR_JEREMY)
    {
        is_npc = false;
    }

    // Create new character
    characters.push_back(make_unique<character>(index, position, is_npc));
    character *new_char = characters.back().get();

    if (global_data_ptr->process_stage != GARBAGE_TO_BLACK)
    {
        if (index == CHAR_GINGER)
        {
            new_char->is_follow = true;
        }
    }

    // Track player
    if (!is_npc && !player_ptr)
    {
        player_ptr = new_char;
    }

    return new_char;
}

bool character_manager::remove_character(character *ch)
{
    if (!ch || ch == player_ptr)
        return false; // Don't remove player

    for (auto it = characters.begin(); it != characters.end(); ++it)
    {
        if (it->get() == ch)
        {
            characters.erase(it);
            return true;
        }
    }
    return false;
}

void character_manager::clear_npcs()
{
    auto it = characters.begin();
    while (it != characters.end())
    {
        if ((*it)->npc)
        {
            it = characters.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

character *character_manager::find_at_position(vector_2 pos, int tolerance)
{
    for (auto &ch : characters)
    {
        if (ch)
        {
            int dx = abs(ch->v_sprite.bounds.position.x.integer() - pos.x.integer());
            int dy = abs(ch->v_sprite.bounds.position.y.integer() - pos.y.integer());

            if (dx <= tolerance && dy <= tolerance)
            {
                return ch.get();
            }
        }
    }
    return nullptr;
}

character *character_manager::find_by_index(int index)
{
    for (auto &ch : characters)
    {
        if (ch && ch->index == index)
        {
            return ch.get();
        }
    }
    return nullptr;
}

void character_manager::alert()
{
    vector_2 player_pos = player_ptr->v_sprite.real_position().position;

    sprite_ptr alert_box = sprite_items::hearts.create_sprite(player_pos.x, player_pos.y - 48, 0);
    music::stop();
    sound_items::snd_alert.play();

    for (int t = 0; t < 64; t++)
    {
        core::update();
    }

    global_data_ptr->entry_position_raw = player_pos;
    global_data_ptr->entry_direction = player_ptr->face;
    status = BATTLE;
}

void character_manager::update(map_manager *current_map = nullptr)
{
    bool db_inactive = true;

    // Handle dialogue box
    if (db.has_value())
    {
        db_inactive = db.value().is_ended();
        db.value().update();

        if (keypad::a_pressed())
        {
            db.value().index++;
            db.value().init(this);
        }

        if (db.value().is_ended())
        {
            db.reset();
        }
    }

    // Handle characters
    for (auto &ch : characters)
    {
        if (ch)
        {
            ch->update(current_map, db_inactive);

            if (ch->is_follow)
            {
                auto f_ch = find_by_index(ch->follow_id);
                vector_2 f_pos = {f_ch->v_sprite.bounds.position.x / 32, f_ch->v_sprite.bounds.position.y / 32};

                switch (f_ch->face)
                {
                case DIR_UP:
                {
                    ch->move_to = {f_pos.x.integer(), f_pos.y.integer() + 1};
                    break;
                }
                case DIR_DOWN:
                {
                    ch->move_to = {f_pos.x.integer(), f_pos.y.integer() - 1};
                    break;
                }
                case DIR_LEFT:
                {
                    ch->move_to = {f_pos.x.integer() + 1, f_pos.y.integer()};
                    break;
                }
                case DIR_RIGHT:
                {
                    ch->move_to = {f_pos.x.integer() - 1, f_pos.y.integer()};
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }
    }
}