#ifndef __INCLUDE_CURSESLIB__
#define __INCLUDE_CURSESLIB__

struct cursesLib {
  bool curses_started;
  void startCurses();
  void endCurses();

  cursesLib();
};

#endif