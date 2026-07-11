#include "screen_log_type_actions.h"
#include "app_audio_mark_list.h"
#include "app_mark_list.h"
#include "app_overlimt_list.h"

#include "lvgl.h"
#include <stdio.h>
#include <string.h>

#include "gui_guider.h"
#include "query_context.h"

extern lv_ui guider_ui;

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

static query_data_type_t active_detail_type(void)
{
    lv_obj_t *act = lv_screen_active();
    if(act == guider_ui.screen_overlimt) return QUERY_DATA_TYPE_OVERLIMIT;
    if(act == guider_ui.screen_mark) return QUERY_DATA_TYPE_NORMAL_MARK;
    if(act == guider_ui.screen_audiomark) return QUERY_DATA_TYPE_AUDIO_MARK;
    return QUERY_DATA_TYPE_NONE;
}

static bool *active_detail_del_flag(query_data_type_t type)
{
    switch(type) {
    case QUERY_DATA_TYPE_OVERLIMIT:
        return &guider_ui.screen_overlimt_del;
    case QUERY_DATA_TYPE_NORMAL_MARK:
        return &guider_ui.screen_mark_del;
    case QUERY_DATA_TYPE_AUDIO_MARK:
        return &guider_ui.screen_audiomark_del;
    default:
        break;
    }
    return NULL;
}

static void deinit_detail_type(query_data_type_t type)
{
    switch(type) {
    case QUERY_DATA_TYPE_OVERLIMIT:
        ui_overlimt_deinit();
        break;
    case QUERY_DATA_TYPE_NORMAL_MARK:
        ui_mark_deinit();
        break;
    case QUERY_DATA_TYPE_AUDIO_MARK:
        audiomark_deinit();
        break;
    default:
        break;
    }
}

static void render_detail_type(query_data_type_t type, const char *json_path)
{
    switch(type) {
    case QUERY_DATA_TYPE_OVERLIMIT:
        ui_overlimt_render_from_json(&guider_ui, json_path);
        break;
    case QUERY_DATA_TYPE_NORMAL_MARK:
        ui_mark_render_from_json(&guider_ui, json_path);
        break;
    case QUERY_DATA_TYPE_AUDIO_MARK:
        ui_audiomark_render_from_json(&guider_ui, json_path);
        break;
    default:
        break;
    }
}

static void load_detail_screen(query_data_type_t type, bool *old_scr_del)
{
    switch(type) {
    case QUERY_DATA_TYPE_OVERLIMIT:
        ui_load_scr_animation(&guider_ui,
                              &guider_ui.screen_overlimt,
                              guider_ui.screen_overlimt_del,
                              old_scr_del,
                              setup_scr_screen_overlimt,
                              LV_SCR_LOAD_ANIM_NONE,
                              100,
                              100,
                              false,
                              false);
        break;

    case QUERY_DATA_TYPE_NORMAL_MARK:
        ui_load_scr_animation(&guider_ui,
                              &guider_ui.screen_mark,
                              guider_ui.screen_mark_del,
                              old_scr_del,
                              setup_scr_screen_mark,
                              LV_SCR_LOAD_ANIM_NONE,
                              100,
                              100,
                              false,
                              false);
        break;

    case QUERY_DATA_TYPE_AUDIO_MARK:
        ui_load_scr_animation(&guider_ui,
                              &guider_ui.screen_audiomark,
                              guider_ui.screen_audiomark_del,
                              old_scr_del,
                              setup_scr_screen_audiomark,
                              LV_SCR_LOAD_ANIM_NONE,
                              100,
                              100,
                              false,
                              false);
        break;

    default:
        break;
    }
}

static void switch_detail_type(query_data_type_t target_type)
{
    query_context_t ctx;
    char json_path[128];

    if(!query_context_get(&ctx)) {
        printf("[LOG_TYPE] switch failed: no query context\r\n");
        return;
    }

    if(!build_query_json_path(json_path, sizeof(json_path), ctx.boarding_key, target_type)) {
        printf("[LOG_TYPE] switch failed: build path failed, type=%d\r\n", (int)target_type);
        return;
    }

    query_data_type_t active_type = active_detail_type();
    query_context_set_data_type(target_type);

    printf("[LOG_TYPE] switch boarding_key=%s type=%d json=%s\r\n",
           ctx.boarding_key,
           (int)target_type,
           json_path);

    if(active_type == target_type) {
        render_detail_type(target_type, json_path);
        return;
    }

    bool dummy_old_del = false;
    bool *old_scr_del = active_detail_del_flag(active_type);
    if(!old_scr_del) old_scr_del = &dummy_old_del;

    deinit_detail_type(active_type);
    load_detail_screen(target_type, old_scr_del);
    render_detail_type(target_type, json_path);
}

void screen_log_type_cont_1_custom_code(lv_event_t *e)
{
    (void)e;
    switch_detail_type(QUERY_DATA_TYPE_OVERLIMIT);
}

void screen_log_type_cont_2_custom_code(lv_event_t *e)
{
    (void)e;
    switch_detail_type(QUERY_DATA_TYPE_NORMAL_MARK);
}

void screen_log_type_cont_3_custom_code(lv_event_t *e)
{
    (void)e;
    switch_detail_type(QUERY_DATA_TYPE_AUDIO_MARK);
}
