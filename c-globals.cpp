// Apparently required for nanosleep, although seems to work
// on all the systems I've tried without directly including the library...
#include <time.h>
#include <string>

#include "config.h"

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
#include "c-bullet.h"

namespace glib {
  keyHandler *keyHandle = new keyHandler();
  screenEvents *screenHandle = new screenEvents();
  cursesLib *curses = new cursesLib();
  renderEngine *render = new renderEngine();
};

namespace data {
  braniac *braniacs = new braniac[MAX_ENEMIES_PER_CLASS];
  lander *landers = new lander[MAX_ENEMIES_PER_CLASS];
  tank *tanks = new tank[MAX_ENEMIES_PER_CLASS];

  bullet *bullets = new bullet[MAX_BULLETS];

  // Tracks how many frames since the enemies' last move.
  // Delay until next move #defined as ENEMY_MS
  int *lastEnemyMove = new int(0);

  playerShip *player = new playerShip();
  int *beam_frames_rendered = new int(0);
}

/**
 * Anything below this point is legacy.
 * Before, there were two pthreads driving the software that called
 * the following functions to process their data. There were some
 * crazy issues with screen tearing on certain systems, hence
 * why it's not been collated into the main file.
 *
 * I figure the performance gained by multithreading isn't worth
 * the headaches with trying to implement mutexes for an OOP project.
 *
 * If you're really keen to multithread, you can utilise the namespace
 * and its functions by modifying the main() function to the following:

    glib::curses->startCurses();

    glib::screenHandle->menu();

    pthread_t screen;
    pthread_t process;

    pthread_create(&screen, NULL, drivers::screen, (void*) 0);
    pthread_create(&process, NULL, drivers::process, (void*) 0);

    pthread_join(screen, NULL);
    pthread_join(process, NULL);
 
 *
 * Don't forget to include <pthreads.h>! Have fun!
 *
 **/

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

        // === 2. Render Enemies on Screen ===
        glib::render->renderEnemies(window);
        
        // === 3. Render Player on Screen ===
        glib::render->renderPlayer(window);

        // === 4. Render Bullets on Screen ===
        glib::render->renderBullets(window);
        glib::render->renderBeam(window);

        // === 5. Refresh Window ===
        // Slight fix for windows, which doesnt hide the cursor.
        // Move the cursor to position 0, 0.
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
     *   3. Process Bullets (Moves and checks hit of bullets.)
     */

    // === 1. Process input data/move player ===
    int ch = wgetch(listener);
    if (ch != ERR) {
      glib::keyHandle->handleKey(ch);
    }

    if (glib::screenHandle->getCurrentScreen() == 'g') {
      // === 1(a). Process player ===
      data::player->onTick();

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
        int braniacs_alive = 0;
        int tanks_alive = 0;
        int landers_alive = 0;

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

        // === 3. Process Bullets ===
        for (int i = 0; i < MAX_BULLETS; ++i) {
          if (data::bullets[i].onTick()) {
            data::bullets[i].blank();
          }
        }

      }
    }

    // implement FPS cap delay
    nanosleep(&delay, &delay2);
  }

  return nothing; // -Wall requires return
}