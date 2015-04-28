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
  for (int i = 0; i < storage.drank_glasses; i++) {
    schedule += ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
  }
  for (unsigned int i = storage.drank_glasses; i < storage.target_number; i++) {
    if (schedule > now) {
      if (i == 0) {
        storage.s_wakeup_id = wakeup_schedule(schedule, timing_handler_reason_firstday, true);
      } else {
        storage.s_wakeup_id = wakeup_schedule(schedule, timing_handler_reason_timer, true);
      }
      storage_persist();
      return;
    }
    schedule += ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
  }

  schedule = mktime(lt);
  schedule += 3600*24;
  storage.s_wakeup_id = wakeup_schedule(schedule, timing_handler_reason_firstday, true);
  storage_persist();
}

static void reschedule_bookkeeping(void) {
  LOG_FUNC();
  if (wakeup_query(storage.s_bookkeeping_id, NULL))
    return;

  time_t now; time(&now);
  struct tm *lt = localtime(&now);

  lt->tm_hour = 0;
  lt->tm_min  = 0;

  time_t schedule = mktime(lt);
  schedule += 3600*24;
  storage.s_bookkeeping_id = wakeup_schedule(schedule, timing_handler_reason_bookkeeping, true);
  storage_persist();
}

static void wakeup_handler(WakeupId id, int32_t reason) {
  LOG_FUNC();
  if (reason == timing_handler_reason_snoozed) {
    wakeup_cancel(storage.s_snooze_id);
    storage.s_snooze_id = E_UNKNOWN;
    storage_persist();
  } else if (reason == timing_handler_reason_timer || reason == timing_handler_reason_firstday) {
    reschedule_timer();
  } else if (reason == timing_handler_reason_bookkeeping) {
    reschedule_bookkeeping();
  }

  callback((timing_handler_reason)reason);
}

static void timing_handler_handle(bool enable) {
  LOG_FUNC();
  
  if (storage.s_wakeup_id == E_UNKNOWN && enable) {
    reschedule_timer();
  } else if (storage.s_wakeup_id >= 0 && !enable) {
    wakeup_cancel(storage.s_wakeup_id);
    wakeup_cancel(storage.s_snooze_id);
    storage.s_wakeup_id = E_UNKNOWN;
    storage.s_snooze_id = E_UNKNOWN;
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

  time_t now; time(&now);
  int interval_seconds = ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
  time_t schedule = now + interval_seconds/3;

  storage.s_snooze_id = wakeup_schedule(schedule, timing_handler_reason_snoozed, true);
  storage_persist();
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
  
  reschedule_bookkeeping();
}
