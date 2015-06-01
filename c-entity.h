#ifndef __INCLUDE_ENTITY__
#define __INCLUDE_ENTITY__

#include <string>

class entity {
private:
  int pos_x;
  int pos_y;
  int move_speed;
  int health;

  // consider display to be a 5 width, 3 height character array
  std::string display;

public:
  void setX(int);
  void setY(int);

  int getX();
  int getY();

  virtual void moveLeft();
  virtual void moveRight(); 

  void setMoveSpeed(int); // number of pixels to move when move*() called.
  int getMoveSpeed();

  void setHealth(int);
  void removeHealth(int);
  int getHealth();
  bool isDead();

  void setDisplay(std::string);
  std::string getDisplay();

  virtual void onTick();

  entity();
  virtual ~entity() {}; // virtual destructor to keep -Wall happy
};

#endif