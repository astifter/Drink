#include <pebble.h>

#include "main_window.h"
#include "time_selector.h"
#include "int_selector.h"
#include "statistics.h"
  
#include "data.h"
#include "timing_handler.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_menulayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  Layer* s_window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(s_window_layer);
  
  // s_menulayer_1
  s_menulayer_1 = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menulayer_1, s_window);
  layer_add_child(s_window_layer, (Layer *)s_menulayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  menu_layer_destroy(s_menulayer_1);
}
// END AUTO-GENERATED UI CODE

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 5

void first_reminder_selected(struct tm value) {
  storage.first_reminder = value;
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

void interval_selected(struct tm value) {
  storage.interval = value;
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

void target_number_selected(int value) {
  storage.target_number = value;
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "Statistics");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Settings");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  char buffer[50];
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0: {
          if (storage.drank_glasses == 0) {
            snprintf(buffer, 50, "You drank nothing.");
          } else if (storage.drank_glasses == 1) {
            snprintf(buffer, 50, "You drank 1 glass.");
          } else if (storage.drank_glasses > 1) {
            snprintf(buffer, 50, "You drank %d glasses.", storage.drank_glasses);
          }
          menu_cell_basic_draw(ctx, cell_layer, "Statistics", buffer, NULL);
        } break;
      }
      break;
    case 1:
      switch (cell_index->row) {
        case 0: {
          if (storage.reminders_activated) {
            snprintf(buffer, 10, "enabled");
          } else {
            snprintf(buffer, 10, "disabled");
          }
          menu_cell_basic_draw(ctx, cell_layer, "Reminder", buffer, NULL);
        } break;
        case 1: {
          snprintf(buffer, 10, "%02d:%02d", storage.first_reminder.tm_hour, storage.first_reminder.tm_min);
          menu_cell_basic_draw(ctx, cell_layer, "First Reminder", buffer, NULL);
        } break;
        case 2: {
          snprintf(buffer, 10, "%02d:%02d", storage.interval.tm_hour, storage.interval.tm_min);
          menu_cell_basic_draw(ctx, cell_layer, "Interval", buffer, NULL);
        } break;
        case 3: {
          snprintf(buffer, 10, "%d glasses", storage.target_number);
          menu_cell_basic_draw(ctx, cell_layer, "Target", buffer, NULL);
        } break;
        case 4: {
          if (storage.auto_dismiss) {
            snprintf(buffer, 10, "on");
          } else {
            snprintf(buffer, 10, "off");
          }
          menu_cell_basic_draw(ctx, cell_layer, "Auto Dismiss", buffer, NULL);
        } break; 
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0: {
          show_statistics();
        } break;
      }
      break;
    case 1:
      switch (cell_index->row) {
        case 0: {
          storage.reminders_activated = !storage.reminders_activated;
          storage_persist();
          if(storage.reminders_activated) {
            timing_handler_enable();
          } else {
            timing_handler_cancel();
          }
          layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
        } break;
        case 1: {
          show_time_selector(storage.first_reminder, first_reminder_selected);
        } break;
        case 2: {
          show_time_selector(storage.interval, interval_selected);
        } break;
        case 3: {
          show_int_selector(storage.target_number, true, target_number_selected);
        } break;
        case 4: {
          storage.auto_dismiss = !storage.auto_dismiss;
          layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
        } break;
      }
      break;
  }
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_main_window(void) {
  initialise_ui();
  menu_layer_set_callbacks(s_menulayer_1, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_main_window(void) {
  window_stack_remove(s_window, true);
}
