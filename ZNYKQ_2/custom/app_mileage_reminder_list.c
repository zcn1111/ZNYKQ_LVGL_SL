#include "app_mileage_reminder_list.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "guider_customer_fonts.h"
#include "cJSON.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern lv_ui guider_ui;

#ifndef MILEAGE_REMINDER_JSON_PATH
#define MILEAGE_REMINDER_JSON_PATH "S:/CB05U01TEST/mileage_reminders.json"
#endif

#define MR_JSON_MAX_SIZE      (16 * 1024)
#define MR_MAX_ITEMS          100

#define MR_ROW_X              7
#define MR_ROW_Y0             4
#define MR_ROW_W              216
#define MR_ROW_H              29
#define MR_ROW_GAP            7
#define MR_ROW_RADIUS         3

#define MR_MILEAGE_X          8
#define MR_MILEAGE_Y          3
#define MR_MILEAGE_W          128
#define MR_MILEAGE_H          22

#define MR_STATUS_X           151
#define MR_STATUS_Y           5
#define MR_STATUS_W           58
#define MR_STATUS_H           18

#define MR_ROW_BG_HEX         0x7661D8
#define MR_TXT_HEX            0xFFFFFF
#define MR_STATUS_HEX         0xFFF32B
#define MR_EMPTY_HEX          0xFFFFFF

#define MR_FONT               (&lv_customer_font_PingFangHeavy_16)
#define MR_TITLE_FONT         (&lv_customer_font_PingFangHeavy_20)

typedef struct {
    char mileage_text[32];
    bool reminded;
} mileage_reminder_item_t;

static char *read_text_file_lvfs_limit(const char *path, size_t *out_len)
{
    if(out_len) *out_len = 0;
    if(!path) return NULL;

    lv_fs_file_t f;
    lv_fs_res_t rr = lv_fs_open(&f, path, LV_FS_MODE_RD);
    if(rr != LV_FS_RES_OK) {
        printf("[MILEAGE] lv_fs_open failed (%d): %s\n", (int)rr, path);
        return NULL;
    }

    char *buf = (char *)lv_malloc(MR_JSON_MAX_SIZE + 1);
    if(!buf) {
        lv_fs_close(&f);
        return NULL;
    }

    size_t total = 0;
    while(total < MR_JSON_MAX_SIZE) {
        uint32_t br = 0;
        uint32_t want = (uint32_t)(MR_JSON_MAX_SIZE - total);

        rr = lv_fs_read(&f, buf + total, want, &br);
        if(rr != LV_FS_RES_OK) {
            printf("[MILEAGE] lv_fs_read failed (%d)\n", (int)rr);
            lv_free(buf);
            lv_fs_close(&f);
            return NULL;
        }

        if(br == 0) break;
        total += br;
    }

    lv_fs_close(&f);

    if(total == 0 || total >= MR_JSON_MAX_SIZE) {
        lv_free(buf);
        return NULL;
    }

    buf[total] = '\0';
    if(out_len) *out_len = total;
    return buf;
}

static cJSON *json_get_items_array(cJSON *root)
{
    if(cJSON_IsArray(root)) return root;
    if(!cJSON_IsObject(root)) return NULL;

    static const char *keys[] = {
        "items",
        "records",
        "data",
        "list",
        "rows",
        "reminders",
        "mileage_reminders",
        "mileageReminders",
        "mileage_list",
        "mileageList"
    };

    for(size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        cJSON *arr = cJSON_GetObjectItem(root, keys[i]);
        if(cJSON_IsArray(arr)) return arr;
    }

    cJSON *child = NULL;
    cJSON_ArrayForEach(child, root) {
        if(cJSON_IsArray(child)) return child;
    }

    return NULL;
}

static cJSON *json_get_first(cJSON *obj, const char *k1, const char *k2, const char *k3)
{
    cJSON *j = obj ? cJSON_GetObjectItem(obj, k1) : NULL;
    if(j) return j;
    j = obj ? cJSON_GetObjectItem(obj, k2) : NULL;
    if(j) return j;
    return obj ? cJSON_GetObjectItem(obj, k3) : NULL;
}

