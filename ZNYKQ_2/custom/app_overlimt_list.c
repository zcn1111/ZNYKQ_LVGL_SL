#include "app_overlimt_list.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "guider_customer_fonts.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "cJSON.h"

/* ============== 可选：ESP32 真机打点 ============== */
#define OVER_USE_ESP_TIMER   0
#define OVER_PRINT_TIMING    0

#if OVER_USE_ESP_TIMER
#include "esp_timer.h"
static inline int64_t TICK_MS(void) { return esp_timer_get_time() / 1000; }
#else
static inline int64_t TICK_MS(void) { return 0; }
#endif

/* ================= Font ================= */
#ifndef APP_LOG_FONT
#define APP_LOG_FONT (&lv_customer_font_PingFangHeavy_16)
#endif

/* ================= JSON 限制 ================= */
#define OVER_JSON_MAX_SIZE   (64 * 1024)
#define OVER_MAX_RECORDS     (200)

/* ================= VList config ================= */
#define VLIST_ITEM_H        82
#define VLIST_GAP_PX        8
#define VLIST_CACHE_EXTRA   2

/* ================= Style ================= */
#define CARD_RADIUS  10
#define CARD_BG_HEX  0xF4D9DE
#define RED_HEX      0xD60000

/* ===== 模板坐标（相对 item root） ===== */
#define ITEM_X        5
#define ITEM_Y0       6

#define X_DATE        18
#define Y_DATE        7
#define W_DATE        95
#define H_DATE        21

#define X_TIME        124
#define Y_TIME        7
#define W_TIME        64
#define H_TIME        17

#define X_MILEAGE     18
#define Y_MILEAGE     31
#define W_MILEAGE     100
#define H_MILEAGE     17

#define X_SPEED       124
#define Y_SPEED       33
#define W_SPEED       89
#define H_SPEED       18

#define X_V           18
#define Y_V           57
#define W_V           84
#define H_V           20

#define X_H           124
#define Y_H           59
#define W_H           84
#define H_H           20

#define ITEM_W        220

/* ================= Data model ================= */
typedef struct {
    char ts[20];              /* 20260122093552 */
    char date[11];            /* 2026-01-22 */
    char time[9];             /* 09:35:52 */
    char mileage_text[32];    /* K1100+579 */
    int  speed_kmh;           /* 65 */
    int  acc_v_01;            /* 14 */
    int  acc_h_01;            /* 40 */
    int  v_level;             /* 1~4 */
    int  h_level;             /* 1~4 */
} overlimt_record_t;

/* ================= Helpers ================= */
static const char *lvl_to_roman(int lvl)
{
    switch(lvl) {
    case 1: return "I";
    case 2: return "II";
    case 3: return "III";
    case 4: return "IV";
    default: return "0";
    }
}

static int roman_to_lvl(const char *s)
{
    if(!s) return 0;
    if(strcmp(s, "I") == 0)   return 1;
    if(strcmp(s, "II") == 0)  return 2;
    if(strcmp(s, "III") == 0) return 3;
    if(strcmp(s, "IV") == 0)  return 4;
    return 0;
}

/* ts: 20260122093552 -> 2026-01-22 / 09:35:52 */
static void parse_ts_14_to_date_time(const char *ts14,
                                     char *date_out, size_t date_sz,
                                     char *time_out, size_t time_sz)
{
    if(date_out && date_sz > 0) date_out[0] = '\0';
    if(time_out && time_sz > 0) time_out[0] = '\0';

    if(!ts14) return;
    if(strlen(ts14) < 14) return;

    if(date_out && date_sz >= 11) {
        lv_snprintf(date_out, date_sz, "%.4s-%.2s-%.2s",
                    ts14, ts14 + 4, ts14 + 6);
    }

    if(time_out && time_sz >= 9) {
        lv_snprintf(time_out, time_sz, "%.2s:%.2s:%.2s",
                    ts14 + 8, ts14 + 10, ts14 + 12);
    }
}

static int json_get_int_flexible(cJSON *obj, const char *key, int def_val)
{
    cJSON *j = cJSON_GetObjectItem(obj, key);
    if(cJSON_IsNumber(j)) return j->valueint;
    if(cJSON_IsString(j) && j->valuestring) return atoi(j->valuestring);
    return def_val;
}

