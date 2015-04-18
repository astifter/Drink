#include <pebble.h>
  
#include "counting_window.h"
#include "data_entry.h"

#include "data.h"
#include "timing_handler.h"

void timing_callback(timing_handler_reason reason) {
  switch(reason) {
    case timing_handler_reason_startup: {
      show_counting_window(); 
    } break;
    case timing_handler_reason_firstday: {
      storage_dobookkeeping();
      show_data_entry(false); 
    } break;
    case timing_handler_reason_timer: {
      show_data_entry(false); 
    } break;
    case timing_handler_reason_snoozed: {
      show_data_entry(true); 
    } break;
  }
}

int main(void) {
  storage_init();
  timing_handler_init(timing_callback);
  app_event_loop();
  storage_persist();
}
