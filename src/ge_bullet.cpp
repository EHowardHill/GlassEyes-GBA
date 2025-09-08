// ge_bullet.cpp - Optimized implementation with improved spread
#include "ge_bullet.h"
#include "ge_globals.h"
#include "bn_sprite_items_hearts.h"
#include "bn_math.h"

using namespace bn;

const bullet_pattern PATTERNS[BULLET_SIZE] = {
    // vx    vy   amp  per  acc  flags
    {0, 2, 0, 0, 0, 0x00},  // BULLET_FALL
    {0, -2, 0, 0, 0, 0x00}, // BULLET_RISE
    {0, 2, 2, 2, 0, 0x00},  // BULLET_WAVE - flag 0x02 = use wave
    {0, 2, 0, 0, 3, 0x04}, // BULLET_ACCEL - flag 0x04 = accelerate
    {2, 2, 0, 0, 0, 0x08},  // BULLET_BOUNCE - flag 0x08 = bounce
    {2, 2, 0, 0, 0, 0x10},  // BULLET_HOMING - flag 0x10 = home
};

const int SINE_TABLE[64] = {
    0, 6, 12, 18, 24, 30, 35, 40, 45, 49, 53, 56, 59, 61, 63, 64,
    64, 64, 63, 61, 59, 56, 53, 49, 45, 40, 35, 30, 24, 18, 12, 6,
    0, -6, -12, -18, -24, -30, -35, -40, -45, -49, -53, -56, -59, -61, -63, -64,
    -64, -64, -63, -61, -59, -56, -53, -49, -45, -40, -35, -30, -24, -18, -12, -6};

bullet::bullet(int16_t x_pos, int16_t y_pos, uint8_t anim_type)
{
    compact.x = x_pos;
    compact.y = y_pos;
    compact.ticker = 0;
    compact.type = anim_type;
    compact.state1 = 0;
    compact.state2 = 0;

    // Create sprite at actual position
    item = sprite_items::hearts.create_sprite(
        fixed(x_pos) / 4,
        fixed(y_pos) / 4,
        4);

    // Initialize type-specific state
    switch (anim_type)
    {
    case BULLET_BOUNCE:
        compact.state1 = (global_data_ptr->bn_random.get_int(2) ? 1 : -1); // x direction
        break;
    case BULLET_HOMING:
        // Initialize with a random target position across the screen width
        compact.state1 = global_data_ptr->bn_random.get_int(-50, 50); // Target x (-50 to 50 in normal coords)
        compact.state2 = 0;                                           // Frame counter for target switching
        break;
    default:
        break;
    }
}

