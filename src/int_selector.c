#include <pebble.h>

#include "int_selector.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_34_medium_numbers;
static GBitmap *s_res_image_up;
static GBitmap *s_res_image_down;
static TextLayer *s_textlayer_1;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  s_res_image_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_UP);
  s_res_image_down = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DOWN);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(6, 54, 110, 42));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "00");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_image_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_down);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_image_up);
  gbitmap_destroy(s_res_image_down);
}
// END AUTO-GENERATED UI CODE

static int selected;
static bool keep_positive;
static char selected_int_string1[20];
static IntSelected callback;

static void update_selected_time_string(void) {
  if (keep_positive && selected < 0)
    selected = 0;
  snprintf(selected_int_string1, 10, "%d", selected);
  text_layer_set_text(s_textlayer_1, selected_int_string1);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  selected++;
  update_selected_time_string();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  selected--;
  update_selected_time_string();
}

static void config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
  callback(selected);
}

void show_int_selector(int initial, bool k, IntSelected c) {
  callback = c;
  selected = initial;
  keep_positive = k;
  initialise_ui();
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  update_selected_time_string();
  window_stack_push(s_window, true);
}

void hide_int_selector(void) {
  window_stack_remove(s_window, true);
}
