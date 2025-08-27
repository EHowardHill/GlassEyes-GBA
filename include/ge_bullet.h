// ge_bullet.h - Memory-optimized version
#ifndef GE_BULLET_H
#define GE_BULLET_H

#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"

#define bullet_count 16

enum BULLET_ANIM_TYPES : uint8_t
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

// Shared pattern data - ONE copy for all bullets of same type
struct bullet_pattern
{
    int8_t vx_base;    // Base velocity * 16 (store as int8)
    int8_t vy_base;    // Base velocity * 16
    uint8_t amplitude; // 0-255 range
    uint8_t period;    // 0-255 frames
    int8_t accel;      // Acceleration * 128
    uint8_t flags;     // Bit flags for behavior
};

// Pre-computed patterns (stored in ROM, no RAM usage)
extern const bullet_pattern PATTERNS[BULLET_SIZE];

// Compact bullet struct - only 8 bytes + sprite pointer!
struct bullet
{
    bn::optional<bn::sprite_ptr> item;
    
    // Only 8 bytes of state data using union
    union {
        struct {
            int16_t x;        // Position * 4 (gives us -8192 to 8191 range)
            int16_t y;        // Position * 4
            uint8_t ticker;   // 0-255 frames
            uint8_t type;     // Animation type
            int8_t state1;    // Generic state (zig direction, orbit angle, etc.)
            int8_t state2;    // Generic state (speed modifier, radius, etc.)
        } compact;
        
        // Alternative view for patterns that need it
        struct {
            int16_t x;
            int16_t y;
            uint16_t ticker_and_type; // Combined ticker (lower 8) and type (upper 8)
            int16_t velocity;          // Combined vx/vy for some patterns
        } alt;
    };
    
    bullet(int16_t x_pos, int16_t y_pos, uint8_t anim_type);
    void update();
    static void populate(bn::vector<bullet, bullet_count>* bullets, int anim_type);
    
    // Helper to get actual position
    bn::fixed get_x() const { return bn::fixed(compact.x) / 4; }
    bn::fixed get_y() const { return bn::fixed(compact.y) / 4; }
    void set_pos(bn::fixed x, bn::fixed y) {
        compact.x = (x * 4).integer();
        compact.y = (y * 4).integer();
    }
};

// Global lookup tables (stored in ROM, shared by all bullets)
extern const int8_t SINE_TABLE[64];  // Pre-computed sine wave values

#endif