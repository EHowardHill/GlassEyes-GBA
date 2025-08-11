#ifndef GE_BATTLE_H
#define GE_BATTLE_H

#include "bn_core.h"
#include "bn_log.h"
#include "bn_regular_bg_item.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_list.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_music_items_info.h"
#include "bn_sound_items.h"
#include "bn_math.h"

#include "bn_camera_ptr.h"
#include "bn_regular_bg_items_bg_battle_grid.h"
#include "bn_regular_bg_items_bg_battle_box.h"
#include "bn_regular_bg_items_bg_battle_action.h"
#include "bn_sprite_items_spr_jeremy_01.h"
#include "bn_sprite_items_spr_visker_01.h"
#include "bn_sprite_items_hearts.h"
#include "bn_sprite_items_battle_icons.h"
#include "bn_sprite_items_battle_chars.h"

#include "ge_text.h"

using namespace bn;

enum BATTLE_STAGE
{
    stage_heart_moving,
    stage_main,
    stage_talking,
    stage_recv,
    stage_status,
    stage_attack
};

enum BULLET_STYLE
{
    bullet_fall
};

enum STATUS_BAR_BUTTONS
{
    STATUS_BAR_NONE,
    STATUS_BAR_ATTACK,
    STATUS_BAR_ACT,
    STATUS_BAR_ITEM,
    STATUS_BAR_SPARE,
    STATUS_BAR_DEFEND,
    STATUS_BAR_SIZE
};

enum BATTLE_RESULT
{
    RESULT_FIRST,
    RESULT_UP,
    RESULT_DOWN,
    RESULT_SPARE,
    RESULT_LAST_WIN,
    RESULT_LAST_LOSE,
    RESULT_SIZE
};

struct attack_unit_dissolve
{
    optional<sprite_ptr> spr;
};

struct attack_bar
{
    optional<sprite_ptr> header;
    optional<sprite_ptr> recv_bar;
    optional<sprite_ptr> unit;
    vector<attack_unit_dissolve, 5> dissolves;
    bool is_ended = false;

    attack_bar(int y_delta, int index);
    void update();
};

struct attack
{
    vector<attack_bar, 3> attack_bars;
    bool is_ended = false;

    attack();
    void update();
};

struct status_bar_items
{
    optional<text> icon_label;
    int index = 0;

    status_bar_items();

    void update_label();
    void update();
};

struct status_bar_menu
{
    optional<sprite_ptr> battle_icons[5];
    optional<text> icon_label;
    int index = 0;

    status_bar_menu();

    void update_label();
    void update();
};

struct status_bar
{
    optional<sprite_ptr> char_img;
    optional<text> name;
    optional<text> hp;
    regular_bg_ptr action_bg = regular_bg_items::bg_battle_action.create_bg(0, 0);

    static int selected_menu;
    optional<status_bar_menu> sb_menu;
    optional<status_bar_items> sb_items;

    status_bar();
    void update();
};

struct recv
{
    int ticker = 0;
    vector<sprite_ptr, 64> bullets;
    int bullet_style = bullet_fall;
    regular_bg_ptr bg_action = regular_bg_items::bg_battle_box.create_bg(0, 0);
    vector_2 eye_pos = {0, 0};
    unsigned int random_value = 32;
    sprite_ptr heart = sprite_items::hearts.create_sprite(
        0,
        0,
        1);

    recv();
    void update();
    void spawn_bullets();
};

vector_2 moveTowards(vector_2 from, vector_2 towards, fixed_t<4> speed);
int battle_map();

#endif