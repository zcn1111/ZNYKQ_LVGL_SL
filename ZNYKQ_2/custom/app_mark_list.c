#include "app_mark_list.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "guider_customer_fonts.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "cJSON.h"

/* ================= Font ================= */
#ifndef APP_MARK_FONT
#define APP_MARK_FONT (&lv_customer_font_PingFangHeavy_16)
#endif

/* ================= JSON 限制 ================= */
#define MARK_JSON_MAX_SIZE   (64 * 1024)
#define MARK_MAX_RECORDS     (300)

/* ================= VList config ================= */
#define VLIST_ITEM_H        84
#define VLIST_GAP_PX        7
#define VLIST_CACHE_EXTRA   2

/* ================= Card style（与超限/添乘一致） ================= */
#define CARD_RADIUS         10
#define CARD_BG_HEX         0xF4D9DE
#define CARD_BG_OPA         LV_OPA_COVER

#define TXT_TITLE_HEX       0x1F1F1F
#define TXT_SUB_HEX         0x404040
#define TXT_EMP_HEX         0xC00000

/* ================= Item layout ================= */
#define ITEM_X              5
#define ITEM_Y0             7
#define ITEM_W              219

#define X_DATE              12
#define Y_DATE              8
#define W_DATE              96
#define H_DATE              17

#define X_TIME              130
#define Y_TIME              8
#define W_TIME              74
#define H_TIME              17

#define X_TYPE              12
#define Y_TYPE              31
#define W_TYPE              190
#define H_TYPE              17

#define X_MILEAGE           12
#define Y_MILEAGE           54
#define W_MILEAGE           118
#define H_MILEAGE           17

#define X_SPEED             132
#define Y_SPEED             54
#define W_SPEED             72
#define H_SPEED             17

/* ================= Data model ================= */
typedef struct {
    char ts[20];
    char date[11];        /* YYYY-MM-DD */
    char time[9];         /* HH:MM:SS */
    char mark_type[32];
    char mileage_text[32];
    char speed_kmh[16];
} mark_record_t;

/* ================= 全局数据 ================= */
static mark_record_t *g_records = NULL;
static int g_records_cnt = 0;

static bool     s_cache_ok = false;
static uint32_t s_cache_hash = 0;
static char     s_cache_path[128] = {0};

static lv_obj_t *s_mark_list = NULL;

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
        printf("[MARK] lv_fs_open failed (%d): %s\n", (int)rr, path);
        return NULL;
    }

    char *buf = (char *)lv_malloc(MARK_JSON_MAX_SIZE + 1);
    if(!buf) {
        lv_fs_close(&f);
        return NULL;
    }

    size_t total = 0;
    while(total < MARK_JSON_MAX_SIZE) {
        uint32_t br = 0;
        uint32_t want = (uint32_t)(MARK_JSON_MAX_SIZE - total);

        rr = lv_fs_read(&f, buf + total, want, &br);
        if(rr != LV_FS_RES_OK) {
            printf("[MARK] lv_fs_read failed (%d)\n", (int)rr);
            lv_free(buf);
            lv_fs_close(&f);
            return NULL;
        }

        if(br == 0) break;
        total += br;
    }

    lv_fs_close(&f);

    if(total == 0) {
        printf("[MARK] file empty: %s\n", path);
        lv_free(buf);
        return NULL;
    }

    if(total >= MARK_JSON_MAX_SIZE) {
        printf("[MARK] file too large (>= %d bytes): %s\n", MARK_JSON_MAX_SIZE, path);
        lv_free(buf);
        return NULL;
    }

    buf[total] = '\0';
    if(out_len) *out_len = total;
    return buf;
}

/* ts: 20260122095002 -> 2026-01-22 / 09:50:02 */
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

static bool mark_parse_all_records(const char *json_path)
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
        printf("[MARK] cJSON_Parse failed at: %s\n", ep ? ep : "(null)");
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
    if(n > MARK_MAX_RECORDS) {
        start = n_all - MARK_MAX_RECORDS;
        n = MARK_MAX_RECORDS;
    }

    records_free_all();
    g_records = (mark_record_t *)lv_malloc(sizeof(mark_record_t) * (size_t)n);
    if(!g_records) {
        cJSON_Delete(root);
        lv_free(txt);
        return false;
    }
    memset(g_records, 0, sizeof(mark_record_t) * (size_t)n);

    int out = 0;
    for(int i = start; i < start + n; i++) {
        cJSON *it = cJSON_GetArrayItem(arr, i);
        if(!cJSON_IsObject(it)) continue;

        cJSON *j_ts           = cJSON_GetObjectItem(it, "ts");
        cJSON *j_mark_type    = cJSON_GetObjectItem(it, "mark_type");
        cJSON *j_mileage_text = cJSON_GetObjectItem(it, "mileage_text");
        cJSON *j_speed_kmh    = cJSON_GetObjectItem(it, "speed_kmh");

        if(!cJSON_IsString(j_ts)) {
            continue;
        }

        mark_record_t *r = &g_records[out];
        memset(r, 0, sizeof(*r));

        strncpy(r->ts, j_ts->valuestring, sizeof(r->ts) - 1);
        r->ts[sizeof(r->ts) - 1] = '\0';

        parse_ts_14_to_date_time(r->ts, r->date, sizeof(r->date), r->time, sizeof(r->time));

        if(cJSON_IsString(j_mark_type)) {
            strncpy(r->mark_type, j_mark_type->valuestring, sizeof(r->mark_type) - 1);
            r->mark_type[sizeof(r->mark_type) - 1] = '\0';
        }
        if(cJSON_IsString(j_mileage_text)) {
            strncpy(r->mileage_text, j_mileage_text->valuestring, sizeof(r->mileage_text) - 1);
            r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';
        }
        if(cJSON_IsString(j_speed_kmh)) {
            strncpy(r->speed_kmh, j_speed_kmh->valuestring, sizeof(r->speed_kmh) - 1);
            r->speed_kmh[sizeof(r->speed_kmh) - 1] = '\0';
        }

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

    return true;
}

