#include "c-swift.h"

void swiftShip::fire() {
  // faster fire rate, less damage
}

swiftShip::swiftShip() {
  std::string display;

  display += "     ";  //      
  display += "  |  ";  //   |   !
  display += "/***\\"; // /***\ !

  this->setDisplay(display);
}