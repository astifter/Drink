#include <pebble.h>

#include "data.h"

#define STORAGE_ID 0x21124424
DrinkData storage;
  
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
    storage.s_wakeup_id_valid = false;
    storage.s_wakeup_id = -1;
    storage.s_snooze_id = -1;
    storage.auto_dismiss = false;
    storage.vibrate_on_reminder = true;
    storage_persist();
  }
}

void storage_persist(void) {
  persist_write_data(STORAGE_ID, &storage, sizeof(DrinkData));  
}

void storage_dobookkeeping(void) {
  storage.drank_glasses = 0;
  storage_persist();
}

int storage_get_glasses_string(char* str, bool add_linebreaks) {
  if (add_linebreaks) {
    if (storage.drank_glasses == 0) {
      return snprintf(str, 50, "You drank\nnothing\ntoday!");
    } else if (storage.drank_glasses == 1) {
      return snprintf(str, 50, "You drank\n1\nglass!");
    } else if (storage.drank_glasses > 1) {
      return snprintf(str, 50, "You drank\n%d\nglasses!", storage.drank_glasses);
    }
  } else {
    if (storage.drank_glasses == 0) {
      return snprintf(str, 50, "You drank nothing.");
    } else if (storage.drank_glasses == 1) {
      return snprintf(str, 50, "You drank 1 glass.");
    } else if (storage.drank_glasses > 1) {
      return snprintf(str, 50, "You drank %d glasses.", storage.drank_glasses);
    }
  }
  return 0;
}
