// ge_battle_optimized.cpp - Optimized version with reduced memory usage

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_random.h"

#include "bn_regular_bg_items_bg_battle_grid.h"
#include "bn_regular_bg_items_bg_dialogue_box.h"
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

// Simplified global state
static vector<battle_action, 4> g_available_actions;

struct battle_state
{
    int party_size = 1;
    int current_actor = -1;
    int defense_stacks = 0;
    int selected_menu = STATUS_BAR_NONE;
    int stage = stage_talking;
    int result = RESULT_FIRST;
    int y_delta = 0;

    // Reusable dialogue state
    conversation *active_conv = nullptr;
    int dlg_index = 0;
    int dlg_size = 0;
    int dlg_ticker = 0;
    text dlg_lines[3] = {{nullptr, {-40, 32}}, {nullptr, {-40, 48}}, {nullptr, {-40, 64}}};
    optional<sprite_ptr> portrait;
    optional<regular_bg_ptr> bg_ptr;

    // Combat entities
    optional<sprite_ptr> player_sprite;
    optional<sprite_ptr> enemy_sprite;
    int player_state = 0;
    int enemy_state = 0;
    int player_ticker = 0;
    int enemy_ticker = 0;

    // Menu state
    int menu_index = 0;
    int act_index = 0;
    int act_count = 0;
    battle_action *actions[4] = {nullptr, nullptr, nullptr, nullptr};

    // Attack bar
    optional<sprite_ptr> attack_header;
    optional<sprite_ptr> attack_recv;
    optional<sprite_ptr> attack_unit;

    // Recv state
    optional<sprite_ptr> heart;
    vector_2 heart_pos = {0, 0};
    int recv_ticker = 0;
    vector<bullet, 16> bullets;

    // UI elements
    optional<sprite_ptr> char_img;
    optional<sprite_ptr> battle_icons[5];
    optional<text> labels[5];
};

// Helper functions
static bool is_alive(int idx, battle_state *bs)
{
    return idx >= 0 && idx < bs->party_size && global_data_ptr->hp[idx] > 0;
}

static int next_living(int from, battle_state *bs)
{
    for (int i = 1; i <= bs->party_size; ++i)
    {
        int idx = (from + i) % bs->party_size;
        if (is_alive(idx, bs))
            return idx;
    }
    return -1;
}

static int random_living(battle_state *bs)
{
    int cnt = 0, alive[4];
    for (int i = 0; i < bs->party_size; ++i)
    {
        if (is_alive(i, bs))
            alive[cnt++] = i;
    }
    return cnt > 0 ? alive[global_data_ptr->bn_random.get_int(0, cnt)] : -1;
}

static const char *get_name(int i)
{
    const char *names[] = {"JEREMY", "GINGER", "SEBELLUS", "VISTA"};
    return i < 4 ? names[i] : "ALLY";
}

// Dialogue functions
static void init_dialogue(conversation *conv, battle_state *bs)
{
    if (!bs->bg_ptr)
    {
        bs->bg_ptr = regular_bg_items::bg_dialogue_box.create_bg(0, 0);
    }

    bs->active_conv = conv;
    bs->dlg_index = 0;
    bs->dlg_size = 0;
    bs->dlg_ticker = 0;

    // Count lines
    for (int i = 0; i < 64 && (*conv)[i].action != ACT_END; ++i)
    {
        bs->dlg_size++;
    }

    if (bs->dlg_size > 0)
    {
        const auto &line = (*conv)[0];
        if (line.portrait)
        {
            bs->portrait = line.portrait->create_sprite(-84, 56, line.emotion);
        }

        for (int i = 0; i < 3; ++i)
        {
            bs->dlg_lines[i].init(line.raw_text[i]);
            bs->dlg_lines[i].size = line.size;
        }
    }
}

static void update_dialogue(battle_state *bs)
{
    if (!bs->active_conv || bs->dlg_index >= bs->dlg_size)
        return;

    const auto &line = (*bs->active_conv)[bs->dlg_index];

    // Update portrait animation
    if (bs->portrait && line.portrait)
    {
        bs->portrait->set_tiles(line.portrait->tiles_item(),
                                (line.emotion * 2) + (bs->dlg_ticker % 12 < 6));
    }

    // Update text
    if (bs->dlg_ticker % 3 == 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (!bs->dlg_lines[i].is_ended())
            {
                bs->dlg_lines[i].update(line.portrait, false);
                break;
            }
        }
    }

    // Update letter effects
    for (int i = 0; i < 3; ++i)
    {
        for (auto &letter : bs->dlg_lines[i].letters)
        {
            letter.update(line.shake, line.size);
        }
    }

    bs->dlg_ticker++;
}

