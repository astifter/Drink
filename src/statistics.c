#include <pebble.h>

#include "statistics.h"

static Window *s_window;
static TextLayer *s_text_layer;
static ScrollLayer *s_scroll_layer;
static char s_scroll_text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam quam tellus, fermentu  m quis vulputate quis, vestibulum interdum sapien. Vestibulum lobortis pellentesque pretium. Quisque ultricies purus e  u orci convallis lacinia. Cras a urna mi. Donec convallis ante id dui dapibus nec ullamcorper erat egestas. Aenean a m  auris a sapien commodo lacinia. Sed posuere mi vel risus congue ornare. Curabitur leo nisi, euismod ut pellentesque se  d, suscipit sit amet lorem. Aliquam eget sem vitae sem aliquam ornare. In sem sapien, imperdiet eget pharetra a, lacin  ia ac justo. Suspendisse at ante nec felis facilisis eleifend.";

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  Layer* s_window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(s_window_layer);
  
  s_text_layer = text_layer_create(GRect(0, 0, 144, 1000));
  text_layer_set_text(s_text_layer, s_scroll_text);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  GSize max_size = text_layer_get_content_size(s_text_layer);
  text_layer_set_size(s_text_layer, max_size);

  s_scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(s_scroll_layer, s_window);
  scroll_layer_set_content_size(s_scroll_layer, GSize(144, max_size.h + 4));

  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_text_layer));

  layer_add_child(s_window_layer, scroll_layer_get_layer(s_scroll_layer));
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_text_layer);
  scroll_layer_destroy(s_scroll_layer);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_statistics(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_statistics(void) {
  window_stack_remove(s_window, true);
}
