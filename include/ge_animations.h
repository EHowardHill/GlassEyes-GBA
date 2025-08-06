#include "ge_text.h"
#include "ge_sprites.h"

// FRAMES   -   SIZE    -   SPEED   -   LOOPS

// Default
const animation anim_walk = {{0, 1, 0, 2}, 4, SP_DEFAULT, true};
const animation anim_stand = {{0}, 1, SP_DEFAULT, false};

// Vista
const animation vista_reminisce = {{9}, 1, SP_DEFAULT, true};
const animation vista_pickup = {{6, 10, 6, 11}, 4, SP_SLOW, false};

// Visker
const animation visker_poke = {{9, 10}, 2, SP_DEFAULT, true};

// Jeremy
const animation jeremy_lay_down = {{9}, 1, SP_DEFAULT, true};