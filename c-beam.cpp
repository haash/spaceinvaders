#include "config.h"
#include "c-globals.h"
#include <curses.h>

#include "c-beam.h"

void beamShip::onTick() {
  this->last_fire++;
  
  int *bfr = data::beam_frames_rendered;

  if (*bfr > 0) {
    (*bfr)++;
    if (*bfr >= this->max_beam_frames) {
      *bfr = 0;
    }
  }
}

void beamShip::fire() {
  *(data::beam_frames_rendered) = 1;
  this->last_fire = 0;
}

beamShip::beamShip() {
  std::string display;

  display += "     ";  //      
  display += " /v\\ "; //  /v\  !
  display += "|=-=|";  // |=-=| !

  this->setDisplay(display);
  this->setFireRate(200); // number of frames to wait before can fire again.

  *(data::beam_frames_rendered) = 0;
  this->max_beam_frames = MAX_BEAM_FRAMES;
}