#include <pebble.h>
  
#include "counting_window.h"
#include "data.h"

Window *my_window;

void handle_init(void) {
  my_window = window_create();
  show_counting_window();
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  storage_init();
  handle_init();
  app_event_loop();
  handle_deinit();
  storage_persist();
}
