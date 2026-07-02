#include "app_boarding_log_list.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "guider_customer_fonts.h"
#include "query_context.h"
#include "app_audio_mark_list.h"
#include "app_overlimt_list.h"
#include "app_mark_list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "cJSON.h"

/* ================= Font ================= */
#ifndef APP_LOG_FONT
#define APP_LOG_FONT (&lv_customer_font_PingFangHeavy_16)
#endif

#ifndef APP_LOG_TITLE_FONT
#define APP_LOG_TITLE_FONT (&lv_customer_font_PingFangHeavy_20)
#endif

#ifndef APP_LOG_TIME_FONT
#define APP_LOG_TIME_FONT (&lv_font_PingFangMedium_18)
#endif

/* ================= JSON 限制 ================= */
#define BOARDING_JSON_MAX_SIZE   (64 * 1024)
#define BOARDING_MAX_RECORDS     (200)

/* ================= 根目录 ================= */
#ifndef QUERY_ROOT_DIR
#define QUERY_ROOT_DIR "S:/CB05U01TEST"
#endif

/* ================= VList config ================= */
#define VLIST_ITEM_H        200
#define VLIST_GAP_PX        7
#define VLIST_CACHE_EXTRA   2

/* ================= Card style（按你最新 screen_log 参考） ================= */
#define CARD_RADIUS         10
#define CARD_BG_HEX         0xFFFFFF
#define CARD_BG_OPA         0

#define TXT_HEX             0x191E52
#define BTN_BG_HEX          0xB5BAF0
#define TXT_TIME_HEX        0x6D6E71
#define TXT_VALUE_HEX       0x0444F0
#define CARD_DOUBLE_CLICK_MS 450

/* ================= Item layout ================= */
#define ITEM_X              3
#define ITEM_Y0             7
#define ITEM_W              224

#define X_DATE              12
#define Y_DATE              36
#define W_DATE              200
#define H_DATE              24

#define X_ROUTE             12
#define Y_ROUTE             10
#define W_ROUTE             200
#define H_ROUTE             24

#define X_TRAIN             152
#define Y_TRAIN             75
#define W_TRAIN             60
#define H_TRAIN             24

#define X_LOCO              152
#define Y_LOCO              118
#define W_LOCO              60
#define H_LOCO              24

#define X_BTN1              152
#define Y_BTN               162
#define W_BTN               60
#define H_BTN               24

#define X_BTN2              81
#define X_BTN3              152

#define X_REL1              0
#define Y_REL1              0
#define W_REL               ITEM_W
#define H_REL               VLIST_ITEM_H

#define X_REL2              74
#define Y_REL2              2

#define X_REL3              149
#define Y_REL3              0

/* ================= Data model ================= */
typedef struct {
    char boarding_key[32];
    char start_ts[20];

    char date[11];       /* YYYY-MM-DD */
    char time[9];        /* HH:MM:SS */

    char route_name[64];
    char loco_no[24];
    char train_no[24];
    char line_name[32];
    char dir[8];

    int level3_count;
    int mark_count;
    int audio_count;
} boarding_record_t;

/* ================= 全局数据 ================= */
static boarding_record_t *g_records = NULL;
static int g_records_cnt = 0;

/* 缓存 */
static bool     s_cache_ok = false;
static uint32_t s_cache_hash = 0;
static char     s_cache_path[128] = {0};

/* ================= Helpers ================= */
static void records_free_all(void)
{
    if(g_records) {
        lv_free(g_records);
        g_records = NULL;
    }
    g_records_cnt = 0;
}

static uint32_t fnv1a32(const uint8_t *p, size_t n)
{
    uint32_t h = 2166136261u;
    for(size_t i = 0; i < n; i++) {
        h ^= p[i];
        h *= 16777619u;
    }
    return h;
}

static char *read_text_file_lvfs_limit(const char *path, size_t *out_len)
{
    if(out_len) *out_len = 0;
    if(!path) return NULL;

    lv_fs_file_t f;
    lv_fs_res_t rr = lv_fs_open(&f, path, LV_FS_MODE_RD);
    if(rr != LV_FS_RES_OK) {
        printf("[BOARDING] lv_fs_open failed (%d): %s\n", (int)rr, path);
        return NULL;
    }

    char *buf = (char *)lv_malloc(BOARDING_JSON_MAX_SIZE + 1);
    if(!buf) {
        lv_fs_close(&f);
        return NULL;
    }

    size_t total = 0;
    while(total < BOARDING_JSON_MAX_SIZE) {
        uint32_t br = 0;
        uint32_t want = (uint32_t)(BOARDING_JSON_MAX_SIZE - total);

        rr = lv_fs_read(&f, buf + total, want, &br);
        if(rr != LV_FS_RES_OK) {
            printf("[BOARDING] lv_fs_read failed (%d)\n", (int)rr);
            lv_free(buf);
            lv_fs_close(&f);
            return NULL;
        }

        if(br == 0) break;
        total += br;
    }

    lv_fs_close(&f);

    if(total == 0) {
        printf("[BOARDING] file empty: %s\n", path);
        lv_free(buf);
        return NULL;
    }

    if(total >= BOARDING_JSON_MAX_SIZE) {
        printf("[BOARDING] file too large (>= %d bytes): %s\n", BOARDING_JSON_MAX_SIZE, path);
        lv_free(buf);
        return NULL;
    }

    buf[total] = '\0';
    if(out_len) *out_len = total;
    return buf;
}

