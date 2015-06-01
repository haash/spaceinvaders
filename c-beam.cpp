#include "c-beam.h"

void beamShip::fire() {
  // massive beam of fire, but slow fire rate
}

beamShip::beamShip() {
  std::string display;

  display += "     ";  //      
  display += " /v\\ "; //  /v\  !
  display += "|=-=|";  // |=-=| !

  this->setDisplay(display);
}