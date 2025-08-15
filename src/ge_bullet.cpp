// ge_bullet.cpp
/*
This is for Gameboy Advance, so the screen resolution is 240x160.
Character box is 128x128 (-64 to +64 in both x and y)
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
        // Easiest - simple downward movement
        vx = 0;
        vy = fixed(0.8); // Slower for easier dodging
        break;
    }
    case BULLET_RISE:
    {
        // Slightly harder - comes from below
        vx = 0;
        vy = fixed(-1.2); // Faster than fall for surprise factor
        break;
    }
    case BULLET_ZIGZAG:
    {
        // Medium - sharp horizontal movements while falling
        vy = fixed(1.5);
        amplitude = fixed(28);                // Increased for more aggressive movement
        period = 20 + int(random_value % 12); // Faster zigzag
        zig_dir = (random_value & 1) ? 1 : -1;
        base_x = pos.x();
        break;
    }
    case BULLET_WAVE:
    {
        // Medium - smooth sine-like movement
        vy = fixed(1.3);
        amplitude = fixed(40); // Wide sweeps
        period = 36 + int(random_value % 20);
        base_x = pos.x();
        break;
    }
    case BULLET_ACCEL:
    {
        // Medium-Hard - starts slow, speeds up significantly
        vy = fixed(0.3);
        ax = fixed(0.08);                              // Increased acceleration
        max_speed = fixed(3.5);                        // Higher max speed
        vx = fixed((int(random_value % 5) - 2) * 0.2); // Slight horizontal drift
        break;
    }
    case BULLET_BOUNCE:
    {
        // Hard - fast horizontal bouncing with vertical movement
        vy = fixed(1.4);                                      // Increased vertical speed
        vx = ((random_value & 1) ? fixed(2.0) : fixed(-2.0)); // Faster horizontal
        min_x = fixed(-60);                                   // Keep within character box
        max_x = fixed(60);
        break;
    }
    case BULLET_HOMING:
    {
        // Hard - aggressive homing toward player
        vx = fixed(0.8) * ((random_value & 1) ? 1 : -1);
        vy = fixed(0.8);
        turn_rate = fixed(0.08); // More aggressive turning
        max_speed = fixed(2.8);  // Faster max speed
        break;
    }
    case BULLET_ORBIT:
    {
        // Very Hard - orbiting bullets that descend
        orbit_cx = pos.x();
        orbit_cy = pos.y();
        rel_x = fixed(30); // Orbit radius
        rel_y = fixed(0);
        radial_drift = fixed(0);
        vy = fixed(1.2); // Add downward movement
        break;
    }
    case BULLET_SPIRAL:
    {
        // Hardest - spiraling inward while descending
        orbit_cx = pos.x();
        orbit_cy = pos.y();
        rel_x = fixed(45); // Start further out
        rel_y = fixed(0);
        radial_drift = fixed(-0.35); // Faster spiral inward
        vy = fixed(1.0);             // Downward movement
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
        y += vy;
        break;
    }
    case BULLET_RISE:
    {
        y += vy; // vy is negative, so this rises
        break;
    }
    case BULLET_ZIGZAG:
    {
        // Sharp zigzag with more aggressive movement
        x += zig_dir * fixed(3.5); // Increased horizontal speed

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
        // Smooth wave motion
        x = base_x + tri_wave(ticker, period, amplitude);
        y += vy;
        break;
    }
    case BULLET_ACCEL:
    {
        // Accelerate with both vertical and horizontal components
        vy += ax;
        if (vy > max_speed)
            vy = max_speed;
        y += vy;
        x += vx; // Slight drift adds unpredictability
        break;
    }
    case BULLET_BOUNCE:
    {
        x += vx;
        y += vy;
        // Bounce within character box bounds
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
        // More aggressive homing
        fixed tx = s_target.x() - x;
        fixed ty = s_target.y() - y;

        // Improved normalization
        fixed atx = bn::abs(tx);
        fixed aty = bn::abs(ty);
        fixed m = (atx > aty ? atx : aty);

        if (m > fixed(10)) // Only home if not too close
        {
            fixed ux = tx / m;
            fixed uy = ty / m;

            // Stronger steering
            vx = vx + turn_rate * (ux * 2 - vx);
            vy = vy + turn_rate * (uy * 2 - vy);
        }

        // Clamp speed
        fixed spd_sq = vx * vx + vy * vy;
        fixed max_sq = max_speed * max_speed;
        if (spd_sq > max_sq)
        {
            fixed scale = max_speed / bn::sqrt(spd_sq);
            vx = vx * scale;
            vy = vy * scale;
        }

        x += vx;
        y += vy;
        break;
    }
    case BULLET_ORBIT:
    {
        // Rotate while moving down
        fixed nx = ROT_C * rel_x - ROT_S * rel_y;
        fixed ny = ROT_S * rel_x + ROT_C * rel_y;

        rel_x = nx;
        rel_y = ny;

        // Move the orbit center downward
        orbit_cy += vy;

        x = orbit_cx + rel_x;
        y = orbit_cy + rel_y;
        break;
    }
    case BULLET_SPIRAL:
    {
        // Rotate and spiral inward while descending
        fixed nx = ROT_C * rel_x - ROT_S * rel_y;
        fixed ny = ROT_S * rel_x + ROT_C * rel_y;

        // Spiral inward
        if (radial_drift != 0)
        {
            fixed mm = bn::max(bn::abs(nx), bn::abs(ny));
            if (mm > fixed(3)) // Don't collapse completely
            {
                nx += radial_drift * (nx / mm);
                ny += radial_drift * (ny / mm);
            }
        }

        rel_x = nx;
        rel_y = ny;

        // Move center downward
        orbit_cy += vy;

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
            // Spawn across full width, staggered heights
            fixed x_pos = (fixed(int(random_value % 120)) - fixed(60));
            fixed y_pos = fixed(-70 - (b * 20)); // Well-spaced
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_RISE:
        {
            // Spawn from below, across width
            fixed x_pos = (fixed(int(random_value % 120)) - fixed(60));
            fixed y_pos = fixed(70 + (b * 16));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_ZIGZAG:
        {
            // Spawn near top but not too high
            fixed x_pos = (fixed(int(random_value % 100)) - fixed(50));
            fixed y_pos = fixed(-65 - (b * 12));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.amplitude = fixed(20 + int(random_value % 20));
            nb.period = 18 + int((random_value >> 8) % 24);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_WAVE:
        {
            // Spawn across width with varied waves
            fixed x_pos = (fixed(int(random_value % 100)) - fixed(50));
            fixed y_pos = fixed(-65 - (b * 14));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.amplitude = fixed(25 + int(random_value % 30));
            nb.period = 30 + int((random_value >> 8) % 30);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_ACCEL:
        {
            // Spawn in threatening positions
            fixed x_pos = (fixed(int(random_value % 100)) - fixed(50));
            fixed y_pos = fixed(-60 - (b * 10));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.vx = fixed((int(random_value & 7) - 3) * 0.15); // -0.45 to +0.45
            nb.ax = fixed(0.06 + (int((random_value >> 8) % 6)) * 0.015);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_BOUNCE:
        {
            // Start from varied positions
            fixed x_pos = (fixed(int(random_value % 100)) - fixed(50));
            fixed y_pos = fixed(-55 - (b * 10));
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.vx = ((random_value & 1) ? fixed(2.2) : fixed(-2.2));
            nb.vy = fixed(1.2 + (int(random_value >> 8) % 4) * 0.1);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_HOMING:
        {
            // Spawn from edges for more aggressive approach angles
            int side = random_value % 4;
            fixed x_pos, y_pos;
            if (side < 2)
            {
                x_pos = (side == 0) ? fixed(-60) : fixed(60);
                y_pos = fixed(-50 - (b * 8));
            }
            else
            {
                x_pos = (fixed(int(random_value % 80)) - fixed(40));
                y_pos = fixed(-65 - (b * 6));
            }
            bullet nb(fixed_point(x_pos, y_pos), anim_type);
            nb.turn_rate = fixed(0.07 + (int((random_value >> 8) % 5)) * 0.015);
            nb.max_speed = fixed(2.2 + int((random_value >> 12) % 5) * 0.15);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_ORBIT:
        {
            // Create orbiting patterns that move down
            fixed cx = (fixed(int(random_value % 80)) - fixed(40));
            fixed cy = fixed(-60 - (b * 8));
            bullet nb(fixed_point(cx, cy), anim_type);
            // Distribute around circle
            int angle_step = (b * 360) / bullet_count;
            nb.rel_x = fixed(30) * ((angle_step < 180) ? 1 : -1);
            nb.rel_y = fixed(30) * (((angle_step % 180) < 90) ? 1 : -1);
            nb.vy = fixed(1.0 + (int(random_value % 4)) * 0.1);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        case BULLET_SPIRAL:
        {
            // Create threatening spiral patterns
            fixed cx = (fixed(int(random_value % 60)) - fixed(30));
            fixed cy = fixed(-55 - (b * 6));
            bullet nb(fixed_point(cx, cy), anim_type);
            // Start at different angles
            int angle_step = (b * 360) / bullet_count;
            fixed radius = fixed(40 - (b * 2)); // Varied starting radius
            nb.rel_x = radius * ((angle_step < 180) ? 1 : -1);
            nb.rel_y = radius * (((angle_step % 180) < 90) ? 1 : -1);
            nb.radial_drift = fixed(-0.25 - (int(random_value % 3)) * 0.05);
            nb.vy = fixed(0.8 + (int(random_value >> 8) % 4) * 0.1);
            random_value = rng_next(random_value);
            bullets->push_back(nb);
            break;
        }
        default:
            break;
        }
    }
}