static const mark_record_t *records_get(int idx)
{
    if(idx < 0 || idx >= g_records_cnt) return NULL;
    return &g_records[idx];
}

/* ================= Virtual list ================= */
typedef struct {
    lv_obj_t *root;
    lv_obj_t *lbl_date;
    lv_obj_t *lbl_time;
    lv_obj_t *lbl_type;
    lv_obj_t *lbl_mileage;
    lv_obj_t *lbl_speed;
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

static void set_label_common_style(lv_obj_t *lbl, lv_color_t color)
{
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_CLIP);
    lv_obj_clear_flag(lbl, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_border_width(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, APP_MARK_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(lbl, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void vlist_bind_item(vlist_item_t *it, int rec_index)
{
    if(!it || !it->root) return;

    if(rec_index < 0 || rec_index >= g_records_cnt) {
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    const mark_record_t *r = records_get(rec_index);
    if(!r) {
        lv_obj_add_flag(it->root, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_clear_flag(it->root, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(it->lbl_date, r->date[0] ? r->date : "--");
    lv_label_set_text(it->lbl_time, r->time[0] ? r->time : "--:--:--");
    lv_label_set_text(it->lbl_type, r->mark_type[0] ? r->mark_type : "--");
    lv_label_set_text(it->lbl_mileage, r->mileage_text[0] ? r->mileage_text : "--");

    char speed_buf[24];
    if(r->speed_kmh[0]) {
        lv_snprintf(speed_buf, sizeof(speed_buf), "%skm/h", r->speed_kmh);
    } else {
        lv_snprintf(speed_buf, sizeof(speed_buf), "--");
    }
    lv_label_set_text(it->lbl_speed, speed_buf);
}

static void vlist_scroll_cb(lv_event_t *e)
{
    (void)e;
    vlist_update();
}

static void vlist_create(lv_ui *ui)
{
    vlist_t *vl = &g_vlist;

    vl->list = ui->screen_mark_cont_list;
    vl->item_h = VLIST_ITEM_H;
    vl->first_index = -1;
    vl->total_cnt = g_records_cnt;

    int step = vl->item_h + VLIST_GAP_PX;

    /* 统一在这里清旧 UI */
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
        printf("[MARK] vlist items alloc failed\n");
        return;
    }
    memset(vl->items, 0, sizeof(vlist_item_t) * (size_t)vl->visible_cnt);

    for(int i = 0; i < vl->visible_cnt; i++) {
        vlist_item_t *it = &vl->items[i];

        it->root = lv_obj_create(vl->list);
        lv_obj_set_size(it->root, ITEM_W, vl->item_h);
        lv_obj_set_pos(it->root, ITEM_X, ITEM_Y0);
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

        it->lbl_date = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_date, X_DATE, Y_DATE);
        lv_obj_set_size(it->lbl_date, W_DATE, H_DATE);
        set_label_common_style(it->lbl_date, lv_color_hex(TXT_TITLE_HEX));

        it->lbl_time = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_time, X_TIME, Y_TIME);
        lv_obj_set_size(it->lbl_time, W_TIME, H_TIME);
        set_label_common_style(it->lbl_time, lv_color_hex(TXT_TITLE_HEX));

        it->lbl_type = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_type, X_TYPE, Y_TYPE);
        lv_obj_set_size(it->lbl_type, W_TYPE, H_TYPE);
        set_label_common_style(it->lbl_type, lv_color_hex(TXT_EMP_HEX));

        it->lbl_mileage = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_mileage, X_MILEAGE, Y_MILEAGE);
        lv_obj_set_size(it->lbl_mileage, W_MILEAGE, H_MILEAGE);
        set_label_common_style(it->lbl_mileage, lv_color_hex(TXT_TITLE_HEX));

        it->lbl_speed = lv_label_create(it->root);
        lv_obj_set_pos(it->lbl_speed, X_SPEED, Y_SPEED);
        lv_obj_set_size(it->lbl_speed, W_SPEED, H_SPEED);
        set_label_common_style(it->lbl_speed, lv_color_hex(TXT_SUB_HEX));

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
void ui_mark_deinit(void)
{
    /* 只释放业务资源，不碰 LVGL 对象 */
    vlist_destroy();
    records_free_all();
    s_mark_list = NULL;
}

void ui_mark_render_from_json(lv_ui *ui, const char *json_path)
{
    if(!ui || !ui->screen_mark_cont_list) return;

    s_mark_list = ui->screen_mark_cont_list;

    if(ui->screen_mark_label_14) {
        lv_label_set_text(ui->screen_mark_label_14, "暂无普通标记");
        lv_obj_add_flag(ui->screen_mark_label_14, LV_OBJ_FLAG_HIDDEN);
    }

    /* 这里只清业务状态，不主动 clean UI */
    vlist_destroy();

    if(!json_path || !json_path[0]) {
        if(ui->screen_mark_label_14) {
            lv_obj_clear_flag(ui->screen_mark_label_14, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    if(!mark_parse_all_records(json_path)) {
        records_free_all();
        if(ui->screen_mark_label_14) {
            lv_obj_clear_flag(ui->screen_mark_label_14, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    if(g_records_cnt <= 0) {
        records_free_all();
        if(ui->screen_mark_label_14) {
            lv_obj_clear_flag(ui->screen_mark_label_14, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    vlist_create(ui);
    lv_obj_scroll_to_y(ui->screen_mark_cont_list, 0, LV_ANIM_OFF);
    vlist_update();
}