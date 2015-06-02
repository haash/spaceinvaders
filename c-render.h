#ifndef __INCLUDE_RENDERENGINE__
#define __INCLUDE_RENDERENGINE__

#include <curses.h>

struct renderEngine {
  void renderBraniacs(WINDOW*) const;
  void renderLanders(WINDOW*) const;
  void renderTanks(WINDOW*) const;

  void renderEnemies(WINDOW*) const;

  void renderPlayer(WINDOW*) const;

  void renderBullets(WINDOW*) const;
  void renderBeam(WINDOW*) const;
};

#endif