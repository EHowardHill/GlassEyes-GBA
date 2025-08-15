// Implementation file (ge_text.cpp)

#include "bn_log.h"
#include "bn_sound_items.h"
#include "bn_sound_items_info.h"
#include "bn_random.h"

#include "ge_sprites.h"
#include "ge_globals.h"
#include "ge_text.h"
#include "ge_character_manager.h"

#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_music_items_info.h"

// Sprites
#include "bn_sprite_items_spr_font_01.h"
#include "bn_regular_bg_items_bg_dialogue_box.h"

#include "bn_sprite_items_db_ch_jeremy.h"
#include "bn_sprite_items_db_ch_vista.h"
#include "bn_sprite_items_db_ch_visker.h"

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

vector<toast, 16> text::toasts;

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

text::text(const char *value, vector_2 start_) : start(start_)
{
    if (value)
        init(value);
}

text::text(const string<20> &value, vector_2 start_) : start(start_)
{
    reference = value;
    letters.clear();
    current_x = 0;
    index = 0;
}

void text::init(const char *value)
{
    if (!value)
    {
        reference.clear();
        letters.clear();
        current_x = 0;
        index = 0;
        return;
    }

    // Convert C-string to bn::string
    reference = string<20>(value);
    letters.clear();
    current_x = 0;
    index = 0;
}