/* start_ts: 20260122093000 -> 2026-01-22 / 09:30:00 */
static void parse_ts_14_to_date_time(const char *ts14,
                                     char *date_out, size_t date_sz,
                                     char *time_out, size_t time_sz)
{
    if(date_out && date_sz > 0) date_out[0] = '\0';
    if(time_out && time_sz > 0) time_out[0] = '\0';

    if(!ts14) return;
    if(strlen(ts14) < 14) return;

    if(date_out && date_sz >= 11) {
        lv_snprintf(date_out, date_sz, "%.4s-%.2s-%.2s", ts14, ts14 + 4, ts14 + 6);
    }

    if(time_out && time_sz >= 9) {
        lv_snprintf(time_out, time_sz, "%.2s:%.2s:%.2s", ts14 + 8, ts14 + 10, ts14 + 12);
    }
}

static int json_value_to_int(cJSON *j, int def_val)
{
    if(cJSON_IsNumber(j)) return j->valueint;
    if(cJSON_IsString(j) && j->valuestring) return atoi(j->valuestring);
    if(cJSON_IsArray(j)) return cJSON_GetArraySize(j);

    if(cJSON_IsObject(j)) {
        const char *count_keys[] = {"count", "total", "num", "value", "cnt", NULL};
        for(int i = 0; count_keys[i]; i++) {
            int v = json_value_to_int(cJSON_GetObjectItem(j, count_keys[i]), -1);
            if(v >= 0) return v;
        }
    }

    return def_val;
}

static int json_get_int_flexible(cJSON *obj, const char *key, int def_val)
{
    cJSON *j = obj ? cJSON_GetObjectItem(obj, key) : NULL;
    return json_value_to_int(j, def_val);
}

static int json_get_stat_int(cJSON *record, const char *key, int def_val)
{
    const char *containers[] = {"stats", "counts", "stat", "count", NULL};

    for(int i = 0; containers[i]; i++) {
        cJSON *stats = cJSON_GetObjectItem(record, containers[i]);
        if(cJSON_IsObject(stats)) {
            int v = json_get_int_flexible(stats, key, -1);
            if(v >= 0) return v;
        }
    }

    return json_get_int_flexible(record, key, def_val);
}
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


static cJSON *json_get_items_array(cJSON *root)
{
    if(cJSON_IsArray(root)) return root;
    if(!cJSON_IsObject(root)) return NULL;

    cJSON *arr = cJSON_GetObjectItem(root, "items");
    if(cJSON_IsArray(arr)) return arr;

    arr = cJSON_GetObjectItem(root, "records");
    if(cJSON_IsArray(arr)) return arr;

    arr = cJSON_GetObjectItem(root, "data");
    if(cJSON_IsArray(arr)) return arr;

    return NULL;
}

static bool json_level_is_iii(cJSON *j)
{
    if(cJSON_IsNumber(j)) return j->valueint == 3;
    if(cJSON_IsString(j) && j->valuestring) {
        return strcmp(j->valuestring, "III") == 0 ||
               strcmp(j->valuestring, "iii") == 0 ||
               strcmp(j->valuestring, "3") == 0;
    }
    return false;
}

static bool json_item_has_level3(cJSON *item, bool *has_level_key)
{
    const char *level_keys[] = {
        "level", "level_v", "level_h", "alarm_level",
        "overlimit_level", "overlimit_type", "type", "data_type",
        NULL
    };

    if(has_level_key) *has_level_key = false;
    if(!cJSON_IsObject(item)) return false;

    for(int i = 0; level_keys[i]; i++) {
        cJSON *v = cJSON_GetObjectItem(item, level_keys[i]);
        if(!v) continue;
        if(has_level_key) *has_level_key = true;
        if(json_level_is_iii(v)) return true;
    }

    return false;
}

