#include "screen_log_type_actions.h"
#include "app_audio_mark_list.h"
#include "app_mark_list.h"
#include "app_overlimt_list.h"

#include "lvgl.h"
#include <stdio.h>
#include <string.h>

#include "gui_guider.h"
#include "query_context.h"

/* 全局 UI 对象 */
extern lv_ui guider_ui;


/* 根目录建议统一定义 */
#ifndef QUERY_ROOT_DIR
#define QUERY_ROOT_DIR "S:/CB05U01TEST"
#endif

static bool build_query_json_path(char *out, size_t out_sz,
                                  const char *boarding_key,
                                  query_data_type_t type)
{
    if(!out || out_sz == 0 || !boarding_key || boarding_key[0] == '\0') {
        return false;
    }

    switch(type) {
    case QUERY_DATA_TYPE_OVERLIMIT:
        lv_snprintf(out, out_sz, "%s/%s/alarm.json", QUERY_ROOT_DIR, boarding_key);
        return true;

    case QUERY_DATA_TYPE_NORMAL_MARK:
        lv_snprintf(out, out_sz, "%s/%s/mark.json", QUERY_ROOT_DIR, boarding_key);
        return true;

    case QUERY_DATA_TYPE_AUDIO_MARK:
        lv_snprintf(out, out_sz, "%s/%s/audio.json", QUERY_ROOT_DIR, boarding_key);
        return true;

    default:
        break;
    }

    return false;
}

/* 超限数据查询：Gui Guider 已负责切屏，这里只加载数据 */
void screen_log_type_cont_1_custom_code(lv_event_t *e)
{
    (void)e;

    query_context_t ctx;
    char json_path[128];

    query_context_set_data_type(QUERY_DATA_TYPE_OVERLIMIT);

    if(!query_context_get(&ctx)) {
        printf("[LOG_TYPE] overlimit: no query context\r\n");
        return;
    }

    if(!build_query_json_path(json_path, sizeof(json_path),
                              ctx.boarding_key,
                              QUERY_DATA_TYPE_OVERLIMIT)) {
        printf("[LOG_TYPE] overlimit: build path failed\r\n");
        return;
    }

    printf("[LOG_TYPE] overlimit boarding_key=%s\r\n", ctx.boarding_key);
    printf("[LOG_TYPE] overlimit json=%s\r\n", json_path);

    ui_overlimt_render_from_json(&guider_ui, json_path);
}

/* 普通标记查询：Gui Guider 已负责切屏，这里只加载数据 */
void screen_log_type_cont_2_custom_code(lv_event_t *e)
{
    (void)e;

    query_context_t ctx;
    char json_path[128];

    query_context_set_data_type(QUERY_DATA_TYPE_NORMAL_MARK);

    if(!query_context_get(&ctx)) {
        printf("[LOG_TYPE] normal mark: no query context\r\n");
        return;
    }

    if(!build_query_json_path(json_path, sizeof(json_path),
                              ctx.boarding_key,
                              QUERY_DATA_TYPE_NORMAL_MARK)) {
        printf("[LOG_TYPE] normal mark: build path failed\r\n");
        return;
    }

    printf("[LOG_TYPE] normal mark boarding_key=%s\r\n", ctx.boarding_key);
    printf("[LOG_TYPE] normal mark json=%s\r\n", json_path);

    ui_mark_render_from_json(&guider_ui, json_path);
}

/* 录音标记查询：Gui Guider 已负责切屏，这里只加载数据 */
void screen_log_type_cont_3_custom_code(lv_event_t *e)
{
    (void)e;

    query_context_t ctx;
    char json_path[128];

    query_context_set_data_type(QUERY_DATA_TYPE_AUDIO_MARK);

    if(!query_context_get(&ctx)) {
        printf("[LOG_TYPE] audio mark: no query context\r\n");
        return;
    }

    if(!build_query_json_path(json_path, sizeof(json_path),
                              ctx.boarding_key,
                              QUERY_DATA_TYPE_AUDIO_MARK)) {
        printf("[LOG_TYPE] audio mark: build path failed\r\n");
        return;
    }

    printf("[LOG_TYPE] audio mark boarding_key=%s\r\n", ctx.boarding_key);
    printf("[LOG_TYPE] audio mark json=%s\r\n", json_path);

    ui_audiomark_render_from_json(&guider_ui, json_path);
}