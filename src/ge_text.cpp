#include "bn_log.h"
#include "ge_text.h"

// Sprites
#include "bn_sprite_items_db_ch_jeremy.h"

#include "bn_sprite_items_spr_font_01.h"

#include "bn_regular_bg_items_bg_dialogue_box.h"

using namespace bn;

constexpr char alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '[', ']', '\\', '{', '}', '|', ';', '\'', ':', '"', ',', '.', '/',
    '<', '>', '?'};

void text::update(const string<20> &value)
{
    letters.clear();
    int current_x = start.x.integer();

    for (const char ch : value)
    {
        if (ch == ' ')
        {
            current_x += 8;
            continue;
        }

        int char_index = -1;
        for (size_t i = 0; i < sizeof(alphabet); ++i)
        {
            if (ch == alphabet[i])
            {
                char_index = i;
                break;
            }
        }

        if (char_index != -1 && letters.size() < letters.max_size())
        {
            sprite_ptr letter = sprite_items::spr_font_01.create_sprite(current_x, start.y, char_index);
            letters.push_back(move(letter));
            current_x += 8;
        }
    }
}

text::text(const string<20> &value, vector_2 start_) : start(start_)
{
    update(value);
}

void text::set_position(int x, int y)
{
    int current_x = x;
    for (auto &letter : letters)
    {
        letter.set_position(current_x, y);
        current_x += 8;
    }
}

void text::set_visible(bool visible)
{
    for (auto &letter : letters)
    {
        letter.set_visible(visible);
    }
}

dialogue_box::dialogue_box()
{
    character = sprite_items::db_ch_jeremy.create_sprite(0, 0);
    box = regular_bg_items::bg_dialogue_box.create_bg(0, 0);
    pointer = sprite_items::spr_font_01.create_sprite(-52, 32, 73);
}

void dialogue_box::load(conversation *new_conversation)
{
    active_conversation = new_conversation;
    index = 0;
    size = 0;

    for (int i = 0; i < 128; i++)
    {
        if ((*active_conversation)[i].action == ACT_END)
        {
            break;
        }
        size++;
    }
}

void dialogue_box::init()
{
    // Change from pointer to reference or direct access
    const dialogue_line &line = (*active_conversation)[index];

    if (line.action == ACT_END)
    {
        return;
    }

    if (line.character != nullptr)
    {
        character = line.character->create_sprite(-84, 56, line.emotion);
    }
    else
    {
        character.reset();
    }

    for (int t = 0; t < 3; t++)
    {
        lines[t].update(line.raw_text[t]);
    }
}

void dialogue_box::update()
{
}

bool dialogue_box::is_ended()
{
    return (*active_conversation)[index].action == ACT_END;
}