void text::update(const bn::sprite_item *portrait = nullptr)
{
    if (index >= reference.size() || is_ended())
    {
        return;
    }

    if (index % 2 == 0)
    {
        if (portrait == &sprite_items::db_ch_vista)
        {
            sound_items::snd_dialogue_vista.play(0.7);
        }
        else if (portrait == &sprite_items::db_ch_jeremy)
        {
            sound_items::snd_dialogue_jeremy.play(0.7);
        }
        else if (portrait == &sprite_items::db_ch_visker)
        {
            sound_items::snd_dialogue_visker.play(0.4);
        }
        else
        {
            sound_items::snd_dialogue_generic.play(0.5);
        }
    }

    char ch = reference[index];

    int current_size = SIZE_DEFAULT;
    int spacing = 8; // Default portrait spacing

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

void text::render()
{
    do
    {
        int current_size = SIZE_DEFAULT;
        int spacing = 8; // Default portrait spacing

        // Adjust spacing based on size
        if (current_size == SIZE_LARGE)
        {
            spacing = 16; // Larger spacing for large text
        }
        else if (current_size == SIZE_SMALL)
        {
            spacing = 6;
        }

        char ch = reference[index];

        if (ch == ' ')
        {
            current_x += spacing;
            index++;
        }
        else
        {
            // Create letter at current position
            letter new_letter = {ch, {start.x + current_x, start.y}};

            if (new_letter.char_index > -1)
            {
                letters.push_back(new_letter);
                current_x += spacing;
                index++;
            }

            BN_LOG(reference.size());
        }

    } while (index < reference.size());
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
    portrait = sprite_items::db_ch_jeremy.create_sprite(0, 0);
    box = regular_bg_items::bg_dialogue_box.create_bg(0, 0);
    pointer = sprite_items::spr_font_01.create_sprite(-52, 32, 73);
    ticker = 0;
    active_conversation = nullptr;
    index = 0;
    size = 0;

    lines[0].start = {-40, 32};
    lines[1].start = {-40, 48};
    lines[2].start = {-40, 64};
}

void dialogue_box::load(conversation *new_conversation)
{
    active_conversation = new_conversation;
    index = 0;
    size = 0;

    for (int i = 0; i < 64; i++)
    {
        if ((*active_conversation)[i].action == ACT_END)
        {
            break;
        }
        size++;
    }
}

void dialogue_box::init(character_manager *ch_man)
{
    if (!active_conversation || is_ended())
    {
        return;
    }

    bool skip = false;

    do
    {
        skip = false;
        const dialogue_line &line = (*active_conversation)[index];

        if (line.portrait != nullptr)
        {
            portrait = line.portrait->create_sprite(-84, 56, line.emotion);
        }
        else
        {
            portrait.reset();
        }

        if (line.action == ACT_SPAWN)
        {
            ch_man->add_character(line.index, line.navigate, true);
        }

        auto ch = ch_man->find_by_index(line.index);
        if (ch)
        {
            if (line.navigate.x != 0 && line.navigate.y != 0)
            {
                if (line.action == ACT_TELEPORT)
                {
                    ch->v_sprite.bounds.position.x = line.navigate.x * 32;
                    ch->v_sprite.bounds.position.y = line.navigate.y * 32;
                }
                else if (line.action == ACT_WALK)
                {
                    ch->move_to = line.navigate;
                }
            }
            ch->idle_animation = line.anim;

            if (ch->idle_animation != nullptr)
            {
                BN_LOG(ch->idle_animation->size);
            }
        }

        for (int t = 0; t < 3; t++)
        {
            BN_LOG(line.raw_text[t]);
            lines[t].init(line.raw_text[t]);
            lines[t].size = line.size;
        }

        switch (line.action)
        {
        case ACT_SFX_KNOCK:
        {
            sound_items::sfx_knock.play();
            break;
        }
        case ACT_MUSIC_VISKER:
        {
            music::stop();
            music_items::theme_visker.play(0.75);
            break;
        }
        case ACT_MUSIC_GINGER:
        {
            music::stop();
            music_items::theme_ginger.play(0.75);
            break;
        }
        case ACT_MUSIC_FADEOUT:
        {
            break;
        }
        case ACT_MUSIC_RESET:
        {
            music::stop();
            global_data_ptr->bg_track->play(0.5);
            break;
        }
        case ACT_FIGHT:
        {
            auto pos = ch_man->player_ptr->v_sprite.bounds.position;
            global_data_ptr->entry_position = {pos.x.integer() / 32, pos.y.integer() / 32};
            global_data_ptr->battle_foe = line.index;
            ch_man->alert();
            break;
        }
        case ACT_PROGRESS:
        {
            global_data_ptr->process_stage = line.index;
            ch_man->status = NEW_CHAPTER;
            break;
        }
        case ACT_CUE_GINGER:
        {
            auto ginger = ch_man->find_by_index(CHAR_GINGER);
            ginger->is_follow = true;
            ginger->follow_id = CHAR_JEREMY;
            break;
        }
        default:
        {
            break;
        }
        }

        if (line.emotion == EM_SKIP)
        {
            skip = true;
            index++;
        }
    } while (skip);
}

void dialogue_box::update()
{
    if (!active_conversation || index >= size)
    {
        return;
    }

    auto l = (*active_conversation)[index];

    // Only update portrait if it exists and has a valid portrait pointer
    if (portrait.has_value() && l.portrait != nullptr)
    {
        portrait.value().set_tiles(l.portrait->tiles_item(), (l.emotion * 2));
    }

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

    // Speed controls go around here:

    if (portrait.has_value() && l.portrait != nullptr && ticker % 12 < 6)
    {
        portrait.value().set_tiles(l.portrait->tiles_item(), (l.emotion * 2) + 1);
    }

    if (ticker % 3 == 0)
    {
        for (int t = 0; t < 3; t++)
        {
            if (!lines[t].is_ended())
            {
                lines[t].update(l.portrait);
                return;
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

char digit_conv(int digit)
{
    return (char)((digit % 10) + 48);
}

void text::add_toast(string<20> value, vector_2 pos)
{
    toast new_toast;

    if (text::at_location(pos))
    {
        pos.y -= 16;
    }

    new_toast.value = text(value, pos);
    new_toast.ticker = 0;
    text::toasts.push_back(new_toast);

    // Render the new toast
    text::toasts.back().value.render();
}

void text::add_toast(int value, vector_2 pos)
{
    string<20> temp;

    // Handle zero case
    if (value == 0)
    {
        temp.push_back('0');
    }
    else
    {
        // Check if negative and store the sign
        bool is_negative = false;
        if (value < 0)
        {
            is_negative = true;
            value = -value; // Make it positive for digit extraction
        }

        // Extract digits and store them in reverse
        string<20> reversed;
        while (value > 0)
        {
            reversed.push_back(digit_conv(value));
            value = value / 10;
        }

        // Add negative sign first if needed
        if (is_negative)
        {
            temp.push_back('-');
        }

        // Now push the digits to temp in correct order
        for (int i = reversed.size() - 1; i >= 0; i--)
        {
            temp.push_back(reversed[i]);
        }
    }

    toast new_toast;

    if (text::at_location(pos))
    {
        pos.y -= 16;
    }

    new_toast.value = text(temp, pos);
    new_toast.ticker = 0;
    text::toasts.push_back(new_toast);

    // Render the new toast
    text::toasts.back().value.render();
}

bool text::at_location(vector_2 pos)
{
    for (auto &t : text::toasts)
    {
        if (t.value.start.x == pos.x && t.value.start.y == pos.y)
        {
            return true;
        }
    }
    return false;
}

void text::update_toasts()
{
    int delete_toast = -1;
    for (int t = 0; t < text::toasts.size(); t++)
    {
        text::toasts.at(t).ticker++;

        if (text::toasts.at(t).ticker > 96)
        {
            delete_toast = t;
        }
    }
    if (delete_toast > -1)
    {
        text::toasts.erase(text::toasts.begin() + delete_toast);
    }
}