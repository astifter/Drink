#include <pebble.h>

#include "main_window.h"
#include "time_selector.h"
#include "int_selector.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_menulayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  
  // s_menulayer_1
  s_menulayer_1 = menu_layer_create(GRect(0, 0, 144, 160));
  menu_layer_set_click_config_onto_window(s_menulayer_1, s_window);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_menulayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  menu_layer_destroy(s_menulayer_1);
}
// END AUTO-GENERATED UI CODE

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

static struct tm first_reminder;
static struct tm interval;
static unsigned int target_number;

void first_reminder_selected(struct tm value) {
  first_reminder = value;
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

void interval_selected(struct tm value) {
  interval = value;
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

void target_number_selected(int value) {
  target_number = value;
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
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
      menu_cell_basic_header_draw(ctx, cell_layer, "Settings");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0: {
          char buffer[10];
          snprintf(buffer, 10, "%02d:%02d", first_reminder.tm_hour, first_reminder.tm_min);
          menu_cell_basic_draw(ctx, cell_layer, "First Reminder", buffer, NULL);
        } break;
        case 1: {
          char buffer[10];
          snprintf(buffer, 10, "%02d:%02d", interval.tm_hour, interval.tm_min);
          menu_cell_basic_draw(ctx, cell_layer, "Interval", buffer, NULL);
        } break;
        case 2: {
          char buffer[10];
          snprintf(buffer, 10, "%d glasses", target_number);
          menu_cell_basic_draw(ctx, cell_layer, "Target", buffer, NULL);
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
          show_time_selector(first_reminder, first_reminder_selected);
        } break;
        case 1: {
          show_time_selector(interval, interval_selected);
        } break;
        case 2: {
          show_int_selector(target_number, true, target_number_selected);
        } break;
      }
      break;
  }
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_main_window(void) {
  first_reminder.tm_min = 0;
  first_reminder.tm_hour = 9;
  interval.tm_min = 30;
  interval.tm_hour = 1;
  target_number = 6;
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
