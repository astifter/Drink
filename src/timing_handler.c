#include <pebble.h>
  
#include "timing_handler.h"
#include "data.h"
#include "watchface_base/logging_helper.h"
#include "data_logging.h"
  
timing_handler_callback callback;

void cancel(WakeupId* id, int logid) {
  if ((*id) != E_UNKNOWN) {
    wakeup_cancel(*id);
    (*id) = E_UNKNOWN;
    storage_persist();
    if (logid >= 0)
      data_logging_do(logid, data_logging_type_t_canceled);
  }
}

void re_schedule(WakeupId* id, time_t t, int reason, int logid) {
  time_t now = time(NULL);
  time_t scheduled;
  bool schedule = false;
  bool resschedule = false;
  if (wakeup_query(*id, &scheduled)) {
    // timer is scheduled, make sure its not in the past (which should not happen) and has same required timestamp
    if (scheduled < now) {
      data_logging_do(logid, data_logging_type_t_tolate);
      cancel(id, logid);
      schedule = resschedule = true;
    } 
    if (scheduled != t) {
      cancel(id, logid);
      schedule = resschedule = true;
    }
  } else {
    schedule = true;
  } 
  
  if (schedule) {
    (*id) = wakeup_schedule(t, reason, true);
    if (!resschedule)
      data_logging_do3(logid, data_logging_type_t_scheduled, *id, t);
    else
      data_logging_do3(logid, data_logging_type_t_rescheduled, *id, t);
    storage_persist();
  }
}

static void reschedule_bookkeeping(void) {
  LOG_FUNC();

  time_t now; time(&now);
  struct tm *lt = localtime(&now);

  lt->tm_hour = 0;
  lt->tm_min  = 0;
  lt->tm_sec  = 0;

  time_t schedule = mktime(lt);
  schedule += 3600*24;
  re_schedule(&storage.s_bookkeeping_id, schedule, timing_handler_reason_bookkeeping, data_logging_type_tim_bookkeeping);
}

static void reschedule(void) {
  LOG_FUNC();

  time_t now; time(&now);
  struct tm *lt = localtime(&now);

  if (storage.reminders_activated) {
    lt->tm_hour = storage.first_reminder.tm_hour;
    lt->tm_min  = storage.first_reminder.tm_min;
    lt->tm_sec  = 0;

    time_t schedule = mktime(lt);
    for (int i = 0; i < storage.drank_glasses; i++) {
      schedule += ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
    }
    for (unsigned int i = storage.drank_glasses; i < storage.target_number; i++) {
      if (schedule > now) {
        cancel(&storage.s_bookkeeping_id, data_logging_type_tim_bookkeeping);
        if (i == 0) {
          re_schedule(&storage.s_wakeup_id, schedule, timing_handler_reason_firstday, data_logging_type_tim_timer);
        } else {
          re_schedule(&storage.s_wakeup_id, schedule, timing_handler_reason_timer, data_logging_type_tim_timer);
        }
        return;
      }
      schedule += ((storage.interval.tm_hour * 60) + storage.interval.tm_min) * 60;
    }
  }

  cancel(&storage.s_wakeup_id, data_logging_type_tim_timer);
  reschedule_bookkeeping();
}

static void wakeup_handler(WakeupId id, int32_t reason) {
  LOG_FUNC();
  if (reason == timing_handler_reason_snoozed) {
    cancel(&storage.s_snooze_id, -1);
  } else if (reason == timing_handler_reason_timer || reason == timing_handler_reason_firstday) {
    reschedule();
  } else if (reason == timing_handler_reason_bookkeeping) {
    reschedule();
  }

  data_logging_do(data_logging_type_wakeup_handler, reason);
  callback((timing_handler_reason)reason);
}

static void timing_handler_handle(bool enable) {
  LOG_FUNC();
  
  if (storage.s_wakeup_id == E_UNKNOWN && enable) {
    reschedule();
  } else if (storage.s_wakeup_id >= 0 && !enable) {
    cancel(&storage.s_wakeup_id, data_logging_type_tim_timer);
    cancel(&storage.s_snooze_id, data_logging_type_tim_snooze);
    reschedule_bookkeeping();
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

  re_schedule(&storage.s_snooze_id, schedule, timing_handler_reason_snoozed, data_logging_type_tim_snooze);
}

void timing_handler_reschedule(void) {
  reschedule();
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
  
  reschedule();
}
