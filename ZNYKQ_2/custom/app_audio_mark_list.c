#include "app_audio_mark_list.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "guider_customer_fonts.h"
#include "widgets_init.h"
// #include "audio_process.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cJSON.h"

/* ================= Font ================= */
#define AM_FONT   (&lv_customer_font_PingFangHeavy_16)

/* ================= JSON 限制 ================= */
#define AM_JSON_MAX_SIZE   (32 * 1024)
#define AM_MAX_RECORDS     (120)

/* ================= item 尺寸/布局 ================= */
#define ITEM_W        (220)
#define ITEM_H        (82)
#define ITEM_RADIUS   (10)
#define ITEM_X        (5)
#define ITEM_Y0       (6)
#define ITEM_GAP_Y    (8)

#define ITEM_BG_OPA   (LV_OPA_COVER)

/* ================= 颜色 ================= */
#define CARD_BG_HEX      (0xF4D9DE)
#define CARD_BG_PRS_HEX  (0xEFCBD2)

/* ================= 播放按钮区域 ================= */
#define PLAY_BTN_X    (135)
#define PLAY_BTN_Y    (26)
#define PLAY_BTN_W    (50)
#define PLAY_BTN_H    (50)

/* ================= vlist ================= */
#define VLIST_CACHE_EXTRA  2

/* ================= 弱符号：播放回调 ================= */
__attribute__((weak))
void audiomark_on_play(const char *file, const char *ts)
{
    (void)ts;
    (void)file;
    // audio_set_playback_filename(file);
    // audio_start_playback();
    /* 你在别处覆盖这个函数即可 */
}

/* ================= Data model ================= */
typedef struct {
    char ts[20];             /* 20260122094009 */
    char date[11];           /* 2026-01-22 */
    char time[9];            /* 09:40:09 */
    char file[64];           /* wav 文件名 */
    char mileage_text[32];   /* K1100+404 */
    char speed_kmh[16];      /* "123" */
} audiomark_record_t;

/* ================= RAM records + cache ================= */
static audiomark_record_t *g_am = NULL;
static int g_am_cnt = 0;

static bool     s_cache_ok = false;
static uint32_t s_cache_hash = 0;
static char     s_cache_path[128] = {0};

/* ================= UI list 指针（仅记录，不在 deinit 中清 UI） ================= */
static lv_obj_t *s_list = NULL;

/* ================= Virtual list structs ================= */
typedef struct {
    lv_obj_t *root;

    lv_obj_t *lb_date;
    lv_obj_t *lb_time;
    lv_obj_t *lb_mileage;
    lv_obj_t *lb_speed;

    lv_obj_t *img_play;
    lv_obj_t *icon;
} am_item_t;

typedef struct {
    lv_obj_t *list;
    lv_obj_t *spacer;

    int item_h;
    int step;
    int visible_cnt;
    int total_cnt;
    int first_index;

    am_item_t *items;
} am_vlist_t;

static am_vlist_t g_vl;

/* ================= Utilities ================= */
static void records_free_all(void)
{
    if(g_am) {
        lv_free(g_am);
        g_am = NULL;
    }
    g_am_cnt = 0;
}

static void audiomark_vlist_destroy_only(void)
{
    if(g_vl.items) {
        lv_free(g_vl.items);
        g_vl.items = NULL;
    }

    g_vl.list = NULL;
    g_vl.spacer = NULL;
    g_vl.first_index = -1;
    g_vl.total_cnt = 0;
    g_vl.visible_cnt = 0;
    g_vl.item_h = 0;
    g_vl.step = 0;
}

