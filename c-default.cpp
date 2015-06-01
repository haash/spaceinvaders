#include "c-default.h"

defaultShip::defaultShip() {
  std::string display;

  display += "     ";  //      
  display += "_/^\\_"; // _/^\_
  display += "|###|";  // |###|

  this->setDisplay(display);
}