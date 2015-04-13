#include <pebble.h>

#include "data.h"

#define STORAGE_ID 0x21124424
  
void storage_init(void) {
  if (persist_exists(STORAGE_ID)) {
    persist_read_data(STORAGE_ID, &storage, sizeof(DrinkData));
  } else {
    storage.first_reminder.tm_min = 0;
    storage.first_reminder.tm_hour = 9;
    storage.interval.tm_min = 30;
    storage.interval.tm_hour = 1;
    storage.target_number = 6;
    storage.reminders_activated = false;
    storage.first_screen_shown = false;
    storage.drank_glasses = 0;
    storage_persist();
  }
}

void storage_persist(void) {
  persist_write_data(STORAGE_ID, &storage, sizeof(DrinkData));  
}
