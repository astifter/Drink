#include <pebble.h>

#include "time_selector.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_34_medium_numbers;
static GBitmap *s_res_image_up;
static GBitmap *s_res_image_left_right;
static GBitmap *s_res_image_down;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;
static Layer *s_layer_1;
static Layer *s_layer_2;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  s_res_image_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_UP);
  s_res_image_left_right = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LEFT_RIGHT);
  s_res_image_down = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DOWN);
  
  // s_layer_1
  s_layer_1 = layer_create(GRect(9, 56, 46, 40));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_layer_1);
  
  // s_layer_2
  s_layer_2 = layer_create(GRect(65, 56, 46, 40));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_layer_2);

  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(4, 55, 54, 42));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "00");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(60, 55, 54, 42));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text(s_textlayer_2, "00");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_2, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(6, 55, 105, 42));
  text_layer_set_background_color(s_textlayer_3, GColorClear);
  text_layer_set_text(s_textlayer_3, ":");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_3, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_image_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_left_right);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_down);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  layer_destroy(s_layer_1);
  layer_destroy(s_layer_2);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_image_up);
  gbitmap_destroy(s_res_image_left_right);
  gbitmap_destroy(s_res_image_down);
}
// END AUTO-GENERATED UI CODE

static void s_layer_1_draw(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  // Draw a black filled rectangle with sharp corners
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 4, GCornersAll);
}

static struct tm selected_time;
static int selected_time_index;
static char selected_time_string1[20];
static char selected_time_string2[20];
static TimeSelected callback;

static void update_selected_time_string(void) {
  if (selected_time.tm_min < 0) {
    selected_time.tm_min += 60;
    selected_time.tm_hour--;
  } else if (selected_time.tm_min >= 60) {
    selected_time.tm_min -= 60;
    selected_time.tm_hour++;
  }
  if (selected_time.tm_hour < 0) {
    selected_time.tm_hour += 24;
  } else if (selected_time.tm_hour >= 24) {
    selected_time.tm_hour -= 24;
  }
  snprintf(selected_time_string1, 10, "%02d", selected_time.tm_hour);
  text_layer_set_text(s_textlayer_1, selected_time_string1);
  snprintf(selected_time_string2, 10, "%02d", selected_time.tm_min);
  text_layer_set_text(s_textlayer_2, selected_time_string2);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (selected_time_index == 1)
    selected_time.tm_hour++;
  else
    selected_time.tm_min+=5;
  update_selected_time_string();
}

static void selector_visibilty(void) {
  if (selected_time_index == 1) {
    layer_set_hidden(s_layer_1, false);
    layer_set_hidden(s_layer_2, true);
    text_layer_set_text_color(s_textlayer_1, GColorWhite);
    text_layer_set_text_color(s_textlayer_2, GColorBlack);
  } else if (selected_time_index == 2) {
    layer_set_hidden(s_layer_1, true);
    layer_set_hidden(s_layer_2, false);
    text_layer_set_text_color(s_textlayer_1, GColorBlack);
    text_layer_set_text_color(s_textlayer_2, GColorWhite);
  }
  layer_mark_dirty(text_layer_get_layer(s_textlayer_1));
  layer_mark_dirty(text_layer_get_layer(s_textlayer_2));
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (selected_time_index == 1) {
    selected_time_index = 2;
  } else if (selected_time_index == 2) {
    selected_time_index = 1;
  }
  selector_visibilty();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (selected_time_index == 1)
    selected_time.tm_hour--;
  else
    selected_time.tm_min-=5;
  update_selected_time_string();
}

static void config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
  callback(selected_time);
}

void show_time_selector(struct tm initial, TimeSelected c) {
  callback = c;
  selected_time = initial;
  selected_time_index = 1;
  initialise_ui();
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, config_provider);
  layer_set_update_proc(s_layer_1, s_layer_1_draw);
  layer_set_update_proc(s_layer_2, s_layer_1_draw);
  selector_visibilty();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  update_selected_time_string();
  window_stack_push(s_window, true);
}

void hide_uint_selector(void) {
  window_stack_remove(s_window, true);
}
