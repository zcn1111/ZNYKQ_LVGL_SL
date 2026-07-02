#ifndef APP_MARK_LIST_H
#define APP_MARK_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

void ui_mark_deinit(void);
void ui_mark_render_from_json(lv_ui *ui, const char *json_path);

#ifdef __cplusplus
}
#endif

#endif /* APP_MARK_LIST_H */