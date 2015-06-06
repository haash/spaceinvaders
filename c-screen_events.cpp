#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include "config.h"

#include <curses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "c-globals.h"

char screenEvents::getCurrentScreen() const {
  return this->current_screen;
}

void screenEvents::setScreen(char screenNumber) {
  this->current_screen = screenNumber;
}

WINDOW* screenEvents::getListener() {
  if (this->listener == 0) {
    this->listener = newwin(1, 1, 0, 0);
    
    // Set the listener to not wait for input on
    // this particular screen.
    nodelay(listener, true);
  }
  return this->listener;
}

WINDOW* screenEvents::getMainWindow() {
  if (this->main_window == 0) {
    this->main_window = newwin(LINES, COLS, 0, 0);
    this->drawBorder(this->main_window, '+', '|', '-');
  }
  return this->main_window;
}

bool screenEvents::addWindow(WINDOW* new_window) {
  for (int i = 0; i < 9; ++i) {
    if (this->all_windows[i] == 0) {
      this->all_windows[i] = new_window;
      return true;
    }
  }
  return false;
}

// convenience function, make sure all the addWindow() functionality
// gets done.
WINDOW* screenEvents::createWindow(int h, int w, int y, int x) {
  WINDOW* new_window = newwin(h, w, y, x);
  this->addWindow(new_window);
  return new_window;
}

WINDOW* screenEvents::createWindowWithBorder(int h, int w, int y, int x) {
  WINDOW* new_window = this->createWindow(h, w, y, x);
  this->drawBorder(new_window, '+', '|', '-');
  return new_window;
}

void screenEvents::removeWindows() {
  for (int i = 0; i < 9; ++i) {
    if (this->all_windows[i] != 0) {
      this->removeWindow(this->all_windows[i]);
    }
  }
}

bool screenEvents::removeWindow(WINDOW* old_window) {
  for (int i = 0; i < 9; ++i) {
    if (this->all_windows[i] == old_window) {
      delwin(this->all_windows[i]);
      this->all_windows[i] = 0;
      return true;
    }
  }
  return false; // doesn't exist
}

void screenEvents::printCenter(WINDOW *window, int y, const char *str) const {
  int length_str = strlen(str);

  int w, h;
  getmaxyx(window, h, w);

  int str_start = (w - length_str) / 2;

  for (int i = 0; i < length_str; ++i)
    mvwaddch(window, y, str_start + i, str[i]);
}

int screenEvents::printWrapped(WINDOW *window, int y, int padding,
                                const char *str) const {
  int length_str = strlen(str);

  int w, h;
  getmaxyx(window, h, w);

  int x = padding;

  for (int i = 0; i < length_str; ++i) {
    // wrap to a new line if about to exceed length
    // TODO(Mitch): Fix wrap spacing properly on words later
    if (x > (w - padding - 2) && str[i + 1] != ' ') {
      if (str[i] != ' ') {
        mvwaddch(window, y, x, '-');
        i--;
      }

      x = padding;
      y += 1;
    } else {
      // output current character
      mvwaddch(window, y, x, str[i]);
      x++;
    }
    
  }

  return y;
}

void screenEvents::drawBorder(WINDOW *window, char corners, 
                              char vertical, char horizontal) const {
  int w, h;
  getmaxyx(window, h, w);

  // corners
  mvwaddch(window, 0, 0, corners);
  mvwaddch(window, h - 1, 0, corners);
  mvwaddch(window, 0, w - 1, corners);
  mvwaddch(window, h - 1, w - 1, corners);

  // top/bottom
  for (int x = 1; x < (w - 1); ++x) {
    mvwaddch(window, 0, x, horizontal);
    mvwaddch(window, h - 1, x, horizontal);
  }

  // left/right
  for (int y = 1; y < (h - 1); ++y) {
    mvwaddch(window, y, 0, vertical);
    mvwaddch(window, y, w - 1, vertical);
  }
}

void screenEvents::preNewScreen() {
  this->removeWindows();
  erase();
  refresh();
}

void screenEvents::generateBG(char ch) {
  srand(time(NULL));

  WINDOW *bg = this->createWindow(LINES, COLS, 0, 0);
  for (int y = 0; y < LINES; ++y) {
    for (int x = 0; x < COLS; ++x) {
      char star = ' ';
      if (rand() % 3 == 1) {
        star = ch;
      }
      mvwaddch(bg, y, x, star);
    }
  }
  wrefresh(bg);
}

