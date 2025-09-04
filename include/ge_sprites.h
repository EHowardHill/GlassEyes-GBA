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
    optional<sprite_ptr> sprite_ptr_raw[2];
    bound bounds;
    int frame;

    v_sprite_ptr();
    v_sprite_ptr(const sprite_item *sprite_item_ptr_, vector_2 position = {0, 0}, int width = 32, int height = 32, int frame_ = 0);
    ~v_sprite_ptr();

    void move(vector_2 direction);
    bound real_position() const;
    void set_frame(int frame_);
    static void update(bool character_box_ended);

    static void clear_all()
    {
        for (auto *item : manager)
        {
            if (item->sprite_ptr_raw[0].has_value())
                item->sprite_ptr_raw[0].reset();
            if (item->sprite_ptr_raw[1].has_value())
                item->sprite_ptr_raw[1].reset();
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
    CHAR_CROKE,
    ITEM_BUTTON,
    ITEM_SPIKE,
    ITEM_X,
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
    CH_TYPE_NPC,
    CH_TYPE_ELEMENT,
    CH_TYPE_PUSHABLE,
    CH_TYPE_ITEM,
    CH_TYPE_DECORATION,
    CH_TYPE_DECORATION_ABOVE
};

struct character
{
    v_sprite_ptr v_sprite;
    const animation *current_animation;
    const animation *idle_animation;

    int id = 0;
    int index = CHAR_VISTA;
    int ticker = 0;
    int frame = 0;
    int face = DIR_DOWN;

    vector_2 move_to = {0, 0};
    bool is_follow = false;
    int follow_id = CHAR_JEREMY;

    bool is_sticky = false;
    bool is_pressed = false;

    character(int index_, vector_2 start_);

    void update(map_manager *current_map, bool character_box_ended);

    static void add(list<character, 32> *characters, int character_id, vector_2 location);

    int type()
    {
        switch (index)
        {
        case CHAR_VISTA:
            return CH_TYPE_PLAYER;
        case CHAR_JEREMY:
            return CH_TYPE_PLAYER;
        default:
            return CH_TYPE_NPC;
        }
    }
};

#endif // GE_SPRITES_H