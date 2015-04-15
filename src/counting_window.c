#include <pebble.h>

#include "main_window.h"
#include "data.h"
  
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_up;
static GBitmap *s_res_image_settings;
static GFont s_res_gothic_18_bold;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  Layer* s_window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(s_window_layer);
  
  s_res_image_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_UP);
  s_res_image_settings = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SETTINGS);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_settings);
  layer_add_child(s_window_layer, (Layer *)s_actionbarlayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(0, 0, 144, 62));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "You drank x glasses!");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(s_window_layer, (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(0, 115, 118, 16));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text(s_textlayer_2, "Stats & Settings");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentRight);
  layer_add_child(s_window_layer, (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(0, 67, 118, 16));
  text_layer_set_text(s_textlayer_3, "I drank a glass!");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentRight);
  layer_add_child(s_window_layer, (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  gbitmap_destroy(s_res_image_up);
  gbitmap_destroy(s_res_image_settings);
}
// END AUTO-GENERATED UI CODE

static char drank_glasses_string[100];

static void update_drank_glasses(void) {
  if (!storage.first_screen_shown) {
    snprintf(drank_glasses_string, 99, "Visit settings to\nenable continous\nreminders.");
    storage.first_screen_shown = true;
  } else {
    if (storage.drank_glasses == 0) {
      snprintf(drank_glasses_string, 99, "You drank\nnothing\ntoday!");
    } else if (storage.drank_glasses == 1) {
      snprintf(drank_glasses_string, 99, "You drank\n1\nglass!");
    } else if (storage.drank_glasses > 1) {
      snprintf(drank_glasses_string, 99, "You drank\n%d\nglasses!", storage.drank_glasses);
    }
  }
  text_layer_set_text(s_textlayer_1, drank_glasses_string);  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  storage.drank_glasses++;
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