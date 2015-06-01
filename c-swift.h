#ifndef __INCLUDE_SWIFT__
#define __INCLUDE_SWIFT__

#include "c-player_ship.h"

class swiftShip : public playerShip {
public:
  void fire();

  swiftShip();
};

#endif