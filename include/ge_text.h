// ge_text.h

#ifndef GE_TEXT_H
#define GE_TEXT_H

#include "bn_vector.h"
#include "bn_string.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "ge_structs.h"

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

struct text
{
    vector<sprite_ptr, 20> letters;
    vector_2 start;

    text(const string<20> &value, vector_2 start_ = {0, 0});

    void update(const string<20> &value);
    void set_position(int x, int y);
    void set_visible(bool visible);
};

struct dialogue_line
{
    int id = 0;
    const sprite_item *character;
    int emotion = EM_DEFAULT;
    int action = ACT_SPEAK;
    const string<20> raw_text[3];
    vector_2 navigate;
    int speed = SP_DEFAULT;
    int branches[3];

    static dialogue_line end_marker() {
        dialogue_line line;
        line.character = nullptr;
        line.action = ACT_END;
        return line;
    }

    // Default constructor
    dialogue_line() = default;

    // Full constructor
    dialogue_line(
        const sprite_item *char_,
        int emotion_ = EM_DEFAULT,
        int action_ = ACT_SPEAK,
        const string<20> &line1 = "",
        const string<20> &line2 = "",
        const string<20> &line3 = "") : character(char_),
                                        emotion(emotion_),
                                        action(action_),
                                        raw_text{line1, line2, line3}
    {
        navigate = {0, 0};
        speed = SP_DEFAULT;
    }

    // Full constructor
    dialogue_line(
        sprite_item *char_,
        int emotion_ = EM_DEFAULT,
        int action_ = ACT_SPEAK,
        const string<20> &line1 = "",
        const string<20> &line2 = "",
        const string<20> &line3 = "",
        vector_2 navigate_ = {0, 0},
        int speed_ = SP_DEFAULT,
        int branch1 = 0,
        int branch2 = 0,
        int branch3 = 0,
        int id_ = 0) : id(id_),
                       character(char_),
                       emotion(emotion_),
                       action(action_),
                       raw_text{line1, line2, line3},
                       navigate(navigate_),
                       speed(speed_),
                       branches{branch1, branch2, branch3}
    {
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
    optional<sprite_ptr> character;
    optional<sprite_ptr> pointer;
    optional<regular_bg_ptr> box;
    int size;

    dialogue_box();

    ~dialogue_box() {
        character.reset();
        pointer.reset();
        box.reset();
    }

    void load(conversation *new_conversation);
    void init();
    void update();
    bool is_ended();
};

#endif // GE_TEXT_H