#pragma once

typedef enum data_logging_types_e {
  data_logging_type_glass           = 1,
  data_logging_type_bookkeeping     = 2,
  data_logging_type_corrected       = 3,
  data_logging_type_wakeup_handler  = 4,
  data_logging_type_vibrating       = 5,
  data_logging_type_autodismissed   = 6,
  data_logging_type_schedule_bookkeeping = 100,
  data_logging_type_schedule_timer,
  data_logging_type_schedule_snooze,
  data_logging_type_cancel_timer         = 110,
} data_logging_type;

void data_logging_init(void);
void data_logging_destroy(void);
void data_logging_do(data_logging_type t, int d);
