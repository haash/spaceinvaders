// Theoretical maximum fps, set to maybe double expected fps.
#define FPS_HARDCAP 100

// number of frames before enemies move
#define ENEMY_MS 12

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

  // Tracks how many frames since the enemies' last move.
  // Delay until next move #defined as ENEMY_MS
  int *lastEnemyMove = new int(0);

  playerShip *player = new playerShip();
}

namespace drivers {
  void *screen(void*);
  void *process(void*);
};

// driver used by screen thread
void *drivers::screen(void *nothing) {

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

      case 'd':
        // No update, should be initialised in pre-call, see
        // "glib::screenHandle->die()" or "->win()".
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

        // === 1(a). Testing Phase ===
        // mvwprintw(window, 0, 0, "%i %i", data::landers[0].getY(), data::player->getY() - 2);

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

  return nothing; // -Wall requires return
}

// driver used by process thread
void *drivers::process(void *nothing) {

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
     * (Note : Following functions only to be run if in-game)
     *   2. Move Enemies
     *   3. Move Bullets
     *   4. Check if bullets hit target and process
     */

    // === 1. Process input data/move player ===
    int ch = wgetch(listener);
    if (ch != ERR) {
      glib::keyHandle->handleKey(ch);
    }

    if (glib::screenHandle->getCurrentScreen() == 'g') {
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

        if (((ms > 0) && (x5 + ms > COLS - 6)) ||
            ((ms < 0) && (x1 + ms < 5)))
        {
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
        // Also at this point, we're going to check if the game is complete
        // or not, if the enemies have reached the player, or the player
        // has killed all the enemies.
        bool braniacs_alive = 0;
        bool tanks_alive = 0;
        bool landers_alive = 0;

        int braniacs_y = data::braniacs[0].getY();
        int tanks_y = data::landers[0].getY();
        int landers_y = data::landers[0].getY();

        for (int i = 0; i < 5; ++i) {
          data::braniacs[i].setX(data::braniacs[i].getX() + ms);
          if (!data::braniacs[i].isDead()) {
            braniacs_alive++;
          }
          
          data::tanks[i].setX(data::tanks[i].getX() + ms);
          if (!data::tanks[i].isDead()) {
            tanks_alive++;
          }

          data::landers[i].setX(data::landers[i].getX() + ms);
          if (!data::landers[i].isDead()) {
            landers_alive++;
          }
        }

        if (!braniacs_alive && !landers_alive && !tanks_alive) {
          // do end of game screen
        }

        if (landers_alive && landers_y >= data::player->getY() - 2) {
          glib::screenHandle->die();
        } else if (tanks_alive && tanks_y >= data::player->getY() - 2) {
          glib::screenHandle->die();
        } else if (braniacs_alive && braniacs_y >= data::player->getY() - 2) {
          glib::screenHandle->die();
        }

      }
    }

    // implement FPS cap delay
    nanosleep(&delay, &delay2);
  }

  return nothing; // -Wall requires return
}