#include "config.h"

#include "c-render.h"
#include "c-globals.h"

#include <string>

void renderEngine::renderBraniacs(WINDOW* window) const {
  int start_x, start_y;
  for (int i = 0; i < 5; ++i) {
    if (data::braniacs[i].isDead())
      continue;

    start_x = data::braniacs[i].getX() - 2;
    start_y = data::braniacs[i].getY() - 1;

    data::braniacs[i].onTick();

    for (int y = 0; y < 3; ++y) {
      mvwprintw(window, start_y + y, start_x, "%s",
                data::braniacs[i].getDisplay().substr(y * 5, 5).data());
    }
  }
}

void renderEngine::renderLanders(WINDOW* window) const {
  int start_x, start_y;
  for (int i = 0; i < 5; ++i) {
    if (data::landers[i].isDead())
      continue;

    start_x = data::landers[i].getX() - 2;
    start_y = data::landers[i].getY() - 1;

    data::landers[i].onTick();

    for (int y = 0; y < 3; ++y) {
      mvwprintw(window, start_y + y, start_x, "%s",
                data::landers[i].getDisplay().substr(y * 5, 5).data());
    }
  }
}

void renderEngine::renderTanks(WINDOW* window) const {
  int start_x, start_y;
  for (int i = 0; i < 5; ++i) {
    if (data::tanks[i].isDead())
      continue;

    start_x = data::tanks[i].getX() - 2;
    start_y = data::tanks[i].getY() - 1;

    data::tanks[i].onTick();

    for (int y = 0; y < 3; ++y) {
      mvwprintw(window, start_y + y, start_x, "%s",
                data::tanks[i].getDisplay().substr(y * 5, 5).data());
    }
  }
}

void renderEngine::renderEnemies(WINDOW* window) const {
  this->renderBraniacs(window);
  this->renderLanders(window);
  this->renderTanks(window);
}

void renderEngine::renderPlayer(WINDOW* window) const {
  int start_y = data::player->getY() - 1;
  int start_x = data::player->getX() - 2;

  for (int y = 0; y < 3; ++y) {
    mvwprintw(window, start_y + y, start_x, "%s",
      data::player->getDisplay().substr(y * 5, 5).data());
  }
}

void renderEngine::renderBullets(WINDOW* window) const {
  for (int i = 0; i < MAX_BULLETS; ++i) {
    data::bullets[i].render(window);
  }
}

void renderEngine::renderBeam(WINDOW* window) const {
  if (*(data::beam_frames_rendered) > 0) {

    int x = data::player->getX() - 1;

    for (int y = 1; y < data::player->getY() - 2; ++y) {
      mvwprintw(window, y, x, "i#i");
    }
  }
}