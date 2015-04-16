#include <pebble.h>

#include "statistics.h"

#include "data.h"
#include "timing_handler.h"

static Window *s_window;
static TextLayer *s_text_layer;
static ScrollLayer *s_scroll_layer;

static char s_scroll_text[1024];

static void settext(void) {
  char* buffer = s_scroll_text;
  if (storage.drank_glasses == 0) {
    buffer += snprintf(buffer, 50, "You drank nothing today.\n");   
  } else if (storage.drank_glasses == 1) {
    buffer += snprintf(buffer, 50, "You drank 1 glass today.\n");   
  } else {
    buffer += snprintf(buffer, 50, "You drank %d glasses today.\n", storage.drank_glasses);   
  }
    
  time_t next_timestamp;
  bool scheduled = timing_handler_next(&next_timestamp);
  if (scheduled) {
    struct tm *next = localtime(&next_timestamp);
    buffer += snprintf(buffer, 50, "Timer scheduled for:\n%04d-%02d-%02d %02d:%02d\n", 
                                   next->tm_year+1900, next->tm_mon+1, next->tm_mday,
                                   next->tm_hour, next->tm_min);
    bool snoozed = timing_handler_next_snooze(&next_timestamp);
    if (snoozed) {
      next = localtime(&next_timestamp);
      buffer += snprintf(buffer, 50, "Snooze scheduled for:\n%04d-%02d-%02d %02d:%02d\n", 
                                     next->tm_year+1900, next->tm_mon+1, next->tm_mday,
                                     next->tm_hour, next->tm_min);
    }
  } else {
    buffer += snprintf(buffer, 50, "There is no timer scheduled.");
  }

  text_layer_set_text(s_text_layer, s_scroll_text);
}

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  Layer* s_window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(s_window_layer);
  
  s_text_layer = text_layer_create(GRect(0, 0, 144, 1000));
  settext();
  // text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
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
