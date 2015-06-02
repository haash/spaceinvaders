#ifndef __INCLUDE_DEFAULT__
#define __INCLUDE_DEFAULT__

#include "c-player_ship.h"

class defaultShip : public playerShip {
public:
  void fire();
  
  defaultShip();
};

#endif