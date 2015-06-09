#include <pebble.h>
  
#include "counting_window.h"
#include "data_entry.h"
#include "data_logging.h"

#include "data.h"
#include "timing_handler.h"

void timing_callback(timing_handler_reason reason) {
  switch(reason) {
    case timing_handler_reason_startup: {
      show_counting_window(); 
    } break;
    case timing_handler_reason_firstday: {
      show_data_entry(false); 
    } break;
    case timing_handler_reason_timer: {
      show_data_entry(false); 
    } break;
    case timing_handler_reason_snoozed: {
      show_data_entry(true); 
    } break;
    case timing_handler_reason_bookkeeping: {
      storage_dobookkeeping();
    } break;
  }
}

int main(void) {
  data_logging_init();
  storage_init();
  timing_handler_init(timing_callback);
  app_event_loop();
  storage_persist();
  data_logging_destroy();
}
