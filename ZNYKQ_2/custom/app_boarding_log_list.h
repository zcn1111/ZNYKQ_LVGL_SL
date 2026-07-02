#ifndef APP_BOARDING_LOG_LIST_H
#define APP_BOARDING_LOG_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

void ui_boarding_log_deinit(void);
void ui_boarding_log_render_from_json(lv_ui *ui, const char *json_path);

#ifdef __cplusplus
}
#endif

#endif /* APP_BOARDING_LOG_LIST_H */