#include "bn_log.h"

#include "ge_globals.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_actions.h"

int action_listener(map_manager *man, character_manager *ch_man)
{
    if (!ch_man->db.has_value())
    {
        for (auto &chara_ptr : ch_man->characters)
        {
            if (chara_ptr) // Check if the unique_ptr is valid
            {
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
                default:
                {
                    break;
                }
                }
            }
        }
    }

    return NONE;
}