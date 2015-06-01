#ifndef __INCLUDE_ENEMIES__
#define __INCLUDE_ENEMIES__

#include "c-entity.h"

class enemy : public entity {
public:
  virtual void onTick();
  enemy();
};

#endif