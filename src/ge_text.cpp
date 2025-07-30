// Implementation file (ge_text.cpp)

#include "bn_log.h"
#include "bn_sound_items.h"
#include "bn_sound_items_info.h"
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

text::text(const string<20> &value, vector_2 start_) : start(start_)
{
    init(value); // Changed from update to init
}

void text::init(const string<20> &value)
{
    reference = value;
    letters.clear();
    current_x = 0;
    index = 0;
}

void text::update()
{
    if (index >= reference.size() || is_ended())
    {
        return;
    }

    sound_items::snd_dialogue_generic.play();

    char ch = reference[index]; // Declared ch variable

    if (ch == ' ')
    {
        current_x += 8;
        index++;
        return;
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
        sprite_ptr letter = sprite_items::spr_font_01.create_sprite(start.x + current_x, start.y, char_index);
        letters.push_back(move(letter));
        current_x += 8;
        index++;
    }
}

bool text::is_ended()
{
    return index >= reference.size() || reference[index] == '\0'; // Fixed end condition
}

void text::set_position(int x, int y)
{
    start.x = x;
    start.y = y;
    int current_x_pos = x;
    for (auto &letter : letters)
    {
        letter.set_position(current_x_pos, y);
        current_x_pos += 8;
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
    ticker = 0;
    active_conversation = nullptr;
    index = 0;
    size = 0;
}

void dialogue_box::load(conversation *new_conversation)
{
    active_conversation = new_conversation;
    index = 0;
    size = 0;

    for (int i = 0; i < 64; i++) // Changed from 128 to 64 to match typedef
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
    if (!active_conversation || is_ended())
    {
        return;
    }

    const dialogue_line &line = (*active_conversation)[index];

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
        lines[t].init(line.raw_text[t]);
    }
}

void dialogue_box::update()
{
    auto l = (*active_conversation)[index];
    character.value().set_tiles(l.character->tiles_item(), (l.emotion * 2));

    if (!active_conversation || (lines[0].is_ended() && lines[1].is_ended() && lines[2].is_ended()))
    {
        return;
    }

    ticker++;

    if (l.speed == SP_DEFAULT)
    {
        if (ticker % 12 < 6)
        {
            character.value().set_tiles(l.character->tiles_item(), (l.emotion * 2) + 1);
        }

        if (ticker % 4 == 0)
        {
            for (int t = 0; t < 3; t++)
            {
                if (!lines[t].is_ended())
                {
                    lines[t].update();
                    return;
                }
            }
        }
    }
}

bool dialogue_box::is_ended()
{
    if (!active_conversation)
    {
        return true;
    }
    return (*active_conversation)[index].action == ACT_END;
}