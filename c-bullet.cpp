#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include <cstdlib>
#include "config.h"

#include "c-bullet.h"
#include "c-globals.h"

int bullet::getDamage() const {
  return this->damage;
}

void bullet::setDamage(int dmg) {
  this->damage = dmg;
}

int bullet::getX() const {
  return this->pos_x;
}

int bullet::getY() const {
  return this->pos_y;
}

void bullet::setX(int x) {
  this->pos_x = x;
}

void bullet::setY(int y) {
  this->pos_y = y;
}

char bullet::getDisplay() const {
  return this->display;
}

void bullet::setDisplay(char c) {
  this->display = c;
}

void bullet::move() {
  this->setY(this->getY() + this->direction);
  this->last_move = 0;
}

void bullet::render(WINDOW* window) {
  if (this->getDisplay() != ' ') {
    mvwaddch(window, this->getY(), this->getX(), this->getDisplay());
  }
}

bool bullet::hitTest(entity* ent) {
  if (ent->isAlive() &&
      abs(this->getX() - ent->getX()) < 3 &&
      abs(this->getY() - ent->getY()) < 2) {

    return true;
  }

  return false;
}

void bullet::onTick() {
  // the bullet doesn't exist, just initialised
  if (this->getDisplay() == ' ') {
    return;
  }

  this->last_move++;
  if (this->last_move >= this->move_speed) {
    this->move();
  }

  int this_x = this->getX();
  int this_y = this->getY();

  if (this_x < 2 || this_x > COLS || this_y < 2 || this_y > LINES) {
    this->blank();
    return;
  }

  // Hit detection against enemies.
  if (this->direction < 0) {
    for (int i = 0; i < MAX_ENEMIES_PER_CLASS; ++i) {
      if (this->hitTest(&data::landers[i])) {
        data::landers[i].removeHealth(this->getDamage());
        this->blank();
        return;
      } else if (this->hitTest(&data::tanks[i])) {
        data::tanks[i].removeHealth(this->getDamage());
        this->blank();
        return;
      } else if (this->hitTest(&data::braniacs[i])) {
        data::braniacs[i].removeHealth(this->getDamage());
        this->blank();
        return;
      }
    }
  } else { // Hit detection against the player.
    if (abs(this_x - data::player->getX()) < 3 &&
        abs(this_y - data::player->getY()) < 2) {

      data::player->removeHealth(this->getDamage());
      this->blank();
      return;
    }
  }
}

void bullet::create(char display, int pos_x, int pos_y, int damage, 
                    int move_speed, bool isPlayer) {
  this->setDisplay(display);
  this->setX(pos_x);
  this->setY(pos_y);

  this->setDamage(damage);
  this->last_move = 0;
  this->move_speed = move_speed;

  if (isPlayer) {
    this->direction = -1;
  } else {
    this->direction = 1;
  }
}

void bullet::blank() {
  this->setDisplay(' ');
  this->direction = 0;
  this->move_speed = 0;
  this->setDamage(0);
  this->setX(0);
  this->setY(0);
}

bullet::bullet() {
  // This constructor simply initialises a blank bullet.
  // Use bullet::create() to actually start the bullet.
  this->blank();
}