static bool json_value_is_reminded(cJSON *j)
{
    if(cJSON_IsBool(j)) return cJSON_IsTrue(j);
    if(cJSON_IsNumber(j)) return j->valueint != 0;

    if(cJSON_IsString(j) && j->valuestring) {
        const char *s = j->valuestring;
        return strcmp(s, "1") == 0 ||
               strcmp(s, "true") == 0 ||
               strcmp(s, "TRUE") == 0 ||
               strcmp(s, "yes") == 0 ||
               strcmp(s, "YES") == 0 ||
               strcmp(s, "Y") == 0 ||
               strcmp(s, "on") == 0 ||
               strcmp(s, "reminded") == 0 ||
               strcmp(s, "done") == 0 ||
               strcmp(s, "\xE5\xB7\xB2\xE6\x8F\x90\xE9\x86\x92") == 0;
    }

    return false;
}

static int parse_mileage_items(const char *json_path,
                               mileage_reminder_item_t *items,
                               int max_items)
{
    if(!items || max_items <= 0) return 0;

    size_t len = 0;
    char *txt = read_text_file_lvfs_limit(json_path, &len);
    if(!txt) return 0;

    cJSON *root = cJSON_Parse(txt);
    lv_free(txt);
    if(!root) {
        printf("[MILEAGE] cJSON_Parse failed: %s\n", json_path);
        return 0;
    }

    cJSON *arr = json_get_items_array(root);
    if(!cJSON_IsArray(arr)) {
        printf("[MILEAGE] no items array: %s\n", json_path);
        cJSON_Delete(root);
        return 0;
    }

    int out = 0;
    int n = cJSON_GetArraySize(arr);
    for(int i = 0; i < n && out < max_items; i++) {
        cJSON *it = cJSON_GetArrayItem(arr, i);
        mileage_reminder_item_t *r = &items[out];
        memset(r, 0, sizeof(*r));

        if(cJSON_IsString(it) && it->valuestring && it->valuestring[0] != '\0') {
            strncpy(r->mileage_text, it->valuestring, sizeof(r->mileage_text) - 1);
            r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';
            out++;
            continue;
        }

        if(!cJSON_IsObject(it)) continue;

        cJSON *j_mileage = json_get_first(it, "mileage_text", "mileage", "text");
        if(!j_mileage) j_mileage = json_get_first(it, "mileageText", "mileage_value", "name");
        if(!j_mileage) j_mileage = json_get_first(it, "mileage_no", "mileageNo", "mileage_num");
        if(!j_mileage) j_mileage = json_get_first(it, "line_mileage", "km_text", "kilometer");
        if(!j_mileage) j_mileage = json_get_first(it, "k", "km", "value");
        if(!cJSON_IsString(j_mileage) || !j_mileage->valuestring || j_mileage->valuestring[0] == '\0') {
            continue;
        }

        strncpy(r->mileage_text, j_mileage->valuestring, sizeof(r->mileage_text) - 1);
        r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';

        cJSON *j_reminded = json_get_first(it, "reminded", "is_reminded", "done");
        if(!j_reminded) j_reminded = json_get_first(it, "isReminded", "remind", "warned");
        if(!j_reminded) j_reminded = json_get_first(it, "is_remind", "isRemind", "has_reminded");
        if(!j_reminded) j_reminded = cJSON_GetObjectItem(it, "status");
        r->reminded = json_value_is_reminded(j_reminded);
        out++;
    }

    printf("[MILEAGE] parsed %d items from %s\n", out, json_path);
    cJSON_Delete(root);
    return out;
}

static void fix_list_style(lv_obj_t *list)
{
    lv_obj_set_layout(list, LV_LAYOUT_NONE);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_outline_width(list, 0, 0);
    lv_obj_set_style_shadow_width(list, 0, 0);
    lv_obj_set_style_bg_opa(list, 0, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_ELASTIC);
}

static void set_label_style(lv_obj_t *lbl, lv_color_t color, lv_text_align_t align)
{
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_CLIP);
    lv_obj_clear_flag(lbl, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(lbl, 0, 0);
    lv_obj_set_style_radius(lbl, 0, 0);
    lv_obj_set_style_bg_opa(lbl, 0, 0);
    lv_obj_set_style_shadow_width(lbl, 0, 0);
    lv_obj_set_style_pad_all(lbl, 0, 0);
    lv_obj_set_style_text_color(lbl, color, 0);
    lv_obj_set_style_text_font(lbl, MR_FONT, 0);
    lv_obj_set_style_text_opa(lbl, 255, 0);
    lv_obj_set_style_text_letter_space(lbl, 0, 0);
    lv_obj_set_style_text_line_space(lbl, 0, 0);
    lv_obj_set_style_text_align(lbl, align, 0);
}

