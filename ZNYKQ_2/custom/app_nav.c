#include "app_nav.h"
#include "gui_guider.h"

/* ================= 配置 ================= */

#define NAV_STACK_MAX 8

/* ================= 内部数据 ================= */

static ui_page_t s_nav_stack[NAV_STACK_MAX];
static int s_nav_top = 0;

/* ================= 内部辅助 ================= */

/*
 * 根据当前 active screen，获取对应页面描述
 */
static ui_page_t nav_get_current_page(lv_ui *ui)
{
    lv_obj_t *act = lv_screen_active();

    if(act == ui->screen_main) {
        return nav_make_page(ui, screen_main, setup_scr_screen_main);
    }
    if(act == ui->screen_set) {
        return nav_make_page(ui, screen_set, setup_scr_screen_set);
    }
    if(act == ui->screen_overlimt) {
        return nav_make_page(ui, screen_overlimt, setup_scr_screen_overlimt);
    }
    if(act == ui->screen_about) {
        return nav_make_page(ui, screen_about, setup_scr_screen_about);
    }
    if(act == ui->screen_Tag) {
        return nav_make_page(ui, screen_Tag, setup_scr_screen_Tag);
    }
    if(act == ui->screen_audio_Tag) {
        return nav_make_page(ui, screen_audio_Tag, setup_scr_screen_audio_Tag);
    }
    if(act == ui->screen_warn1) {
        return nav_make_page(ui, screen_warn1, setup_scr_screen_warn1);
    }
    if(act == ui->screen_warn2) {
        return nav_make_page(ui, screen_warn2, setup_scr_screen_warn2);
    }
    if(act == ui->screen_warn3) {
        return nav_make_page(ui, screen_warn3, setup_scr_screen_warn3);
    }
    if(act == ui->screen_warn4) {
        return nav_make_page(ui, screen_warn4, setup_scr_screen_warn4);
    }

    if(act == ui->screen_volume) {
        return nav_make_page(ui, screen_volume, setup_scr_screen_volume);
    }
    if(act == ui->screen_bledata) {
        return nav_make_page(ui, screen_bledata, setup_scr_screen_bledata);
    }
    if(act == ui->screen_bledata) {
        return nav_make_page(ui, screen_sleep_set, setup_scr_screen_sleep_set);
    }
    if(act == ui->screen_bledata) {
        return nav_make_page(ui, screen_Instructions, setup_scr_screen_Instructions);
    }



    ui_page_t empty = {0};
    return empty;
}

/* ================= 对外接口 ================= */

void nav_init(void)
{
    s_nav_top = 0;
}

void nav_reset(void)
{
    s_nav_top = 0;
}

bool nav_can_back(void)
{
    return (s_nav_top > 0);
}

void nav_push_and_load(void *ui_ptr,
                       ui_page_t target,
                       lv_screen_load_anim_t anim_type,
                       uint32_t time,
                       uint32_t delay)
{
    lv_ui *ui = (lv_ui *)ui_ptr;
    if(ui == NULL || target.screen == NULL) return;

    /* 当前页面压栈 */
    ui_page_t current = nav_get_current_page(ui);
    if(current.screen && s_nav_top < NAV_STACK_MAX) {
        s_nav_stack[s_nav_top++] = current;
    }

    /* 加载目标页面 */
    ui_load_scr_animation(
        ui,
        target.screen,
        *target.screen_del,          /* new_scr_del */
        current.screen_del,          /* ⭐ 旧页面的 del 标志 */
        target.setup,
        anim_type,
        time,
        delay,
        true,
        true
    );
}

void nav_back(void *ui_ptr,
              lv_screen_load_anim_t anim_type,
              uint32_t time,
              uint32_t delay)
{
    lv_ui *ui = (lv_ui *)ui_ptr;
    if(ui == NULL || s_nav_top <= 0) return;

    /* 出栈 */
    ui_page_t prev = s_nav_stack[--s_nav_top];

    /* 当前页面（返回前） */
    ui_page_t current = nav_get_current_page(ui);

    if(prev.screen == NULL) return;

    /* 返回加载 */
    ui_load_scr_animation(
        ui,
        prev.screen,
        *prev.screen_del,            /* new_scr_del */
        current.screen_del,          /* ⭐ 旧页面的 del 标志 */
        prev.setup,
        anim_type,
        time,
        delay,
        true,
        true
    );
}
