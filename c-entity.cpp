#include "c-globals.h"
#include "c-entity.h"

void entity::setX(int x) {
  this->pos_x = x;
}

void entity::setY(int y) {
  this->pos_y = y;
}

int entity::getX() const {
  return this->pos_x;
}

int entity::getY() const {
  return this->pos_y;
}

void entity::moveLeft() {
  this->setX(this->getX() - this->getMoveSpeed());
}

void entity::moveRight() {
  this->setX(this->getX() + this->getMoveSpeed());
}

void entity::setMoveSpeed(int speed) {
  this->move_speed = speed;
}

int entity::getMoveSpeed() const {
  return this->move_speed;
}

void entity::setHealth(int health) {
  this->health = health;
}

void entity::removeHealth(int health) {
  (*data::score) += health;
  this->health -= health;
}

int entity::getHealth() const {
  return this->health;
}

bool entity::isDead() const {
  if (this->health <= 0) {
    return true;
  }
  return false;
}

bool entity::isAlive() const {
  return !this->isDead();
}

void entity::setDisplay(std::string display) {
  this->display = display;
}

std::string entity::getDisplay() const {
  return this->display;
}

void entity::onTick() {} // for children

entity::entity() {
  this->setX(0);
  this->setY(0);
  this->setMoveSpeed(1);

  std::string display;

  display += "  x  ";
  display += " xox ";
  display += "  x  ";

  this->setDisplay(display);
}