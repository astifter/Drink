#include <pebble.h>

#include "counting_window.h"
#include "main_window.h"

#include "data.h"
  
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_24_bold;
static GBitmap *s_res_image_plus;
static GBitmap *s_res_image_wrench;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_res_image_plus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLUS);
  s_res_image_wrench = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WRENCH);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_plus);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_wrench);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(0, 0, 144, 62));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "You drank x glasses!");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(0, 106, 115, 28));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text(s_textlayer_2, "Settings");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_2, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(0, 58, 115, 28));
  text_layer_set_background_color(s_textlayer_3, GColorClear);
  text_layer_set_text(s_textlayer_3, "Add a glass!");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_3, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  gbitmap_destroy(s_res_image_plus);
  gbitmap_destroy(s_res_image_wrench);
}
// END AUTO-GENERATED UI CODE

static char drank_glasses_string[100];

static void update_drank_glasses(void) {
  if (!storage.first_screen_shown) {
    snprintf(drank_glasses_string, 99, "Visit settings to\nenable continous\nreminders.");
    storage.first_screen_shown = true;
    storage_persist();
  } else {
    storage_get_glasses_string(drank_glasses_string, true);
  }
  text_layer_set_text(s_textlayer_1, drank_glasses_string);  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  storage.drank_glasses++;
  storage_persist();
  update_drank_glasses();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_main_window();
  update_drank_glasses();
}

static void config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_counting_window(void) {
  initialise_ui();
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, config_provider);
  update_drank_glasses();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_counting_window(void) {
  window_stack_remove(s_window, true);
}
