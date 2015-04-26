#include <pebble.h>

#include "logging_helper.h"

void app_log_battery_state(BatteryChargeState s) {
    LOG_EXT(LOG_BATTERY, "s.charge_percent: %d", s.charge_percent);
    LOG_EXT(LOG_BATTERY, "s.is_charging: %d", s.is_charging);
    LOG_EXT(LOG_BATTERY, "s.is_plugged: %d", s.is_plugged);
}

char* logging_error_to_text(int error) {
  switch (error) {
    case S_SUCCESS: return "S_SUCCESS";
    case E_ERROR: return "E_ERROR";
    case E_UNKNOWN: return "E_UNKNOWN";
    case E_INTERNAL: return "E_INTERNAL";
    case E_INVALID_ARGUMENT: return "E_INVALID_ARGUMENT";
    case E_OUT_OF_MEMORY: return "E_OUT_OF_MEMORY";
    case E_OUT_OF_STORAGE: return "E_OUT_OF_STORAGE";
    case E_OUT_OF_RESOURCES: return "E_OUT_OF_RESOURCES";
    case E_RANGE: return "E_RANGE";
    case E_DOES_NOT_EXIST: return "E_DOES_NOT_EXIST";
    case E_INVALID_OPERATION: return "E_INVALID_OPERATION";
    case E_BUSY: return "E_BUSY";
    case S_NO_MORE_ITEMS: return "S_NO_MORE_ITEMS";
    case S_NO_ACTION_REQUIRED: return "S_NO_ACTION_REQUIRED";
  }
  return "unknown";
}
