#ifndef __INCLUDE_BRANIAC__
#define __INCLUDE_BRANIAC__

#include "c-enemy.h"

struct braniac : public enemy {
  void fire();
  void onTick();

  braniac();
};

#endif