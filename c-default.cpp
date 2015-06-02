#include "config.h"
#include "c-globals.h"

#include "c-default.h"

void defaultShip::fire() {
  // fire a bullet that travels upwards
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (data::bullets[i].getDisplay() == ' ') {
      data::bullets[i].create('o', // bullet display
                              this->getX(), // x pos
                              this->getY() - 2, // y pos
                              80, // damage
                              2, // move speed (frames to wait)
                              true); // is player's bullet
      
      this->last_fire = 0;
      break;
    }
  }
}

defaultShip::defaultShip() {
  std::string display;

  display += "     ";  //      
  display += "_/^\\_"; // _/^\_
  display += "|###|";  // |###|

  this->setDisplay(display);
  
  this->setFireRate(100); // number of frames to wait before can fire again.
}