static int count_detail_json_items(const char *boarding_key, query_data_type_t type, bool level3_only)
{
    char json_path[128];
    if(!build_query_json_path(json_path, sizeof(json_path), boarding_key, type)) {
        return -1;
    }

    char *txt = read_text_file_lvfs_limit(json_path, NULL);
    if(!txt) return -1;

    cJSON *root = cJSON_Parse(txt);
    lv_free(txt);
    if(!root) return -1;

    int count = -1;
    cJSON *arr = json_get_items_array(root);
    if(cJSON_IsArray(arr)) {
        if(level3_only) {
            bool has_level_any = false;
            count = 0;
            int n = cJSON_GetArraySize(arr);
            for(int i = 0; i < n; i++) {
                bool has_level_key = false;
                if(json_item_has_level3(cJSON_GetArrayItem(arr, i), &has_level_key)) {
                    count++;
                }
                if(has_level_key) has_level_any = true;
            }
            if(!has_level_any) count = n;
        } else {
            count = cJSON_GetArraySize(arr);
        }
    } else {
        count = json_get_int_flexible(root, level3_only ? "level3_count" : "count", -1);
    }

    cJSON_Delete(root);
    return count;
}

static bool boarding_parse_all_records(const char *json_path)
{
    if(!json_path) return false;

    size_t len = 0;
    char *txt = read_text_file_lvfs_limit(json_path, &len);
    if(!txt) return false;

    uint32_t h = fnv1a32((const uint8_t *)txt, len);
    if(s_cache_ok &&
       strcmp(s_cache_path, json_path) == 0 &&
       s_cache_hash == h &&
       g_records && g_records_cnt > 0)
    {
        lv_free(txt);
        return true;
    }

    cJSON *root = cJSON_Parse(txt);
    if(!root) {
        const char *ep = cJSON_GetErrorPtr();
        printf("[BOARDING] cJSON_Parse failed at: %s\n", ep ? ep : "(null)");
        lv_free(txt);
        return false;
    }

    cJSON *arr = NULL;
    if(cJSON_IsArray(root)) {
        arr = root;
    } else {
        arr = cJSON_GetObjectItem(root, "records");
        if(!cJSON_IsArray(arr)) arr = cJSON_GetObjectItem(root, "items");
    }

    if(!cJSON_IsArray(arr)) {
        cJSON_Delete(root);
        lv_free(txt);
        return false;
    }

    int n_all = cJSON_GetArraySize(arr);
    if(n_all <= 0) {
        cJSON_Delete(root);
        lv_free(txt);
        records_free_all();

        s_cache_ok = true;
        s_cache_hash = h;
        strncpy(s_cache_path, json_path, sizeof(s_cache_path) - 1);
        s_cache_path[sizeof(s_cache_path) - 1] = '\0';
        return true;
    }

    int start = 0;
    int n = n_all;
    if(n > BOARDING_MAX_RECORDS) {
        start = n_all - BOARDING_MAX_RECORDS;
        n = BOARDING_MAX_RECORDS;
    }

    records_free_all();
    g_records = (boarding_record_t *)lv_malloc(sizeof(boarding_record_t) * (size_t)n);
    if(!g_records) {
        cJSON_Delete(root);
        lv_free(txt);
        return false;
    }
    memset(g_records, 0, sizeof(boarding_record_t) * (size_t)n);

    int out = 0;
    bool detail_counts_used = false;
    for(int i = start; i < start + n; i++) {
        cJSON *it = cJSON_GetArrayItem(arr, i);
        if(!cJSON_IsObject(it)) continue;

        cJSON *j_boarding_key = cJSON_GetObjectItem(it, "boarding_key");
        cJSON *j_start_ts     = cJSON_GetObjectItem(it, "start_ts");
        cJSON *j_route_name   = cJSON_GetObjectItem(it, "route_name");
        cJSON *j_loco_no      = cJSON_GetObjectItem(it, "loco_no");
        cJSON *j_train_no     = cJSON_GetObjectItem(it, "train_no");
        cJSON *j_line_name    = cJSON_GetObjectItem(it, "line_name");
        cJSON *j_dir          = cJSON_GetObjectItem(it, "dir");

        if(!cJSON_IsString(j_start_ts) || !cJSON_IsString(j_route_name) || !cJSON_IsString(j_boarding_key)) {
            continue;
        }

        boarding_record_t *r = &g_records[out];
        memset(r, 0, sizeof(*r));

        strncpy(r->boarding_key, j_boarding_key->valuestring, sizeof(r->boarding_key) - 1);
        r->boarding_key[sizeof(r->boarding_key) - 1] = '\0';

        strncpy(r->start_ts, j_start_ts->valuestring, sizeof(r->start_ts) - 1);
        r->start_ts[sizeof(r->start_ts) - 1] = '\0';

        parse_ts_14_to_date_time(r->start_ts, r->date, sizeof(r->date), r->time, sizeof(r->time));

        strncpy(r->route_name, j_route_name->valuestring, sizeof(r->route_name) - 1);
        r->route_name[sizeof(r->route_name) - 1] = '\0';

        if(cJSON_IsString(j_loco_no)) {
            strncpy(r->loco_no, j_loco_no->valuestring, sizeof(r->loco_no) - 1);
            r->loco_no[sizeof(r->loco_no) - 1] = '\0';
        }
        if(cJSON_IsString(j_train_no)) {
            strncpy(r->train_no, j_train_no->valuestring, sizeof(r->train_no) - 1);
            r->train_no[sizeof(r->train_no) - 1] = '\0';
        }
        if(cJSON_IsString(j_line_name)) {
            strncpy(r->line_name, j_line_name->valuestring, sizeof(r->line_name) - 1);
            r->line_name[sizeof(r->line_name) - 1] = '\0';
        }
        if(cJSON_IsString(j_dir)) {
            strncpy(r->dir, j_dir->valuestring, sizeof(r->dir) - 1);
            r->dir[sizeof(r->dir) - 1] = '\0';
        }

        r->level3_count = json_get_stat_int(it, "level3_count", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "level3", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "iii_count", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "iii", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "III", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "alarm_count", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "alarm", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "overlimit_count", -1);
        if(r->level3_count < 0) r->level3_count = json_get_stat_int(it, "overlimit", -1);
        if(r->level3_count < 0) {
            detail_counts_used = true;
            r->level3_count = count_detail_json_items(r->boarding_key, QUERY_DATA_TYPE_OVERLIMIT, true);
        }
        if(r->level3_count < 0) r->level3_count = 0;

        r->mark_count = json_get_stat_int(it, "mark_count", -1);
        if(r->mark_count < 0) r->mark_count = json_get_stat_int(it, "normal_mark_count", -1);
        if(r->mark_count < 0) r->mark_count = json_get_stat_int(it, "mark", -1);
        if(r->mark_count < 0) r->mark_count = json_get_stat_int(it, "normal_mark", -1);
        if(r->mark_count < 0) {
            detail_counts_used = true;
            r->mark_count = count_detail_json_items(r->boarding_key, QUERY_DATA_TYPE_NORMAL_MARK, false);
        }
        if(r->mark_count < 0) r->mark_count = 0;

        r->audio_count = json_get_stat_int(it, "audio_count", -1);
        if(r->audio_count < 0) r->audio_count = json_get_stat_int(it, "audio_mark_count", -1);
        if(r->audio_count < 0) r->audio_count = json_get_stat_int(it, "record_count", -1);
        if(r->audio_count < 0) r->audio_count = json_get_stat_int(it, "audio", -1);
        if(r->audio_count < 0) r->audio_count = json_get_stat_int(it, "audio_mark", -1);
        if(r->audio_count < 0) r->audio_count = json_get_stat_int(it, "record", -1);
        if(r->audio_count < 0) {
            detail_counts_used = true;
            r->audio_count = count_detail_json_items(r->boarding_key, QUERY_DATA_TYPE_AUDIO_MARK, false);
        }
        if(r->audio_count < 0) r->audio_count = 0;

        if(r->boarding_key[0] == '\0' || r->route_name[0] == '\0') continue;
        out++;
    }

    cJSON_Delete(root);
    lv_free(txt);

    g_records_cnt = out;
    if(g_records_cnt <= 0) {
        records_free_all();
    }

    s_cache_ok = !detail_counts_used;
    s_cache_hash = h;
    strncpy(s_cache_path, json_path, sizeof(s_cache_path) - 1);
    s_cache_path[sizeof(s_cache_path) - 1] = '\0';

    return true;
}

