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
#include "bn_sprite_items_battle_squares.h"

#include "ge_character_manager.h"
#include "ge_actions.h"
#include "ge_globals.h"
#include "ge_battle.h"

using namespace bn;

attack_bar::attack_bar(int y, int index)
{
    header = sprite_items::battle_chars.create_sprite(-42, y, index);
    recv_bar = sprite_items::battle_squares.create_sprite(-10, y, index);
    unit = sprite_items::battle_squares.create_sprite(64, y, 2);
};

void attack_bar::update()
{
    if (unit.has_value())
    {
        unit.value().set_x(unit.value().x() - 0.5);

        if (keypad::a_pressed())
        {

            int distance = abs(unit.value().x().integer() - recv_bar.value().x().integer());
            if (distance < 5)
            {
                global_data_ptr->enemy_hp[0] -= 5 - distance;
                text::add_toast((5 - distance) * -1, {96, -36});
            }

            BN_LOG(global_data_ptr->enemy_hp[0]);

            sound_items::snd_alert.play();
            is_ended = true;
        }

        if (unit.value().x() <= -42)
        {
            unit.reset();
            is_ended = true;
        }
    }
};

attack::attack()
{
    attack_bars.emplace_back(0, 0);
}

void attack::update()
{
    is_ended = true;
    for (auto &bar : attack_bars)
    {
        bar.update();
        if (!bar.is_ended)
            is_ended = false;
    }
    if (is_ended)
    {
        // status_bar::selected_menu = STATUS_BAR_NONE;
    }
}

int status_bar::selected_menu = STATUS_BAR_NONE;

status_bar_items::status_bar_items()
{
    icon_label = {"* No Items", {-22, -12 + (18 * 0)}};
    icon_label.value().render();
}

void status_bar_items::update()
{
    if (keypad::b_pressed())
    {
        sound_items::snd_alert.play();
        status_bar::selected_menu = STATUS_BAR_NONE;
    }
}

status_bar_menu::status_bar_menu()
{
    icon_label = {"ATTACK", {0, -12 + (18 * 0)}};
    icon_label.value().render();

    battle_icons[0] = sprite_items::battle_icons.create_sprite(-22, -12, 0);
    battle_icons[1] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * 1), 1);
    battle_icons[2] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * 2), 2);
    battle_icons[3] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * 3), 3);
    battle_icons[4] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * 4), 4);
}

void status_bar_menu::update_label()
{
    sound_items::snd_dialogue_generic.play();
    icon_label.reset();
    const char *label;
    switch (index + 1)
    {
    case STATUS_BAR_ATTACK:
    {
        label = "ATTACK";
        break;
    }
    case STATUS_BAR_ACT:
    {
        label = "ACT";
        break;
    }
    case STATUS_BAR_ITEM:
    {
        label = "ITEM";
        break;
    }
    case STATUS_BAR_SPARE:
    {
        label = "SPARE";
        break;
    }
    case STATUS_BAR_DEFEND:
    {
        label = "DEFEND";
        break;
    }
    default:
    {
        break;
    }
    }
    icon_label = {label, {0, -12 + (18 * index)}};
    icon_label.value().render();
};

void status_bar_menu::update()
{
    if (keypad::up_pressed())
    {
        index--;
        if (index < 0) // Check against 0, not STATUS_BAR_ATTACK
            index = 4; // Wrap to last item (DEFEND)
        update_label();
    }
    else if (keypad::down_pressed())
    {
        index++;
        if (index > 4)
            index = 0;
        update_label();
    }
    else if (keypad::a_pressed())
    {
        sound_items::snd_alert.play();
        status_bar::selected_menu = index + 1;
    }
}

status_bar::status_bar()
{
    char_img = sprite_items::battle_chars.create_sprite(-42, -42, 0);

    string<20> hp_counter = "HP:";
    hp_counter.push_back((char)((global_data_ptr->hp[0] / 10) + 48));
    hp_counter.push_back((char)((global_data_ptr->hp[0] % 10) + 48));
    hp_counter.push_back('/');
    hp_counter.push_back((char)((global_data_ptr->max_hp[0] / 10) + 48));
    hp_counter.push_back((char)((global_data_ptr->max_hp[0] % 10) + 48));

    name = {"JEREMY", {0, -52}};
    hp = {hp_counter, {0, -36}};

    name.value().render();
    hp.value().render();

    sb_menu.emplace();
};

