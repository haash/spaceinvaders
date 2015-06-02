#ifndef __INCLUDE_PLAYER_SHIP__
#define __INCLUDE_PLAYER_SHIP__

#include "c-entity.h"

class playerShip : public entity {
private:
  int fire_rate;

protected:
  int last_fire;

public:
  virtual void onTick();

  int getFireRate() const;
  void setFireRate(int);

  bool canFire() const;
  virtual void fire();

  playerShip();
};

#endif