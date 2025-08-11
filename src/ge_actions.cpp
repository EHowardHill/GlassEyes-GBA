#include "bn_log.h"

#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_actions.h"

#include "ge_map_data.cpp"

static vector_2 previous_tile = {-1, -1};
static bool action_triggered = false;
static bool buffer_active = false;

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
        bool is_interactive_action = (action == CONVO_GARBAGE_SIGN01 ||
                                      action == CONVO_VISKER_SIGN);

        // Reset action_triggered flag when entering a new tile
        if (new_tile)
        {
            action_triggered = false;
            previous_tile = current_tile;

            if (action != 0 && !is_interactive_action)
            {
                global_data_ptr->action_iterations[action]++;
            }
        }

        if (is_interactive_action && keypad::a_pressed() && !buffer_active)
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
            default:
            {
                break;
            }
            }
        }
        else if (action != 0 && !is_interactive_action && !action_triggered)
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
                else if (global_data_ptr->action_iterations[CONVO_GARBAGE_2] > 1)
                {
                    ch_man->db.emplace();
                    ch_man->db->load(&garbage_02b);
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