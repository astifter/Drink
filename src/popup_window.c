#include <pebble.h>

#include "popup_window.h"

#include "data.h"
#include "timing_handler.h"
#include "watchface_base/logging_helper.h"
#include "data_logging.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_plus;
static GBitmap *s_res_image_snooze;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_24_bold;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  
  s_res_image_plus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLUS);
  s_res_image_snooze = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SNOOZE);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_plus);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_snooze);
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
  text_layer_set_text(s_textlayer_2, "Snooze");
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
  gbitmap_destroy(s_res_image_snooze);
}
// END AUTO-GENERATED UI CODE

static char drank_glasses_string[100];

static void update_drank_glasses(void) {
  LOG_FUNC();
  storage_get_glasses_string(drank_glasses_string, true);
  text_layer_set_text(s_textlayer_1, drank_glasses_string);  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  LOG_FUNC();
  storage.drank_glasses++;
  storage_persist();
  data_logging_do(data_logging_type_glass, storage.drank_glasses);
  update_drank_glasses();
  timing_handler_reschedule();
}

static void snooze_click_handler(ClickRecognizerRef recognizer, void *context) {
  LOG_FUNC();
  timing_handler_snooze();
  hide_popup_window();
}

static void config_provider(void *ctx) {
  LOG_FUNC();
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, snooze_click_handler);
}

static void config_provider_nosnooze(void *ctx) {
  LOG_FUNC();
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void handle_window_unload(Window* window) {
  LOG_FUNC();
  destroy_ui();
}

void vibrate(void) {
  LOG_FUNC();
  if (storage.vibrate_on_reminder) {
    data_logging_do(data_logging_type_vibrating, 0);
    static const uint32_t const segments[] = { 200, 100, 400 };
    VibePattern pat = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH(segments),
    };
    vibes_enqueue_custom_pattern(pat);
  }
}

static AppTimer* dismiss_timer;
static void dismiss_timer_callback(void *data) {
  LOG_FUNC();
  data_logging_do(data_logging_type_autodismissed, 0);
  hide_popup_window();
}
static AppTimer* vibrate_timer;
static void vibrate_timer_callback(void *data) {
  LOG_FUNC();
  vibrate();
}

void show_popup_window(bool hide_snoozing) {
  LOG_FUNC();
  vibrate();
  vibrate_timer = app_timer_register(2*60*1000, vibrate_timer_callback, NULL);
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
  if (storage.auto_dismiss)
    dismiss_timer = app_timer_register(5*60*1000, dismiss_timer_callback, NULL);
}

void hide_popup_window(void) {
  LOG_FUNC();
  app_timer_cancel(vibrate_timer);
  if (storage.auto_dismiss)
    app_timer_cancel(dismiss_timer);
  window_stack_remove(s_window, true);
}
