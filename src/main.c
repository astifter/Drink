#include <pebble.h>
  
#include "main_window.h"

Window *my_window;

void handle_init(void) {
  my_window = window_create();
  show_main_window();
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
