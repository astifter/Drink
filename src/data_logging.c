#include <pebble.h>

#include "data_logging.h"

#include "watchface_base/logging_helper.h"

static DataLoggingSessionRef data_logging_ref;

typedef struct data_logging_s {
  int       type; 
  time_t    timestamp;
  int       data1;
  int       data2;
  int       data3;
  uint16_t  timestamp_ms;
  uint16_t  spare1;
} data_logging_t;

void data_logging_do0(data_logging_type t) {
  data_logging_do3(t, 0, 0, 0);
}

void data_logging_do (data_logging_type t, int d) {
  data_logging_do3(t, d, 0, 0);
}

void data_logging_do2(data_logging_type t, int d1, int d2) {
  data_logging_do3(t, d1, d2, 0);
}

void data_logging_do3(data_logging_type t, int d1, int d2, int d3) {
  data_logging_t s;
  s.type = t;
  time_ms(&s.timestamp, &s.timestamp_ms);
  s.spare1 = 0;
  s.data1 = d1;
  s.data2 = d2;
  s.data3 = d3;
  LOG_EXT(LOG_DATA_LOG, "%lu.%08d,size:%d", s.timestamp, s.timestamp_ms, sizeof(uint16_t));
  data_logging_log(data_logging_ref, &s, 1);
}

void data_logging_init(void) {
  data_logging_ref = data_logging_create(0x0, DATA_LOGGING_BYTE_ARRAY, sizeof(data_logging_t), false);
}

void data_logging_destroy(void) {
  data_logging_finish(data_logging_ref);
}
