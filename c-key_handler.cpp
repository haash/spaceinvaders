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
  switch (glib::screenHandle->getCurrentScreen()) {
    // Menu key handler.
    case 'm':
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
        case 'x':
        case 'q':
          erase();
          refresh();
          exit(1);
      }

    // End menu key handler.
      break;

    // Game key handler.
    case 'g':
      switch (key) {
        case 'a':
          if (data::player->getX() > 5)
            data::player->moveLeft();
          break;

        case 'd':
          if (data::player->getX() < COLS - 6)
            data::player->moveRight();
          break;

        case 'x':
          glib::screenHandle->win();
          break;
      }

    // End game key handler.
      break;

    // Death key handler.
    case 'd':
      switch (key) {
        case 'x':
        case 'q':
          erase();
          refresh();
          exit(1);
          break;

        default:
          glib::screenHandle->menu();
          break;
      }

    // End death key handler.
      break;
  }
}