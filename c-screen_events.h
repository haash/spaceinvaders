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
  char getCurrentScreen();
  void setScreen(char);

  WINDOW* getListener();
  WINDOW* getMainWindow();

  bool addWindow(WINDOW*);

  WINDOW* createWindow(int, int, int, int);
  WINDOW* createWindowWithBorder(int, int, int, int);

  void removeWindows(); // install gentoo
  bool removeWindow(WINDOW*);

  void printCenter(WINDOW*, int, const char*);
  int printWrapped(WINDOW*, int, int, const char*);

  void drawBorder(WINDOW*, char, char, char);

  /*
   * Call this before any of the following functions.
   * It does things like initialise the listener, and 
   * clears the screen for future input.
   */
  void preNewScreen(); 
  
  void menu();
  void setupGame();

  screenEvents();
};

#endif