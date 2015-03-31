#include "time_selector.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_img_actionbar_up;
static GBitmap *s_res_img_actionbar_checkmark;
static GBitmap *s_res_img_actionbar_down;
static GFont s_res_bitham_34_medium_numbers;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_textlayer_1;

static struct tm selected_time;
static int selected_time_index = 1;
static char selected_time_string[20];

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (selected_time_index == 1)
    selected_time.tm_hour++;
  else
    selected_time.tm_min++;
  snprintf(selected_time_string, 10, "%02d:%02d", selected_time.tm_hour, selected_time.tm_min); 
  text_layer_set_text(s_textlayer_1, selected_time_string);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (selected_time_index == 1)
    selected_time_index = 2;
  else if (selected_time_index == 2)
    selected_time_index = 1;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (selected_time_index == 1)
    selected_time.tm_hour--;
  else
    selected_time.tm_min--;
  snprintf(selected_time_string, 10, "%02d:%02d", selected_time.tm_hour, selected_time.tm_min); 
  text_layer_set_text(s_textlayer_1, selected_time_string);
}

static void config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  s_res_img_actionbar_up = gbitmap_create_with_resource(RESOURCE_ID_IMG_ACTIONBAR_UP);
  s_res_img_actionbar_checkmark = gbitmap_create_with_resource(RESOURCE_ID_IMG_ACTIONBAR_RIGHT);
  s_res_img_actionbar_down = gbitmap_create_with_resource(RESOURCE_ID_IMG_ACTIONBAR_DOWN);
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  
    // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, config_provider);
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorClear);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_img_actionbar_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_img_actionbar_checkmark);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_img_actionbar_down);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 51, 122, 42));
  snprintf(selected_time_string, 10, "%02d:%02d", selected_time.tm_hour, selected_time.tm_min); 
  text_layer_set_text(s_textlayer_1, selected_time_string);
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_bitham_34_medium_numbers);
  text_layer_set_text_color(s_textlayer_1, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(s_textlayer_1);
  gbitmap_destroy(s_res_img_actionbar_up);
  gbitmap_destroy(s_res_img_actionbar_checkmark);
  gbitmap_destroy(s_res_img_actionbar_down);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_time_selector(struct tm initial_time) {
  selected_time = initial_time;
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_time_selector(void) {
  window_stack_remove(s_window, true);
}

struct tm get_selected_time(void) {
  return selected_time;
}