static const boarding_record_t *records_get(int idx)
{
    if(idx < 0 || idx >= g_records_cnt) return NULL;
    const boarding_record_t *r = &g_records[idx];
    if(r->boarding_key[0] == '\0') return NULL;
    return r;
}

/* ================= Virtual list structs ================= */
typedef struct {
    lv_obj_t *root;
    lv_obj_t *lbl_date;
    lv_obj_t *lbl_route;
    lv_obj_t *lbl_train;
    lv_obj_t *lbl_loco;

    lv_obj_t *btn_1;
    lv_obj_t *btn_1_label;
    lv_obj_t *btn_2;
    lv_obj_t *btn_2_label;
    lv_obj_t *btn_3;
    lv_obj_t *btn_3_label;

    lv_obj_t *btn_overlimit_rel;
    lv_obj_t *btn_overlimit_rel_label;
    lv_obj_t *btn_mark_rel;
    lv_obj_t *btn_mark_rel_label;
    lv_obj_t *btn_audio_rel;
    lv_obj_t *btn_audio_rel_label;

    int record_index;
} vlist_item_t;

typedef struct {
    lv_obj_t *list;
    lv_obj_t *spacer;
    int item_h;
    int visible_cnt;
    int total_cnt;
    int first_index;
    vlist_item_t *items;
} vlist_t;

