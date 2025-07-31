// Implementation file (ge_text.cpp)

#include "bn_log.h"
#include "bn_sound_items.h"
#include "bn_sound_items_info.h"
#include "bn_random.h"

#include "ge_globals.h"
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

letter::letter(char ch, vector_2 ideal_position_) : ideal_position(ideal_position_)
{
    char_index = -1;

    for (size_t i = 0; i < sizeof(alphabet); ++i)
    {
        if (ch == alphabet[i])
        {
            char_index = i;
            break;
        }
    }

    if (char_index > -1)
    {
        sprite = sprite_items::spr_font_01.create_sprite(ideal_position_.x, ideal_position_.y, char_index);
    }
}

void letter::update(bool shake, int size)
{
    if (size == SIZE_LARGE)
    {
        sprite.value().set_scale(2, 2);
    }
    else if (size == SIZE_SMALL)
    {
        sprite.value().set_scale(0.75, 0.75);
    }

    if (shake)
    {
        temp_position = {
            ideal_position.x + global_data_ptr->bn_random.get_int(6) - 3,
            ideal_position.y + global_data_ptr->bn_random.get_int(6) - 3,
        };

        if (sprite.value().x().integer() < temp_position.x.integer())
        {
            sprite.value().set_x(sprite.value().x() + 1);
        }
        else if (sprite.value().x() > temp_position.x.integer())
        {
            sprite.value().set_x(sprite.value().x() - 1);
        }

        if (sprite.value().y().integer() < temp_position.y.integer())
        {
            sprite.value().set_y(sprite.value().y() + 1);
        }
        else if (sprite.value().y().integer() > temp_position.y.integer())
        {
            sprite.value().set_y(sprite.value().y() - 1);
        }
    }
}

text::text(const string<20> &value, vector_2 start_) : start(start_)
{
    init(value);
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

    char ch = reference[index];

    int current_size = SIZE_NORMAL;
    int spacing = 8; // Default character spacing

    // Adjust spacing based on size
    if (current_size == SIZE_LARGE)
    {
        spacing = 16; // Larger spacing for large text
    }
    else if (current_size == SIZE_SMALL)
    {
        spacing = 6; // Smaller spacing for small text
    }

    if (ch == ' ')
    {
        current_x += spacing; // Use the size-adjusted spacing
        index++;
        return;
    }

    // Create letter at current position
    letter new_letter = {ch, {start.x + current_x, start.y}};

    if (new_letter.char_index > -1)
    {
        letters.push_back(new_letter);
        current_x += spacing; // Use size-adjusted spacing instead of hardcoded 8
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
        letter.ideal_position = {current_x_pos, y};
        current_x_pos += 8;
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
        lines[t].size = line.size;
    }
}

void dialogue_box::update()
{
    auto l = (*active_conversation)[index];
    character.value().set_tiles(l.character->tiles_item(), (l.emotion * 2));

    for (int t = 0; t < 3; t++)
    {
        for (auto &letter_ : lines[t].letters)
        {
            letter_.update(l.shake, l.size);
        }
    }

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

        if (ticker % 3 == 0)
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