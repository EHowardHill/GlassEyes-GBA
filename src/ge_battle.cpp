// ge_battle.cpp

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
#include "bn_vector.h"
#include "bn_random.h"

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
#include "ge_battle.h"
#include "ge_bullet.h"

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
        unit.value().set_x(unit.value().x() - 1.2);

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

status_bar_act::status_bar_act()
{
    // Initialize with empty state
    action_count = 0;
}

void status_bar_act::init(vector<battle_action, 4> &available_actions)
{
    action_count = 0;

    // Populate actions array with available (unused) actions
    for (int i = 0; i < available_actions.size() && action_count < 4; i++)
    {
        if (!available_actions[i].used)
        {
            actions[action_count] = &available_actions[i];
            action_count++;
        }
    }

    update_labels();
}

void status_bar_act::update_labels()
{
    // Clear all labels first
    for (int i = 0; i < 4; i++)
    {
        icon_labels[i].reset();
    }

    if (action_count == 0)
    {
        // Show "No Actions" if there are no available actions
        icon_labels[0] = {"* No Actions", {-22, -12}};
        icon_labels[0].value().render();
    }
    else
    {
        // Show available actions
        for (int i = 0; i < action_count; i++)
        {
            string<32> label_text = "* ";
            label_text.append(actions[i]->name);

            // Highlight current selection
            if (i == index)
            {
                label_text = "> ";
                label_text.append(actions[i]->name);
            }

            icon_labels[i] = {label_text, {-22, -12 + (18 * i)}};
            icon_labels[i].value().render();
        }
    }
}

void status_bar_act::update()
{
    if (action_count > 0)
    {
        if (keypad::up_pressed())
        {
            if (index > 0)
            {
                index--;
                sound_items::snd_dialogue_generic.play();
                update_labels();
            }
        }
        else if (keypad::down_pressed())
        {
            if (index < action_count - 1)
            {
                index++;
                sound_items::snd_dialogue_generic.play();
                update_labels();
            }
        }
        else if (keypad::a_pressed())
        {
            // Mark action as used and trigger it
            actions[index]->used = true;
            sound_items::snd_alert.play();
            status_bar::selected_menu = STATUS_BAR_ACT | (index << 8); // Encode action index
        }
    }

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
        if (index < 0)
            index = 4;
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
            else if (selected_menu == STATUS_BAR_ACT)
            {
                sb_menu.reset();
                sb_act.emplace();
                // Initialize ACT menu with available actions (handled in battle_map)
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
    else if (sb_act.has_value())
    {
        sb_act.value().update();

        if (selected_menu == STATUS_BAR_NONE)
        {
            sb_act.reset();
            sb_menu.emplace();
        }
        else if (selected_menu & STATUS_BAR_ACT)
        {
            // Action was selected, transition handled in battle_map
        }
    }
}

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
    int random_bullet_type = global_data_ptr->bn_random.get_int(0, global_data_ptr->enemy_allowed_moveset);
    bullet::populate(&bullets, random_bullet_type);
}

