#include "c-key_handler.h"

#include "c-globals.h"

// add the curses library for handling screen stuff
#include <curses.h>
#include <cstdlib>

// type stuffs
#include "c-beam.h"
#include "c-default.h"
#include "c-swift.h"

void keyHandler::handleKey(char key) {
  if (glib::screenHandle->getCurrentScreen() == 'm') {
    switch (key) {
      case '1':
        delete data::player;
        data::player = new defaultShip();
        glib::screenHandle->setupGame();
        break;
      case '2':
        delete data::player;
        data::player = new swiftShip();
        glib::screenHandle->setupGame();
        break;
      case '3':
        delete data::player;
        data::player = new beamShip();
        glib::screenHandle->setupGame();
        break;
      case '4':
        erase();
        refresh();
        exit(1);
    }
  } else if (glib::screenHandle->getCurrentScreen() == 'g') {
    switch (key) {
      case 'a':
        if (data::player->getX() > 5)
          data::player->moveLeft();
        break;

      case 'd':
        if (data::player->getX() < COLS - 6)
          data::player->moveRight();
        break;
    }
  }
}