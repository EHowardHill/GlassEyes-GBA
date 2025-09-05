#include "bn_log.h"
#include "bn_music.h"
#include "bn_music_items.h"

#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_actions.h"

#include "ge_map_data.h"

static vector_2 previous_tile = {-1, -1};
static bool action_triggered = false;
static bool buffer_active = false;

constexpr int is_interactive_array[999] = {
    CONVO_GARBAGE_SIGN01,
    CONVO_VISKER_SIGN,
    CONVO_GARBAGE_03_SIGN,
    CONVO_VISKER_AFTER_FIGHT,
    CONVO_GARBAGE_04_SIGN,
    CONVO_GARBAGE_05_SIGN,
    LAB_COMPUTER,
    CONVO_FOREST_01_SIGN,
    CONVO_FOREST_02_SIGN,
    CONVO_FOREST_03_SIGN,
    CONVO_COWBEMMIE_01_SIGN,
    TEMMIE_01,
    TEMMIE_02,
    TEMMIE_03,
    TEMMIE_04,
    TEMMIE_05,
    TEMMIE_06,
    TEMMIE_07,
    CONVO_CAVE_02_SIGN,
    CAVE_04_SIGN,
    -1};

bool is_interactive(int action)
{
    for (int t = 0; t < 999; t++)
    {
        if (is_interactive_array[t] == -1)
        {
            return false;
        }
        else if (is_interactive_array[t] == action)
        {
            return true;
        }
    }
    return false;
}

