#pragma once

#include <pebble.h>

typedef struct {
  bool first_screen_shown;
  int drank_glasses;
  struct tm first_reminder;
  struct tm interval;
  unsigned int target_number;
  bool reminders_activated;
} DrinkData;
DrinkData storage;

void storage_init(void);
void storage_persist(void);
