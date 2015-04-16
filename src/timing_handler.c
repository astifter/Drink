#include <pebble.h>
  
#include "data.h"
#include "timing_handler.h"
  
timing_handler_callback callback;

static void reschedule_timer(void) {
  time_t t; time(&t);
  storage.s_wakeup_id = wakeup_schedule(t+180, timing_handler_reason_timer, true);
}

static void wakeup_handler(WakeupId id, int32_t r) {
  timing_handler_reason reason = (timing_handler_reason)r;
  if (reason == timing_handler_reason_snoozed) {
    storage.s_snooze_id = -1;
  } else if (reason == timing_handler_reason_timer) {
    reschedule_timer();
  }

  callback((timing_handler_reason)reason);
}

bool timing_handler_next(time_t *timestamp) {
  if (storage.s_wakeup_id_valid) {
    return wakeup_query(storage.s_wakeup_id, timestamp);
  } else {
    return false;
  }
}

bool timing_handler_next_snooze(time_t *timestamp) {
  if (storage.s_wakeup_id_valid) {
    return wakeup_query(storage.s_snooze_id, timestamp);
  } else {
    return false;
  }
}

static void timing_handler_handle(bool enable) {
  //Check the event is not already scheduled
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
  timing_handler_handle(true);
}
void timing_handler_cancel(void) {
  timing_handler_handle(false);
}

void timing_handler_snooze(void) {
  if (storage.s_wakeup_id_valid) {
    int interval_seconds = ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
    time_t scheduled_time;
    wakeup_query(storage.s_wakeup_id, &scheduled_time);
    scheduled_time -= (interval_seconds*2)/3;
    storage.s_snooze_id = wakeup_schedule(scheduled_time, timing_handler_reason_snoozed, true);
    storage_persist();
  }  
}

timing_handler_reason timing_handler_init(timing_handler_callback c) {
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

    return reason;
  } else {
    return timing_handler_reason_startup;
  }
}
