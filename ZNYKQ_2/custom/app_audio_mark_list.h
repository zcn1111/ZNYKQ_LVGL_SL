#pragma once

#include <stdbool.h>
#include "gui_guider.h"
#ifdef __cplusplus
extern "C" {
#endif

//typedef struct _lv_ui lv_ui;

/* 可选：释放内部资源 */
void audiomark_deinit(void);

/* 从 JSON 渲染录音标记列表（推荐 JSON <= 32KB） */
bool ui_audiomark_render_from_json(lv_ui *ui, const char *path);

#ifdef __cplusplus
}
#endif
