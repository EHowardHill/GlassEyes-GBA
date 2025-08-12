// ge_actions.h

#include "ge_text.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_animations.h"

using namespace bn;

#include "bn_sprite_items_db_ch_vista.h"
#include "bn_sprite_items_db_ch_visker.h"
#include "bn_sprite_items_db_ch_jeremy.h"

constexpr conversation test_convo = {
    {0, nullptr, EM_SKIP, ACT_WALK, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, &vista_reminisce, {2, 6}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "In her cabin,", "most days were", "just the same.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "She read her book", "and cared for her", "sawdust doll.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_SFX_KNOCK, "But today,", "there was a knock", "at the door.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_DEFAULT, ACT_WALK, "Yes?", "Who is it?", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, &vista_pickup, {1, 3}},
    {0, &sprite_items::db_ch_vista, EM_SULLEN, ACT_DEFAULT, "Hmm,", "What could this be?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_HAPPY, ACT_DEFAULT, "\"Dear Vista,", "I don't know if you", "remember me..\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_EMBARRASSED, ACT_DEFAULT, "Let me think..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_01 = {
    {0, nullptr, EM_SKIP, ACT_MUSIC_VISKER, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SKIP, ACT_DEFAULT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_JEREMY, &jeremy_lay_down, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "Hmm, he feels warm.", "", "Ewww.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, &visker_poke, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_PAIN, ACT_DEFAULT, "Ow, my head..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_DEFAULT, "DAGNABBIT", "First human down the", "chute in ages,", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_DEFAULT, "And he's ALIVE?", "Hey, could you do me", "a huge favor pal?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "..W-What?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_JEREMY, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Hurry up and die,", "I'm real real hungry", "for human right now.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_ANNOYED, ACT_DEFAULT, "Well, excuse ME.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "*sigh*", "It's okay.", "I forgive you.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_SAD, ACT_DEFAULT, "I need to get out", "of here.", "Where's the exit?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_CONFUSED, ACT_DEFAULT, "Why would I tell you", "I want you to die", "so I can eat you..?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "I tell you what.", "I'll give you a key", "on one condition.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Go to Avalon village", "and send back a", "turkey.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "That's for \"VISKER", "AT THE TRASH CHUTE\".", "Okay?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Uh.. Sure.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Write it down cos", "it's super hard.", "\"1234\".", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "..Got it.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_WALK, "Now if you'll excuse", "me, I'll be waiting", "for a DEAD human.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {2, 2}},
    {0, nullptr, EM_SKIP, ACT_MUSIC_RESET, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_02 = {
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Hey, you, uh..", "Everything okay?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Cool.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_02b = {
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Aaaaany moment now..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_03 = {
    {0, &sprite_items::db_ch_visker, EM_SKIP, ACT_TELEPORT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {14, 5}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_WALK, "HEY HEY HEY", "Where do you think", "YOU'RE going?", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {19, 6}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Home?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SIDE_EYE, ACT_DEFAULT, "You do understand", "how dangerous it is", "for a human, right?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..Yes?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_WAT, ACT_DEFAULT, "Muetos hate humans.", "They'll all want", "you dead!", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..like you?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "I..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SIDE_EYE, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_DEFAULT, "Well played.", "", "Proceed.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SKIP, ACT_DEFAULT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {2, 2}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_sign01 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "MT. GARBAGE", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "(No Littering)", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_sign01b = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Look, man,", "I'm a sign.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "I only say", "exactly one thing.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Not what you might", "call a", "conversationalist.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_sign02 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "VISKER'S HOUSE", "NO HUMANS ALLOWED!!", "UNLESS YOU'RE DEAD", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation visker_h_01 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "This pile is mostly", "take-away food.", "(He took it away)", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation visker_h_02 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "A pile of tabloids.", "Also Infinite Jest.", "All are dog-eared.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Under the desk..", "Oh dear..", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "You've never seen", "so many", "Reader's Digests.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation visker_h_03 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "His nest is made of", "discarded t-shirts.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Half of them are", "from the exact same", "family reunion.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_04 = {
    {0, &sprite_items::db_ch_visker, EM_SAD, ACT_WALK, "So you're pretty", "close to the exit,", "and I just, uh..", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {7, 8}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "I figured you didn't", "look like a FIGHTER,", "you know?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SULLEN, ACT_DEFAULT, "Since you'll be", "doing a lot of it", "anyways,", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_HAPPY, ACT_DEFAULT, "How 'bout we fight.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SIDE_EYE, ACT_DEFAULT, "If you win, you'll", "be good enough for", "the forest.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_EXCITED, ACT_DEFAULT, "If *I* win, then", "I get to eat you.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Win-win scenario.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I'm not really-", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "Great.", "It's settled then.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_01 = {
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "You see that?", "That's your", "\"THIRD EYE\".", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "It's a direct", "window to your soul.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "I'll go first 'cos", "I'm hungry as", "all get-out.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_02 = {
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "You seem pretty", "good at this, huh?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "Let's try THIS then!", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_03 = {
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_DEFAULT, "Ow", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_04 = {
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "Okay, fair's fair.", "You may proceed.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "But if you die", "out there..", "Remember,", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "All trash makes its", "way back to Visker.", "Eventually.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

int action_listener(map_manager *man, character_manager *ch_man);