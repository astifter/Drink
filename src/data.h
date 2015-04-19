#pragma once

#include <pebble.h>

typedef struct {
  bool first_screen_shown;
  int drank_glasses;

  struct tm first_reminder;
  struct tm interval;
  unsigned int target_number;
  bool auto_dismiss;
  bool vibrate_on_reminder;

  bool reminders_activated;
  bool s_wakeup_id_valid;
  WakeupId s_wakeup_id;
  WakeupId s_snooze_id;
} DrinkData;
extern DrinkData storage;

void storage_init(void);
void storage_persist(void);

void storage_dobookkeeping(void);
int storage_get_glasses_string(char* str, bool add_linebreaks);
