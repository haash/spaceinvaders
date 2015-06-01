#include "c-entity.h"

void entity::setX(int x) {
  this->pos_x = x;
}

void entity::setY(int y) {
  this->pos_y = y;
}

int entity::getX() {
  return this->pos_x;
}

int entity::getY() {
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

int entity::getMoveSpeed() {
  return this->move_speed;
}

void entity::setHealth(int health) {
  this->health = health;
}

void entity::removeHealth(int health) {
  this->health -= health;
}

int entity::getHealth() {
  return this->health;
}

bool entity::isDead() {
  if (this->health <= 0) {
    return true;
  }
  return false;
}

void entity::setDisplay(std::string display) {
  this->display = display;
}

std::string entity::getDisplay() {
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