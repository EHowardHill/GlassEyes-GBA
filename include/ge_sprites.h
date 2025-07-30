#ifndef GE_PRIMITIVES_H
#define GE_PRIMITIVES_H

#include "bn_core.h"
#include "bn_sprites.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_optional.h"
#include "bn_vector.h"

#include "ge_structs.h"

using namespace bn;

bool within_bounds(bound me, bound you);

struct v_sprite_ptr
{
    static vector<v_sprite_ptr*, 12> manager;
    static vector_2 camera;

    const sprite_item *sprite_item_ptr;
    optional<sprite_ptr> sprite_ptr_raw;
    bound bounds;
    int frame = 0;

    v_sprite_ptr(const sprite_item *sprite_item_ptr_, vector_2 position = {0, 0}, int width = 32, int height = 32, int frame_ = 0);
    ~v_sprite_ptr();

    void move(vector_2 direction);
    bound real_position() const;
    static void update();
};

#endif // GE_PRIMITIVES_H