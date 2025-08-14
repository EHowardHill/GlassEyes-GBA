// ge_bullet.cpp
/*
This is for Gameboy Advance, so the screen resolution is 240x160.
*/
#include "ge_bullet.h"
#include "ge_structs.h"
#include "bn_math.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_hearts.h"

using namespace bn;

unsigned int bullet::random_value = 32;
fixed_point bullet::s_target = fixed_point(0, 0);

static inline unsigned int rng_next(unsigned int v)
{
    // extremely tiny LCG-ish step (good enough for patterns)
    return (v * 1664525u + 1013904223u);
}

// fast triangle wave in [-amp, +amp] without trig
// t: frame ticker, period>0
static inline fixed tri_wave(int t, int period, fixed amp)
{
    if (period <= 0)
        return fixed(0);
    int m = t % period;
    int half = period / 2;
    int q = (m < half) ? m : (period - m);
    // scale q in [0, half] to [0, amp], then map to [-amp, +amp]
    fixed v = amp * fixed(q) / fixed(half > 0 ? half : 1);
    return (m < half) ? v : -v;
}

bullet::bullet(fixed_point pos, int anim_type_) : anim_type(anim_type_)
{
    item = sprite_items::hearts.create_sprite(pos.x(), pos.y(), 0);

    // basic defaults
    vx = 0;
    vy = 1;

    switch (anim_type)
    {
    case BULLET_FALL:
    {
        vx = 0;
        vy = 1;
        break;
    }
    case BULLET_RISE:
    {
        vx = 0;
        vy = -1;
        break;
    }
    case BULLET_ZIGZAG:
    {
        // downward with sharp left/right ramps (triangle wave sign)
        vy = fixed(1.2);
        amplitude = fixed(22); // horizontal swing
        period = 28 + int(random_value % 16);
        zig_dir = (random_value & 1) ? 1 : -1;
        base_x = pos.x();
        break;
    }
    case BULLET_WAVE:
    {
        // smooth side-to-side while falling
        vy = fixed(1);
        amplitude = fixed(32);
        period = 48 + int(random_value % 24);
        base_x = pos.x();
        break;
    }
    case BULLET_ACCEL:
    {
        // starts slow, speeds up
        vy = fixed(0.2);
        ax = fixed(0.05); // we’ll use ax to accelerate vy each frame
        max_speed = fixed(2.8);
        break;
    }
    case BULLET_BOUNCE:
    {
        // horizontal bouncing, slow vertical drift
        vy = fixed(0.8);
        vx = ((random_value & 1) ? fixed(1.2) : fixed(-1.2));
        min_x = fixed(-116);
        max_x = fixed(116);
        break;
    }
    case BULLET_HOMING:
    {
        // gentle homing toward s_target
        // initial small velocity so steering has something to work with
        vx = fixed(0.6) * ((random_value & 1) ? 1 : -1);
        vy = fixed(0.6);
        turn_rate = fixed(0.05);
        max_speed = fixed(2.2);
        break;
    }
    case BULLET_ORBIT:
    {
        // set orbit center near current pos; start at a small offset
        orbit_cx = pos.x();
        orbit_cy = pos.y();
        rel_x = fixed(24); // start offset to the right
        rel_y = fixed(0);
        radial_drift = fixed(0); // pure orbit
        break;
    }
    case BULLET_SPIRAL:
    {
        orbit_cx = pos.x();
        orbit_cy = pos.y();
        rel_x = fixed(36);
        rel_y = fixed(0);
        radial_drift = fixed(-0.20); // spiral inward
        break;
    }
    default:
    {
        break;
    }
    }
}

