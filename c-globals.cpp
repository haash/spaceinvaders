// Apparently required for nanosleep, although seems to work
// on all the systems I've tried without directly including the library...
#include <time.h>
#include <string>

#include "config.h"

#include "c-key_handler.h"
#include "c-screen_events.h"
#include "c-curses_lib.h"
#include "c-render.h"

#include "c-beam.h"
#include "c-braniac.h"
#include "c-default.h"
#include "c-lander.h"
#include "c-swift.h"
#include "c-tank.h"
#include "c-bullet.h"

namespace glib {
  keyHandler *keyHandle = new keyHandler();
  screenEvents *screenHandle = new screenEvents();
  cursesLib *curses = new cursesLib();
  renderEngine *render = new renderEngine();
}

namespace data {
  braniac *braniacs = new braniac[MAX_ENEMIES_PER_CLASS];
  lander *landers = new lander[MAX_ENEMIES_PER_CLASS];
  tank *tanks = new tank[MAX_ENEMIES_PER_CLASS];

  bullet *bullets = new bullet[MAX_BULLETS];

  // Tracks how many frames since the enemies' last move.
  // Delay until next move #defined as ENEMY_MS
  int *lastEnemyMove = new int(0);

  playerShip *player = new playerShip();
  int *beam_frames_rendered = new int(0);

  int *score = new int(0);
}