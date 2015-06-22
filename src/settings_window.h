#pragma once

typedef void (*glasses_changed_callback)(void);

void show_main_window(glasses_changed_callback c);
void hide_main_window(void);
