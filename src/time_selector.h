#pragma once

#include <pebble.h>

typedef void (*TimeSelected)(struct tm value);
void show_time_selector(struct tm initial, TimeSelected callback);
void hide_time_selector(void);
