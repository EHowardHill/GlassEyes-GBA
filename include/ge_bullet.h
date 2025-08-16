// ge_bullet.h

#ifndef GE_BULLET_H
#define GE_BULLET_H

#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"

#define bullet_count 16

enum BULLET_ANIM_TYPES
{
    BULLET_FALL,
    BULLET_RISE,
    BULLET_ZIGZAG,
    BULLET_WAVE,
    BULLET_ACCEL,
    BULLET_BOUNCE,
    BULLET_HOMING,
    BULLET_ORBIT,
    BULLET_SPIRAL,
    BULLET_SIZE
};

struct bullet
{
    bn::optional<bn::sprite_ptr> item;
    int anim_type;
    int ticker = 0;

    // Motion state (kept simple / fixed-point friendly)
    bn::fixed vx = 0;
    bn::fixed vy = 0;
    bn::fixed ax = 0;         // for ACCEL
    bn::fixed amplitude = 16; // for ZIGZAG/WAVE
    int period = 48;          // frames per cycle
    bn::fixed base_x = 0;     // x anchor for WAVE
    int zig_dir = 1;          // direction flip for ZIGZAG
    bn::fixed min_x = -120;   // screen-ish bounds for BOUNCE
    bn::fixed max_x = 120;

    // Homing
    bn::fixed turn_rate = bn::fixed(0.04); // radians-ish per frame (tuned)
    bn::fixed max_speed = bn::fixed(2.0);

    // Orbit/Spiral (center + relative vector we rotate a tiny angle each frame)
    bn::fixed orbit_cx = 0;
    bn::fixed orbit_cy = 0;
    bn::fixed rel_x = 0;
    bn::fixed rel_y = 0;
    // Small rotation each frame: cos≈0.999, sin≈0.035 (≈2 degrees)
    static constexpr bn::fixed ROT_C = bn::fixed(0.999);
    static constexpr bn::fixed ROT_S = bn::fixed(0.035);
    bn::fixed radial_drift = 0; // <0 to spiral inward, >0 outward

    bullet(bn::fixed_point pos, int anim_type_);

    void update();
    static void populate(bn::vector<bullet, bullet_count> *bullets, int anim_type);

    // Set by your game each frame before update() if you use HOMING
    static bn::fixed_point s_target;
};

#endif