void screenEvents::menu() {
  this->preNewScreen();
  this->generateBG('.');

  int window_start_x = COLS / 8;
  int window_width = 6 * COLS / 8;

  WINDOW *title_window = this->createWindowWithBorder(5, window_width, 0, 
                                                      window_start_x);
  this->printCenter(title_window, 2, "Space Invaders");
  wrefresh(title_window);

  WINDOW *main_content = this->createWindowWithBorder(11, window_width, 6, 
                                                      window_start_x);

  // will take up > 1 line, autowrap and take the current line output
  int current_y = this->printWrapped(main_content, 2, 2, 
    "Press the corresponding number to select from the menu.");

  current_y = this->printWrapped(main_content, current_y + 2, 2,
    "1: Start New Game (Default Character).");
 
  current_y = this->printWrapped(main_content, current_y + 1, 2,
    "2: Start New Game (Swift).");

  current_y = this->printWrapped(main_content, current_y + 1, 2,
    "3: Start New Game (Beam).");

  current_y = this->printWrapped(main_content, current_y + 1, 2,
    "4: Exit Game.");
  wrefresh(main_content);
  
  mvwprintw(main_content, 0, 0, "%x", main_content);

  WINDOW *copyright = this->createWindowWithBorder(6, window_width,
                                                   current_y + 10, 
                                                   window_start_x);
  current_y = this->printWrapped(copyright, 2, 2, 
    "Use WASD to control the ship, and spacebar to fire.");
  this->printWrapped(copyright, current_y + 1, 2, 
    "Created by Mitch Phillips (2015).");

  wrefresh(copyright);

  this->setScreen('m');
}

void screenEvents::setupGame() {
  this->preNewScreen();

  delete[] data::braniacs;
  delete[] data::landers;
  delete[] data::tanks;

  data::braniacs = new braniac[MAX_ENEMIES_PER_CLASS];
  data::landers = new lander[MAX_ENEMIES_PER_CLASS];
  data::tanks = new tank[MAX_ENEMIES_PER_CLASS];

  for (int i = 0; i < MAX_ENEMIES_PER_CLASS; ++i) {
    data::braniacs[i].setX(5 + i * 7);
    data::landers[i].setX(5 + i * 7);
    data::tanks[i].setX(5 + i * 7);

    // if you use 3x3 characters, update this spacing...
    data::braniacs[i].setY(2);
    data::tanks[i].setY(5);
    data::landers[i].setY(8);
  }

  // setup bullets
  delete[] data::bullets;
  data::bullets = new bullet[MAX_BULLETS];

  // setup player
  data::player->setX(COLS / 2);
  data::player->setY(LINES - 4);
  *(data::beam_frames_rendered) = 0;

  this->setScreen('g');
}

void screenEvents::die() {
  this->preNewScreen();
  this->generateBG('x');

  int window_start_x = COLS / 6;
  int window_width = 4 * COLS / 6;
  WINDOW *main = this->createWindowWithBorder(9, window_width,
                                              LINES / 2 - 4, window_start_x);

  this->printCenter(main, 2, "You have died.");
  mvwprintw(main, 4, 2, "You scored: %i", *data::score);

  this->printWrapped(main, 5, 2, 
    "Press \"x\" or \"q\" to quit the game. Otherwise, press \"m\" to "
    "go back to the menu.");

  wrefresh(main);
  this->setScreen('d');
}

void screenEvents::win() {
  this->preNewScreen();
  this->generateBG('*');

  int window_start_x = COLS / 6;
  int window_width = 4 * COLS / 6;
  WINDOW *main = this->createWindowWithBorder(9, window_width,
                                              LINES / 2 - 4, window_start_x);

  this->printCenter(main, 2, "Congratulations, you won!");
  mvwprintw(main, 4, 2, "You scored: %i", *data::score);

  this->printWrapped(main, 5, 2, 
    "Press \"x\" or \"q\" to quit the game. Otherwise, press \"m\" to "
    "go back to the menu.");

  wrefresh(main);

  // use same screen keyhandlers as the death screen.
  this->setScreen('d');
}

screenEvents::screenEvents() {
  for (int i = 0; i < 10; ++i) {
    this->all_windows[i] = 0;
  }
  this->listener = 0;
}