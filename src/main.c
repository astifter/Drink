#include <pebble.h>
  
#include "main_window.h"
#include "popup_window.h"
#include "data_logging.h"

#include "data.h"
#include "timing_handler.h"

void timing_callback(timing_handler_reason reason) {
  switch(reason) {
    case timing_handler_reason_startup: {
      show_main_window(); 
    } break;
    case timing_handler_reason_firstday: {
      show_popup_window(false); 
    } break;
    case timing_handler_reason_timer: {
      show_popup_window(false); 
    } break;
    case timing_handler_reason_snoozed: {
      show_popup_window(true); 
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
