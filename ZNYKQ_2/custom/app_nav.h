#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "lvgl.h"
#include "gui_guider.h"
#ifdef __cplusplus
extern "C" {
#endif


/* 一个“页面描述”，用于返回栈：包含 screen 指针、del 标志、setup 函数 */
typedef struct {
    lv_obj_t **      screen;      /* 指向 ui->screen_xxx 的指针（&ui->screen_xxx） */
    bool *           screen_del;   /* 指向 ui->screen_xxx_del 的指针（&ui->screen_xxx_del） */
    ui_setup_scr_t   setup;        /* setup_scr_screen_xxx */
} ui_page_t;

/* 初始化导航模块（可选，建议在 custom_init 或 setup_ui 后调用一次） */
void nav_init(void);

/* 清空返回栈（例如回主界面时调用） */
void nav_reset(void);

/*
 * 跳转到目标页面（会把当前页面压栈）
 *
 * 参数：
 *   ui     : GUI Guider 的 lv_ui 指针（如 &guider_ui）
 *   target : 目标页面描述（见 nav_make_page 宏）
 *
 * anim_type/time/delay 让你可选是否带动画
 */
void nav_push_and_load(void *ui,
                       ui_page_t target,
                       lv_screen_load_anim_t anim_type,
                       uint32_t time,
                       uint32_t delay);

/*
 * 返回上一个页面（从栈里弹出并加载）
 * anim_type/time/delay 同上
 */
void nav_back(void *ui,
              lv_screen_load_anim_t anim_type,
              uint32_t time,
              uint32_t delay);

/*
 * 便捷宏：生成 ui_page_t
 * 用法示例：
 *   nav_push_and_load(&guider_ui,
 *                     nav_make_page(&guider_ui, screen_overlimt, setup_scr_screen_overlimt),
 *                     LV_SCREEN_LOAD_ANIM_NONE, 0, 0);
 *
 * 说明：
 * - member_name 要传 screen 成员名（不带 ui-> 前缀），例如 screen_main / screen_overlimt
 * - setup_fn    传 setup_scr_screen_xxx 函数名
 */
#define nav_make_page(ui_ptr, member_name, setup_fn) \
    ((ui_page_t){ \
        .screen = &((lv_ui*)(ui_ptr))->member_name, \
        .screen_del = &((lv_ui*)(ui_ptr))->member_name##_del, \
        .setup = (ui_setup_scr_t)(setup_fn) \
    })

/* 可选：查询返回栈是否为空 */
bool nav_can_back(void);

#ifdef __cplusplus
}
#endif
