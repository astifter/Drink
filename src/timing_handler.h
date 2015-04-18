#pragma once
  
#include <pebble.h>  
  
typedef enum {
  timing_handler_reason_startup,
  timing_handler_reason_firstday,
  timing_handler_reason_timer,
  timing_handler_reason_snoozed,
} timing_handler_reason;

typedef void(*timing_handler_callback)(timing_handler_reason reason);

void timing_handler_init(timing_handler_callback);
void timing_handler_enable(void);
void timing_handler_snooze(void);
void timing_handler_cancel(void);
bool timing_handler_next(time_t *timestamp);  
bool timing_handler_next_snooze(time_t *timestamp);  
