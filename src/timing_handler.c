#include <pebble.h>
  
#include "timing_handler.h"
#include "data.h"
#include "watchface_base/logging_helper.h"
  
timing_handler_callback callback;

static void reschedule_timer(void) {
  LOG_FUNC();
  time_t now; time(&now);
  struct tm *lt = localtime(&now);

  lt->tm_hour = storage.first_reminder.tm_hour;
  lt->tm_min  = storage.first_reminder.tm_min;

  time_t schedule = mktime(lt);
  for (unsigned int i = 0; i < storage.target_number; i++) {
    if (schedule > now) {
      storage.s_wakeup_id = wakeup_schedule(schedule, timing_handler_reason_timer, true);
      storage_persist();
      return;
    }
    schedule += ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
  }

  schedule = mktime(lt);
  schedule += 3600*24;
  storage.s_wakeup_id = wakeup_schedule(schedule, timing_handler_reason_firstday, true);
  storage_persist();
  return;
}

static void wakeup_handler(WakeupId id, int32_t r) {
  LOG_FUNC();
  timing_handler_reason reason = (timing_handler_reason)r;
  if (reason == timing_handler_reason_snoozed) {
    storage.s_snooze_id = -1;
  } else if (reason == timing_handler_reason_timer || reason == timing_handler_reason_firstday) {
    reschedule_timer();
  }

  callback((timing_handler_reason)reason);
}

bool timing_handler_next(time_t *timestamp) {
  LOG_FUNC();
  if (storage.s_wakeup_id_valid) {
    return wakeup_query(storage.s_wakeup_id, timestamp);
  } else {
    return false;
  }
}

bool timing_handler_next_snooze(time_t *timestamp) {
  LOG_FUNC();
  if (storage.s_wakeup_id_valid) {
    return wakeup_query(storage.s_snooze_id, timestamp);
  } else {
    return false;
  }
}

static void timing_handler_handle(bool enable) {
  LOG_FUNC();
  
  if (!storage.s_wakeup_id_valid && enable) {
    reschedule_timer();
    storage.s_wakeup_id_valid = true;
    storage_persist();
  } else if (storage.s_wakeup_id_valid && !enable) {
    wakeup_cancel_all();
    storage.s_wakeup_id = -1;
    storage.s_wakeup_id_valid = false;
    storage_persist();
  }
}

void timing_handler_enable(void) {
  LOG_FUNC();
  timing_handler_handle(true);
}
void timing_handler_cancel(void) {
  LOG_FUNC();
  timing_handler_handle(false);
}

void timing_handler_snooze(void) {
  LOG_FUNC();
  if (storage.s_wakeup_id_valid) {
    int interval_seconds = ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
    time_t scheduled_time;
    wakeup_query(storage.s_wakeup_id, &scheduled_time);
    scheduled_time -= (interval_seconds*2)/3;
    storage.s_snooze_id = wakeup_schedule(scheduled_time, timing_handler_reason_snoozed, true);
    storage_persist();
  }  
}

void timing_handler_init(timing_handler_callback c) {
  LOG_FUNC();
  callback = c;

  // Subscribe to Wakeup API
  wakeup_service_subscribe(wakeup_handler);

  // Was this a wakeup launch?
  if (launch_reason() == APP_LAUNCH_WAKEUP) {
    // The app was started by a wakeup
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the wakeup
    wakeup_get_launch_event(&id, &reason);

    wakeup_handler(id, reason);
  } else {
    wakeup_handler(0, timing_handler_reason_startup);
  }
}