static void ui_show_message_in_over_list(lv_ui *ui, const char *msg)
{
    if(!ui || !ui->screen_overlimt_cont_list) return;

    lv_obj_t *parent = ui->screen_overlimt_cont_list;
    lv_obj_clean(parent);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(parent, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(parent, 8, 0);

    lv_obj_t *lbl = lv_label_create(parent);
    lv_label_set_text(lbl, msg ? msg : "");
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(lbl, lv_pct(100));
    lv_obj_set_style_text_font(lbl, APP_LOG_FONT, 0);
}

/* ================= Parse-all RAM records ================= */
static overlimt_record_t *g_records = NULL;
static int g_records_cnt = 0;

static bool     s_cache_ok = false;
static uint32_t s_cache_hash = 0;
static char     s_cache_path[128] = {0};

static lv_obj_t *s_overlimt_list = NULL;

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
        printf("[OVER] lv_fs_open failed (%d): %s\n", (int)rr, path);
        return NULL;
    }

    char *buf = (char *)lv_malloc(OVER_JSON_MAX_SIZE + 1);
    if(!buf) {
        lv_fs_close(&f);
        return NULL;
    }

    size_t total = 0;
    while(total < OVER_JSON_MAX_SIZE) {
        uint32_t br = 0;
        uint32_t want = (uint32_t)(OVER_JSON_MAX_SIZE - total);

        rr = lv_fs_read(&f, buf + total, want, &br);
        if(rr != LV_FS_RES_OK) {
            printf("[OVER] lv_fs_read failed (%d)\n", (int)rr);
            lv_free(buf);
            lv_fs_close(&f);
            return NULL;
        }

        if(br == 0) break;
        total += br;
    }

    lv_fs_close(&f);

    if(total == 0) {
        printf("[OVER] file empty: %s\n", path);
        lv_free(buf);
        return NULL;
    }

    if(total >= OVER_JSON_MAX_SIZE) {
        printf("[OVER] file too large (>= %d bytes): %s\n", OVER_JSON_MAX_SIZE, path);
        lv_free(buf);
        return NULL;
    }

    buf[total] = '\0';
    if(out_len) *out_len = total;
    return buf;
}

/* 支持：
   1) {"items":[...]}
   2) {"records":[...]}
   3) 顶层数组 [...]
*/
static bool overlimt_parse_all_records(const char *json_path)
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
        printf("[OVER] cJSON_Parse failed at: %s\n", ep ? ep : "(null)");
        lv_free(txt);
        return false;
    }

    cJSON *arr = NULL;
    if(cJSON_IsArray(root)) {
        arr = root;
    } else {
        arr = cJSON_GetObjectItem(root, "items");
        if(!cJSON_IsArray(arr)) arr = cJSON_GetObjectItem(root, "records");
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
    if(n > OVER_MAX_RECORDS) {
        start = n_all - OVER_MAX_RECORDS;
        n = OVER_MAX_RECORDS;
    }

    records_free_all();
    g_records = (overlimt_record_t *)lv_malloc(sizeof(overlimt_record_t) * (size_t)n);
    if(!g_records) {
        cJSON_Delete(root);
        lv_free(txt);
        return false;
    }
    memset(g_records, 0, sizeof(overlimt_record_t) * (size_t)n);

    int out = 0;
    for(int i = start; i < start + n; i++) {
        cJSON *it = cJSON_GetArrayItem(arr, i);
        if(!cJSON_IsObject(it)) continue;

        cJSON *j_ts      = cJSON_GetObjectItem(it, "ts");
        cJSON *j_mileage = cJSON_GetObjectItem(it, "mileage_text");
        cJSON *j_level_v = cJSON_GetObjectItem(it, "level_v");
        cJSON *j_level_h = cJSON_GetObjectItem(it, "level_h");

        if(!cJSON_IsString(j_ts) || !cJSON_IsString(j_mileage)) {
            continue;
        }

        overlimt_record_t *r = &g_records[out];
        memset(r, 0, sizeof(*r));

        strncpy(r->ts, j_ts->valuestring, sizeof(r->ts) - 1);
        r->ts[sizeof(r->ts) - 1] = '\0';

        parse_ts_14_to_date_time(r->ts,
                                 r->date, sizeof(r->date),
                                 r->time, sizeof(r->time));

        strncpy(r->mileage_text, j_mileage->valuestring, sizeof(r->mileage_text) - 1);
        r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';

        r->speed_kmh = json_get_int_flexible(it, "speed_kmh", 0);
        r->acc_v_01  = json_get_int_flexible(it, "acc_v", 0);
        r->acc_h_01  = json_get_int_flexible(it, "acc_h", 0);

        if(cJSON_IsString(j_level_v)) {
            r->v_level = roman_to_lvl(j_level_v->valuestring);
        } else if(cJSON_IsNumber(j_level_v)) {
            r->v_level = j_level_v->valueint;
        } else {
            r->v_level = 0;
        }

        if(cJSON_IsString(j_level_h)) {
            r->h_level = roman_to_lvl(j_level_h->valuestring);
        } else if(cJSON_IsNumber(j_level_h)) {
            r->h_level = j_level_h->valueint;
        } else {
            r->h_level = 0;
        }

        if(r->mileage_text[0] == '\0') continue;
        out++;
    }

    cJSON_Delete(root);
    lv_free(txt);

    g_records_cnt = out;
    if(g_records_cnt <= 0) {
        records_free_all();
    }

    s_cache_ok = true;
    s_cache_hash = h;
    strncpy(s_cache_path, json_path, sizeof(s_cache_path) - 1);
    s_cache_path[sizeof(s_cache_path) - 1] = '\0';

    printf("[OVER] parsed count = %d\n", g_records_cnt);
    return true;
}

