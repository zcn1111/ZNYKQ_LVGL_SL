#pragma once

#include <stdbool.h>
#include "gui_guider.h"
#ifdef __cplusplus
extern "C" {
#endif

//typedef struct _lv_ui lv_ui;   /* gui_guider 里通常 typedef 了 lv_ui，这里前置声明 */

/* 初始化/释放（可选） */
void ui_overlimt_deinit(void);

/* 从 JSON 渲染超限列表（JSON <= 32KB 推荐） */
void ui_overlimt_render_from_json(lv_ui *ui, const char *json_path);

#ifdef __cplusplus
}
#endif
