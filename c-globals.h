#ifndef __INCLUDE_GLOBALS__
#define __INCLUDE_GLOBALS__

// glib definitions
#include "c-key_handler.h"
#include "c-screen_events.h"
#include "c-curses_lib.h"
#include "c-render.h"

// type definitions for enemies
#include "c-braniac.h"
#include "c-lander.h"
#include "c-tank.h"
#include "c-player_ship.h"

namespace glib {
  extern keyHandler *keyHandle;
  extern screenEvents *screenHandle;
  extern cursesLib *curses;
  extern renderEngine *render;
};

namespace data {
  extern braniac *braniacs;
  extern lander *landers;
  extern tank *tanks;

  extern int *lastEnemyMove;

  extern playerShip *player; // undefined at this stage
};

namespace drivers {
  extern void *screen(void*);
  extern void *process(void*);
};

#endif