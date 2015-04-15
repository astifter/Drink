#pragma once
  
#include <pebble.h>  
  
typedef enum {
  timing_handler_reason_startup;
  timing_handler_reason_timer;
  timing_handler_reason_snoozed;
} timing_handler_reason;

typedef (*timing_handler_callback)(timing_handler_reason reason);

timing_handler_reason timing_handler_init(timing_handler_callback);
void timing_handler_enable(void);
void timing_handler_snooze(uint32_t minutes);
void timing_handler_cancel(void);
  