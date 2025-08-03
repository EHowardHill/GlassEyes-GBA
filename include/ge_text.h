// ge_text.h

#ifndef GE_TEXT_H
#define GE_TEXT_H

#include "bn_vector.h"
#include "bn_string.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "ge_structs.h"
#include "ge_sprites.h"
#include "ge_character_manager.h"

using namespace bn;

enum actions
{
    ACT_SPEAK,
    ACT_EMOTE,
    ACT_TURN,
    ACT_WALK,
    ACT_TELEPORT,
    ACT_CUE_EVENT,
    ACT_MULTIPLE_CHOICE,
    ACT_END
};

enum speed
{
    SP_DEFAULT,
    SP_SLOW,
    SP_FAST,
    SP_SHAKE
};

enum emotion
{
    EM_DEFAULT,
    EM_HAPPY,
    EM_EXCITED,
    EM_ANGRY,
    EM_SAD,
    EM_DISAPPOINTED,
    EM_SULLEN,
    EM_SIDE_EYE,
    EM_EMBARRASSED,
    EM_LAUGH,
    EM_ANNOYED,
    EM_WAT
};

enum size
{
    SIZE_NORMAL,
    SIZE_SMALL,
    SIZE_LARGE
};

struct letter
{
    optional<sprite_ptr> sprite;
    vector_2 ideal_position;
    vector_2 temp_position;
    int char_index;

    letter(char ch, vector_2 ideal_position_);

    void update(bool shake, int size);
};

struct text
{
    vector<letter, 20> letters;
    vector_2 start;
    int index = 0;
    int current_x = 0;
    bool active = false;
    string<20> reference; // Changed from const reference to owned string
    int size = SIZE_NORMAL;

    text(const string<20> &value, vector_2 start_ = {0, 0});

    void init(const string<20> &value);
    void update(); // Added missing update declaration
    void set_position(int x, int y);
    bool is_ended();
};

struct dialogue_line
{
    int id = 0;
    const sprite_item *portrait = nullptr;
    int emotion = EM_DEFAULT;
    int action = ACT_SPEAK;
    string<20> raw_text[3]; // Removed const to allow assignment

    int index;
    const animation *spr_animation;
    vector_2 navigate = {0, 0};

    int speed = SP_DEFAULT;
    int branches[3] = {0, 0, 0};
    bool shake = false;
    int size = SIZE_NORMAL;

    static dialogue_line end_marker()
    {
        dialogue_line line;
        line.portrait = nullptr;
        line.action = ACT_END;
        return line;
    }

    // Default constructor
    dialogue_line() = default;

    // Constructor for simple dialogue
    dialogue_line(
        const sprite_item *char_,
        int emotion_ = EM_DEFAULT,
        int action_ = ACT_SPEAK,
        const string<20> &line1 = "",
        const string<20> &line2 = "",
        const string<20> &line3 = "",
        bool shake_ = false,
        int index_ = 0,
        vector_2 navigate_ = {0, 0},
        int size_ = SIZE_NORMAL) : portrait(char_),
                                   emotion(emotion_),
                                   action(action_),
                                   index(index_),
                                   spr_animation(nullptr),
                                   navigate(navigate_),
                                   speed(SP_DEFAULT),
                                   branches{0, 0, 0},
                                   shake(shake_),
                                   size(size_)
    {
        raw_text[0] = line1;
        raw_text[1] = line2;
        raw_text[2] = line3;
    }

    // Full constructor with all parameters
    dialogue_line(
        const sprite_item *char_,
        int emotion_,
        int action_,
        const string<20> &line1,
        const string<20> &line2,
        const string<20> &line3,
        int index_,
        const animation *spr_animation_,
        vector_2 navigate_,
        int speed_,
        int branch1,
        int branch2,
        int branch3,
        int id_) : id(id_),
                   portrait(char_),
                   emotion(emotion_),
                   action(action_),
                   index(index_),
                   spr_animation(spr_animation_),
                   navigate(navigate_),
                   speed(speed_),
                   branches{branch1, branch2, branch3}
    {
        raw_text[0] = line1;
        raw_text[1] = line2;
        raw_text[2] = line3;
    }
};

typedef const dialogue_line conversation[64];

struct dialogue_box
{
    text lines[3] = {
        {"", {-42, 32}},
        {"", {-42, 32 + 16}},
        {"", {-42, 32 + 16 + 16}}};
    conversation *active_conversation;
    int index;
    optional<sprite_ptr> portrait;
    optional<sprite_ptr> pointer;
    optional<regular_bg_ptr> box;
    int size;
    int ticker = 0;

    dialogue_box();

    ~dialogue_box()
    {
        portrait.reset();
        pointer.reset();
        box.reset();
    }

    void load(conversation *new_conversation);
    void init(character_manager * ch_man);
    void update();
    bool is_ended();
};

#endif // GE_TEXT_H