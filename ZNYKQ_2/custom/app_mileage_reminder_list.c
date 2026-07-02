#include "app_mileage_reminder_list.h"

#include "lvgl.h"
#include "gui_guider.h"
#include "guider_customer_fonts.h"
#include "cJSON.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if LV_USE_GUIDER_SIMULATOR
#if defined(_WIN32)
#include <direct.h>
#define MR_GETCWD _getcwd
#else
#include <unistd.h>
#define MR_GETCWD getcwd
#endif
#endif

extern lv_ui guider_ui;

#ifndef QUERY_ROOT_DIR
#define QUERY_ROOT_DIR "S:/CB05U01TEST"
#endif

#ifndef MILEAGE_REMINDER_JSON_NAME
#define MILEAGE_REMINDER_JSON_NAME "mileage_reminders.json"
#endif

#ifndef MILEAGE_REMINDER_JSON_PATH
#define MILEAGE_REMINDER_JSON_PATH QUERY_ROOT_DIR "/" MILEAGE_REMINDER_JSON_NAME
#endif

#define MILEAGE_BOARDING_PROBE_JSON_PATH QUERY_ROOT_DIR "/boarding_records.json"

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
#define MR_DEBUG_ITEM_LOG_LIMIT 20

typedef struct {
    char mileage_text[32];
    bool reminded;
} mileage_reminder_item_t;

static const char *json_type_name(const cJSON *j)
{
    if(!j) return "null";
    if(cJSON_IsObject(j)) return "object";
    if(cJSON_IsArray(j)) return "array";
    if(cJSON_IsString(j)) return "string";
    if(cJSON_IsNumber(j)) return "number";
    if(cJSON_IsBool(j)) return "bool";
    if(cJSON_IsNull(j)) return "null";
    return "unknown";
}

static void debug_print_object_keys(const char *prefix, cJSON *obj)
{
    if(!cJSON_IsObject(obj)) return;

    printf("[MILEAGE] %s keys:", prefix);
    int printed = 0;
    cJSON *child = NULL;
    cJSON_ArrayForEach(child, obj) {
        printf(" %s(%s)", child->string ? child->string : "?", json_type_name(child));
        printed++;
        if(printed >= 16) {
            printf(" ...");
            break;
        }
    }
    printf("\n");
}

static const char *lv_path_without_drive(const char *path)
{
    if(path && path[0] != '\0' && path[1] == ':') return path + 2;
    return path ? path : "";
}

static void debug_print_fs_mapping(void)
{
    static bool printed = false;
    if(printed) return;
    printed = true;

    const char *root_no_drive = lv_path_without_drive(QUERY_ROOT_DIR);
    printf("[MILEAGE] lv root dir=%s\n", QUERY_ROOT_DIR);
    printf("[MILEAGE] lv json path=%s\n", MILEAGE_REMINDER_JSON_PATH);

#ifdef LV_FS_POSIX_PATH
    printf("[MILEAGE] LV_FS_POSIX_PATH=%s\n", LV_FS_POSIX_PATH);
    printf("[MILEAGE] actual dir candidate POSIX=%s%s\n", LV_FS_POSIX_PATH, root_no_drive);
#endif

#ifdef LV_FS_STDIO_PATH
    printf("[MILEAGE] LV_FS_STDIO_PATH=%s\n", LV_FS_STDIO_PATH);
    printf("[MILEAGE] actual dir candidate STDIO=%s%s\n", LV_FS_STDIO_PATH, root_no_drive);
#endif

#if LV_USE_GUIDER_SIMULATOR
    char cwd[512] = {0};
    if(MR_GETCWD(cwd, sizeof(cwd))) {
        printf("[MILEAGE] process cwd=%s\n", cwd);
        printf("[MILEAGE] actual dir candidate CWD=%s%s\n", cwd, root_no_drive);
    } else {
        printf("[MILEAGE] process cwd unavailable\n");
    }
#endif
}

