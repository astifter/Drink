#include <pebble.h>
  
#include "counting_window.h"
#include "data_entry.h"
#include "data.h"

int main(void) {
  storage_init();
  show_counting_window();
  app_event_loop();
  storage_persist();
}
