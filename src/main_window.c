#include <pebble.h>

#include "main_window.h"
#include "time_selector.h"
#include "int_selector.h"
#include "statistics.h"
  
#include "data.h"
#include "timing_handler.h"

static GBitmap *s_help_qr_code;
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_menulayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  
  // s_menulayer_1
  s_menulayer_1 = menu_layer_create(GRect(0, 0, 144, 152));
  menu_layer_set_click_config_onto_window(s_menulayer_1, s_window);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_menulayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  menu_layer_destroy(s_menulayer_1);
}
// END AUTO-GENERATED UI CODE

#define NUM_MENU_SECTIONS       3
#define NUM_FIRST_MENU_ITEMS    1
#define NUM_SECOND_MENU_ITEMS   7
#define NUM_THIRD_MENU_ITEMS    1

void store_and_update_reminder(void) {
  storage_persist();
  if(storage.reminders_activated) {
    timing_handler_cancel();
    timing_handler_enable();
  } else {
    timing_handler_cancel();
  }
  layer_mark_dirty(menu_layer_get_layer(s_menulayer_1));
}

void first_reminder_selected(struct tm value) {
  storage.first_reminder = value;
  store_and_update_reminder();
}

void interval_selected(struct tm value) {
  storage.interval = value;
  store_and_update_reminder();
}

void target_number_selected(int value) {
  storage.target_number = value;
  store_and_update_reminder();
}

void number_glasses_corrected(int value) {
  storage.drank_glasses = value;
  storage_persist();
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
    case 2:
      return NUM_THIRD_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static int16_t menu_get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  switch (cell_index->section) {
    case 0:
      return 40;
    case 1:
      return 40;
    case 2:
      return 144;
  }
  return 40;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "Statistics");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Settings");
      break;
    case 2:
      menu_cell_basic_header_draw(ctx, cell_layer, "Help");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  char buffer[50];
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0: {
          storage_get_glasses_string(buffer, false);
          menu_cell_basic_draw(ctx, cell_layer, "Statistics", buffer, NULL);
        } break;
      }
      break;
    case 1:
      switch (cell_index->row) {
        case 0: {
          if (storage.reminders_activated) {
            snprintf(buffer, 49, "enabled");
          } else {
            snprintf(buffer, 49, "disabled");
          }
          menu_cell_basic_draw(ctx, cell_layer, "Reminder", buffer, NULL);
        } break;
        case 1: {
          snprintf(buffer, 49, "%02d:%02d", storage.first_reminder.tm_hour, storage.first_reminder.tm_min);
          menu_cell_basic_draw(ctx, cell_layer, "First Reminder", buffer, NULL);
        } break;
        case 2: {
          snprintf(buffer, 49, "%02d:%02d hours", storage.interval.tm_hour, storage.interval.tm_min);
          menu_cell_basic_draw(ctx, cell_layer, "Interval", buffer, NULL);
        } break;
        case 3: {
          snprintf(buffer, 49, "%d glasses", storage.target_number);
          menu_cell_basic_draw(ctx, cell_layer, "Target", buffer, NULL);
        } break;
        case 4: {
          if (storage.auto_dismiss) {
            snprintf(buffer, 49, "on");
          } else {
            snprintf(buffer, 49, "off");
          }
          menu_cell_basic_draw(ctx, cell_layer, "Auto Dismiss", buffer, NULL);
        } break; 
        case 5: {
          if (storage.vibrate_on_reminder) {
            snprintf(buffer, 49, "on");
          } else {
            snprintf(buffer, 49, "off");
          }
          menu_cell_basic_draw(ctx, cell_layer, "Vibrate", buffer, NULL);
        } break; 
        case 6: {
          menu_cell_basic_draw(ctx, cell_layer, "Manual Correction", "Change # of logged drinks.", NULL);
        } break; 
      }
      break;
    case 2:
      switch (cell_index->row) {
        case 0: {
          GSize size = layer_get_frame(cell_layer).size;
          graphics_draw_bitmap_in_rect(ctx, s_help_qr_code, GRect(0, 0, size.w, size.h));
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
          store_and_update_reminder();
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
          store_and_update_reminder();
        } break;
        case 5: {
          storage.vibrate_on_reminder = !storage.vibrate_on_reminder;
          store_and_update_reminder();
        } break;
        case 6: {
          show_int_selector(storage.drank_glasses, true, number_glasses_corrected);
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
  s_help_qr_code = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_QR_URL);
  menu_layer_set_callbacks(s_menulayer_1, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_cell_height = menu_get_cell_height_callback,
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