static void debug_probe_lvfs_path(const char *label, const char *path)
{
    if(!path || path[0] == '\0') return;

    lv_fs_file_t f;
    lv_fs_res_t rr = lv_fs_open(&f, path, LV_FS_MODE_RD);
    if(rr == LV_FS_RES_OK) {
        printf("[MILEAGE] probe ok: %s=%s\n", label ? label : "path", path);
        lv_fs_close(&f);
    } else {
        printf("[MILEAGE] probe failed (%d): %s=%s\n", (int)rr, label ? label : "path", path);
    }
}

static void debug_list_lvfs_dir(const char *path)
{
    if(!path || path[0] == '\0') return;

    lv_fs_dir_t dir;
    lv_fs_res_t rr = lv_fs_dir_open(&dir, path);
    if(rr != LV_FS_RES_OK) {
        printf("[MILEAGE] dir open failed (%d): %s\n", (int)rr, path);
        return;
    }

    printf("[MILEAGE] dir list begin: %s\n", path);
    for(int i = 0; i < 32; i++) {
        char fn[128] = {0};
        rr = lv_fs_dir_read(&dir, fn, sizeof(fn));
        if(rr != LV_FS_RES_OK) {
            printf("[MILEAGE] dir read failed (%d): %s\n", (int)rr, path);
            break;
        }
        if(fn[0] == '\0') break;
        printf("[MILEAGE] dir item[%d]: %s\n", i, fn);
    }
    lv_fs_dir_close(&dir);
    printf("[MILEAGE] dir list end: %s\n", path);
}

static char *read_text_file_lvfs_limit(const char *path, size_t *out_len)
{
    if(out_len) *out_len = 0;
    if(!path || path[0] == '\0') {
        printf("[MILEAGE] read abort: empty path\n");
        return NULL;
    }

    printf("[MILEAGE] open begin: %s\n", path);
    lv_fs_file_t f;
    lv_fs_res_t rr = lv_fs_open(&f, path, LV_FS_MODE_RD);
    if(rr != LV_FS_RES_OK) {
        printf("[MILEAGE] lv_fs_open failed (%d): %s\n", (int)rr, path);
        printf("[MILEAGE] expected same folder as boarding: %s\n", MILEAGE_BOARDING_PROBE_JSON_PATH);
        if(strcmp(path, MILEAGE_BOARDING_PROBE_JSON_PATH) != 0) {
            debug_probe_lvfs_path("boarding_probe", MILEAGE_BOARDING_PROBE_JSON_PATH);
            debug_list_lvfs_dir(QUERY_ROOT_DIR);
        }
        return NULL;
    }
    printf("[MILEAGE] open ok: %s\n", path);

    char *buf = (char *)lv_malloc(MR_JSON_MAX_SIZE + 1);
    if(!buf) {
        printf("[MILEAGE] lv_malloc failed: %u bytes\n", (unsigned)(MR_JSON_MAX_SIZE + 1));
        lv_fs_close(&f);
        return NULL;
    }

    size_t total = 0;
    while(total < MR_JSON_MAX_SIZE) {
        uint32_t br = 0;
        uint32_t want = (uint32_t)(MR_JSON_MAX_SIZE - total);

        rr = lv_fs_read(&f, buf + total, want, &br);
        if(rr != LV_FS_RES_OK) {
            printf("[MILEAGE] lv_fs_read failed (%d), total=%u\n", (int)rr, (unsigned)total);
            lv_free(buf);
            lv_fs_close(&f);
            return NULL;
        }

        if(br == 0) break;
        total += br;
    }

    lv_fs_close(&f);

    if(total == 0) {
        printf("[MILEAGE] read empty file: %s\n", path);
        lv_free(buf);
        return NULL;
    }

    if(total >= MR_JSON_MAX_SIZE) {
        printf("[MILEAGE] read too large or clipped: total=%u limit=%u path=%s\n",
               (unsigned)total, (unsigned)MR_JSON_MAX_SIZE, path);
        lv_free(buf);
        return NULL;
    }

    buf[total] = '\0';
    if(out_len) *out_len = total;
    printf("[MILEAGE] read ok: %u bytes from %s\n", (unsigned)total, path);
    return buf;
}

