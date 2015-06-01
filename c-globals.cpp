// maximum fps, set to maybe double expected fps
#define FPS_HARDCAP 100
// number of frames before enemies move
#define ENEMY_MS 15

// Apparently required for nanosleep, although seems to work
// on all the systems I've tried without directly including the library...
#include <time.h>
#include <string>

#include "c-key_handler.h"
#include "c-screen_events.h"
#include "c-curses_lib.h"
#include "c-render.h"

#include "c-beam.h"
#include "c-braniac.h"
#include "c-default.h"
#include "c-lander.h"
#include "c-swift.h"
#include "c-tank.h"

namespace glib {
  keyHandler *keyHandle = new keyHandler();
  screenEvents *screenHandle = new screenEvents();
  cursesLib *curses = new cursesLib();
  renderEngine *render = new renderEngine();
};

namespace data {
  braniac *braniacs = new braniac[5];
  lander *landers = new lander[5];
  tank *tanks = new tank[5];

  int *lastEnemyMove = new int(0); // 5fps ago

  playerShip *player = new playerShip();
}

namespace drivers {
  // driver used by screen thread
  void *screen(void *nothing) {

    // === Initialise time delays ===
    struct timespec delay, delay2;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000000000 / FPS_HARDCAP;

    // === Start our screen rendering engine ===
    while (true) {
      switch (glib::screenHandle->getCurrentScreen()) {
        case 'm': // Menu screen.
          // No update, should be initialised in pre-call, see
          // "glib::screenHandle->menu()".
          break;

        case 'g': // Main game screen.
          /*
           * Main Game Render Function Heirachy:
           *   1. Setup Screen
           *   2. Render Enemies on Screen
           *   3. Render Player on Screen
           *   4. Render Bullets on Screen
           *   5. Refresh Window
           */

          // === 1. Setup Screen ===
          WINDOW* window = glib::screenHandle->getMainWindow();
          werase(window);
          glib::screenHandle->drawBorder(window, '+', '|', '-');

          // === 2. Render Enemies on Screen ===
          glib::render->renderEnemies(window);
          
          // === 3. Render Player on Screen ===
          glib::render->renderPlayer(window);

          // === 4. Render Bullets on Screen ===

          // === 5. Refresh Window ===
          // Slight fix for windows, which doesnt hide the cursor.
          wmove(window, 0, 0);

          wrefresh(window);

          break; // end case (game currentScreen == game screen)

      } // end current screen switch statement

      // implement FPS cap delay
      nanosleep(&delay, &delay2);
    } // end our while loop

    return (void*) 1; // -Wall requires return
  }

  // driver used by process thread
  void *process(void *nothing) {

    // === Initialise time delays and grab the listener window ===
    struct timespec delay, delay2;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000000000 / FPS_HARDCAP;

    WINDOW* listener = glib::screenHandle->getListener();

    // === Start our I/O process ===
    while (true) {

      /*
       * Function heirachy:
       *   1. Process input data/move player
       *   2. Move Enemies
       *   3. Move Bullets
       *   4. Check if bullets hit target and process
       */

      // === 1. Process input data/move player ===
      int ch = wgetch(listener);
      if (ch != ERR) {
        glib::keyHandle->handleKey(ch);
      }

      // === 2. Move Enemies ===
      int *lem = data::lastEnemyMove;
      (*lem)++;
      if (*lem >= ENEMY_MS) {
        (*lem) = 0;

        // Make sure we move the correct way.
        // All this does is invert the movespeed of the objects and
        // reduce their Y axis if they are going to breach the screen.
        int ms = data::braniacs[4].getMoveSpeed();
        int x1 = data::braniacs[0].getX();
        int x5 = data::braniacs[4].getX();

        if (((ms > 0) && (x5 + ms > COLS - 8)) ||
            ((ms < 0) && (x1 + ms < 6)))
        {
          mvwprintw(glib::screenHandle->getMainWindow(), 0, 0, "%i %i", x5 + ms, LINES - 5);
          for (int i = 0; i < 5; ++i) {
            // All classes should have the same movespeed.
            data::braniacs[i].setMoveSpeed(-1 * ms);
            data::braniacs[i].setY(data::braniacs[i].getY() + 1);

            data::landers[i].setMoveSpeed(-1 * ms);
            data::landers[i].setY(data::landers[i].getY() + 1);

            data::tanks[i].setMoveSpeed(-1 * ms);
            data::tanks[i].setY(data::tanks[i].getY() + 1);
          }
        }

        // Move the enemy to the left/right. Use entity::moveRight()
        // because we invert the movement speed in the previous block of code.
        for (int i = 0; i < 5; ++i) {
          data::braniacs[i].setX(data::braniacs[i].getX() + ms);
          data::landers[i].setX(data::landers[i].getX() + ms);
          data::tanks[i].setX(data::tanks[i].getX() + ms);
        }
      }

      // implement FPS cap delay
      nanosleep(&delay, &delay2);
    }

    return (void*) 1; // -Wall requires return
  }
};