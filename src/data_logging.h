#pragma once

typedef enum data_logging_types_e {
  data_logging_type_glass           = 1,
  data_logging_type_bookkeeping     = 2,
  data_logging_type_corrected       = 3,
  data_logging_type_wakeup_handler  = 4,
  data_logging_type_vibrating       = 5,
  data_logging_type_autodismissed   = 6,
  data_logging_type_tim_bookkeeping = 100,
  data_logging_type_tim_timer,
  data_logging_type_tim_snooze,
} data_logging_type;

typedef enum data_logging_types_offsets_e {
  data_logging_type_t_scheduled,
  data_logging_type_t_canceled,
  data_logging_type_t_tolate,
  data_logging_type_t_rescheduled,
} data_logging_type_t_action;

void data_logging_init(void);
void data_logging_destroy(void);
void data_logging_do0(data_logging_type t);
void data_logging_do (data_logging_type t, int d);
void data_logging_do2(data_logging_type t, int d1, int d2);
void data_logging_do3(data_logging_type t, int d1, int d2, int d3);