void recv::update()
{
    heart.set_position(eye_pos.x, eye_pos.y);

    for (int t = 0; t < bullets.size(); t++)
    {
        auto bullet = &bullets.at(t);
        bullet->update();

        if (bullet->item.value().visible())
        {
            if (abs(eye_pos.x - bullet->item.value().x().integer()) + abs(eye_pos.y - bullet->item.value().y().integer()) < 12)
            {
                sound_items::sfx_damage.play();
                global_data_ptr->hp[0] -= 2;
                text::add_toast(-2, {-96, -36});
                bullet->item.value().set_visible(false);
            }
        }
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

enum BATTLE_ANIM_TYPE
{
    INTRO,
    IDLE,
    ATTACK,
    RECV,
    DEFEND
};

int battle_map()
{
    music::stop();
    music_items::boss.play();

    int stage = stage_talking;
    int result = RESULT_FIRST;
    character_manager char_mgr;

    int player_ticker = 0;
    int enemy_ticker = 0;

    vector<conversation *, 32> convos[RESULT_SIZE];
    vector<battle_action, 4> battle_actions;
    conversation *pending_act_conversation = nullptr;

    switch (global_data_ptr->battle_foe)
    {
    case FOE_VISKERS_01:
    {
        global_data_ptr->enemy_max_hp[0] = 12;
        global_data_ptr->enemy_hp[0] = 12;
        global_data_ptr->enemy_allowed_moveset = 3;

        convos[RESULT_FIRST].push_back(&garbage_fight_01);
        convos[RESULT_UP].push_back(&garbage_fight_02);
        convos[RESULT_UP].push_back(&garbage_fight_03);
        convos[RESULT_LAST_WIN].push_back(&garbage_fight_04);

        battle_actions.emplace_back("Check", &garbage_fight_01);    // Example action
        battle_actions.emplace_back("Talk", &garbage_fight_02);     // Example action
        battle_actions.emplace_back("Threaten", &garbage_fight_03); // Example action
        break;
    }
    case FOE_VISKERS_02:
    {
        global_data_ptr->enemy_max_hp[0] = 99;
        global_data_ptr->enemy_hp[0] = 99;
        global_data_ptr->enemy_allowed_moveset = BULLET_SIZE;

        convos[RESULT_FIRST].push_back(&garbage_fight_05);
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

    sprite_ptr enemy01 = sprite_items::visker_battle_intro.create_sprite(
        enemy_pos.x,
        enemy_pos.y,
        0);

    int y_delta = 0;
    bool conversation_in_progress = false;
    bool should_transition_to_recv = false;

    optional<recv> obj_recv;
    optional<status_bar> obj_status_bar;
    optional<attack> obj_attack;

    int player_state = INTRO;
    int enemy_state = INTRO;

    while (true)
    {
        bg_grid.set_position(bg_grid.x() - 1, bg_grid.y() - 1);

        switch (player_state)
        {
        case INTRO:
        {
            if (player_ticker < (11 * 5))
            {
                player01.set_tiles(sprite_items::jeremy_battle_intro.tiles_item(), player_ticker / 5);
                player_ticker++;
            }
            else
            {
                player_state = IDLE;
            }
            break;
        }
        default:
        {
            break;
        }
        }

        player01.set_y(player_pos.y + y_delta);

        switch (enemy_state)
        {
        case INTRO:
        {
            if (enemy_ticker < (7 * 5))
            {
                enemy01.set_tiles(sprite_items::visker_battle_intro.tiles_item(), enemy_ticker / 5);
                enemy_ticker++;
            }
            else
            {
                enemy_state = IDLE;
            }
            break;
        }
        case IDLE:
        {
            enemy01.set_tiles(sprite_items::visker_battle_intro.tiles_item(), ((enemy_ticker / 5) % 4) + 6);
            enemy_ticker++;
            break;
        }
        case ATTACK:
        {
            enemy01.set_tiles(sprite_items::visker_battle_intro.tiles_item(), ((enemy_ticker / 5) % 6) + 10);
            enemy_ticker++;
            break;
        }
        default:
        {
            break;
        }
        }

        enemy01.set_y(enemy_pos.y + y_delta);

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
            enemy_state = ATTACK;

            if (global_data_ptr->enemy_hp[0] <= 0)
            {
                music::stop();
                text::toasts.clear();
                return CONTINUE;
            }

            if (!obj_recv.has_value())
            {
                obj_recv.emplace();
            }
            obj_recv.value().update();

            if (obj_recv.value().ticker > 250)
            {
                obj_recv.reset();
                stage = stage_status;
            }
            break;
        }
        case stage_status:
        {
            enemy_state = IDLE;

            if (global_data_ptr->hp[0] <= 0)
            {
                music::stop();
                text::toasts.clear();
                return CONTINUE;
            }

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
                else if (status_bar::selected_menu == STATUS_BAR_ACT)
                {
                    // Initialize ACT menu with available actions
                    if (obj_status_bar.value().sb_act.has_value())
                    {
                        obj_status_bar.value().sb_act.value().init(battle_actions);
                    }
                }
                else if (status_bar::selected_menu & STATUS_BAR_ACT)
                {
                    // An action was selected
                    int action_index = (status_bar::selected_menu >> 8) & 0xFF;
                    if (obj_status_bar.value().sb_act.has_value())
                    {
                        pending_act_conversation = obj_status_bar.value().sb_act.value().actions[action_index]->convo;
                    }
                    status_bar::selected_menu = STATUS_BAR_NONE;
                    obj_status_bar.reset();
                    stage = stage_act;
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
        case stage_act:
        {
            if (pending_act_conversation && !char_mgr.db.has_value())
            {
                char_mgr.db.emplace();
                char_mgr.db->load(pending_act_conversation);
                char_mgr.db->init(&char_mgr);
                pending_act_conversation = nullptr;
                stage = stage_talking;
                result = RESULT_ACT;
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
