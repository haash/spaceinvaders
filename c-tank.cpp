#include "c-tank.h"

tank::tank() {
  this->setHealth(this->getHealth() * 2);
  
  std::string display;

  display += "     ";  //      
  display += "/~~~\\"; // /~~~\ !
  display += "\\-v-/"; // \-v-/ !

  this->setDisplay(display);
}