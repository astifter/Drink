#pragma once

typedef void (*glasses_changed_callback)(void);

void show_settings_window(glasses_changed_callback c);
void hide_settings_window(void);
