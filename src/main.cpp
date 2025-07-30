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
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '`',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '~',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '_',
    '+',
    '[',
    ']',
    '\\',
    '{',
    '}',
    '|',
    ';',
    '\'',
    ':',
    '"',
    ',',
    '.',
    '/',
    '<',
    '>',
    '?'};

struct text
{
    vector<sprite_ptr, 24> letters;
    int start_x = 0;
    int start_y = 0;

    text(const string & value) {
        for (const char ch : value) {
            for (const char al : alphabet) {
                if (ch == al) {
                    auto letter = spr_font_01.create_sprite(start_x, start_y);
                    start_x += 8;
                }
            }
        }
    }
};


int main()
{
    core::init();

    v_sprite_ptr wizard(&sprite_items::wizard);

    while (true)
    {
        wizard.move(
            {(keypad::right_held() * -1) + (keypad::left_held()),
             (keypad::down_held() * -1) + (keypad::up_held())});

        v_sprite_ptr::update();
        core::update();
    }
}