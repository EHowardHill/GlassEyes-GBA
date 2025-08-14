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
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "It body warm.", "", "Ewww.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, &visker_poke, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_PAIN, ACT_DEFAULT, "Ow, my head..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_DEFAULT, "NO, NO LIKE,", "First human down the", "chute in ages,", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_DEFAULT, "And it ALIVE?", "Hey, do something", "for me, human.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "..W-What?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_JEREMY, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Go and DIE. NOW.", "I'm so so HUNGRY", "for HUMAN.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_ANNOYED, ACT_DEFAULT, "Well, excuse ME.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "*sigh*", "It okay.", "I forgive you.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_SAD, ACT_DEFAULT, "I need to get out", "of here.", "Where's the exit?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_CONFUSED, ACT_DEFAULT, "Why would I say?", "I want you to die", "so I eat you.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "..Okay, I let you", "go but you have to", "do me favor.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Go to Avalon.", "Send me TURKEY.", "Hungry for turkey.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Tell them it for", "\"VISKER\".", "\"King of TRASH\".", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Uh.. Sure.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_WALK, "Now if you'll excuse", "me, I'll be waiting", "for a DEAD human.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {2, 2}},
    {0, nullptr, EM_SKIP, ACT_MUSIC_RESET, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_02 = {
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Hey, you, uh..", "Everything okay?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Cool.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_02b = {
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Dead human fall..", "", "Any moment now..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_03 = {
    {0, &sprite_items::db_ch_visker, EM_SKIP, ACT_TELEPORT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {14, 5}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_WALK, "HEY HEY HEY", "Where YOU going?", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {19, 6}},
    {0, &sprite_items::db_ch_jeremy, EM_CONFUSED, ACT_DEFAULT, "Home?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SIDE_EYE, ACT_DEFAULT, "But it like,", "SO dangerous.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..and my", "other options", "are what?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_WAT, ACT_DEFAULT, "Muetos hate humans.", "They ALL want", "you dead.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..like you?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "I..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SIDE_EYE, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_DEFAULT, "Human win", "this time.", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {2, 2}},
    {0, nullptr, EM_SKIP, ACT_DEFAULT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
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
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "VISKER HOUSE", "NO HUMANS ALLOWED", "UNLESS you DEAD", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
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

constexpr conversation garbage_sign03 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "TO WHOEVER IS", "PAINTING ALL", "THESE POLKA DOTS", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "PAINT COSTS THE", "CITY MONEY", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "STOP IT", "THANK YOU", "MGMT", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_04 = {
    {0, &sprite_items::db_ch_visker, EM_SAD, ACT_WALK, "So.. Human, uh..", "Close close to", "garbage exit..", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Human doesn't", "LOOK like fighter", "though.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "Since human will", "got to fight", "a lot..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "I fight human.", "First.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {5, 8}},
    {0, &sprite_items::db_ch_visker, EM_SIDE_EYE, ACT_DEFAULT, "If human wins,", "then good enough", "to go home.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "If *I* win, then", "I get yummy", "human meal.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Win-win scenario.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I'm not really-", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "Yay.", "Let's fight now.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_FIGHT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_01 = {
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "You see that?", "That your", "\"THIRD EYE\".", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "It a direct", "window to your soul.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "I so so hungry.", "I go first.", "Mkay?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_02 = {
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "Human is good", "at this.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "But Human is not", "ready for", "THIS!", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_03 = {
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_DEFAULT, "Ow", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_04 = {
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_DEFAULT, "OW OW OW", "OKAY, HUMAN WIN", "HUMAN WIN", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_05 = {
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "Fine.", "Grrr.", "Human can go.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "But if human", "*dies*..", "Remember..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "All trash returns", "back to VISKER.", "KING of TRASH.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_05b = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Have anything", "else to say?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "What a weirdo..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

int action_listener(map_manager *man, character_manager *ch_man);