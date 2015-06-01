#ifndef __INCLUDE_PLAYER_SHIP__
#define __INCLUDE_PLAYER_SHIP__

#include "c-entity.h"

class playerShip : public entity {
public:
  virtual void fire();

  playerShip();
};

#endif