static cJSON *json_get_items_array(cJSON *root)
{
    printf("[MILEAGE] root type: %s\n", json_type_name(root));
    if(cJSON_IsArray(root)) {
        printf("[MILEAGE] use root array\n");
        return root;
    }

    if(!cJSON_IsObject(root)) return NULL;
    debug_print_object_keys("root", root);

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
        if(cJSON_IsArray(arr)) {
            printf("[MILEAGE] use array key: %s\n", keys[i]);
            return arr;
        }
    }

    cJSON *child = NULL;
    cJSON_ArrayForEach(child, root) {
        if(cJSON_IsArray(child)) {
            printf("[MILEAGE] fallback array key: %s\n", child->string ? child->string : "?");
            return child;
        }
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
    printf("[MILEAGE] parse begin: path=%s max_items=%d\n",
           json_path ? json_path : "(null)", max_items);

    if(!items || max_items <= 0) {
        printf("[MILEAGE] parse abort: invalid output buffer or max_items\n");
        return 0;
    }

    size_t len = 0;
    char *txt = read_text_file_lvfs_limit(json_path, &len);
    if(!txt) {
        printf("[MILEAGE] parse abort: read file returned NULL\n");
        return 0;
    }
    printf("[MILEAGE] parse input length=%u\n", (unsigned)len);

    cJSON *root = cJSON_Parse(txt);
    lv_free(txt);
    if(!root) {
        const char *err = cJSON_GetErrorPtr();
        printf("[MILEAGE] cJSON_Parse failed: %s, error=%s\n",
               json_path ? json_path : "(null)", err ? err : "(null)");
        return 0;
    }

    cJSON *arr = json_get_items_array(root);
    if(!cJSON_IsArray(arr)) {
        printf("[MILEAGE] no items array: %s\n", json_path ? json_path : "(null)");
        cJSON_Delete(root);
        return 0;
    }

    int out = 0;
    int n = cJSON_GetArraySize(arr);
    printf("[MILEAGE] array size=%d max_items=%d\n", n, max_items);

    for(int i = 0; i < n && out < max_items; i++) {
        cJSON *it = cJSON_GetArrayItem(arr, i);
        mileage_reminder_item_t *r = &items[out];
        memset(r, 0, sizeof(*r));

        if(cJSON_IsString(it) && it->valuestring && it->valuestring[0] != '\0') {
            strncpy(r->mileage_text, it->valuestring, sizeof(r->mileage_text) - 1);
            r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';
            if(i < MR_DEBUG_ITEM_LOG_LIMIT) {
                printf("[MILEAGE] accept item[%d] string -> row[%d], mileage=%s\n",
                       i, out, r->mileage_text);
            }
            out++;
            continue;
        }

        if(!cJSON_IsObject(it)) {
            if(i < MR_DEBUG_ITEM_LOG_LIMIT) {
                printf("[MILEAGE] skip item[%d]: type=%s\n", i, json_type_name(it));
            }
            continue;
        }

        cJSON *j_mileage = json_get_first(it, "mileage_text", "mileage", "text");
        if(!j_mileage) j_mileage = json_get_first(it, "mileageText", "mileage_value", "name");
        if(!j_mileage) j_mileage = json_get_first(it, "mileage_no", "mileageNo", "mileage_num");
        if(!j_mileage) j_mileage = json_get_first(it, "line_mileage", "km_text", "kilometer");
        if(!j_mileage) j_mileage = json_get_first(it, "k", "km", "value");
        if(!j_mileage) {
            if(i < MR_DEBUG_ITEM_LOG_LIMIT) {
                printf("[MILEAGE] skip item[%d]: missing mileage field\n", i);
                debug_print_object_keys("item", it);
            }
            continue;
        }

        if(!cJSON_IsString(j_mileage) || !j_mileage->valuestring || j_mileage->valuestring[0] == '\0') {
            if(i < MR_DEBUG_ITEM_LOG_LIMIT) {
                printf("[MILEAGE] skip item[%d]: mileage type=%s empty=%d\n",
                       i, json_type_name(j_mileage),
                       (cJSON_IsString(j_mileage) && j_mileage->valuestring && j_mileage->valuestring[0] == '\0') ? 1 : 0);
            }
            continue;
        }

        strncpy(r->mileage_text, j_mileage->valuestring, sizeof(r->mileage_text) - 1);
        r->mileage_text[sizeof(r->mileage_text) - 1] = '\0';

        cJSON *j_reminded = json_get_first(it, "reminded", "is_reminded", "done");
        if(!j_reminded) j_reminded = json_get_first(it, "isReminded", "remind", "warned");
        if(!j_reminded) j_reminded = json_get_first(it, "is_remind", "isRemind", "has_reminded");
        if(!j_reminded) j_reminded = cJSON_GetObjectItem(it, "status");
        r->reminded = json_value_is_reminded(j_reminded);

        if(i < MR_DEBUG_ITEM_LOG_LIMIT) {
            printf("[MILEAGE] accept item[%d] -> row[%d], mileage=%s, reminded=%d, status_type=%s",
                   i, out, r->mileage_text, r->reminded ? 1 : 0, json_type_name(j_reminded));
            if(cJSON_IsString(j_reminded) && j_reminded->valuestring) {
                printf(", status=%s", j_reminded->valuestring);
            }
            printf("\n");
        }

        out++;
    }

    if(n > MR_DEBUG_ITEM_LOG_LIMIT) {
        printf("[MILEAGE] item detail log capped at %d of %d items\n", MR_DEBUG_ITEM_LOG_LIMIT, n);
    }
    printf("[MILEAGE] parsed %d items from %s\n", out, json_path ? json_path : "(null)");
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
    printf("[MILEAGE] show empty label\n");
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
    printf("[MILEAGE] render begin: ui=%p path=%s\n",
           (void *)ui, json_path ? json_path : "(null)");
    debug_print_fs_mapping();

    if(!ui) {
        printf("[MILEAGE] render abort: ui is NULL\n");
        return;
    }

    if(!ui->screen_mileage_reminder_cont_list) {
        printf("[MILEAGE] render abort: screen_mileage_reminder_cont_list is NULL\n");
        return;
    }

    if(ui->screen_mileage_reminder_label_3) {
        lv_label_set_text(ui->screen_mileage_reminder_label_3, "\xE6\x8F\x90\xE9\x86\x92\xE9\x87\x8C\xE7\xA8\x8B");
        lv_obj_set_style_text_font(ui->screen_mileage_reminder_label_3, MR_TITLE_FONT, 0);
    } else {
        printf("[MILEAGE] title label is NULL\n");
    }

    mileage_reminder_item_t items[MR_MAX_ITEMS];
    memset(items, 0, sizeof(items));
    int count = parse_mileage_items(json_path, items, MR_MAX_ITEMS);
    printf("[MILEAGE] render parsed count=%d\n", count);
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
    printf("[MILEAGE] render rows done: count=%d\n", count);
}


void ui_mileage_reminder_deinit(lv_ui *ui)
{
    printf("[MILEAGE] deinit: ui=%p list=%p\n",
           (void *)ui, ui ? (void *)ui->screen_mileage_reminder_cont_list : NULL);
    if(!ui || !ui->screen_mileage_reminder_cont_list) return;
    lv_obj_clean(ui->screen_mileage_reminder_cont_list);
}

void ui_mileage_reminder_render_default(lv_ui *ui)
{
    printf("[MILEAGE] render default path=%s\n", MILEAGE_REMINDER_JSON_PATH);
    ui_mileage_reminder_render_from_json(ui, MILEAGE_REMINDER_JSON_PATH);
}

void screen_mileage_reminder_custom_code(lv_event_t *e)
{
    printf("[MILEAGE] enter custom code: event=%d\n", e ? (int)lv_event_get_code(e) : -1);
    ui_mileage_reminder_render_default(&guider_ui);
}

void screen_mileage_reminder_deinit_custom_code(lv_event_t *e)
{
    printf("[MILEAGE] deinit custom code: event=%d\n", e ? (int)lv_event_get_code(e) : -1);
    ui_mileage_reminder_deinit(&guider_ui);
}
