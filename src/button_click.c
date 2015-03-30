#include <pebble.h>
#include <main_window.h>

static void init(void) {
  show_main_window();
}

static void deinit(void) {
  hide_main_window();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}