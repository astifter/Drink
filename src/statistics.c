#include <pebble.h>

#include "statistics.h"

#include "data.h"
#include "timing_handler.h"
#include "watchface_base/logging_helper.h"

static Window *s_window;
static TextLayer *s_text_layer;
static ScrollLayer *s_scroll_layer;

static char s_scroll_text[1024];

static int print_timer_info(char* buffer, char* name, WakeupId id) {
  time_t next_timestamp;
  if (id == E_UNKNOWN) {
    return snprintf(buffer, 50, "%s not scheduled. (%s)\n", name, logging_error_to_text(id));
  } else {
    if (id < 0) {
      return snprintf(buffer, 50, "%s has error. (%s)\n", name, logging_error_to_text(id));
    } else {
      bool scheduled = wakeup_query(id, &next_timestamp);
      struct tm *next = localtime(&next_timestamp);
      if (scheduled) {
        return snprintf(buffer, 50, "%s scheduled for:\n%04d-%02d-%02d %02d:%02d\n", name,
                                       next->tm_year+1900, next->tm_mon+1, next->tm_mday,
                                       next->tm_hour, next->tm_min);
      } else {
        return snprintf(buffer, 50, "%s not scheduled.\n", name);
      }
    }
  }
}

static void settext(void) {
  char* buffer = s_scroll_text;

  buffer += storage_get_glasses_string(buffer, false);
  buffer += snprintf(buffer, 10, "\n");

  buffer += print_timer_info(buffer, "Reminder", storage.s_wakeup_id);
  buffer += print_timer_info(buffer, "Snooze", storage.s_snooze_id);
  buffer += print_timer_info(buffer, "Bookkeeping", storage.s_bookkeeping_id);

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
