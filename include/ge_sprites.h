// ge_sprites.h

#ifndef GE_SPRITES_H
#define GE_SPRITES_H

#include "bn_core.h"
#include "bn_array.h"
#include "bn_sprites.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_optional.h"
#include "bn_vector.h"
#include "bn_keypad.h"
#include "bn_list.h"

#include "ge_structs.h"
#include "ge_maps.h"

using namespace bn;

bool within_bounds(bound me, bound you);

struct animation
{
    array<int, 64> frames = {0};
    int size = 0;
    int speed = 1;
    bool loop = true;
};

struct v_sprite_ptr
{
    static vector<v_sprite_ptr *, 12> manager;
    static vector_2 camera;

    const sprite_item *sprite_item_ptr;
    optional<sprite_ptr> sprite_ptr_raw;
    optional<sprite_ptr> sprite_ptr_bottom; // For tall sprites
    bound bounds;
    int frame;
    bool tall; // Whether this sprite is tall (two blocks)

    v_sprite_ptr();
    v_sprite_ptr(const sprite_item *sprite_item_ptr_, vector_2 position = {0, 0}, int width = 32, int height = 32, int frame_ = 0, bool tall_ = false);
    ~v_sprite_ptr();

    void move(vector_2 direction);
    bound real_position() const;
    void set_frame(int frame_);
    static void update(bool character_box_ended);

    static void clear_all()
    {
        for (auto *item : manager)
        {
            if (item->sprite_ptr_raw.has_value())
                item->sprite_ptr_raw.reset();
            if (item->sprite_ptr_bottom.has_value())
                item->sprite_ptr_bottom.reset();
        }
        manager.clear();
    }
};

enum character_list
{
    CHAR_DEFAULT,
    CHAR_VISTA,
    CHAR_JEREMY,
    CHAR_VISKER,
    CHAR_VISKER_WIFE,
    CHAR_GINGER,
    CHAR_TYPEWRITER
};

enum facing
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

enum character_type
{
    CH_TYPE_PLAYER,
    CH_TYPE_NPC
};

struct character
{
    int index = CHAR_VISTA;
    vector_2 start;

    v_sprite_ptr v_sprite;
    const animation *current_animation;
    const animation *idle_animation;
    int ticker = 0;
    int frame = 0;
    int face = DIR_DOWN;

    int type = CH_TYPE_NPC;
    vector_2 move_to = {0, 0};
    bool is_follow = false;
    int follow_id = CHAR_JEREMY;

    character(int index_, vector_2 start_, int character_type);

    void update(map_manager *current_map, bool character_box_ended);
    void update_sprite_item(int index_);
    bool is_tall() const; // Helper to determine if character is tall

    static void add(list<character, 64> *characters, int character_id, vector_2 location, int character_type);
    static character *find(list<character, 64> &characters, int index);
};

#endif // GE_SPRITES_H