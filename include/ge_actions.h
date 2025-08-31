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
#include "bn_sprite_items_db_ch_visker_wife.h"
#include "bn_sprite_items_db_ch_ginger.h"

constexpr conversation intro_01 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Deep within the", "woods of the", "old world,", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "There once", "lay a cabin.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Forgotten by", "the man who", "built it,", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Nearly forgotten", "by the woman", "who lived within,", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "The last of her", "kind, a Marelon,", "named Vista.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation test_convo = {
    {0, nullptr, EM_SKIP, ACT_WALK, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, &vista_reminisce, {2, 6}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "In her cabin,", "most days were", "just the same.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "She read her books", "and cared for her", "sawdust doll.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_SFX_KNOCK, "But today,", "there was a knock", "at the door.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_DEFAULT, ACT_WALK, "Yes?", "Who is it?", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISTA, &vista_pickup, {1, 3}},
    {0, &sprite_items::db_ch_vista, EM_SULLEN, ACT_DEFAULT, "Hmm,", "What could this be?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_HAPPY, ACT_DEFAULT, "\"Dear Vista,", "I don't know if you", "remember me..\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_DEFAULT, ACT_DEFAULT, "\"I was your", "daughter, years", "and years ago.\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_vista, EM_DEFAULT, ACT_DEFAULT, "I don't..", "remember..", "a daughter..\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_PROGRESS, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CUTSCENE_TO_GARBAGE, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation intro_02 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "\"I know it's hard,", "I know it's so hard,", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "But I need you", "to remember..\"", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation intro_03 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Cinemint", "presents", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
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
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_WALK, "Excuse me, human.", "I wait now..", "for DEAD human.", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {2, 2}},
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
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_WALK, "Human win", "this time.", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {2, 2}},
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
    {0, &sprite_items::db_ch_visker, EM_SAD, ACT_DEFAULT, "So.. Human, uh..", "Close close to", "garbage exit..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Human doesn't", "LOOK like fighter", "though.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "Since human is", "gonna fight..", "a lot..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_HAPPY, ACT_DEFAULT, "I fight human.", "First.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SIDE_EYE, ACT_DEFAULT, "If human wins,", "then good enough", "to go home.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "If *I* win, then", "I get yummy", "human meal.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "Win-win scenario.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I'm not really-", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "Yay.", "Let's fight now.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_FIGHT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, FOE_VISKERS_01, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_01 = {
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "You see that?", "That your", "\"THIRD EYE\".", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DEFAULT, ACT_DEFAULT, "It a direct", "window to your soul.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_EXCITED, ACT_DEFAULT, "I so so hungry.", "I go first.", "Mkay?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_02 = {
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "Human is good", "at this.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_SULLEN, ACT_DEFAULT, "But Human is not", "ready for", "NEXT ATTACK.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
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
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "..Okay, then.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_sign04 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "To escape, you must", "return to where", "you came from.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_sign05 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "ABANDON ALL HOPE,", "YE WHO ENTER", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_06 = {
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "Oh boy.", "You again.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker_wife, EM_HAPPY, ACT_DEFAULT, "Huh? Me?", "You not know me,", "human.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "Let me through.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_WALK, "OI! HUMAN!", "Human don't speak to", "my wife like that!!", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {5, 2}},
    {0, &sprite_items::db_ch_jeremy, EM_WAT, ACT_DEFAULT, "You're.. Oh, I see.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANGRY, ACT_DEFAULT, "I now need to fight", "human again for", "wife's honor.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "Ugh..", "I'm never getting", "out of here.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_FIGHT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, FOE_VISKERS_02, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_fight_05 = {
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "Look, just get out", "out of the way.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_ANNOYED, ACT_DEFAULT, "LA LA LA", "CAN'T HEAR YOU", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation garbage_06b = {
    {0, nullptr, EM_SKIP, ACT_DEFAULT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_JEREMY, &jeremy_lay_down, {0, 0}},
    {0, nullptr, EM_SKIP, ACT_TELEPORT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_VISKER, nullptr, {5, 2}},
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "Let's go home, wife.", "I'm hungry.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker_wife, EM_DEFAULT, ACT_DEFAULT, "Why not eat human.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_visker, EM_DISAPPOINTED, ACT_DEFAULT, "I'm angry at human.", "We eat human later.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_PROGRESS, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, GARBAGE_TO_BLACK, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_01 = {
    {0, nullptr, EM_SKIP, ACT_DEFAULT, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_JEREMY, &jeremy_lay_down, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_PAIN, ACT_DEFAULT, "Ugh..", "Everything hurts..", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_GINGER, &ginger_eat_lime, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_PAIN, ACT_DEFAULT, "Man, I am TIRED", "of waking up in", "weird places, ugh.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DISAPPOINTED, ACT_DEFAULT, "And why do I smell", "like..", "Limes?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_02 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Surely an empty cot", "would have been", "better than a rug?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_02b = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "You don't really", "feel tired anymore.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_03 = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "The outpost computer", "must have all sorts", "of secrets.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "The password", "being one of them.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_03b = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "If you want to spam", "random passwords,", "be my guest.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_03c = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Ok.. Let's try..", "", "\"NBOutpost1972\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "It didn't work.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_03d = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Ok.. Let's try..", "", "\"NBOutpost1971\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "It didn't work.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_03e = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Ok.. Let's try..", "", "\"NBOutpost1970\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "It didn't work.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_03f = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Ok.. Let's try..", "", "\"NBOutpost1969\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "LOGIN SUCCESSFUL", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_WAT, ACT_DEFAULT, "Oh crap", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Uh.. Let's see", "what's on here.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "31-Dec-69", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "FINAL LOG.", "Funding cuts are", "sending us home.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Unfortunately,", "we haven't been", "able to find them.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "In all honesty", "this whole year", "feels like it's been", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "a huge waste.", "", "Our prevailing", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "theory as to where", "new muetos come", "from is that", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "they \"pop out of", "the ground\". It's as", "good as any.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_04 = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "You know, once", "I finish my degree,", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "I might be able to", "understand what", "these books mean.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_05 = {
    {0, nullptr, EM_SKIP, ACT_MUSIC_RESET, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SKIP, ACT_MUSIC_GINGER, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_HAPPY, ACT_DEFAULT, "Oh! You're awake!", "Thank goodness.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_EMBARRASSED, ACT_DEFAULT, "You're not, like,", "super injured,", "are you?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Did you put", "lime juice on my", "cuts?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "Well yeah,", "it's antiseptic,", "right?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "It stings but it", "is not", "antiseptic.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "Oh.", "", "Uh..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "", "", "You want a lime?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Fine.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SIDE_EYE, ACT_DEFAULT, "I found you out", "near the garbage", "chute.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SAD, ACT_DEFAULT, "The rat who lives", "out there usually", "eats dead humans.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I've been made", "well-aware.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_HAPPY, ACT_DEFAULT, "I'm Ginger,", "by the way.", "", false, SIZE_DEFAULT, SP_DEFAULT, CHAR_GINGER, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_HAPPY, ACT_DEFAULT, "I'm heading to", "Canterbury.", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "I figured that since", "you were a human,", "you were too.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I really don't want", "to go that far..", "But you're right.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "They won't let me", "back into the city", "without an ID.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_HAPPY, ACT_DEFAULT, "Perfect! You, uh..", "know how to", "get there?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "You're a mueto,", "don't you?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SIDE_EYE, ACT_DEFAULT, "I guess we'll", "have to figure it", "out, huh?", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I guess.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SKIP, ACT_MUSIC_RESET, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_SKIP, ACT_CUE_GINGER, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_06 = {
    {0, &sprite_items::db_ch_ginger, EM_HAPPY, ACT_DEFAULT, "I've been hiding out", "in this old", "outpost lab.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SIDE_EYE, ACT_DEFAULT, "It's nice to have,", "y'know,", "running water.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "Clean bathrooms", "are hard to come by", "out here.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SAD, ACT_DEFAULT, "I'll admit I'm not", "thrilled about", "leaving.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_07 = {
    {0, &sprite_items::db_ch_ginger, EM_WAT, ACT_DEFAULT, "Oh- you don't", "want to open", "the fridge door.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Why not?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SIDE_EYE, ACT_DEFAULT, "The dehydrated", "limes were the only", "fresh things.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SAD, ACT_DEFAULT, "Everything else..", "Let's just say it", "has a smell.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "This place was", "probably abandoned", "for a year or two.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation lab_08 = {
    {0, &sprite_items::db_ch_jeremy, EM_ANGRY, ACT_DEFAULT, "Ugh!", "They emptied out", "the lockers.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation leaving_lab = {
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "So are you going", "to be following me?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "Do you not want", "me to?", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "..", "I don't know", "what I want.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Look.", "Today was going", "to be my first", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "day of college.", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_DEFAULT, ACT_DEFAULT, "Oh?", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I guess I should", "be grateful for", "being alive.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Let's go to", "Canterbury,", "just..", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "I would", "appreciate some", "space.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_ginger, EM_SAD, ACT_DEFAULT, "Right.", "", "Sorry.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, &sprite_items::db_ch_jeremy, EM_DEFAULT, ACT_DEFAULT, "Alright..", "", "Here goes nothing.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_PROGRESS, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, FOREST_01, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

constexpr conversation final_msg = {
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Thanks for", "trying out the", "demo so far!", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "I've only been", "working on this", "for a month,", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "so I'm sure I'll", "make lots of", "progress.", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_DEFAULT, "Discord / X:", "", "\"@cinemint\"", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}},
    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}};

int action_listener(map_manager *man, character_manager *ch_man);