void bullet::update()
{
    if (!item.has_value())
        return;

    fixed x = item.value().x();
    fixed y = item.value().y();

    switch (anim_type)
    {
    case BULLET_FALL:
    {
        y += 1;
        break;
    }
    case BULLET_RISE:
    {
        y -= 1;
        break;
    }
    case BULLET_ZIGZAG:
    {
        // Move horizontally at constant speed in current direction
        x += zig_dir * fixed(2); // horizontal speed

        // Check if we've reached the amplitude limit
        if (bn::abs(x - base_x) >= amplitude)
        {
            // Clamp to amplitude and reverse direction
            x = base_x + zig_dir * amplitude;
            zig_dir = -zig_dir;
        }

        y += vy;
        break;
    }
    case BULLET_WAVE:
    {
        // smooth triangle wave (already smooth enough for 60fps)
        x = base_x + tri_wave(ticker, period, amplitude);
        y += vy;
        break;
    }
    case BULLET_ACCEL:
    {
        // accelerate downward, clamp to max_speed
        vy += ax;
        if (vy > max_speed)
            vy = max_speed;
        y += vy;
        x += vx; // (vx defaults 0, but you can set patterns)
        break;
    }
    case BULLET_BOUNCE:
    {
        x += vx;
        y += vy;
        if (x <= min_x)
        {
            x = min_x;
            vx = -vx;
        }
        if (x >= max_x)
        {
            x = max_x;
            vx = -vx;
        }
        break;
    }
    case BULLET_HOMING:
    {
        // vector toward target
        fixed tx = s_target.x() - x;
        fixed ty = s_target.y() - y;

        // normalize approx without sqrt: scale by max(|tx|,|ty|)
        fixed atx = bn::abs(tx);
        fixed aty = bn::abs(ty);
        fixed m = (atx > aty ? atx : aty);
        fixed ux = (m > 0 ? tx / m : fixed(0));
        fixed uy = (m > 0 ? ty / m : fixed(0));

        // steer slightly toward (ux, uy)
        vx = vx + turn_rate * (ux - vx);
        vy = vy + turn_rate * (uy - vy);

        // clamp speed
        fixed spd_m = bn::max(bn::abs(vx), bn::abs(vy));
        if (spd_m > max_speed)
        {
            vx = vx * (max_speed / spd_m);
            vy = vy * (max_speed / spd_m);
        }

        x += vx;
        y += vy;
        break;
    }
    case BULLET_ORBIT:
    case BULLET_SPIRAL:
    {
        // rotate relative vector by a tiny fixed angle:
        // [rel_x'] = [ c -s ][rel_x]
        // [rel_y']   [ s  c ][rel_y]
        fixed nx = ROT_C * rel_x - ROT_S * rel_y;
        fixed ny = ROT_S * rel_x + ROT_C * rel_y;

        // spiral drift (in/out)
        if (radial_drift != 0)
        {
            // nudge outward/inward a little along the current relative
            // (scale ~ unit by max(|nx|,|ny|) to avoid sqrt)
            fixed mm = bn::max(bn::abs(nx), bn::abs(ny));
            if (mm > 0)
            {
                nx += radial_drift * (nx / mm);
                ny += radial_drift * (ny / mm);
            }
        }

        rel_x = nx;
        rel_y = ny;

        x = orbit_cx + rel_x;
        y = orbit_cy + rel_y;
        break;
    }
    default:
        break;
    }

    item.value().set_x(x);
    item.value().set_y(y);
    ticker++;
}

void bullet::populate(vector<bullet, bullet_count> *bullets, int anim_type)
{
    for (int b = 0; b < bullet_count; b++)
    {
        switch (anim_type)
        {
        case BULLET_FALL:
        {
            fixed x_pos = (fixed(int(random_value % 200)) - fixed(100));
            fixed y_pos = fixed(-64 - (b * 14));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_RISE:
        {
            fixed x_pos = (fixed(int(random_value % 200)) - fixed(100));
            fixed y_pos = fixed(64 + (b * 14));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_ZIGZAG:
        case BULLET_WAVE:
        {
            fixed x_pos = (fixed(int(random_value % 200)) - fixed(100));
            fixed y_pos = fixed(-80 - (b * 10));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            // mild randomization
            nb.amplitude = fixed(16 + int(random_value % 24));
            nb.period = 28 + int((random_value >> 8) % 40);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_ACCEL:
        {
            fixed x_pos = (fixed(int(random_value % 200)) - fixed(100));
            fixed y_pos = fixed(-70 - (b * 12));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.vx = fixed(((int)(random_value & 3) - 1)); // -1..+2 -> -1..+1
            nb.ax = fixed(0.05 + (int((random_value >> 8) % 5)) * 0.01);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_BOUNCE:
        {
            fixed x_pos = (fixed(int(random_value % 200)) - fixed(100));
            fixed y_pos = fixed(-60 - (b * 12));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.vx = ((random_value & 1) ? fixed(1.2) : fixed(-1.2));
            nb.vy = fixed(0.9);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_HOMING:
        {
            fixed x_pos = (fixed(int(random_value % 200)) - fixed(100));
            fixed y_pos = fixed(-70 - (b * 8));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.turn_rate = fixed(0.05 + (int((random_value >> 8) % 4)) * 0.01); // 0.05..0.08
            nb.max_speed = fixed(1.8 + int((random_value >> 12) % 5) * 0.2);    // 1.8..2.6
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_ORBIT:
        case BULLET_SPIRAL:
        {
            // Create a simple ring that orbits its own local center, staggered
            fixed cx = (fixed(int(random_value % 120)) - fixed(60));
            fixed cy = fixed(-50 - (b * 6));
            bullet nb(fixed_point(cx, cy), anim_type);
            // set different starting angles around the ring
            int k = (b * 11) & 31;
            // place rel vector around a square-ish ring (no trig)
            fixed base = (anim_type == BULLET_SPIRAL) ? fixed(36) : fixed(24);
            switch (k % 4)
            {
            case 0:
                nb.rel_x = base;
                nb.rel_y = 0;
                break;
            case 1:
                nb.rel_x = 0;
                nb.rel_y = base;
                break;
            case 2:
                nb.rel_x = -base;
                nb.rel_y = 0;
                break;
            default:
                nb.rel_x = 0;
                nb.rel_y = -base;
                break;
            }
            if (anim_type == BULLET_SPIRAL)
            {
                nb.radial_drift = fixed(-0.20);
            }
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        default:
            break;
        }
    }
}
