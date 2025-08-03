
#include "bn_list.h"
#include "bn_unique_ptr.h"

#include "ge_sprites.h"
#include "ge_character_manager.h"
#include "ge_maps.h"

// Implementation in ge_character_manager.cpp
character_manager::character_manager() : player_ptr(nullptr)
{
}

character *character_manager::add_character(int index, vector_2 position, bool is_npc)
{
    if (characters.size() >= 64)
        return nullptr;

    // Create new character
    characters.push_back(make_unique<character>(index, position, is_npc));
    character *new_char = characters.back().get();

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

void character_manager::update(map_manager *current_map)
{
    for (auto &ch : characters)
    {
        if (ch)
        {
            ch->update(current_map);
        }
    }
}