void status_bar::update()
{
    if (sb_menu.has_value())
    {
        sb_menu.value().update();

        if (selected_menu != STATUS_BAR_NONE)
        {
            if (selected_menu == STATUS_BAR_ITEM)
            {
                sb_menu.reset();
                sb_items.emplace();
            }
        }
    }
    else if (sb_items.has_value())
    {
        sb_items.value().update();

        if (selected_menu == STATUS_BAR_NONE)
        {
            sb_items.reset();
            sb_menu.emplace();
        }
    }
};

vector_2 moveTowards(vector_2 from, vector_2 towards, fixed_t<4> speed)
{
    fixed_t<4> dx = towards.x - from.x;
    fixed_t<4> dy = towards.y - from.y;

    fixed_t<4> distance = sqrt(dx * dx + dy * dy);

    fixed_t<4> moveX = (dx / distance) * speed;
    fixed_t<4> moveY = (dy / distance) * speed;

    if (distance <= speed)
    {
        from.x = towards.x;
        from.y = towards.y;
    }
    else
    {
        from.x += moveX;
        from.y += moveY;
    }

    return from;
}

recv::recv()
{
    for (int t = 0; t < 32; t++)
    {
        auto bullet = sprite_items::hearts.create_sprite((random_value % 100) - 50, -64 - (t * 14));
        random_value = ((random_value + 17) * (random_value * 31));
        bullets.push_back(bullet);
    }
}

void recv::update()
{
    heart.set_position(eye_pos.x, eye_pos.y);

    int delete_at = -1;
    for (int t = 0; t < bullets.size(); t++)
    {
        auto bullet = &bullets.at(t);

        switch (bullet_style)
        {
        case bullet_fall:
        {
            bullet->set_y(bullet->y() + 1);
            break;
        }
        default:
        {
            break;
        }
        }

        if (abs(eye_pos.x - bullet->x().integer()) + abs(eye_pos.y - bullet->y().integer()) < 12)
        {
            sound_items::sfx_damage.play();
            global_data_ptr->hp[0] -= 2;
            text::add_toast(-2, {-96, -36});
            delete_at = t;
        }
    }
    if (delete_at != -1)
    {
        bullets.erase(bullets.begin() + delete_at);
    }

    eye_pos.x += keypad::right_held();
    eye_pos.x -= keypad::left_held();
    eye_pos.y += keypad::down_held();
    eye_pos.y -= keypad::up_held();
    if (eye_pos.x < -48)
        eye_pos.x = -48;
    if (eye_pos.x > 48)
        eye_pos.x = 48;
    if (eye_pos.y < -48)
        eye_pos.y = -48;
    if (eye_pos.y > 48)
        eye_pos.y = 48;

    ticker++;
}