static vlist_t g_vlist;

static void vlist_update(void);

/* ================= 透明按钮事件 ================= */
static void jump_to_target_page(const boarding_record_t *r, query_data_type_t type)
{
    if(!r) return;

    query_context_set_boarding_key(r->boarding_key);
    query_context_set_data_type(type);

    char json_path[128];
    if(!build_query_json_path(json_path, sizeof(json_path), r->boarding_key, type)) {
        printf("[BOARDING] build path failed, key=%s, type=%d\n", r->boarding_key, (int)type);
        return;
    }

    extern lv_ui guider_ui;

    switch(type) {
    case QUERY_DATA_TYPE_OVERLIMIT:
        printf("[BOARDING] overlimit key=%s path=%s\n", r->boarding_key, json_path);
        /* 按你的真实页面名替换 */
        ui_load_scr_animation(&guider_ui,
                      &guider_ui.screen_overlimt,
                      guider_ui.screen_overlimt_del,
                      &guider_ui.screen_log_del,
                      setup_scr_screen_overlimt,
                      LV_SCR_LOAD_ANIM_NONE,
                      100,
                      100,
                      false,
                      false);
        ui_overlimt_render_from_json(&guider_ui, json_path);
        break;

    case QUERY_DATA_TYPE_NORMAL_MARK:
        printf("[BOARDING] mark key=%s path=%s\n", r->boarding_key, json_path);
        ui_load_scr_animation(&guider_ui,
                      &guider_ui.screen_mark,
                      guider_ui.screen_mark_del,
                      &guider_ui.screen_log_del,
                      setup_scr_screen_mark,
                      LV_SCR_LOAD_ANIM_NONE,
                      100,
                      100,
                      false,
                      false);
        ui_mark_render_from_json(&guider_ui, json_path);
        break;

    case QUERY_DATA_TYPE_AUDIO_MARK:
        printf("[BOARDING] audio key=%s path=%s\n", r->boarding_key, json_path);
        ui_load_scr_animation(&guider_ui,
                      &guider_ui.screen_audiomark,
                      guider_ui.screen_audiomark_del,
                      &guider_ui.screen_log_del,
                      setup_scr_screen_audiomark,
                      LV_SCR_LOAD_ANIM_NONE,
                      100,
                      100,
                      false,
                      false);
        ui_audiomark_render_from_json(&guider_ui, json_path);
        break;

    default:
        break;
    }

    ui_boarding_log_deinit();
}

static void boarding_btn_overlimit_cb(lv_event_t *e)
{
    vlist_item_t *it = (vlist_item_t *)lv_event_get_user_data(e);
    if(!it) return;
    const boarding_record_t *r = records_get(it->record_index);
    if(!r) return;

    static int last_record_index = -1;
    static uint32_t last_tick = 0;
    uint32_t now = lv_tick_get();

    if(last_record_index == it->record_index &&
       (uint32_t)(now - last_tick) <= CARD_DOUBLE_CLICK_MS) {
        last_record_index = -1;
        last_tick = 0;
        jump_to_target_page(r, QUERY_DATA_TYPE_OVERLIMIT);
        return;
    }

    last_record_index = it->record_index;
    last_tick = now;
}

static void boarding_btn_mark_cb(lv_event_t *e)
{
    vlist_item_t *it = (vlist_item_t *)lv_event_get_user_data(e);
    if(!it) return;
    const boarding_record_t *r = records_get(it->record_index);
    if(!r) return;
    jump_to_target_page(r, QUERY_DATA_TYPE_NORMAL_MARK);
}

static void boarding_btn_audio_cb(lv_event_t *e)
{
    vlist_item_t *it = (vlist_item_t *)lv_event_get_user_data(e);
    if(!it) return;
    const boarding_record_t *r = records_get(it->record_index);
    if(!r) return;
    jump_to_target_page(r, QUERY_DATA_TYPE_AUDIO_MARK);
}

static void vlist_destroy(void)
{
    if(g_vlist.items) {
        lv_free(g_vlist.items);
        g_vlist.items = NULL;
    }
    g_vlist.list = NULL;
    g_vlist.spacer = NULL;
    g_vlist.item_h = 0;
    g_vlist.visible_cnt = 0;
    g_vlist.total_cnt = 0;
    g_vlist.first_index = -1;
}

