// main.cpp
#include "bn_core.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "ge_sprites.h"

// Sprites
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_items_spr_font_01.h"

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

#include "ge_structs.h"

struct text
{
    vector<sprite_ptr, 24> letters;
    vector_2 start;

    text(const string<24> &value, vector_2 start_ = {0, 0}) : start(start_)
    {
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

    // Method to update text position
    void set_position(int x, int y)
    {
        int current_x = x;
        for (auto &letter : letters)
        {
            letter.set_position(current_x, y);
            current_x += 8;
        }
    }

    // Method to hide/show text
    void set_visible(bool visible)
    {
        for (auto &letter : letters)
        {
            letter.set_visible(visible);
        }
    }
};

int main()
{
    core::init();

    // Create wizard sprite
    v_sprite_ptr wizard(&sprite_items::wizard);

    // Create some example text
    text hello_text("Hello GBA!", {-40, 0});

    while (true)
    {
        // Move wizard based on input
        wizard.move(
            {(keypad::left_held() ? -1 : 0) + (keypad::right_held() ? 1 : 0),
             (keypad::up_held() ? -1 : 0) + (keypad::down_held() ? 1 : 0)});

        v_sprite_ptr::update();
        core::update();
    }
}