int battle_map()
{
    int stage = stage_talking;
    int result = RESULT_FIRST;
    character_manager char_mgr;

    int player_ticker = 0;

    vector<conversation *, 64> convos[RESULT_SIZE];

    switch (global_data_ptr->battle_foe)
    {
    case FOE_VISKERS_01:
    {
        convos[RESULT_FIRST].push_back(&garbage_fight_01);
        convos[RESULT_UP].push_back(&garbage_fight_02);
        convos[RESULT_UP].push_back(&garbage_fight_03);
        convos[RESULT_LAST_WIN].push_back(&garbage_fight_04);
        break;
    }
    default:
    {
        break;
    }
    }

    sound_items::snd_fight_start.play();
    {
        sprite_ptr heart = sprite_items::hearts.create_sprite(
            global_data_ptr->entry_position_raw.x,
            global_data_ptr->entry_position_raw.y - 32,
            1);
        heart.set_z_order(-1);

        {
            sprite_ptr character[2] = {
                sprite_items::spr_jeremy_01.create_sprite(
                    global_data_ptr->entry_position_raw.x,
                    global_data_ptr->entry_position_raw.y - 32,
                    global_data_ptr->entry_direction * 2),
                sprite_items::spr_jeremy_01.create_sprite(
                    global_data_ptr->entry_position_raw.x,
                    global_data_ptr->entry_position_raw.y,
                    (global_data_ptr->entry_direction * 2) + 1)};

            for (int t = 0; t < 12; t++)
            {
                heart.set_visible(t % 4 >= 2);
                core::update();
            }
        }

        while (abs(heart.x()) <= 1 && abs(heart.y()) <= 1)
        {
            vector_2 new_pos = moveTowards({heart.x().integer(), heart.y().integer()}, {0, 0}, 2);
            heart.set_position(new_pos.x, new_pos.y);
            core::update();
        }
    }

    auto bg_grid = regular_bg_items::bg_battle_grid.create_bg(0, 0);

    vector_2 player_pos = {-96, 16};
    vector_2 enemy_pos = {96, 16};

    sprite_ptr player01 = sprite_items::jeremy_battle_intro.create_sprite(
        player_pos.x,
        player_pos.y,
        0);

    sprite_ptr enemy01[2] = {
        sprite_items::spr_visker_01.create_sprite(
            enemy_pos.x,
            enemy_pos.y - 32,
            0),
        sprite_items::spr_visker_01.create_sprite(
            enemy_pos.x,
            enemy_pos.y,
            1)};

    enemy01[0].set_horizontal_flip(true);
    enemy01[1].set_horizontal_flip(true);

    int y_delta = 0;
    bool conversation_in_progress = false;
    bool should_transition_to_recv = false;

    optional<recv> obj_recv;
    optional<status_bar> obj_status_bar;
    optional<attack> obj_attack;

    while (true)
    {
        bg_grid.set_position(bg_grid.x() - 1, bg_grid.y() - 1);

        if (player_ticker < (11 * 5))
        {
            player01.set_tiles(sprite_items::jeremy_battle_intro.tiles_item(), player_ticker / 5);
            player_ticker++;
        }

        player01.set_y(player_pos.y + y_delta);

        enemy01[0].set_y(player_pos.y + y_delta - 32);
        enemy01[1].set_y(player_pos.y + y_delta);

        if (char_mgr.db.has_value())
        {
            if (y_delta > -32)
            {
                y_delta -= 4;
            }
        }
        else
        {
            if (y_delta < 0)
            {
                y_delta += 4;
            }
        }

        switch (stage)
        {
        case stage_talking:
        {
            if (global_data_ptr->enemy_hp[0] <= 0)
            {
                result = RESULT_LAST_WIN;
            }
            else if (global_data_ptr->hp[0] <= 0)
            {
                result = RESULT_LAST_LOSE;
            }

            if (conversation_in_progress)
            {
                if (!char_mgr.db.has_value())
                {
                    conversation_in_progress = false;
                    should_transition_to_recv = true;
                }
            }
            else if (should_transition_to_recv)
            {
                stage = stage_recv;
                should_transition_to_recv = false;
            }
            else if (!char_mgr.db.has_value())
            {
                auto convo = &convos[result];

                if (convo->size() > 0)
                {
                    char_mgr.db.emplace();
                    char_mgr.db->load(convo->at(0));
                    char_mgr.db->init(&char_mgr);
                    convo->erase(convo->begin());
                    conversation_in_progress = true;
                }
                else
                {
                    should_transition_to_recv = true;
                }
            }
            break;
        }
        case stage_recv:
        {
            if (global_data_ptr->enemy_hp[0] <= 0)
            {
                return CONTINUE;
            }

            if (!obj_recv.has_value())
            {
                obj_recv.emplace();
            }
            obj_recv.value().update();

            if (obj_recv.value().ticker > 500)
            {
                obj_recv.reset();
                stage = stage_status;
            }
            break;
        }
        case stage_status:
        {
            if (!obj_status_bar.has_value())
            {
                obj_status_bar.emplace();
            }
            else
            {
                obj_status_bar.value().update();

                if (status_bar::selected_menu == STATUS_BAR_ATTACK)
                {
                    status_bar::selected_menu = STATUS_BAR_NONE;
                    obj_status_bar.reset();
                    stage = stage_attack;
                }
            }

            break;
        }
        case stage_attack:
        {
            if (!obj_attack.has_value())
            {
                obj_attack.emplace();
            }
            else
            {
                obj_attack.value().update();
                if (obj_attack.value().is_ended)
                {
                    obj_attack.reset();
                    stage = stage_talking;
                    result = RESULT_UP;
                }
            }
            break;
        }
        default:
        {
            break;
        }
        }

        text::update_toasts();
        char_mgr.update(nullptr);
        core::update();
    }
}
