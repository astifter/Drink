#include "time_selector.h"
#include "pebble.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

static Window *s_main_window;
static MenuLayer *s_menu_layer;

static struct tm first_reminder;
static struct tm interval;
static unsigned int target_number = 6;

static unsigned int number_glasses = 0;

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
          snprintf(buffer, 10, "%d", target_number); 
          menu_cell_basic_draw(ctx, cell_layer, "Target", buffer, NULL);
        } break;
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->row) {
    case 0: {
      show_time_selector(first_reminder);
    } break;
  }

}

static void main_window_load(Window *window) {
  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  first_reminder.tm_min = 0;
  first_reminder.tm_hour = 9;
  interval.tm_min = 30;
  interval.tm_hour = 1;
  
  init();
  app_event_loop();
  deinit();
}
