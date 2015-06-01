// Library includes
#include <pthread.h>

// Include all child classes (parents should be called through the child
// class includes).

#include "c-beam.h"
#include "c-braniac.h"
#include "c-default.h"
#include "c-lander.h"
#include "c-swift.h"
#include "c-tank.h"

/* 
 * Include global namespaces.
 * This includes all of libraries, like ncurses, etc.
 */

#include "c-globals.h"

int main() {
  beamShip bar;
  glib::curses->startCurses();
  
  glib::screenHandle->menu();

  pthread_t screen;
  pthread_t process;

  pthread_create(&screen, NULL, drivers::screen, (void*) 0);
  pthread_create(&process, NULL, drivers::process, (void*) 0);

  pthread_join(screen, NULL);
  pthread_join(process, NULL);

  return 0;
}