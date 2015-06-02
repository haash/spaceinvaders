
/***
 * Including all of our libraries.
 * Child classes should include their parent's header files.
 *
 **/

#include <time.h>
#include <string>
#include <cstdlib>

#include "config.h"

#include "c-beam.h"
#include "c-braniac.h"
#include "c-default.h"
#include "c-lander.h"
#include "c-swift.h"
#include "c-tank.h"
#include "c-bullet.h"

/***
 * Include global namespaces.
 * This includes all of libraries, like ncurses, etc.
 *
 **/

#include "c-globals.h"

int main() {
  // Initialise ncurses and start the menu screen.
  glib::curses->startCurses();
  glib::screenHandle->menu();

  // === Initialise time delays ===
  struct timespec delay, delay2;
  delay.tv_sec = 0;
  delay.tv_nsec = 1000000000 / FPS_HARDCAP;

  // Grab the primary window listener for our I/O engine.
  WINDOW* listener = glib::screenHandle->getListener();

  // Start our process loop. Should run all game, all the time.
  while (true) {
    /***
     * +===================================+
     * | Start our screen rendering engine |
     * +===================================+
     *
     * === Main Game Render Function Heirachy ===
     *   1. Setup Screen
     *   2. Render Enemies on Screen
     *   3. Render Player on Screen
     *   4. Render Bullets on Screen
     *   5. Refresh Window
     *
     **/
    switch (glib::screenHandle->getCurrentScreen()) {
      case 'm': // Menu screen.
        // No update, should be initialised in pre-call, see
        // "glib::screenHandle->menu()".
        break;

      case 'd': // Death Screen/Win Screen
        // No update, should be initialised in pre-call, see
        // "glib::screenHandle->die()" or "->win()".
        break;

      case 'g': // Main game screen.

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

    /***
     * +===================================+
     * |  Start our I/O rendering engine   |
     * +===================================+
     *
     * === Function heirachy ===
     *  1. Process input data/move player
     *  (Note : Following functions only to be run if in-game)
     *  2. Move Enemies and fire bullets. (braniac)
     *  3. Process Bullets (Moves and checks hit of bullets.)
     *
     **/

    // === 1. Process input data/move player ===
    int ch = wgetch(listener);
    if (ch != ERR) {
      glib::keyHandle->handleKey(ch);
    }

    if (glib::screenHandle->getCurrentScreen() == 'g') {
      // === 1(a). Process player ===
      data::player->onTick();

      if (data::player->getHealth() <= 0) {
        glib::screenHandle->die();
      }

      // === 2. Move Enemies ===
      int *lem = data::lastEnemyMove;
      (*lem)++;
      if (*lem >= ENEMY_MS) {
        (*lem) = 0;

        /**
         * Make sure we move the correct way.
         * All this does is invert the movespeed of the objects and
         * reduce their Y axis if they are going to breach the screen.
         *
         **/
        int ms = data::braniacs[MAX_ENEMIES_PER_CLASS - 1].getMoveSpeed();
        int x_first = data::braniacs[0].getX();
        int x_last = data::braniacs[MAX_ENEMIES_PER_CLASS - 1].getX();

        if (((ms > 0) && (x_last + ms > COLS - 6)) ||
            ((ms < 0) && (x_first + ms < 5)))
        {
          for (int i = 0; i < MAX_ENEMIES_PER_CLASS; ++i) {
            // All classes should have the same movespeed.
            data::braniacs[i].setMoveSpeed(-1 * ms);
            data::braniacs[i].setY(data::braniacs[i].getY() + 1);

            data::landers[i].setMoveSpeed(-1 * ms);
            data::landers[i].setY(data::landers[i].getY() + 1);

            data::tanks[i].setMoveSpeed(-1 * ms);
            data::tanks[i].setY(data::tanks[i].getY() + 1);
          }
        }

        /***
         * Move the enemy to the left/right. Use entity::moveRight()
         * because we invert the movement speed in the previous block of code.
         * Also at this point, we're going to check if the game is complete
         * or not, if the enemies have reached the player, or the player
         * has killed all the enemies.
         *
         **/
        int braniacs_alive = 0;
        int tanks_alive = 0;
        int landers_alive = 0;

        int braniacs_y = data::braniacs[0].getY();
        int tanks_y = data::landers[0].getY();
        int landers_y = data::landers[0].getY();

        for (int i = 0; i < MAX_ENEMIES_PER_CLASS; ++i) {
          data::braniacs[i].setX(data::braniacs[i].getX() + ms);
          if (data::braniacs[i].isAlive()) {
            braniacs_alive++;
            data::braniacs[i].onTick();
          }
          
          data::tanks[i].setX(data::tanks[i].getX() + ms);
          if (data::tanks[i].isAlive()) {
            tanks_alive++;
            data::tanks[i].onTick();
          }

          data::landers[i].setX(data::landers[i].getX() + ms);
          if (data::landers[i].isAlive()) {
            landers_alive++;
            data::landers[i].onTick();
          }
        }

        if (!braniacs_alive && !landers_alive && !tanks_alive) {
          glib::screenHandle->win();
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

        // === 3a. Process Beam ===
        if (*(data::beam_frames_rendered) > 0) {
          for (int i = 0; i < MAX_ENEMIES_PER_CLASS; ++i) {
            // Check if the enemy is alive, and if they are in the beam.
            // If so, remove health (per tick) as defined in "config.h".
            if (data::braniacs[i].isAlive()) {
              if (abs(data::braniacs[i].getX() - data::player->getX()) 
                  <= 3) {
                data::braniacs[i].removeHealth(BEAM_DAMAGE);
              }
            }

            if (data::tanks[i].isAlive()) {
              if (abs(data::tanks[i].getX() - data::player->getX()) 
                  <= 3) {
                // Tanks take 60% damage from the beam, to balance the game.
                data::tanks[i].removeHealth(BEAM_DAMAGE * 0.6);
              }
            }

            if (data::landers[i].isAlive()) {
              if (abs(data::landers[i].getX() - data::player->getX()) 
                  <= 3) {
                data::landers[i].removeHealth(BEAM_DAMAGE);
              }
            }
          }
        }
      } // End in-game specific I/O behaviour.
    } // End game specific I/O behaviour.

    // implement FPS cap delay
    nanosleep(&delay, &delay2);
  } // End process loop.

  return 0;
}