// ge_text.h

#ifndef GE_TEXT_H
#define GE_TEXT_H

#include "bn_vector.h"
#include "bn_string.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "ge_structs.h"
#include "ge_sprites.h"

// Forward declaration to avoid circular dependency
struct character_manager;

using namespace bn;

enum actions
{
    ACT_DEFAULT,
    ACT_EMOTE,
    ACT_TURN,
    ACT_WALK,
    ACT_TELEPORT,
    ACT_SPAWN,
    ACT_MULTIPLE_CHOICE,
    ACT_END,
    ACT_SFX_KNOCK,
    ACT_MUSIC_VISKER,
    ACT_MUSIC_RESET
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
    EM_LAUGH,
    EM_EMBARRASSED,
    EM_ANNOYED,
    EM_WAT,
    EM_CONFUSED,
    EM_PAIN,
    EM_SKIP,
};

enum size
{
    SIZE_DEFAULT,
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
    string<20> reference;
    int size = SIZE_DEFAULT;

    text(const char *value = nullptr, vector_2 start_ = {0, 0});
    text(const string<20> &value, vector_2 start_ = {0, 0});

    void init(const char *value);
    void init(const string<20> &value);
    void update(const sprite_item * portrait);
    void render();
    void set_position(int x, int y);
    bool is_ended();
};

struct dialogue_line
{
    int id = 0;
    const sprite_item *portrait = nullptr;
    int emotion = EM_DEFAULT;
    int action = ACT_DEFAULT;
    const char *raw_text[3] = {nullptr, nullptr, nullptr};

    int branches[3] = {0, 0, 0};
    bool shake = false;
    int size = SIZE_DEFAULT;
    int speed = SP_DEFAULT;

    int index = 0;
    const animation *anim = nullptr;
    vector_2 navigate = {0, 0};

    constexpr dialogue_line(
        int id_ = 0,
        const sprite_item *char_ = nullptr,
        int emotion_ = EM_DEFAULT,
        int action_ = ACT_DEFAULT,
        const char *line1 = nullptr,
        const char *line2 = nullptr,
        const char *line3 = nullptr,
        bool shake_ = false,
        int size_ = SIZE_DEFAULT,
        int speed_ = SP_DEFAULT,
        int index_ = 0,
        const animation *anim_ = nullptr,
        vector_2 navigate_ = {0, 0}) : id(id_),
                                       portrait(char_),
                                       emotion(emotion_),
                                       action(action_),
                                       raw_text{line1, line2, line3},
                                       shake(shake_),
                                       size(size_),
                                       speed(speed_),
                                       index(index_),
                                       anim(anim_),
                                       navigate(navigate_)
    {
    }
};

typedef const dialogue_line conversation[128];

struct dialogue_box
{
    optional<sprite_ptr> portrait;
    optional<regular_bg_ptr> box;
    optional<sprite_ptr> pointer;
    int ticker;
    conversation *active_conversation;
    int index;
    int size;
    text lines[3];

    dialogue_box();
    void load(conversation *new_conversation);
    void init(character_manager *ch_man);
    void update();
    bool is_ended();
};

#endif // GE_TEXT_H