void audiomark_deinit(void)
{
    /* 只释放业务资源，不碰 LVGL 对象，避免切屏/事件中死机 */
    audiomark_vlist_destroy_only();
    records_free_all();

    s_list = NULL;

    /* 如需强制每次重读，可打开：
       s_cache_ok = false;
    */
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

/* ts: 20260122094009 -> 2026-01-22 / 09:40:09 */
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

/* 循环读直到 EOF，不依赖 seek/tell */
static char *read_text_file_lvfs_limit(const char *path, size_t *out_len)
{
    if(out_len) *out_len = 0;
    if(!path) return NULL;

    lv_fs_file_t f;
    lv_fs_res_t rr = lv_fs_open(&f, path, LV_FS_MODE_RD);
    if(rr != LV_FS_RES_OK) {
        printf("[AM] lv_fs_open failed (%d): %s\n", (int)rr, path);
        return NULL;
    }

    char *buf = (char *)lv_malloc(AM_JSON_MAX_SIZE + 1);
    if(!buf) {
        lv_fs_close(&f);
        return NULL;
    }

    size_t total = 0;
    while(total < AM_JSON_MAX_SIZE) {
        uint32_t br = 0;
        uint32_t want = (uint32_t)(AM_JSON_MAX_SIZE - total);

        rr = lv_fs_read(&f, buf + total, want, &br);
        if(rr != LV_FS_RES_OK) {
            printf("[AM] lv_fs_read failed (%d)\n", (int)rr);
            lv_free(buf);
            lv_fs_close(&f);
            return NULL;
        }

        if(br == 0) break;
        total += br;
    }

    lv_fs_close(&f);

    if(total == 0) {
        lv_free(buf);
        return NULL;
    }

    if(total >= AM_JSON_MAX_SIZE) {
        printf("[AM] file too large (>= %d bytes): %s\n", AM_JSON_MAX_SIZE, path);
        lv_free(buf);
        return NULL;
    }

    buf[total] = '\0';
    if(out_len) *out_len = total;
    return buf;
}

/* 显示提示文本 */
static void ui_show_message(lv_ui *ui, const char *msg)
{
    if(!ui || !ui->screen_audiomark_cont_list) return;

    lv_obj_t *parent = ui->screen_audiomark_cont_list;

    lv_obj_clean(parent);

    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(parent, 0, 0);

    lv_obj_t *lbl = lv_label_create(parent);
    lv_label_set_text(lbl, msg ? msg : "暂无标记数据");
    lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(lbl, &lv_font_PingFangHeavy_20, 0);
    lv_obj_center(lbl);
}

/* ================= Parse JSON -> g_am[] ================= */
/* 支持：
   1) {"items":[...]}
   2) {"records":[...]}
   3) 顶层数组 [...]
   字段：
   ts/file/mileage_text/speed_kmh
*/
static bool audiomark_parse_all(const char *json_path)
{
    if(!json_path) return false;

    size_t len = 0;
    char *txt = read_text_file_lvfs_limit(json_path, &len);
    if(!txt) return false;

    uint32_t h = fnv1a32((const uint8_t *)txt, len);
    if(s_cache_ok &&
       strcmp(s_cache_path, json_path) == 0 &&
       s_cache_hash == h &&
       g_am && g_am_cnt > 0)
    {
        lv_free(txt);
        return true;
    }

    cJSON *root = cJSON_Parse(txt);
    if(!root) {
        const char *ep = cJSON_GetErrorPtr();
        printf("[AM] cJSON_Parse failed at: %s\n", ep ? ep : "(null)");
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

    /* 只保留最后 AM_MAX_RECORDS 条 */
    int start = 0;
    int n = n_all;
    if(n > AM_MAX_RECORDS) {
        start = n_all - AM_MAX_RECORDS;
        n = AM_MAX_RECORDS;
    }

    records_free_all();
    g_am = (audiomark_record_t *)lv_malloc(sizeof(audiomark_record_t) * (size_t)n);
    if(!g_am) {
        cJSON_Delete(root);
        lv_free(txt);
        return false;
    }
    memset(g_am, 0, sizeof(audiomark_record_t) * (size_t)n);

    int out = 0;
    for(int i = start; i < start + n; i++) {
        cJSON *it = cJSON_GetArrayItem(arr, i);
        if(!cJSON_IsObject(it)) continue;

        cJSON *j_ts      = cJSON_GetObjectItem(it, "ts");
        cJSON *j_file    = cJSON_GetObjectItem(it, "file");
        cJSON *j_mileage = cJSON_GetObjectItem(it, "mileage_text");
        cJSON *j_speed   = cJSON_GetObjectItem(it, "speed_kmh");

        if(!cJSON_IsString(j_ts) ||
           !cJSON_IsString(j_file) ||
           !cJSON_IsString(j_mileage) ||
           !cJSON_IsString(j_speed))
        {
            continue;
        }

        audiomark_record_t *r = &g_am[out];
        memset(r, 0, sizeof(*r));

        strncpy(r->ts, j_ts->valuestring, sizeof(r->ts) - 1);
        r->ts[sizeof(r->ts) - 1] = '\0';

        parse_ts_14_to_date_time(r->ts,
                                 r->date, sizeof(r->date),
                                 r->time, sizeof(r->time));

        strncpy(r->file, j_file->valuestring, sizeof(r->file) - 1);
        r->file[sizeof(r->file) - 1] = '\0';

        strncpy(r->mileage_text, j_mileage->valuestring, sizeof(r->mileage_text) - 1);
        r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';

        strncpy(r->speed_kmh, j_speed->valuestring, sizeof(r->speed_kmh) - 1);
        r->speed_kmh[sizeof(r->speed_kmh) - 1] = '\0';

        if(r->ts[0] == '\0' || r->file[0] == '\0') continue;
        out++;
    }

    cJSON_Delete(root);
    lv_free(txt);

    g_am_cnt = out;
    if(g_am_cnt <= 0) {
        records_free_all();
    }

    s_cache_ok = true;
    s_cache_hash = h;
    strncpy(s_cache_path, json_path, sizeof(s_cache_path) - 1);
    s_cache_path[sizeof(s_cache_path) - 1] = '\0';

    return true;
}

static const audiomark_record_t *am_get(int idx)
{
    if(idx < 0 || idx >= g_am_cnt) return NULL;
    const audiomark_record_t *r = &g_am[idx];
    if(r->file[0] == '\0') return NULL;
    return r;
}

/* ================= List style fix ================= */
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

/* ================= Play click ================= */
static void on_play_btn_clicked(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    const audiomark_record_t *r = (const audiomark_record_t *)lv_obj_get_user_data(obj);
    if(!r) return;

    audiomark_on_play(r->file, r->ts);
}

/* ================= VList ================= */
static void vlist_update(void);

static void vlist_scroll_cb(lv_event_t *e)
{
    (void)e;
    vlist_update();
}

/* label 通用 */
#define FIX_LBL(_lb) do { \
    lv_label_set_long_mode((_lb), LV_LABEL_LONG_CLIP); \
    lv_obj_clear_flag((_lb), LV_OBJ_FLAG_SCROLLABLE); \
    lv_obj_set_style_pad_all((_lb), 0, 0); \
    lv_obj_set_style_bg_opa((_lb), LV_OPA_TRANSP, 0); \
} while(0)

static void vlist_create(lv_ui *ui)
{
    memset(&g_vl, 0, sizeof(g_vl));

    g_vl.list = ui->screen_audiomark_cont_list;
    g_vl.item_h = ITEM_H;
    g_vl.step = ITEM_H + ITEM_GAP_Y;
    g_vl.first_index = -1;
    g_vl.total_cnt = g_am_cnt;

    /* 统一在这里清旧 UI，对外不在 deinit 里 clean */
    lv_obj_clean(g_vl.list);
    fix_list_style_for_target(g_vl.list);

    lv_obj_add_flag(g_vl.list, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(g_vl.list, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(g_vl.list, LV_SCROLLBAR_MODE_OFF);

    g_vl.visible_cnt = 2 + VLIST_CACHE_EXTRA;
    if(g_vl.visible_cnt < 3) g_vl.visible_cnt = 3;
    if(g_vl.visible_cnt > g_vl.total_cnt) g_vl.visible_cnt = g_vl.total_cnt;
    if(g_vl.visible_cnt <= 0) g_vl.visible_cnt = 1;

    g_vl.spacer = lv_obj_create(g_vl.list);
    lv_obj_remove_style_all(g_vl.spacer);
    lv_obj_clear_flag(g_vl.spacer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(g_vl.spacer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(g_vl.spacer, 1, (lv_coord_t)(g_vl.total_cnt * g_vl.step));
    lv_obj_set_pos(g_vl.spacer, 0, 0);

    g_vl.items = (am_item_t *)lv_malloc(sizeof(am_item_t) * (size_t)g_vl.visible_cnt);
    if(!g_vl.items) {
        ui_show_message(ui, "audiomark vlist alloc failed");
        return;
    }
    memset(g_vl.items, 0, sizeof(am_item_t) * (size_t)g_vl.visible_cnt);

    for(int i = 0; i < g_vl.visible_cnt; i++) {
        am_item_t *it = &g_vl.items[i];

        it->root = lv_obj_create(g_vl.list);
        lv_obj_set_size(it->root, ITEM_W, ITEM_H);

        lv_obj_set_style_radius(it->root, ITEM_RADIUS, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(it->root, ITEM_BG_OPA, LV_PART_MAIN);
        lv_obj_set_style_bg_color(it->root, lv_color_hex(CARD_BG_HEX), LV_PART_MAIN);

        lv_obj_set_style_border_width(it->root, 0, LV_PART_MAIN);
        lv_obj_set_style_outline_width(it->root, 0, LV_PART_MAIN);
        lv_obj_set_style_shadow_width(it->root, 0, LV_PART_MAIN);

        lv_obj_clear_flag(it->root, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_layout(it->root, LV_LAYOUT_NONE);
        lv_obj_set_style_pad_all(it->root, 0, LV_PART_MAIN);

        it->lb_date = lv_label_create(it->root);
        lv_obj_set_pos(it->lb_date, 18, 7);
        lv_obj_set_size(it->lb_date, 95, 21);
        lv_obj_set_style_text_font(it->lb_date, AM_FONT, 0);
        FIX_LBL(it->lb_date);

        it->lb_time = lv_label_create(it->root);
        lv_obj_set_pos(it->lb_time, 126, 7);
        lv_obj_set_size(it->lb_time, 64, 17);
        lv_obj_set_style_text_font(it->lb_time, AM_FONT, 0);
        FIX_LBL(it->lb_time);

        it->lb_mileage = lv_label_create(it->root);
        lv_obj_set_pos(it->lb_mileage, 18, 31);
        lv_obj_set_size(it->lb_mileage, 100, 17);
        lv_obj_set_style_text_font(it->lb_mileage, AM_FONT, 0);
        FIX_LBL(it->lb_mileage);

        it->lb_speed = lv_label_create(it->root);
        lv_obj_set_pos(it->lb_speed, 18, 55);
        lv_obj_set_size(it->lb_speed, 89, 18);
        lv_obj_set_style_text_font(it->lb_speed, AM_FONT, 0);
        FIX_LBL(it->lb_speed);

        it->img_play = lv_img_create(it->root);
        lv_img_set_src(it->img_play, &_voc_RGB565A8_50x50);
        lv_obj_set_pos(it->img_play, PLAY_BTN_X, PLAY_BTN_Y);
        lv_obj_set_size(it->img_play, PLAY_BTN_W, PLAY_BTN_H);

        lv_obj_add_flag(it->img_play, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(it->img_play, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_pad_all(it->img_play, 0, 0);
        lv_obj_set_style_border_width(it->img_play, 0, 0);
        lv_obj_set_style_outline_width(it->img_play, 0, 0);
        lv_obj_set_style_shadow_width(it->img_play, 0, 0);

        lv_obj_add_event_cb(it->img_play, on_play_btn_clicked, LV_EVENT_CLICKED, NULL);

        lv_obj_move_foreground(it->root);
    }

    lv_obj_add_event_cb(g_vl.list, vlist_scroll_cb, LV_EVENT_SCROLL, NULL);
}

static void vlist_bind_item(am_item_t *it, int rec_index)
{
    if(!it || !it->root) return;

    if(rec_index < 0 || rec_index >= g_am_cnt) {
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    const audiomark_record_t *r = am_get(rec_index);
    if(!r) {
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_clear_flag(it->root, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(it->lb_date, r->date[0] ? r->date : "--");
    lv_label_set_text(it->lb_time, r->time[0] ? r->time : "--:--:--");
    lv_label_set_text(it->lb_mileage, r->mileage_text[0] ? r->mileage_text : "--");

    char sp[24];
    if(r->speed_kmh[0]) {
        lv_snprintf(sp, sizeof(sp), "%s km/h", r->speed_kmh);
    } else {
        lv_snprintf(sp, sizeof(sp), "--");
    }
    lv_label_set_text(it->lb_speed, sp);

    lv_obj_set_user_data(it->img_play, (void *)r);
}

static void vlist_update(void)
{
    if(!g_vl.list || !g_vl.items || g_am_cnt <= 0) return;

    lv_coord_t scroll_y = lv_obj_get_scroll_top(g_vl.list);
    int first = (int)(scroll_y / g_vl.step);
    if(first < 0) first = 0;

    int max_first = g_am_cnt - g_vl.visible_cnt;
    if(max_first < 0) max_first = 0;
    if(first > max_first) first = max_first;

    if(first == g_vl.first_index) return;
    g_vl.first_index = first;

    for(int i = 0; i < g_vl.visible_cnt; i++) {
        int disp_index = first + i;
        int rec_index = (g_am_cnt - 1) - disp_index;   /* 最新在上 */

        vlist_bind_item(&g_vl.items[i], rec_index);

        lv_obj_set_pos(g_vl.items[i].root,
                       ITEM_X,
                       (lv_coord_t)(ITEM_Y0 + disp_index * g_vl.step));
    }
}

/* ================= Public API ================= */
bool ui_audiomark_render_from_json(lv_ui *ui, const char *path)
{
    if(!ui || !path) return false;
    if(!ui->screen_audiomark_cont_list) return false;

    s_list = ui->screen_audiomark_cont_list;

    /* 这里只清业务状态，不主动 clean UI */
    audiomark_vlist_destroy_only();

    if(!audiomark_parse_all(path)) {
        records_free_all();
        ui_show_message(ui, "audiomark parse failed or file >=32KB");
        return false;
    }

    if(g_am_cnt <= 0) {
        audiomark_vlist_destroy_only();
        ui_show_message(ui, "No audio marks.");
        return true;
    }

    vlist_create(ui);
    lv_obj_scroll_to_y(ui->screen_audiomark_cont_list, 0, LV_ANIM_OFF);
    vlist_update();

    return true;
}