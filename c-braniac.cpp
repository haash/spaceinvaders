#include <cstdlib>
#include <ctime>

#include "config.h"
#include "c-globals.h"

#include "c-braniac.h"

void braniac::fire() {
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (data::bullets[i].getDisplay() == ' ') {
      data::bullets[i].create('^', // bullet display
                              this->getX(), // x pos
                              this->getY() + 2, // y pos
                              20, // damage
                              2, // move speed (frames to wait)
                              false); // is player's bullet
      
      break;
    }
  }
}

void braniac::onTick() {
  // fire a bullet
  if (rand() % 1000 == 1) {
    this->fire();
  }
}

braniac::braniac() {
  std::string display;

  display += "     ";   //      
  display += " {@} ";   //  {@}  !
  display += " /\"\\ "; //  /"\  !

  this->setDisplay(display);

  srand(time(NULL));
}