int action_listener(map_manager *man, character_manager *ch_man)
{
    if (!ch_man->db.has_value())
    {
        auto chara_ptr = ch_man->player_ptr;
        vector_2 current_position = chara_ptr->v_sprite.bounds.position;

        // Calculate current tile coordinates
        vector_2 current_tile = {
            current_position.x / 32,
            current_position.y / 32};

        // Check if we've moved to a new tile
        bool new_tile = (previous_tile.x != current_tile.x ||
                         previous_tile.y != current_tile.y);

        // Get the action for current position
        int action = man->action(current_position);

        // Reset action_triggered flag when entering a new tile
        if (new_tile)
        {
            action_triggered = false;
            previous_tile = current_tile;

            if (action != 0 && !is_interactive(action))
            {
                global_data_ptr->action_iterations[action]++;
            }
        }

        if (is_interactive(action) && keypad::a_pressed() && !buffer_active)
        {
            global_data_ptr->action_iterations[action]++;
            buffer_active = true;

            switch (action)
            {
            case CONVO_GARBAGE_SIGN01:
            {
                if (global_data_ptr->action_iterations[CONVO_GARBAGE_SIGN01] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_sign01);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[CONVO_GARBAGE_SIGN01] == 2)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_sign01b);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_VISKER_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_sign02);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_GARBAGE_03_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_sign03);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_VISKER_AFTER_FIGHT:
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_05b);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_GARBAGE_04_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_sign04);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_GARBAGE_05_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_sign05);
                ch_man->db->init(ch_man);
                break;
            }
            case LAB_COMPUTER:
            {
                if (global_data_ptr->action_iterations[LAB_COMPUTER] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_03);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[LAB_COMPUTER] == 2)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_03b);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[LAB_COMPUTER] == 3)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_03c);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[LAB_COMPUTER] == 4)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_03d);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[LAB_COMPUTER] == 5)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_03e);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[LAB_COMPUTER] == 6)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_03f);
                    ch_man->db->init(ch_man);
                }
                break;
            }

            case CONVO_FOREST_01_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&forest_dlg_sign_01);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_FOREST_02_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&forest_dlg_sign_02);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_FOREST_03_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&forest_dlg_sign_02);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_COWBEMMIE_01_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_sign);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_01:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_01);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_02:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_02);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_03:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_03);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_04:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_04);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_05:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_05);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_06:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_06);
                ch_man->db->init(ch_man);
                break;
            }
            case TEMMIE_07:
            {
                ch_man->db.emplace();
                ch_man->db->load(&temmie_07);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_CAVE_02_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&cave_sign_01);
                ch_man->db->init(ch_man);
                break;
            }
            case CAVE_04_SIGN:
            {
                ch_man->db.emplace();
                ch_man->db->load(&cave_sign_02);
                ch_man->db->init(ch_man);
                break;
            }
            default:
            {
                break;
            }
            }
        }
        else if (action != 0 && !is_interactive(action) && !action_triggered)
        {
            action_triggered = true; // Mark as triggered

            switch (action)
            {
            case CONVO_SCENE_1:
            {
                ch_man->db.emplace();
                ch_man->db->load(&test_convo);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_GARBAGE_1:
            {
                if (global_data_ptr->action_iterations[CONVO_GARBAGE_1] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_01);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_GARBAGE_2:
            {
                if (global_data_ptr->action_iterations[CONVO_GARBAGE_2] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_02);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_GARBAGE_3:
            {
                if (global_data_ptr->action_iterations[CONVO_GARBAGE_3] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_03);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case TO_GARBAGE_02:
            {
                global_data_ptr->entry_map = &map_garbage_02;
                global_data_ptr->entry_position = {2, 6};
                return 1;
                break;
            }
            case TO_GARBAGE_02b:
            {
                global_data_ptr->entry_map = &map_garbage_02;
                global_data_ptr->entry_position = {13, 2};
                return 1;
                break;
            }
            case TO_VISKERS_HOUSE:
            {
                global_data_ptr->entry_map = &map_viskers_house;
                global_data_ptr->entry_position = {3, 5};
                return 1;
                break;
            }
            case CONVO_VISKER_BED:
            {
                if (global_data_ptr->action_iterations[CONVO_VISKER_BED] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&visker_h_03);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_VISKER_FOOD:
            {
                if (global_data_ptr->action_iterations[CONVO_VISKER_FOOD] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&visker_h_01);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_VISKER_NIGHTSTAND:
            {
                if (global_data_ptr->action_iterations[CONVO_VISKER_NIGHTSTAND] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&visker_h_02);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case TO_GARBAGE_03:
            {
                global_data_ptr->entry_map = &map_garbage_03;
                global_data_ptr->entry_position = {1, 3};
                return 1;
                break;
            }
            case CONVO_VISKER_FIGHT:
            {
                if (global_data_ptr->action_iterations[CONVO_VISKER_FIGHT] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_04);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[CONVO_VISKER_FIGHT] == 2)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_05);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case TO_GARBAGE_04:
            {
                global_data_ptr->entry_map = &map_garbage_04;
                global_data_ptr->entry_position = {29, 4};
                return 1;
                break;
            }
            case GARBAGE_04_EXIT_01:
            {
                if (global_data_ptr->action_iterations[CONVO_GARBAGE_04_SIGN] == 0)
                {
                    global_data_ptr->entry_map = &map_garbage_03;
                    global_data_ptr->entry_position = {2, 8};
                    return 1;
                }
                else
                {
                    global_data_ptr->entry_map = &map_garbage_05;
                    global_data_ptr->entry_position = {4, 45};
                    return 1;
                }
                break;
            }
            case GARBAGE_04_EXIT_02:
            {
                global_data_ptr->entry_map = &map_garbage_04;
                global_data_ptr->entry_position = {10, 12};
                return 1;
                break;
            }
            case GARBAGE_04_EXIT_03:
            {
                global_data_ptr->entry_map = &map_garbage_04;
                global_data_ptr->entry_position = {2, 20};
                return 1;
                break;
            }
            case GARBAGE_04_EXIT_04:
            {
                global_data_ptr->entry_map = &map_garbage_04;
                global_data_ptr->entry_position = {29, 12};
                return 1;
                break;
            }
            case GARBAGE_04_EXIT_05:
            {
                global_data_ptr->entry_map = &map_garbage_04;
                global_data_ptr->entry_position = {29, 20};
                return 1;
                break;
            }
            case EXIT_GARBAGE_01:
            {
                music::stop();
                break;
            }
            case CONVO_GARBAGE_FINAL_01:
            {
                if (global_data_ptr->action_iterations[CONVO_GARBAGE_FINAL_01] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_06);
                    ch_man->db->init(ch_man);
                }
                else
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_06b);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_LAB_RUG:
            {
                if (global_data_ptr->action_iterations[CONVO_LAB_RUG] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_01);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case LAB_BED:
            {
                if (global_data_ptr->action_iterations[LAB_BED] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_02);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[LAB_BED] == 2)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_02b);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_LAB_GINGER:
            {
                if (global_data_ptr->action_iterations[CONVO_LAB_GINGER] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_05);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case LAB_DOOR:
            {
                if (global_data_ptr->action_iterations[LAB_DOOR] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_06);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case LAB_FRIDGE:
            {
                if (global_data_ptr->action_iterations[LAB_FRIDGE] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_07);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case LAB_LOCKER:
            {
                if (global_data_ptr->action_iterations[LAB_LOCKER] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&lab_08);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case TO_LAB_OUTSIDE:
            {
                ch_man->db.emplace();
                ch_man->db->load(&leaving_lab);
                ch_man->db->init(ch_man);
                break;
            }
            case CONVO_FOREST_01:
            {
                if (global_data_ptr->action_iterations[CONVO_FOREST_01] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&forest_dlg_02);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_FOREST_02:
            {
                if (global_data_ptr->action_iterations[CONVO_FOREST_02] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&forest_dlg_01);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_FOREST_03:
            {
                if (global_data_ptr->action_iterations[CONVO_FOREST_02] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&forest_dlg_03);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case CONVO_CROKE_01:
            {
                if (global_data_ptr->action_iterations[CONVO_CROKE_01] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&croke_01);
                    ch_man->db->init(ch_man);
                }
                else if (global_data_ptr->action_iterations[CONVO_CROKE_01] == 2)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&croke_05);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case TO_CAVE_01:
            {
                global_data_ptr->entry_map = &map_cave_01;
                global_data_ptr->entry_position = {7, 12};
                global_data_ptr->ginger_position = {6, 12};
                global_data_ptr->bg_track = nullptr;
                music::stop();
                return 1;
                break;
            }
            case FROM_CAVE_01:
            {
                global_data_ptr->entry_map = &map_forest_01;
                global_data_ptr->entry_position = {67, 3};
                global_data_ptr->ginger_position = {66, 3};
                global_data_ptr->bg_track = &music_items::forest_01;
                music::stop();
                return 1;
                break;
            }
            case CONVO_CAVE_01:
            {
                if (global_data_ptr->action_iterations[CONVO_CAVE_01] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&cave_01);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case TO_CAVE_02:
            {
                global_data_ptr->entry_map = &map_cave_02;
                global_data_ptr->entry_position = {3, 30};
                global_data_ptr->ginger_position = {2, 30};
                return 1;
                break;
            }
            case FOREST_01_SETUP:
            {
                auto croke = ch_man->find_by_index(CHAR_CROKE);
                if (croke != nullptr)
                {
                    ch_man->remove_character(croke);
                }
                break;
            }
            case CONVO_BRIDGE_02:
            {
                if (global_data_ptr->action_iterations[CONVO_BRIDGE_02] == 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&bridge_01);
                    ch_man->db->init(ch_man);
                }
                break;
            }
            case ENTER_BG:
            {
                if (man->bg_ptr.has_value())
                {
                    man->bg_ptr.value().set_visible(true);
                }
                break;
            }
            case EXIT_BG:
            {
                if (man->bg_ptr.has_value())
                {
                    man->bg_ptr.value().set_visible(false);
                }
                break;
            }
            case TO_FOREST_02:
            {
                global_data_ptr->entry_map = &map_forest_01;
                global_data_ptr->entry_position = {105, 3};
                global_data_ptr->ginger_position = {104, 3};
                return 1;
                break;
            }
            case ACT_55:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {19, 30};
                global_data_ptr->ginger_position = {20, 30};
                return 1;
                break;
            }
            case ACT_56:
            {
                global_data_ptr->entry_map = &map_cave_02;
                global_data_ptr->entry_position = {28, 3};
                global_data_ptr->ginger_position = {29, 3};
                return 1;
                break;
            }
            case ACT_57:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {16, 4};
                global_data_ptr->ginger_position = {15, 4};
                return 1;
                break;
            }
            case ACT_58:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {16, 16};
                global_data_ptr->ginger_position = {15, 16};
                return 1;
                break;
            }
            case ACT_59:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {30, 19};
                global_data_ptr->ginger_position = {31, 19};
                return 1;
                break;
            }
            case ACT_60:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {29, 3};
                global_data_ptr->ginger_position = {30, 3};
                return 1;
                break;
            }
            case ACT_61:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {12, 11};
                global_data_ptr->ginger_position = {11, 11};
                return 1;
                break;
            }
            case ACT_62:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {12, 21};
                global_data_ptr->ginger_position = {11, 21};
                return 1;
                break;
            }
            case ACT_63:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {4, 3};
                global_data_ptr->ginger_position = {5, 3};
                return 1;
                break;
            }
            case ACT_64:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {28, 11};
                global_data_ptr->ginger_position = {29, 11};
                return 1;
                break;
            }
            case ACT_65:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {2, 24};
                global_data_ptr->ginger_position = {2, 23};
                return 1;
                break;
            }
            case ACT_66:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {13, 8};
                global_data_ptr->ginger_position = {13, 7};
                return 1;
                break;
            }
            case ACT_67:
            {
                global_data_ptr->entry_map = &map_cave_cowbemmie;
                global_data_ptr->entry_position = {2, 3};
                global_data_ptr->ginger_position = {1, 3};
                global_data_ptr->bg_track = &music_items::bg_dixieland;
                music::stop();
                return 1;
                break;
            }
            case ACT_68:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {5, 23};
                global_data_ptr->ginger_position = {6, 23};
                global_data_ptr->bg_track = &music_items::forest_01;
                music::stop();
                return 1;
                break;
            }
            case BACK_TO_CAVE_03:
            {
                global_data_ptr->entry_map = &map_cave_03;
                global_data_ptr->entry_position = {5, 15};
                global_data_ptr->ginger_position = {4, 15};
                return 1;
                break;
            }
            case TO_FOREST_02b:
            {
                global_data_ptr->entry_map = &map_forest_02;
                global_data_ptr->entry_position = {2, 3};
                global_data_ptr->ginger_position = {1, 3};
                return 1;
                break;
            }
            case TO_FOREST_03:
            {
                global_data_ptr->entry_map = &map_forest_03;
                global_data_ptr->entry_position = {14, 15};
                global_data_ptr->ginger_position = {13, 15};
                return 1;
                break;
            }
            case TO_CAVE_04:
            {
                global_data_ptr->entry_map = &map_cave_04;
                global_data_ptr->entry_position = {16, 3};
                global_data_ptr->ginger_position = {15, 3};
                return 1;
                break;
            }
            case BACK_TO_FOREST_02:
            {
                global_data_ptr->entry_map = &map_forest_02;
                global_data_ptr->entry_position = {27, 3};
                global_data_ptr->ginger_position = {26, 3};
                return 1;
                break;
            }
            default:
            {
                break;
            }
            }
        }
        else
        {
            buffer_active = false;
        }
    }

    return NONE;
}