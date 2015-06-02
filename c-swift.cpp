#include "config.h"
#include "c-globals.h"

#include "c-swift.h"

void swiftShip::fire() {
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (data::bullets[i].getDisplay() == ' ') {
      data::bullets[i].create('.', // bullet display
                              this->getX(), // x pos
                              this->getY() - 2, // y pos
                              35, // damage
                              1, // move speed (frames to wait)
                              true); // is player's bullet
      
      this->last_fire = 0;
      break;
    }
  }
}

swiftShip::swiftShip() {
  std::string display;

  display += "     ";  //      
  display += "  |  ";  //   |   !
  display += "/***\\"; // /***\ !

  this->setDisplay(display);
  this->setFireRate(60); // number of frames to wait before can fire again.
}