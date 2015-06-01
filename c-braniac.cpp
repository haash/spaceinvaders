#include "c-braniac.h"

void braniac::onTick() {
  // fire a bullet
}

braniac::braniac() {
  std::string display;

  display += "     ";   //      
  display += " {@} ";   //  {@}  !
  display += " /\"\\ "; //  /"\  !

  this->setDisplay(display);
}