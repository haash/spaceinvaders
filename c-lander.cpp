#include "c-lander.h"

lander::lander() {
  std::string display;

  display += "     ";  //      
  display += "/...\\"; // /...\ !
  display += " / \\ "; //  / \  !

  this->setDisplay(display);
}