static const overlimt_record_t *records_get(int idx)
{
    if(idx < 0 || idx >= g_records_cnt) return NULL;
    const overlimt_record_t *r = &g_records[idx];
    if(r->mileage_text[0] == '\0') return NULL;
    return r;
}

/* ================= Virtual list structs ================= */
typedef struct {
    lv_obj_t *root;
    lv_obj_t *lbl_date;
    lv_obj_t *lbl_time;
    lv_obj_t *lbl_mileage;
    lv_obj_t *lbl_speed;
    lv_obj_t *lbl_v;
    lv_obj_t *lbl_h;
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

/* ================= VList helpers ================= */
static void vlist_update(void);

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

#define FIX_LBL(_lb) do { \
    lv_label_set_long_mode((_lb), LV_LABEL_LONG_CLIP); \
    lv_obj_clear_flag((_lb), LV_OBJ_FLAG_SCROLLABLE); \
    lv_obj_set_style_pad_all((_lb), 0, 0); \
    lv_obj_set_style_bg_opa((_lb), LV_OPA_TRANSP, 0); \
} while(0)

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

static void vlist_bind_item(vlist_item_t *it, int rec_index)
{
    if(!it || !it->root) return;

    if(rec_index < 0 || rec_index >= g_records_cnt) {
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    const overlimt_record_t *r = records_get(rec_index);
    if(!r) {
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_clear_flag(it->root, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(it->lbl_date, r->date[0] ? r->date : "--");
    lv_label_set_text(it->lbl_time, r->time[0] ? r->time : "--:--:--");
    lv_label_set_text(it->lbl_mileage, r->mileage_text[0] ? r->mileage_text : "--");

    char spd[24];
    if(r->speed_kmh > 0) {
        lv_snprintf(spd, sizeof(spd), "%dkm/h", r->speed_kmh);
    } else {
        lv_snprintf(spd, sizeof(spd), "--");
    }
    lv_label_set_text(it->lbl_speed, spd);

    char vtxt[32];
    lv_snprintf(vtxt, sizeof(vtxt), "垂%s:%d", lvl_to_roman(r->v_level), r->acc_v_01);
    lv_label_set_text(it->lbl_v, vtxt);

    char htxt[32];
    lv_snprintf(htxt, sizeof(htxt), "水%s:%d", lvl_to_roman(r->h_level), r->acc_h_01);
    lv_label_set_text(it->lbl_h, htxt);
}

static void vlist_scroll_cb(lv_event_t *e)
{
    (void)e;
    vlist_update();
}

static void vlist_create(lv_ui *ui)
{
    vlist_t *vl = &g_vlist;

    vl->list = ui->screen_overlimt_cont_list;
    vl->item_h = VLIST_ITEM_H;
    vl->first_index = -1;
    vl->total_cnt = g_records_cnt;

    int step = vl->item_h + VLIST_GAP_PX;

    /* 统一在这里清旧 UI，对外不在 deinit 里 clean */
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
        ui_show_message_in_over_list(ui, "vlist alloc failed");
        return;
    }
    memset(vl->items, 0, sizeof(vlist_item_t) * (size_t)vl->visible_cnt);

    for(int i = 0; i < vl->visible_cnt; i++) {
        vlist_item_t *it = &vl->items[i];

        it->root = lv_obj_create(vl->list);
        lv_obj_set_size(it->root, ITEM_W, vl->item_h);

        lv_obj_set_style_radius(it->root, CARD_RADIUS, 0);
        lv_obj_set_style_bg_opa(it->root, LV_OPA_COVER, 0);
        lv_obj_set_style_bg_color(it->root, lv_color_hex(CARD_BG_HEX), 0);
        lv_obj_clear_flag(it->root, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_layout(it->root, LV_LAYOUT_NONE);
        lv_obj_set_style_pad_all(it->root, 0, 0);
        lv_obj_set_style_pad_row(it->root, 0, 0);
        lv_obj_set_style_pad_column(it->root, 0, 0);

        lv_obj_set_pos(it->root, ITEM_X, ITEM_Y0);

        it->lbl_date = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_date, X_DATE, Y_DATE);
        lv_obj_set_size(it->lbl_date, W_DATE, H_DATE);
        lv_obj_set_style_text_font(it->lbl_date, APP_LOG_FONT, 0);
        FIX_LBL(it->lbl_date);

        it->lbl_time = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_time, X_TIME, Y_TIME);
        lv_obj_set_size(it->lbl_time, W_TIME, H_TIME);
        lv_obj_set_style_text_font(it->lbl_time, APP_LOG_FONT, 0);
        FIX_LBL(it->lbl_time);

        it->lbl_mileage = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_mileage, X_MILEAGE, Y_MILEAGE);
        lv_obj_set_size(it->lbl_mileage, W_MILEAGE, H_MILEAGE);
        lv_obj_set_style_text_font(it->lbl_mileage, APP_LOG_FONT, 0);
        FIX_LBL(it->lbl_mileage);

        it->lbl_speed = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_speed, X_SPEED, Y_SPEED);
        lv_obj_set_size(it->lbl_speed, W_SPEED, H_SPEED);
        lv_obj_set_style_text_font(it->lbl_speed, APP_LOG_FONT, 0);
        FIX_LBL(it->lbl_speed);

        it->lbl_v = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_v, X_V, Y_V);
        lv_obj_set_size(it->lbl_v, W_V, H_V);
        lv_obj_set_style_text_font(it->lbl_v, APP_LOG_FONT, 0);
        lv_obj_set_style_text_color(it->lbl_v, lv_color_hex(RED_HEX), 0);
        FIX_LBL(it->lbl_v);

        it->lbl_h = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_h, X_H, Y_H);
        lv_obj_set_size(it->lbl_h, W_H, H_H);
        lv_obj_set_style_text_font(it->lbl_h, APP_LOG_FONT, 0);
        lv_obj_set_style_text_color(it->lbl_h, lv_color_hex(RED_HEX), 0);
        FIX_LBL(it->lbl_h);

        lv_obj_move_foreground(it->root);
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
void ui_overlimt_deinit(void)
{
    /* 只释放业务资源，不碰 LVGL 对象，避免切屏/事件中死机 */
    vlist_destroy();
    records_free_all();
    s_overlimt_list = NULL;
    /* s_cache_ok = false; */
}

