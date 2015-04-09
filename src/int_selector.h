#pragma once

#include <pebble.h>

typedef void (*IntSelected)(int value);
void show_int_selector(int initial, bool keep_positive, IntSelected callback);
void hide_int_selector(void);
