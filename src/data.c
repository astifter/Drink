#include <pebble.h>

#include "data.h"
#include "watchface_base/logging_helper.h"

#define STORAGE_ID 0x21124424
DrinkData storage;
  
void storage_init(void) {
  LOG_FUNC();
  if (persist_exists(STORAGE_ID)) {
    persist_read_data(STORAGE_ID, &storage, sizeof(DrinkData));
  } else {
    // since we don't now anything about possibly scheduled timers
    // we cancel them all just in case.
    wakeup_cancel_all();

    //time_t now_time_t = time(NULL) + 60;
    //struct tm *now = localtime(&now_time_t);
    storage.first_reminder.tm_hour = 9;
    storage.first_reminder.tm_min = 0;
    storage.interval.tm_hour = 1;
    storage.interval.tm_min = 30;
    storage.target_number = 6;
    storage.reminders_activated = false;
    storage.first_screen_shown = false;
    storage.drank_glasses = 0;
    storage.s_wakeup_id = E_UNKNOWN;
    storage.s_snooze_id = E_UNKNOWN;
    storage.s_bookkeeping_id = E_UNKNOWN;
    storage.auto_dismiss = false;
    storage.vibrate_on_reminder = true;
    storage_persist();
  }
}

void storage_persist(void) {
  LOG_FUNC();
  persist_write_data(STORAGE_ID, &storage, sizeof(DrinkData));  
}

void storage_dobookkeeping(void) {
  LOG_FUNC();
  storage.drank_glasses = 0;
  storage_persist();
}

int storage_get_glasses_string(char* str, bool add_linebreaks) {
  LOG_FUNC();
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
