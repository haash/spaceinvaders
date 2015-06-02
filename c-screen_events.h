#ifndef __INCLUDE_SCREENEVENTS__
#define __INCLUDE_SCREENEVENTS__

#include <curses.h>

class screenEvents {
private:
  char current_screen;
  WINDOW* all_windows[10];
  WINDOW* listener;
  WINDOW* main_window;

public:
  char getCurrentScreen() const;
  void setScreen(char);

  WINDOW* getListener();
  WINDOW* getMainWindow();

  bool addWindow(WINDOW*);

  WINDOW* createWindow(int, int, int, int);
  WINDOW* createWindowWithBorder(int, int, int, int);

  void removeWindows(); // install gentoo
  bool removeWindow(WINDOW*);

  void printCenter(WINDOW*, int, const char*) const;
  int printWrapped(WINDOW*, int, int, const char*) const;

  void drawBorder(WINDOW*, char, char, char) const;

  /*
   * Call this before any of the following functions.
   * It does things like initialise the listener, and 
   * clears the screen for future input.
   */
  void preNewScreen();
  void generateBG(char);
  
  void menu();
  void setupGame();
  void die();
  void win();

  screenEvents();
};

#endif