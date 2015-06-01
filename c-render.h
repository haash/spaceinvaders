#ifndef __INCLUDE_RENDERENGINE__
#define __INCLUDE_RENDERENGINE__

#include <curses.h>

struct renderEngine {
  void renderBraniacs(WINDOW*);
  void renderLanders(WINDOW*);
  void renderTanks(WINDOW*);

  void renderEnemies(WINDOW*);

  void renderPlayer(WINDOW*);

  void renderBullets(WINDOW*);
};

#endif