#include <pebble.h>

#include "data_logging.h"

#include "watchface_base/logging_helper.h"

static DataLoggingSessionRef data_logging_ref;

typedef struct data_logging_s {
  int               type; 
  time_t            timestamp;
  int               data;
} data_logging_t;

void data_logging_do(data_logging_type t, int d) {
  data_logging_t s;
  s.type = t;
  s.timestamp = time(NULL);
  s.data = d;
  data_logging_log(data_logging_ref, &s, 1);
}

void data_logging_init(void) {
  data_logging_ref = data_logging_create(0x0, DATA_LOGGING_BYTE_ARRAY, sizeof(data_logging_t), false);
}

void data_logging_destroy(void) {
  data_logging_finish(data_logging_ref);
}
