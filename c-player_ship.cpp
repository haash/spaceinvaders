#include "c-player_ship.h"

void playerShip::onTick() {
  this->last_fire++;
}

int playerShip::getFireRate() const {
  return this->fire_rate;
}

void playerShip::setFireRate(int rate) {
  this->fire_rate = rate;
}

bool playerShip::canFire() const {
  if (this->last_fire >= this->fire_rate) {
    return true;
  }

  return false;
}

// for use by child classes
void playerShip::fire() {
  this->last_fire = 0;
}

playerShip::playerShip() {
  this->setHealth(50);

  this->last_fire = 0;
  this->setFireRate(15); // number of frames to wait before can fire again.
}