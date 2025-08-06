#include "bn_log.h"

#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_actions.h"

void cue_dialogue(character_manager *ch_man, const dialogue_line *dl)
{
    ch_man->db.emplace();
    ch_man->db->load(&test_convo);
    ch_man->db->init(ch_man);
};

int action_listener(map_manager *man, character_manager *ch_man)
{
    if (!ch_man->db.has_value())
    {
        auto chara_ptr = ch_man->player_ptr;

        int action = man->action(chara_ptr->v_sprite.bounds.position);

        switch (action)
        {
        case CONVO_SCENE_1_INIT:
        {
            ch_man->db.emplace();
            ch_man->db->load(&test_convo);
            ch_man->db->init(ch_man);
            break;
        }
        case CONVO_GARBAGE_1_INIT:
        {
            if (global_data_ptr->action_iterations[CONVO_GARBAGE_1_INIT] == 0)
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_01);
                ch_man->db->init(ch_man);
            }
            break;
        }
        case CONVO_GARBAGE_2_INIT:
        {
            if (global_data_ptr->action_iterations[CONVO_GARBAGE_2_INIT] == 0)
            {
                ch_man->db.emplace();
                ch_man->db->load(&garbage_02);
                ch_man->db->init(ch_man);
            }
            break;
        }
        default:
        {
            break;
        }
        }

        if (action != 0)
        {
            global_data_ptr->action_iterations[action]++;
        }
    }

    return NONE;
}