void ui_overlimt_render_from_json(lv_ui *ui, const char *json_path)
{
    if(!ui || !ui->screen_overlimt_cont_list) return;

    s_overlimt_list = ui->screen_overlimt_cont_list;

    /* 这里只清业务状态，不主动 clean UI */
    vlist_destroy();

    if(!json_path || !json_path[0]) {
        ui_show_message_in_over_list(ui, "json_path is empty.");
        return;
    }

#if OVER_PRINT_TIMING
    int64_t t0 = TICK_MS();
#endif

    if(!overlimt_parse_all_records(json_path)) {
        ui_overlimt_deinit();
        ui_show_message_in_over_list(ui, "parse failed or file too large");
        return;
    }

    if(g_records_cnt <= 0) {
        ui_overlimt_deinit();
        ui_show_message_in_over_list(ui, "No overlimt records.");
        return;
    }

#if OVER_PRINT_TIMING
    int64_t t1 = TICK_MS();
#endif

    vlist_create(ui);

#if OVER_PRINT_TIMING
    int64_t t2 = TICK_MS();
#endif

    lv_obj_scroll_to_y(ui->screen_overlimt_cont_list, 0, LV_ANIM_OFF);
    vlist_update();

#if OVER_PRINT_TIMING
    int64_t t3 = TICK_MS();
    printf("[OVER] parse=%lldms, create=%lldms, update=%lldms, total=%lldms, cnt=%d\n",
           (long long)(t1 - t0),
           (long long)(t2 - t1),
           (long long)(t3 - t2),
           (long long)(t3 - t0),
           g_records_cnt);
#endif
}