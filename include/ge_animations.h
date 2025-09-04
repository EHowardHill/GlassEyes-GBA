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

// Ginger
const animation ginger_eat_lime = {{9}, 1, SP_DEFAULT, true};

// Croke
const animation croke_accuse = {{9}, 1, SP_DEFAULT, true};
const animation croke_lay_down = {{10}, 1, SP_DEFAULT, true};

// Elements
const animation elem_button_up = {{0}, 1, SP_DEFAULT, true};
const animation elem_button_down = {{1}, 1, SP_DEFAULT, true};
const animation elem_spike_up = {{3}, 1, SP_DEFAULT, true};
const animation elem_spike_down = {{2}, 1, SP_DEFAULT, true};