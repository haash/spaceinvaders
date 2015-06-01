#include "c-curses_lib.h"

#include <curses.h>
#include <cstdlib>

void cursesLib::startCurses() {
  if (this->curses_started) {
    refresh();
  }
  else {
    
    // === Initialise curses with some flags ===
    initscr();
    curs_set(0); // doesn't hide cursor on windoze, bugger it
    cbreak();
    noecho();
    intrflush(stdscr, false);
    this->curses_started = true;

    erase();
  }
}

void cursesLib::endCurses() {
  if (this->curses_started && !isendwin()) {
    endwin();
    this->curses_started = false;
  }
}

cursesLib::cursesLib() {
  this->curses_started = false;
}