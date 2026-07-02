#ifndef APP_MILEAGE_REMINDER_LIST_H
#define APP_MILEAGE_REMINDER_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "gui_guider.h"

void ui_mileage_reminder_render_default(lv_ui *ui);
void ui_mileage_reminder_render_from_json(lv_ui *ui, const char *json_path);
void ui_mileage_reminder_deinit(lv_ui *ui);
void screen_mileage_reminder_custom_code(lv_event_t *e);
void screen_mileage_reminder_deinit_custom_code(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* APP_MILEAGE_REMINDER_LIST_H */