static void fix_list_style_for_target(lv_obj_t *list)
{
    if(!list) return;

    lv_obj_set_layout(list, LV_LAYOUT_NONE);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_style_pad_row(list, 0, 0);
    lv_obj_set_style_pad_column(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_outline_width(list, 0, 0);
    lv_obj_set_style_shadow_width(list, 0, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
}

static void set_label_common_style(lv_obj_t *lbl)
{
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_CLIP);
    lv_obj_clear_flag(lbl, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_border_width(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl, lv_color_hex(TXT_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, APP_LOG_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void set_count_label_style(lv_obj_t *lbl)
{
    set_label_common_style(lbl);
    lv_obj_set_style_text_color(lbl, lv_color_hex(TXT_VALUE_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, APP_LOG_TITLE_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lbl, lv_color_hex(BTN_BG_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lbl, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void set_small_btn_style(lv_obj_t *btn)
{
    lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(BTN_BG_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btn, lv_color_hex(TXT_VALUE_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(btn, APP_LOG_TITLE_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(btn, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void set_transparent_rel_btn_style(lv_obj_t *btn)
{
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(btn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(btn, APP_LOG_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void vlist_bind_item(vlist_item_t *it, int rec_index)
{
    if(!it || !it->root) return;

    if(rec_index < 0 || rec_index >= g_records_cnt) {
        it->record_index = -1;
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    const boarding_record_t *r = records_get(rec_index);
    if(!r) {
        it->record_index = -1;
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    it->record_index = rec_index;
    lv_obj_clear_flag(it->root, LV_OBJ_FLAG_HIDDEN);

    char time_buf[32];
    if(r->date[0] && r->time[0]) {
        lv_snprintf(time_buf, sizeof(time_buf), "%s %.5s", r->date, r->time);
    } else {
        lv_snprintf(time_buf, sizeof(time_buf), "--");
    }

    char count_buf[16];
    lv_label_set_text(it->lbl_route, r->route_name[0] ? r->route_name : "--");
    lv_label_set_text(it->lbl_date, time_buf);

    lv_snprintf(count_buf, sizeof(count_buf), "%d", r->level3_count);
    lv_label_set_text(it->lbl_train, count_buf);
    lv_snprintf(count_buf, sizeof(count_buf), "%d", r->mark_count);
    lv_label_set_text(it->lbl_loco, count_buf);
    lv_snprintf(count_buf, sizeof(count_buf), "%d", r->audio_count);
    lv_label_set_text(it->btn_1_label, count_buf);
}

static void vlist_scroll_cb(lv_event_t *e)
{
    (void)e;
    vlist_update();
}

static void vlist_create(lv_ui *ui)
{
    vlist_t *vl = &g_vlist;

    vl->list = ui->screen_log_cont_list;
    vl->item_h = VLIST_ITEM_H;
    vl->first_index = -1;
    vl->total_cnt = g_records_cnt;

    int step = vl->item_h + VLIST_GAP_PX;

    lv_obj_clean(vl->list);
    fix_list_style_for_target(vl->list);

    lv_coord_t vis_h = lv_obj_get_height(vl->list);
    if(vis_h <= 0) vis_h = 200;

    vl->visible_cnt = (int)(vis_h / step) + VLIST_CACHE_EXTRA;
    if(vl->visible_cnt < 3) vl->visible_cnt = 3;
    if(vl->visible_cnt > vl->total_cnt) vl->visible_cnt = vl->total_cnt;
    if(vl->visible_cnt <= 0) vl->visible_cnt = 1;

    vl->spacer = lv_obj_create(vl->list);
    lv_obj_remove_style_all(vl->spacer);
    lv_obj_clear_flag(vl->spacer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(vl->spacer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(vl->spacer, 1, (lv_coord_t)(vl->total_cnt * step));
    lv_obj_set_pos(vl->spacer, 0, 0);

    vl->items = (vlist_item_t *)lv_malloc(sizeof(vlist_item_t) * (size_t)vl->visible_cnt);
    if(!vl->items) {
        printf("[BOARDING] vlist items alloc failed\n");
        return;
    }
    memset(vl->items, 0, sizeof(vlist_item_t) * (size_t)vl->visible_cnt);

    for(int i = 0; i < vl->visible_cnt; i++) {
        vlist_item_t *it = &vl->items[i];

        it->root = lv_obj_create(vl->list);
        lv_obj_set_pos(it->root, ITEM_X, ITEM_Y0);
        lv_obj_set_size(it->root, ITEM_W, vl->item_h);
        lv_obj_set_scrollbar_mode(it->root, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(it->root, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_border_width(it->root, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(it->root, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(it->root, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(it->root, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(it->root, CARD_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(it->root, CARD_BG_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(it->root, lv_color_hex(CARD_BG_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(it->root, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(it->root, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(it->root, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(it->root, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(it->root, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(it->root, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_layout(it->root, LV_LAYOUT_NONE);

        lv_obj_t *img_bg = lv_image_create(it->root);
        lv_obj_set_pos(img_bg, 0, 0);
        lv_obj_set_size(img_bg, ITEM_W, VLIST_ITEM_H);
        lv_obj_clear_flag(img_bg, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(img_bg, &_tccard_RGB565A8_224x200);
        lv_image_set_pivot(img_bg, 50, 50);
        lv_image_set_rotation(img_bg, 0);
        lv_obj_set_style_image_recolor_opa(img_bg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(img_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        it->lbl_date = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_date, X_DATE, Y_DATE);
        lv_obj_set_size(it->lbl_date, W_DATE, H_DATE);
        set_label_common_style(it->lbl_date);
        lv_obj_set_style_text_font(it->lbl_date, APP_LOG_TIME_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(it->lbl_date, lv_color_hex(TXT_TIME_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);

        it->lbl_route = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_route, X_ROUTE, Y_ROUTE);
        lv_obj_set_size(it->lbl_route, W_ROUTE, H_ROUTE);
        set_label_common_style(it->lbl_route);
        lv_obj_set_style_text_font(it->lbl_route, APP_LOG_TITLE_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(it->lbl_route, lv_color_hex(TXT_HEX), LV_PART_MAIN | LV_STATE_DEFAULT);

        it->lbl_train = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_train, X_TRAIN, Y_TRAIN);
        lv_obj_set_size(it->lbl_train, W_TRAIN, H_TRAIN);
        set_count_label_style(it->lbl_train);

        it->lbl_loco = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_loco, X_LOCO, Y_LOCO);
        lv_obj_set_size(it->lbl_loco, W_LOCO, H_LOCO);
        set_count_label_style(it->lbl_loco);

        /* 视觉按钮 */
        it->btn_1 = lv_button_create(it->root);
        lv_obj_set_pos(it->btn_1, X_BTN1, Y_BTN);
        lv_obj_set_size(it->btn_1, W_BTN, H_BTN);
        set_small_btn_style(it->btn_1);
        it->btn_1_label = lv_label_create(it->btn_1);
        lv_label_set_text(it->btn_1_label, "0");
        lv_label_set_long_mode(it->btn_1_label, LV_LABEL_LONG_CLIP);
        lv_obj_align(it->btn_1_label, LV_ALIGN_LEFT_MID, 2, 0);
        lv_obj_set_style_pad_all(it->btn_1, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(it->btn_1_label, lv_color_hex(TXT_VALUE_HEX), 0);
        lv_obj_set_style_text_font(it->btn_1_label, APP_LOG_TITLE_FONT, 0);
        lv_obj_set_width(it->btn_1_label, LV_PCT(100));
        lv_obj_clear_flag(it->btn_1, LV_OBJ_FLAG_CLICKABLE);

        it->btn_2 = lv_button_create(it->root);
        lv_obj_set_pos(it->btn_2, X_BTN2, Y_BTN);
        lv_obj_set_size(it->btn_2, W_BTN, H_BTN);
        set_small_btn_style(it->btn_2);
        it->btn_2_label = lv_label_create(it->btn_2);
        lv_label_set_text(it->btn_2_label, "标记");
        lv_label_set_long_mode(it->btn_2_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(it->btn_2_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(it->btn_2, 0, LV_STATE_DEFAULT);
        lv_obj_set_width(it->btn_2_label, LV_PCT(100));
        lv_obj_clear_flag(it->btn_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(it->btn_2, LV_OBJ_FLAG_HIDDEN);

        it->btn_3 = lv_button_create(it->root);
        lv_obj_set_pos(it->btn_3, X_BTN3, Y_BTN);
        lv_obj_set_size(it->btn_3, W_BTN, H_BTN);
        set_small_btn_style(it->btn_3);
        it->btn_3_label = lv_label_create(it->btn_3);
        lv_label_set_text(it->btn_3_label, "录音");
        lv_label_set_long_mode(it->btn_3_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(it->btn_3_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(it->btn_3, 0, LV_STATE_DEFAULT);
        lv_obj_set_width(it->btn_3_label, LV_PCT(100));
        lv_obj_clear_flag(it->btn_3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(it->btn_3, LV_OBJ_FLAG_HIDDEN);

        /* 透明点击区域 */
        it->btn_overlimit_rel = lv_button_create(it->root);
        lv_obj_set_pos(it->btn_overlimit_rel, X_REL1, Y_REL1);
        lv_obj_set_size(it->btn_overlimit_rel, W_REL, H_REL);
        set_transparent_rel_btn_style(it->btn_overlimit_rel);
        it->btn_overlimit_rel_label = lv_label_create(it->btn_overlimit_rel);
        lv_label_set_text(it->btn_overlimit_rel_label, "");
        lv_label_set_long_mode(it->btn_overlimit_rel_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(it->btn_overlimit_rel_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(it->btn_overlimit_rel, 0, LV_STATE_DEFAULT);
        lv_obj_set_width(it->btn_overlimit_rel_label, LV_PCT(100));
        lv_obj_add_event_cb(it->btn_overlimit_rel, boarding_btn_overlimit_cb, LV_EVENT_CLICKED, it);

        it->btn_mark_rel = lv_button_create(it->root);
        lv_obj_set_pos(it->btn_mark_rel, X_REL2, Y_REL2);
        lv_obj_set_size(it->btn_mark_rel, W_REL, H_REL);
        set_transparent_rel_btn_style(it->btn_mark_rel);
        it->btn_mark_rel_label = lv_label_create(it->btn_mark_rel);
        lv_label_set_text(it->btn_mark_rel_label, "");
        lv_label_set_long_mode(it->btn_mark_rel_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(it->btn_mark_rel_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(it->btn_mark_rel, 0, LV_STATE_DEFAULT);
        lv_obj_set_width(it->btn_mark_rel_label, LV_PCT(100));
        lv_obj_clear_flag(it->btn_mark_rel, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(it->btn_mark_rel, LV_OBJ_FLAG_HIDDEN);

        it->btn_audio_rel = lv_button_create(it->root);
        lv_obj_set_pos(it->btn_audio_rel, X_REL3, Y_REL3);
        lv_obj_set_size(it->btn_audio_rel, W_REL, H_REL);
        set_transparent_rel_btn_style(it->btn_audio_rel);
        it->btn_audio_rel_label = lv_label_create(it->btn_audio_rel);
        lv_label_set_text(it->btn_audio_rel_label, "");
        lv_label_set_long_mode(it->btn_audio_rel_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(it->btn_audio_rel_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(it->btn_audio_rel, 0, LV_STATE_DEFAULT);
        lv_obj_set_width(it->btn_audio_rel_label, LV_PCT(100));
        lv_obj_clear_flag(it->btn_audio_rel, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(it->btn_audio_rel, LV_OBJ_FLAG_HIDDEN);

        it->record_index = -1;
        lv_obj_move_foreground(it->btn_overlimit_rel);
    }

    lv_obj_add_event_cb(vl->list, vlist_scroll_cb, LV_EVENT_SCROLL, NULL);
}

static void vlist_update(void)
{
    if(!g_vlist.list || !g_vlist.items || g_records_cnt <= 0) return;

    lv_coord_t scroll_y = lv_obj_get_scroll_top(g_vlist.list);

    int step = g_vlist.item_h + VLIST_GAP_PX;
    int first = (int)(scroll_y / step);
    if(first < 0) first = 0;

    int max_first = g_records_cnt - g_vlist.visible_cnt;
    if(max_first < 0) max_first = 0;
    if(first > max_first) first = max_first;

    if(first == g_vlist.first_index) return;
    g_vlist.first_index = first;

    for(int i = 0; i < g_vlist.visible_cnt; i++) {
        int disp_index = first + i;
        int rec_index  = (g_records_cnt - 1) - disp_index; /* 最新在上 */

        vlist_bind_item(&g_vlist.items[i], rec_index);

        lv_obj_set_pos(g_vlist.items[i].root,
                       ITEM_X,
                       (lv_coord_t)(ITEM_Y0 + disp_index * step));
    }
}

/* ================= Public API ================= */
void ui_boarding_log_deinit(void)
{
    vlist_destroy();
    records_free_all();
}

void ui_boarding_log_render_from_json(lv_ui *ui, const char *json_path)
{
    if(!ui || !ui->screen_log_cont_list) return;

    if(ui->screen_log_label_14) {
        lv_label_set_text(ui->screen_log_label_14, "暂无添乘记录");
        lv_obj_add_flag(ui->screen_log_label_14, LV_OBJ_FLAG_HIDDEN);
    }

    vlist_destroy();

    if(!json_path || !json_path[0]) {
        lv_obj_clean(ui->screen_log_cont_list);
        if(ui->screen_log_label_14) {
            lv_obj_clear_flag(ui->screen_log_label_14, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    if(!boarding_parse_all_records(json_path)) {
        records_free_all();
        lv_obj_clean(ui->screen_log_cont_list);
        if(ui->screen_log_label_14) {
            lv_obj_clear_flag(ui->screen_log_label_14, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    if(g_records_cnt <= 0) {
        records_free_all();
        lv_obj_clean(ui->screen_log_cont_list);
        if(ui->screen_log_label_14) {
            lv_obj_clear_flag(ui->screen_log_label_14, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    vlist_create(ui);
    lv_obj_scroll_to_y(ui->screen_log_cont_list, 0, LV_ANIM_OFF);
    vlist_update();
}