static void show_empty(lv_ui *ui)
{
    lv_obj_t *list = ui->screen_mileage_reminder_cont_list;
    lv_obj_clean(list);
    fix_list_style(list);

    lv_obj_t *lbl = lv_label_create(list);
    lv_obj_set_pos(lbl, 0, 82);
    lv_obj_set_size(lbl, lv_obj_get_width(list), 30);
    lv_label_set_text(lbl, "暂无数据");
    set_label_style(lbl, lv_color_hex(MR_EMPTY_HEX), LV_TEXT_ALIGN_CENTER);
}

static void create_row(lv_obj_t *list, const mileage_reminder_item_t *item, int row_index)
{
    lv_coord_t y = (lv_coord_t)(MR_ROW_Y0 + row_index * (MR_ROW_H + MR_ROW_GAP));

    lv_obj_t *row = lv_obj_create(list);
    lv_obj_set_pos(row, MR_ROW_X, y);
    lv_obj_set_size(row, MR_ROW_W, MR_ROW_H);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(row, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(row, MR_ROW_RADIUS, 0);
    lv_obj_set_style_bg_opa(row, 255, 0);
    lv_obj_set_style_bg_color(row, lv_color_hex(MR_ROW_BG_HEX), 0);
    lv_obj_set_style_bg_grad_dir(row, LV_GRAD_DIR_NONE, 0);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_outline_width(row, 0, 0);
    lv_obj_set_style_shadow_width(row, 0, 0);
    lv_obj_set_style_pad_all(row, 0, 0);

    lv_obj_t *mileage = lv_label_create(row);
    lv_obj_set_pos(mileage, MR_MILEAGE_X, MR_MILEAGE_Y);
    lv_obj_set_size(mileage, MR_MILEAGE_W, MR_MILEAGE_H);
    lv_label_set_text(mileage, item->mileage_text);
    set_label_style(mileage, lv_color_hex(MR_TXT_HEX), LV_TEXT_ALIGN_LEFT);

    if(item->reminded) {
        lv_obj_t *status = lv_label_create(row);
        lv_obj_set_pos(status, MR_STATUS_X, MR_STATUS_Y);
        lv_obj_set_size(status, MR_STATUS_W, MR_STATUS_H);
        lv_label_set_text(status, "已提醒");
        set_label_style(status, lv_color_hex(MR_STATUS_HEX), LV_TEXT_ALIGN_RIGHT);
    }
}

void ui_mileage_reminder_render_from_json(lv_ui *ui, const char *json_path)
{
    if(!ui || !ui->screen_mileage_reminder_cont_list) return;

    if(ui->screen_mileage_reminder_label_3) {
        lv_label_set_text(ui->screen_mileage_reminder_label_3, "提醒里程");
        lv_obj_set_style_text_font(ui->screen_mileage_reminder_label_3, MR_TITLE_FONT, 0);
    }

    mileage_reminder_item_t items[MR_MAX_ITEMS];
    memset(items, 0, sizeof(items));
    int count = parse_mileage_items(json_path, items, MR_MAX_ITEMS);
    if(count <= 0) {
        show_empty(ui);
        return;
    }

    lv_obj_t *list = ui->screen_mileage_reminder_cont_list;
    lv_obj_clean(list);
    fix_list_style(list);

    lv_obj_t *spacer = lv_obj_create(list);
    lv_obj_remove_style_all(spacer);
    lv_obj_clear_flag(spacer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(spacer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(spacer, 1, (lv_coord_t)(MR_ROW_Y0 + count * (MR_ROW_H + MR_ROW_GAP)));
    lv_obj_set_pos(spacer, 0, 0);

    for(int i = 0; i < count; i++) {
        create_row(list, &items[i], i);
    }

    lv_obj_scroll_to_y(list, 0, LV_ANIM_OFF);
}


void ui_mileage_reminder_deinit(lv_ui *ui)
{
    if(!ui || !ui->screen_mileage_reminder_cont_list) return;
    lv_obj_clean(ui->screen_mileage_reminder_cont_list);
}

void ui_mileage_reminder_render_default(lv_ui *ui)
{
    ui_mileage_reminder_render_from_json(ui, MILEAGE_REMINDER_JSON_PATH);
}

void screen_mileage_reminder_custom_code(lv_event_t *e)
{
    (void)e;
    ui_mileage_reminder_render_default(&guider_ui);
}

void screen_mileage_reminder_deinit_custom_code(lv_event_t *e)
{
    (void)e;
    ui_mileage_reminder_deinit(&guider_ui);
}
