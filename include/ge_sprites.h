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
};

enum character_list
{
    CHAR_DEFAULT,
    CHAR_VISTA,
    CHAR_JEREMY,
    CHAR_VISKER
};

enum facing
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
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

    bool npc = true;
    vector_2 move_to = {0, 0};

    character(int index_, vector_2 start_, bool npc_);

    void update(map_manager *current_map, bool character_box_ended);
    void update_sprite_item(int index_);
    bool is_tall() const; // Helper to determine if character is tall

    static void add(list<character, 64> *characters, int character_id, vector_2 location, bool npc);
    static character *find(list<character, 64> &characters, int index);
};

#endif // GE_SPRITES_H