void bullet::update()
{
    if (!item.has_value())
        return;

    const bullet_pattern &pattern = PATTERNS[compact.type];

    // Get current position
    int16_t x = compact.x;
    int16_t y = compact.y;

    // Apply movement based on type
    uint8_t flags = pattern.flags;

    if (flags & 0x02) // WAVE
    {
        // Use sine table for smooth wave - keep original implementation as it works well
        uint8_t phase = (compact.ticker * 64) / pattern.period;
        int8_t sine = SINE_TABLE[phase & 63];
        x = compact.x + (sine * pattern.amplitude) / 32;
        y += pattern.vy_base;
    }
    else if (flags & 0x04) // ACCEL
    {
        // Accelerating bullet - keep original
        int16_t current_vy = pattern.vy_base + (compact.ticker * pattern.accel) / 128;
        if (current_vy > 56)
            current_vy = 56;
        y += current_vy;
    }
    else if (flags & 0x08) // BOUNCE
    {
        // Bouncing movement - keep original as it uses full width
        x += pattern.vx_base * compact.state1;
        y += pattern.vy_base;

        if (x < -240 || x > 240)
        {
            compact.state1 = -compact.state1;
            x = (x < 0) ? -240 : 240;
        }
    }
    else if (flags & 0x10) // HOMING
    {
        // Enhanced homing with target switching
        compact.state2++; // Increment frame counter

        // Switch target every 48 frames
        if (compact.state2 >= 48)
        {
            compact.state2 = 0;
            // New random target position across screen width
            compact.state1 = global_data_ptr->bn_random.get_int(-50, 50);
        }

        // Home toward current target (state1 stores target x in normal coords)
        int16_t target_x = compact.state1 * 4; // Convert to our scale
        int16_t dx = (target_x - x) / 20;      // Smoother homing
        int16_t dy = pattern.vy_base;

        // Limit horizontal speed for better gameplay
        if (dx > 8)
            dx = 8;
        if (dx < -8)
            dx = -8;

        x += dx;
        y += dy;
    }
    else if (flags & 0x20) // ORBIT
    {
        // Orbit with varying center points
        uint8_t angle = compact.state1;
        int8_t cos_val = SINE_TABLE[(angle + 16) & 63];
        int8_t sin_val = SINE_TABLE[angle & 63];

        // Update angle for next frame
        compact.state1 = (angle + 2) & 63;

        // Use initial x position as orbit center (stored during creation)
        int16_t center_x = compact.x - (compact.state2 * cos_val) / 32;

        // Calculate position from center with larger radius
        x = center_x + (compact.state2 * cos_val) / 32;
        y = (compact.state2 * sin_val) / 32 + compact.y + pattern.vy_base;
    }
    else if (flags & 0x40) // SPIRAL
    {
        // Spiral with varying center points
        uint8_t angle = compact.state1;
        int8_t cos_val = SINE_TABLE[(angle + 16) & 63];
        int8_t sin_val = SINE_TABLE[angle & 63];

        // Update angle and radius
        compact.state1 = (angle + 3) & 63;
        if (compact.state2 > 12)
        {
            compact.state2 += pattern.accel; // Shrink inward
        }

        // Use initial x position as spiral center
        int16_t center_x = compact.x - (120 * cos_val) / 32; // Use initial radius

        x = center_x + (compact.state2 * cos_val) / 32;
        y = (compact.state2 * sin_val) / 32 + compact.y + pattern.vy_base;
    }
    else // FALL, RISE, or default
    {
        // Simple linear movement
        x += pattern.vx_base;
        y += pattern.vy_base;
    }

    // Update stored position
    compact.x = x;
    compact.y = y;
    compact.ticker++;

    // Update sprite position
    item.value().set_position(fixed(x) / 4, fixed(y) / 4);
}

void bullet::populate(bn::vector<bullet, bullet_count> *bullets, int anim_type)
{
    bullets->clear();

    for (int b = 0; b < bullet_count; b++)
    {
        int16_t x, y;

        // Type-specific starting positions for better spread
        switch (anim_type)
        {
        case BULLET_RISE:
            // Spread across full width for rising bullets
            y = (70 + (b * 8)) * 4;
            break;

        case BULLET_HOMING:
            // Start from edges and corners for more interesting patterns
            if (b < 4)
            {
                x = -240; // Left edge
                y = (-60 + (b * 30)) * 4;
            }
            else if (b < 8)
            {
                x = 240; // Right edge
                y = (-60 + ((b - 4) * 30)) * 4;
            }
            else if (b < 12)
            {
                x = (-30 + ((b - 8) * 15)) * 4; // Top middle
                y = -280;
            }
            else
            {
                x = (-30 + ((b - 12) * 15)) * 4; // Random middle
                y = (-70 - ((b - 12) * 20)) * 4;
            }
            break;

        case BULLET_WAVE:
            // Better spread for wave pattern
            x = (-55 + (b * 7)) * 4;
            y = (-70 - (b * 12)) * 4;
            break;

        case BULLET_BOUNCE:
            // Start from various x positions
            x = (-50 + global_data_ptr->bn_random.get_int(100)) * 4;
            y = (-70 - (b * 10)) * 4;
            break;

        default:
            // Default spread
            x = (global_data_ptr->bn_random.get_int(-55, 55)) * 4;
            y = (-70 - (b * 15)) * 4;
            break;
        }

        bullet nb(x, y, anim_type);

        // Add type-specific variations
        if (anim_type == BULLET_WAVE)
        {
            // Vary wave phase for more interesting patterns
            nb.compact.ticker = global_data_ptr->bn_random.get_int(16);
            nb.compact.state2 = 35 + global_data_ptr->bn_random.get_int(45); // Larger amplitude
        }

        bullets->push_back(nb);
    }
}