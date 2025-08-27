// ge_bullet.cpp - Optimized implementation
#include "ge_bullet.h"
#include "ge_globals.h"
#include "bn_sprite_items_hearts.h"
#include "bn_math.h"

using namespace bn;

// Pre-computed patterns in ROM (no RAM usage)
// Order must match BULLET_ANIM_TYPES enum
const bullet_pattern PATTERNS[BULLET_SIZE] = {
    // vx    vy   amp  per  acc  flags
    { 0,  2,   0,   0,   0,  0x00}, // BULLET_FALL
    { 0, -4,   0,   0,   0,  0x00}, // BULLET_RISE
    { 0,  24,  28,  20,   0,  0x01}, // BULLET_ZIGZAG - flag 0x01 = use zigzag
    { 0,  20,  40,  36,   0,  0x02}, // BULLET_WAVE - flag 0x02 = use wave
    { 0,   5,   0,   0,  10,  0x04}, // BULLET_ACCEL - flag 0x04 = accelerate
    {32,  22,   0,   0,   0,  0x08}, // BULLET_BOUNCE - flag 0x08 = bounce
    {12,  12,   0,   0,   0,  0x10}, // BULLET_HOMING - flag 0x10 = home
    { 0,  19,  30,  64,   0,  0x20}, // BULLET_ORBIT - flag 0x20 = orbit (amp=radius)
    { 0,  16,  45,  64,  -3,  0x40}, // BULLET_SPIRAL - flag 0x40 = spiral (acc=drift)
};

// Simple sine approximation table (64 entries = full period)
const int8_t SINE_TABLE[64] = {
    0, 6, 12, 18, 24, 30, 35, 40, 45, 49, 53, 56, 59, 61, 63, 64,
    64, 64, 63, 61, 59, 56, 53, 49, 45, 40, 35, 30, 24, 18, 12, 6,
    0, -6, -12, -18, -24, -30, -35, -40, -45, -49, -53, -56, -59, -61, -63, -64,
    -64, -64, -63, -61, -59, -56, -53, -49, -45, -40, -35, -30, -24, -18, -12, -6
};

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
        4
    );
    
    // Initialize type-specific state
    switch(anim_type)
    {
    case BULLET_ZIGZAG:
        compact.state1 = (global_data_ptr->bn_random.get_int(2) ? 1 : -1); // direction
        break;
    case BULLET_BOUNCE:
        compact.state1 = (global_data_ptr->bn_random.get_int(2) ? 1 : -1); // x direction
        break;
    case BULLET_ORBIT:
    case BULLET_SPIRAL:
        compact.state1 = global_data_ptr->bn_random.get_int(64); // starting angle
        compact.state2 = PATTERNS[anim_type].amplitude; // radius
        break;
    default:
        break;
    }
}

void bullet::update()
{
    if (!item.has_value()) return;
    
    const bullet_pattern& pattern = PATTERNS[compact.type];
    
    // Get current position
    int16_t x = compact.x;
    int16_t y = compact.y;
    
    // Apply movement based on type
    uint8_t flags = pattern.flags;
    
    if (flags & 0x01) // ZIGZAG
    {
        // Sharp zigzag movement
        x += compact.state1 * 14; // Move horizontally
        
        // Check bounds and flip direction (using amplitude as bound)
        int16_t bound = pattern.amplitude * 4; // Convert to our scale
        if (abs(x) > bound) {
            x = (x > 0) ? bound : -bound;
            compact.state1 = -compact.state1;
        }
        y += pattern.vy_base;
    }
    else if (flags & 0x02) // WAVE
    {
        // Use sine table for smooth wave
        uint8_t phase = (compact.ticker * 64) / pattern.period;
        int8_t sine = SINE_TABLE[phase & 63];
        x = compact.x + (sine * pattern.amplitude) / 32; // Scale sine to amplitude
        y += pattern.vy_base;
    }
    else if (flags & 0x04) // ACCEL
    {
        // Accelerating bullet
        int16_t current_vy = pattern.vy_base + (compact.ticker * pattern.accel) / 128;
        if (current_vy > 56) current_vy = 56; // Max speed cap
        y += current_vy;
    }
    else if (flags & 0x08) // BOUNCE
    {
        // Bouncing movement
        x += pattern.vx_base * compact.state1;
        y += pattern.vy_base;
        
        // Bounce off walls (character box bounds)
        if (x < -240 || x > 240) { // -60 to 60 in normal coords
            compact.state1 = -compact.state1;
            x = (x < 0) ? -240 : 240;
        }
    }
    else if (flags & 0x10) // HOMING
    {
        // Simplified homing - just move toward player position
        // Assume player is at (0, 0) or get from global
        int16_t dx = -x / 16; // Simple proportional control
        int16_t dy = -y / 16;
        
        // Add to base velocity
        x += pattern.vx_base + dx;
        y += pattern.vy_base + dy;
    }
    else if (flags & 0x20) // ORBIT
    {
        // Use sine/cosine table for circular motion
        uint8_t angle = compact.state1;
        int8_t cos_val = SINE_TABLE[(angle + 16) & 63]; // cos is sine shifted by 90°
        int8_t sin_val = SINE_TABLE[angle & 63];
        
        // Update angle for next frame
        compact.state1 = (angle + 2) & 63; // Rotate ~5.6 degrees per frame
        
        // Calculate position from center
        x = (compact.state2 * cos_val) / 32;
        y = (compact.state2 * sin_val) / 32 + compact.y + pattern.vy_base;
    }
    else if (flags & 0x40) // SPIRAL
    {
        // Similar to orbit but with shrinking radius
        uint8_t angle = compact.state1;
        int8_t cos_val = SINE_TABLE[(angle + 16) & 63];
        int8_t sin_val = SINE_TABLE[angle & 63];
        
        // Update angle and radius
        compact.state1 = (angle + 3) & 63; // Faster rotation
        if (compact.state2 > 8) {
            compact.state2 += pattern.accel; // accel is negative for inward spiral
        }
        
        x = (compact.state2 * cos_val) / 32;
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

void bullet::populate(bn::vector<bullet, bullet_count>* bullets, int anim_type)
{
    bullets->clear();
    
    for (int b = 0; b < bullet_count; b++)
    {
        // Starting positions in our compact format (x4 scale)
        int16_t x = (global_data_ptr->bn_random.get_int(-60, 60)) * 4;
        int16_t y = (-70 - (b * 15)) * 4; // Spread them out vertically
        
        // Adjust starting position based on type
        switch(anim_type)
        {
        case BULLET_RISE:
            y = (70 + (b * 16)) * 4;
            break;
        case BULLET_HOMING:
            // Start from edges
            if (b % 2) x = (b % 4 < 2) ? -240 : 240;
            break;
        default:
            break;
        }
        
        bullet nb(x, y, anim_type);
        
        // Add some variation
        if (anim_type == BULLET_ZIGZAG || anim_type == BULLET_WAVE) {
            nb.compact.state2 = 15 + global_data_ptr->bn_random.get_int(30); // Vary amplitude
        }
        
        bullets->push_back(nb);
    }
}