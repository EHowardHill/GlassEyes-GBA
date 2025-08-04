// ge_actions.h

#include "ge_text.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_animations.h"

using namespace bn;

#include "bn_sprite_items_db_ch_vista.h"

constexpr conversation test_convo = {
    {0, nullptr, EM_SKIP, ACT_DEFAULT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, &vista_reminisce, {2, 6}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "In her cabin,", "most days were", "just the same.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "She read her books", "and watched over her", "sawdust doll.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_SFX_KNOCK, "But today,", "there was a knock", "at the door.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_DEFAULT, ACT_DEFAULT, "Yes?", "Who is it?", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, &vista_pickup, {1, 3}},
    {0, &sprite_items::db_ch_vista, EM_SULLEN, ACT_DEFAULT, "Hmm,", "What could this be?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_HAPPY, ACT_DEFAULT, "\"Dear Vista,", "I don't know if you", "remember me..\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_SIDE_EYE, ACT_DEFAULT, "Let me think..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

int action_listener(map_manager *man, character_manager *ch_man);