#include <pebble.h>

#include "data_entry.h"

#include "data.h"
#include "timing_handler.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_up;
static GBitmap *s_res_image_snooze;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_14;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  
  s_res_image_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_UP);
  s_res_image_snooze = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SNOOZE);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_snooze);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(0, 0, 144, 62));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "Drink up!");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(0, 115, 118, 16));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text(s_textlayer_2, "Snooze for 30 mins.");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(0, 67, 118, 16));
  text_layer_set_text(s_textlayer_3, "I drank a glass!");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  gbitmap_destroy(s_res_image_up);
  gbitmap_destroy(s_res_image_snooze);
}
// END AUTO-GENERATED UI CODE

static char drank_glasses_string[100];

static void update_drank_glasses(void) {
  if (storage.drank_glasses == 0) {
    snprintf(drank_glasses_string, 99, "You drank\nnothing\ntoday!");
  } else if (storage.drank_glasses == 1) {
    snprintf(drank_glasses_string, 99, "You drank\n1\nglass!");
  } else if (storage.drank_glasses > 1) {
    snprintf(drank_glasses_string, 99, "You drank\n%d\nglasses!", storage.drank_glasses);
  }
  text_layer_set_text(s_textlayer_1, drank_glasses_string);  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  storage.drank_glasses++;
  storage_persist();
  update_drank_glasses();
}

static void snooze_click_handler(ClickRecognizerRef recognizer, void *context) {
  timing_handler_snooze();
}

static void config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, snooze_click_handler);
}
static void config_provider_nosnooze(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_data_entry(bool hide_snoozing) {
  initialise_ui();
  update_drank_glasses();
  if (hide_snoozing) {
    action_bar_layer_set_click_config_provider(s_actionbarlayer_1, config_provider_nosnooze);
    layer_set_hidden(text_layer_get_layer(s_textlayer_2), true);
    action_bar_layer_clear_icon(s_actionbarlayer_1, BUTTON_ID_DOWN);
  } else {
    action_bar_layer_set_click_config_provider(s_actionbarlayer_1, config_provider);
  }
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_data_entry(void) {
  window_stack_remove(s_window, true);
}