static bool advance_dialogue(battle_state *bs)
{
    if (bs->dlg_index < bs->dlg_size - 1)
    {
        bs->dlg_index++;
        bs->dlg_ticker = 0;

        const auto &line = (*bs->active_conv)[bs->dlg_index];

        if (line.portrait)
        {
            bs->portrait = line.portrait->create_sprite(-84, 56, line.emotion);
        }
        else
        {
            bs->portrait.reset();
        }

        for (int i = 0; i < 3; ++i)
        {
            bs->dlg_lines[i].init(line.raw_text[i]);
            bs->dlg_lines[i].size = line.size;
        }
        return true;
    }
    return false;
}

static void clear_dialogue(battle_state *bs)
{
    bs->bg_ptr.reset();
    bs->portrait.reset();
    bs->active_conv = nullptr;
    bs->dlg_ticker = 0;
    for (int i = 0; i < 3; ++i)
    {
        bs->dlg_lines[i].letters.clear();
        bs->dlg_lines[i].index = 0;
        bs->dlg_lines[i].size = 0;
    }
}

static bool is_dialogue_active(battle_state *bs)
{
    return bs->active_conv && bs->dlg_index < bs->dlg_size;
}

static bool are_lines_complete(battle_state *bs)
{
    return bs->dlg_lines[0].is_ended() &&
           bs->dlg_lines[1].is_ended() &&
           bs->dlg_lines[2].is_ended();
}

