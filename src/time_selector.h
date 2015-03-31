#include <pebble.h>

void show_time_selector(struct tm initial_time);
void hide_time_selector(void);
struct tm get_selected_time(void);