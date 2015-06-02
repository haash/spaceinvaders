#ifndef __INCLUDE_BEAM__
#define __INCLUDE_BEAM__

#include "c-player_ship.h"

class beamShip : public playerShip {
private:
  int max_beam_frames;

public:
  int beam_frames_rendered;
  
  void onTick(); // overload for beam fire
  void fire();

  beamShip();
};

#endif