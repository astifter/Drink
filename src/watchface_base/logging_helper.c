#include "logging_helper.h"

void app_log_battery_state(BatteryChargeState s) {
    LOG_EXT(LOG_BATTERY, "s.charge_percent: %d", s.charge_percent);
    LOG_EXT(LOG_BATTERY, "s.is_charging: %d", s.is_charging);
    LOG_EXT(LOG_BATTERY, "s.is_plugged: %d", s.is_plugged);
}
