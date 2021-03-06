#ifndef __INCLUDE_BULLET__
#define __INCLUDE_BULLET__

#include <curses.h>

#include "c-entity.h"

class bullet {
private:
  char display;
  int damage;

  int pos_x;
  int pos_y;

  int last_move;
  int move_speed;
  int direction;

public:
  int getDamage() const;
  void setDamage(int);

  int getX() const;
  int getY() const;

  void setX(int);
  void setY(int);

  char getDisplay() const;
  void setDisplay(char);

  void move();
  void render(WINDOW*);

  bool hitTest(const entity&, int, int) const;
  void onTick();

  void create(char, int, int, int, int, bool);
  void blank();
  bullet();
};

#endif