// Main battle function
int battle_map()
{
    music::stop();
    music_items::boss.play();
    sound_items::snd_fight_start.play();

    auto bg_grid = regular_bg_items::bg_battle_grid.create_bg(0, 0);

    // Create and reset battle state
    battle_state bs;
    g_available_actions.clear();

    // Initialize sprites
    bs.player_sprite = sprite_items::jeremy_battle_intro.create_sprite(-96, 16, 0);
    bs.enemy_sprite = sprite_items::visker_battle_intro.create_sprite(96, 16, 0);
    bs.heart = sprite_items::hearts.create_sprite(0, 0, 1);

    // Setup conversations based on foe
    vector<conversation *, 3> convos[RESULT_SIZE];
    vector<conversation *, 3> spare_convos;
    conversation *act_conversation = nullptr;

    switch (global_data_ptr->battle_foe)
    {
    case FOE_VISKERS_01:
        global_data_ptr->enemy_max_hp[0] = 12;
        global_data_ptr->enemy_hp[0] = 12;
        global_data_ptr->enemy_allowed_moveset = 3;

        convos[RESULT_FIRST].push_back(&garbage_fight_01);
        convos[RESULT_UP].push_back(&garbage_fight_02);
        convos[RESULT_UP].push_back(&garbage_fight_03);
        convos[RESULT_LAST_WIN].push_back(&garbage_fight_04);

        g_available_actions.push_back({"Compliment", &garbage_fight_01});
        g_available_actions.push_back({"Joke", &garbage_fight_01});
        g_available_actions.push_back({"Sarcasm", &garbage_fight_01});

        spare_convos.push_back(&garbage_fight_02);
        spare_convos.push_back(&garbage_fight_03);
        spare_convos.push_back(&garbage_fight_04);
        break;

    case FOE_VISKERS_02:
        global_data_ptr->enemy_max_hp[0] = 99;
        global_data_ptr->enemy_hp[0] = 99;
        global_data_ptr->enemy_allowed_moveset = BULLET_SIZE;

        convos[RESULT_FIRST].push_back(&garbage_fight_05);
        g_available_actions.push_back({"Analyze", &garbage_fight_05});
        g_available_actions.push_back({"Threaten", &garbage_fight_05});
        break;

    default:
        break;
    }

    bool resume_status_after_talking = false;

    // Main game loop
    while (true)
    {
        bg_grid.set_position(bg_grid.x() - 1, bg_grid.y() - 1);

        // Update player animation
        int y_offset = 16 + bs.y_delta;
        switch (bs.player_state)
        {
        case 0: // INTRO
            if (bs.player_ticker < 55)
            {
                bs.player_sprite->set_tiles(sprite_items::jeremy_battle_intro.tiles_item(),
                                            bs.player_ticker / 5);
                bs.player_ticker++;
            }
            else
            {
                bs.player_state = 1;
                bs.player_ticker = 0;
            }
            break;

        case 2: // RECV
            if (bs.player_ticker < 20)
            {
                bs.player_sprite->set_tiles(sprite_items::jeremy_battle_intro.tiles_item(),
                                            (bs.player_ticker / 5) + 11);
                bs.player_ticker++;
            }
            else
            {
                bs.player_sprite->set_tiles(sprite_items::jeremy_battle_intro.tiles_item(), 11);
                bs.player_ticker = 55;
                bs.player_state = 1;
            }
            break;

        default:
            break;
        }
        bs.player_sprite->set_position(-96, y_offset);

        // Update enemy animation
        switch (bs.enemy_state)
        {
        case 0: // INTRO
            if (bs.enemy_ticker < 35)
            {
                bs.enemy_sprite->set_tiles(sprite_items::visker_battle_intro.tiles_item(),
                                           bs.enemy_ticker / 5);
                bs.enemy_ticker++;
            }
            else
            {
                bs.enemy_state = 1;
            }
            break;

        case 1: // IDLE
            bs.enemy_sprite->set_tiles(sprite_items::visker_battle_intro.tiles_item(),
                                       ((bs.enemy_ticker / 5) % 4) + 6);
            bs.enemy_ticker++;
            break;

        case 2: // ATTACK
            bs.enemy_sprite->set_tiles(sprite_items::visker_battle_intro.tiles_item(),
                                       ((bs.enemy_ticker / 5) % 6) + 10);
            bs.enemy_ticker++;
            break;

        default:
            break;
        }
        bs.enemy_sprite->set_position(96, y_offset);

        // Y offset for dialogue
        bs.y_delta += is_dialogue_active(&bs) ? (bs.y_delta > -32 ? -4 : 0) : (bs.y_delta < 0 ? 4 : 0);

        // Stage: Talking
        if (bs.stage == stage_talking)
        {
            if (global_data_ptr->enemy_hp[0] <= 0)
            {
                bs.result = RESULT_LAST_WIN;
            }

            if (!is_dialogue_active(&bs))
            {
                if (act_conversation)
                {
                    init_dialogue(act_conversation, &bs);
                    act_conversation = nullptr;
                }
                else if (bs.result < RESULT_SIZE && convos[bs.result].size() > 0)
                {
                    conversation *c = convos[bs.result].front();
                    convos[bs.result].erase(convos[bs.result].begin());
                    init_dialogue(c, &bs);
                }
                else if (resume_status_after_talking)
                {
                    resume_status_after_talking = false;
                    bs.stage = stage_status;
                }
                else
                {
                    bs.stage = stage_recv;
                }
            }
            else
            {
                update_dialogue(&bs);

                if (keypad::a_pressed() && are_lines_complete(&bs))
                {
                    if (!advance_dialogue(&bs))
                    {
                        clear_dialogue(&bs);
                    }
                }
            }
        }

        // Stage: Recv (enemy turn)
        else if (bs.stage == stage_recv)
        {
            bs.bg_ptr = regular_bg_items::bg_battle_box.create_bg(0, 0);

            if (!bs.heart.has_value())
                bs.heart = sprite_items::hearts.create_sprite(0, 0, 1);

            bs.enemy_state = 2;
            bs.enemy_ticker = 0;

            if (next_living(-1, &bs) < 0 || global_data_ptr->enemy_hp[0] <= 0)
            {
                music::stop();
                text::toasts.clear();
                return CONTINUE;
            }

            // Initialize bullets if needed
            if (bs.recv_ticker == 0)
            {
                int type = global_data_ptr->bn_random.get_int(0, global_data_ptr->enemy_allowed_moveset);
                bullet::populate(&bs.bullets, type);
            }

            // Update heart position
            bs.heart_pos.x += keypad::right_held() - keypad::left_held();
            bs.heart_pos.y += keypad::down_held() - keypad::up_held();

            // Clamp positions
            if (bs.heart_pos.x < -48)
                bs.heart_pos.x = -48;
            if (bs.heart_pos.x > 48)
                bs.heart_pos.x = 48;
            if (bs.heart_pos.y < -48)
                bs.heart_pos.y = -48;
            if (bs.heart_pos.y > 48)
                bs.heart_pos.y = 48;

            bs.heart->set_position(bs.heart_pos.x, bs.heart_pos.y);
            bs.heart->set_tiles(sprite_items::hearts.tiles_item(),
                                bs.defense_stacks > 0 ? 3 : 1);

            // Update bullets
            for (auto &b : bs.bullets)
            {
                b.update();

                if (b.item && b.item->visible())
                {
                    if (abs(bs.heart_pos.x - b.item->x()) +
                            abs(bs.heart_pos.y - b.item->y()) <
                        12)
                    {

                        if (bs.defense_stacks > 0)
                        {
                            bs.defense_stacks--;
                            sound_items::snd_alert.play();
                        }
                        else
                        {
                            int who = random_living(&bs);
                            if (who >= 0)
                            {
                                global_data_ptr->hp[who] -= 2;
                                text::add_toast(-2, {-96, -36});
                                sound_items::sfx_damage.play();
                            }
                        }
                        b.item->set_visible(false);
                    }
                }
            }

            if (++bs.recv_ticker > 250)
            {
                bs.recv_ticker = 0;
                bs.defense_stacks = 0;
                bs.current_actor = -1;
                bs.stage = stage_status;
            }
        }

        // Stage: Status (player menu)
        else if (bs.stage == stage_status)
        {
            bs.bg_ptr.reset();
            bs.enemy_state = 1;
            bs.enemy_ticker = 0;
            bs.heart.reset();
            bs.bullets.clear();

            if (next_living(-1, &bs) < 0)
            {
                music::stop();
                text::toasts.clear();
                return CONTINUE;
            }

            // Get next actor if needed
            if (!bs.char_img)
            {
                int next = next_living(bs.current_actor, &bs);
                if (next < 0)
                {
                    bs.stage = stage_talking;
                    continue;
                }

                bs.current_actor = next;
                bs.char_img = sprite_items::battle_chars.create_sprite(-42, -42, next);

                // Setup labels
                string<20> hp_str = "HP:";
                hp_str.push_back('0' + global_data_ptr->hp[next] / 10);
                hp_str.push_back('0' + global_data_ptr->hp[next] % 10);
                hp_str.push_back('/');
                hp_str.push_back('0' + global_data_ptr->max_hp[next] / 10);
                hp_str.push_back('0' + global_data_ptr->max_hp[next] % 10);

                bs.labels[0] = {get_name(next), {0, -52}};
                bs.labels[1] = {hp_str, {0, -36}};
                bs.labels[0]->render();
                bs.labels[1]->render();

                // Setup main menu
                bs.menu_index = 0;
                bs.selected_menu = STATUS_BAR_NONE;

                for (int i = 0; i < 5; ++i)
                {
                    bs.battle_icons[i] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * i), i);
                }

                const char *menu_labels[] = {"ATTACK", "ACT", "ITEM", "SPARE", "DEFEND"};
                bs.labels[2] = {menu_labels[0], {0, -12}};
                bs.labels[2]->render();
            }

            // Handle menu selection
            if (bs.selected_menu == STATUS_BAR_NONE)
            {
                if (keypad::up_pressed())
                {
                    bs.menu_index = (bs.menu_index + 4) % 5;
                    sound_items::snd_dialogue_generic.play();
                }
                else if (keypad::down_pressed())
                {
                    bs.menu_index = (bs.menu_index + 1) % 5;
                    sound_items::snd_dialogue_generic.play();
                }
                else if (keypad::a_pressed())
                {
                    sound_items::snd_alert.play();
                    bs.selected_menu = bs.menu_index + 1;
                }

                // Update label
                const char *menu_labels[] = {"ATTACK", "ACT", "ITEM", "SPARE", "DEFEND"};
                bs.labels[2].reset();
                bs.labels[2] = {menu_labels[bs.menu_index], {0, -12 + (18 * bs.menu_index)}};
                bs.labels[2]->render();
            }

            // Handle submenu: ACT
            else if (bs.selected_menu == STATUS_BAR_ACT)
            {
                // Initialize ACT menu
                if (bs.act_count == 0)
                {
                    bs.act_index = 0;
                    for (int i = 0; i < g_available_actions.size() && bs.act_count < 4; ++i)
                    {
                        if (!g_available_actions[i].used)
                        {
                            bs.actions[bs.act_count++] = &g_available_actions[i];
                        }
                    }

                    // Clear battle icons
                    for (int i = 0; i < 5; ++i)
                    {
                        bs.battle_icons[i].reset();
                    }
                }

                // Update ACT labels
                for (int i = 0; i < 4; ++i)
                {
                    bs.labels[2 + i].reset();
                    if (i < bs.act_count)
                    {
                        string<32> label = (i == bs.act_index) ? "> " : "* ";
                        label.append(bs.actions[i]->name);
                        bs.labels[2 + i] = {label, {-22, -12 + (18 * i)}};
                        bs.labels[2 + i]->render();
                    }
                }

                // Handle ACT input
                if (keypad::up_pressed() && bs.act_index > 0)
                {
                    bs.act_index--;
                    sound_items::snd_dialogue_generic.play();
                }
                else if (keypad::down_pressed() && bs.act_index < bs.act_count - 1)
                {
                    bs.act_index++;
                    sound_items::snd_dialogue_generic.play();
                }
                else if (keypad::a_pressed() && bs.act_count > 0)
                {
                    sound_items::snd_alert.play();
                    bs.actions[bs.act_index]->used = true;
                    act_conversation = bs.actions[bs.act_index]->convo;

                    // Clear UI
                    bs.char_img.reset();
                    for (int i = 0; i < 5; ++i)
                    {
                        bs.labels[i].reset();
                    }

                    bs.act_count = 0;
                    bs.selected_menu = STATUS_BAR_NONE;
                    bs.stage = stage_talking;
                    resume_status_after_talking = true;
                }
                else if (keypad::b_pressed())
                {
                    sound_items::snd_alert.play();
                    bs.act_count = 0;
                    bs.selected_menu = STATUS_BAR_NONE;

                    // Restore main menu icons
                    for (int i = 0; i < 5; ++i)
                    {
                        bs.battle_icons[i] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * i), i);
                    }
                }
            }

            // Handle other menu selections
            else if (bs.selected_menu == STATUS_BAR_ATTACK)
            {
                bs.char_img.reset();
                for (int i = 0; i < 5; ++i)
                {
                    bs.labels[i].reset();
                    bs.battle_icons[i].reset();
                }
                bs.stage = stage_attack;
            }
            else if (bs.selected_menu == STATUS_BAR_SPARE)
            {
                if (spare_convos.size() > 0)
                {
                    act_conversation = spare_convos.front();
                    spare_convos.erase(spare_convos.begin());

                    bs.char_img.reset();
                    for (int i = 0; i < 5; ++i)
                    {
                        bs.labels[i].reset();
                        bs.battle_icons[i].reset();
                    }

                    bs.stage = stage_talking;
                    resume_status_after_talking = true;
                }
                bs.selected_menu = STATUS_BAR_NONE;
            }
            else if (bs.selected_menu == STATUS_BAR_DEFEND)
            {
                bs.defense_stacks++;
                text::add_toast(0, {-96, -36});
                sound_items::snd_alert.play();

                bs.char_img.reset();
                for (int i = 0; i < 5; ++i)
                {
                    bs.labels[i].reset();
                    bs.battle_icons[i].reset();
                }
                bs.selected_menu = STATUS_BAR_NONE;
            }
            else if (bs.selected_menu == STATUS_BAR_ITEM)
            {
                // Simplified ITEM menu - just show "No Items"
                for (int i = 0; i < 5; ++i)
                {
                    bs.battle_icons[i].reset();
                }
                bs.labels[3].reset();
                bs.labels[3] = {"* No Items", {-22, -12}};
                bs.labels[3]->render();

                if (keypad::b_pressed())
                {
                    sound_items::snd_alert.play();
                    bs.selected_menu = STATUS_BAR_NONE;

                    for (int i = 0; i < 5; ++i)
                    {
                        bs.battle_icons[i] = sprite_items::battle_icons.create_sprite(-22, -12 + (18 * i), i);
                    }
                }
            }
        }

        // Stage: Attack
        else if (bs.stage == stage_attack)
        {
            if (!bs.attack_header)
            {
                bs.attack_header = sprite_items::battle_chars.create_sprite(-42, 0, 0);
                bs.attack_recv = sprite_items::battle_squares.create_sprite(-10, 0, 0);
                bs.attack_unit = sprite_items::battle_squares.create_sprite(64, 0, 2);
            }

            bs.attack_unit->set_x(bs.attack_unit->x() - 1.2);

            if (keypad::a_pressed())
            {
                int dist = abs(bs.attack_unit->x().integer() - bs.attack_recv->x().integer());
                if (dist < 5)
                {
                    global_data_ptr->enemy_hp[0] -= (5 - dist);
                    text::add_toast(-(5 - dist), {96, -36});
                }
                sound_items::snd_alert.play();

                bs.attack_header.reset();
                bs.attack_recv.reset();
                bs.attack_unit.reset();
                bs.stage = stage_talking;
            }
            else if (bs.attack_unit->x() <= -42)
            {
                bs.attack_header.reset();
                bs.attack_recv.reset();
                bs.attack_unit.reset();
                bs.stage = stage_talking;
            }
        }

        text::update_toasts();
        core::update();
    }
}