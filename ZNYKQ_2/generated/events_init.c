/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "app_boarding_log_list.h"
#include "app_mileage_reminder_list.h"
#include "app_boarding_log_list.h"
#include "app_mileage_reminder_list.h"
#include "app_mileage_reminder_list.h"
#include "app_nav.h"
#include "app_mileage_reminder_list.h"
#include "app_overlimt_list.h"

#include "screen_log_type_actions.h"
#include "screen_log_type_actions.h"
#include "screen_log_type_actions.h"
#include "app_mark_list.h"
#include "screen_log_type_actions.h"
#include "screen_log_type_actions.h"
#include "screen_log_type_actions.h"
#include "app_audio_mark_list.h"

#include "screen_log_type_actions.h"
#include "screen_log_type_actions.h"
#include "screen_log_type_actions.h"

static void screen_main_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_log, guider_ui.screen_log_del, &guider_ui.screen_main_del, setup_scr_screen_log, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, false, false);
            ui_boarding_log_render_from_json(&guider_ui, "S:/CB05U01TEST/boarding_records.json");
            break;
        }
        case LV_DIR_BOTTOM:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_set, guider_ui.screen_set_del, &guider_ui.screen_main_del, setup_scr_screen_set, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 200, false, false);
            break;
        }
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_mileage_reminder, guider_ui.screen_mileage_reminder_del, &guider_ui.screen_main_del, setup_scr_screen_mileage_reminder, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            screen_mileage_reminder_custom_code(e);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_main_img_voickey_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_LONG_PRESSED:
    {
        //开始音频标记
        lv_obj_add_flag(guider_ui.screen_main_label_tip, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(guider_ui.screen_main_label_tip2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(guider_ui.screen_main_img_voickey2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_main_img_voickey, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_main_img_play_run_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        //播放最近的音频文件
        lv_obj_add_flag(guider_ui.screen_main_img_play_run, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(guider_ui.screen_main_img_play_stop, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void screen_main_img_play_stop_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_main_img_play_stop, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(guider_ui.screen_main_img_play_run, LV_OBJ_FLAG_HIDDEN);
        //停止播放音频
        break;
    }
    default:
        break;
    }
}

static void screen_main_img_voickey2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_main_img_voickey2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(guider_ui.screen_main_img_voickey, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(guider_ui.screen_main_label_tip, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_main_label_tip2, LV_OBJ_FLAG_HIDDEN);
        //停止音频录制
        break;
    }
    default:
        break;
    }
}

void events_init_screen_main (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_main, screen_main_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_img_voickey, screen_main_img_voickey_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_img_play_run, screen_main_img_play_run_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_img_play_stop, screen_main_img_play_stop_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main_img_voickey2, screen_main_img_voickey2_event_handler, LV_EVENT_ALL, ui);
}

static void screen_mileage_reminder_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_log, guider_ui.screen_log_del, &guider_ui.screen_mileage_reminder_del, setup_scr_screen_log, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            ui_boarding_log_render_from_json(&guider_ui, "S:/CB05U01TEST/boarding_records.json");
            screen_mileage_reminder_deinit_custom_code(e);
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_mileage_reminder_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, false, false);
            screen_mileage_reminder_deinit_custom_code(e);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_mileage_reminder (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_mileage_reminder, screen_mileage_reminder_event_handler, LV_EVENT_ALL, ui);
}

static void screen_set_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_set_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 200, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_set_cont_7_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_push_and_load(
            &guider_ui,
            nav_make_page(&guider_ui, screen_bledata, setup_scr_screen_bledata),
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_set_cont_8_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_push_and_load(
            &guider_ui,
            nav_make_page(&guider_ui, screen_about, setup_scr_screen_about),
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_set_cont_9_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_push_and_load(
            &guider_ui,
            nav_make_page(&guider_ui, screen_volume, setup_scr_screen_volume),
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

void events_init_screen_set (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_set, screen_set_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_cont_7, screen_set_cont_7_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_cont_8, screen_set_cont_8_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_set_cont_9, screen_set_cont_9_event_handler, LV_EVENT_ALL, ui);
}

static void screen_log_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_mileage_reminder, guider_ui.screen_mileage_reminder_del, &guider_ui.screen_log_del, setup_scr_screen_mileage_reminder, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, false, false);
            ui_boarding_log_deinit();
            screen_mileage_reminder_custom_code(e);
            break;
        }
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_main, guider_ui.screen_main_del, &guider_ui.screen_log_del, setup_scr_screen_main, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            ui_boarding_log_deinit();
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_log (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_log, screen_log_event_handler, LV_EVENT_ALL, ui);
}

static void screen_overlimt_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_log, guider_ui.screen_log_del, &guider_ui.screen_overlimt_del, setup_scr_screen_log, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            ui_overlimt_deinit();
            ui_boarding_log_render_from_json(&guider_ui, "S:/CB05U01TEST/boarding_records.json");
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_overlimt_btn_overlimt_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_1_custom_code(e);
        break;
    }
    default:
        break;
    }
}

static void screen_overlimt_btn_mark_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_2_custom_code(e);
        break;
    }
    default:
        break;
    }
}

static void screen_overlimt_btn_audiomark_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_3_custom_code(e);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_overlimt (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_overlimt, screen_overlimt_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_overlimt_btn_overlimt, screen_overlimt_btn_overlimt_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_overlimt_btn_mark, screen_overlimt_btn_mark_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_overlimt_btn_audiomark, screen_overlimt_btn_audiomark_event_handler, LV_EVENT_ALL, ui);
}

static void screen_mark_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_log, guider_ui.screen_log_del, &guider_ui.screen_mark_del, setup_scr_screen_log, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            ui_mark_deinit();
            ui_boarding_log_render_from_json(&guider_ui, "S:/CB05U01TEST/boarding_records.json");
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_mark_btn_overlimt_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_1_custom_code(e);
        break;
    }
    default:
        break;
    }
}

static void screen_mark_btn_mark_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_2_custom_code(e);
        break;
    }
    default:
        break;
    }
}

static void screen_mark_btn_audio_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_3_custom_code(e);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_mark (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_mark, screen_mark_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_mark_btn_overlimt, screen_mark_btn_overlimt_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_mark_btn_mark, screen_mark_btn_mark_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_mark_btn_audio, screen_mark_btn_audio_event_handler, LV_EVENT_ALL, ui);
}

static void screen_audiomark_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_log, guider_ui.screen_log_del, &guider_ui.screen_audiomark_del, setup_scr_screen_log, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            audiomark_deinit();
            ui_boarding_log_render_from_json(&guider_ui, "S:/CB05U01TEST/boarding_records.json");
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_audiomark_btn_overlimt_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_1_custom_code(e);
        break;
    }
    default:
        break;
    }
}

static void screen_audiomark_btn_mark_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_2_custom_code(e);
        break;
    }
    default:
        break;
    }
}

static void screen_audiomark_btn_audio_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        screen_log_type_cont_3_custom_code(e);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_audiomark (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_audiomark, screen_audiomark_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_audiomark_btn_overlimt, screen_audiomark_btn_overlimt_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_audiomark_btn_mark, screen_audiomark_btn_mark_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_audiomark_btn_audio, screen_audiomark_btn_audio_event_handler, LV_EVENT_ALL, ui);
}

static void screen_about_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            nav_back(
                &guider_ui,
                LV_SCR_LOAD_ANIM_NONE,
                0,
                0
            );
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            nav_back(
                &guider_ui,
                LV_SCR_LOAD_ANIM_NONE,
                0,
                0
            );
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_about (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_about, screen_about_event_handler, LV_EVENT_ALL, ui);
}

static void screen_Tag_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

void events_init_screen_Tag (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_Tag_img_back, screen_Tag_img_back_event_handler, LV_EVENT_ALL, ui);
}

static void screen_audio_Tag_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_audio_Tag_img_audio_play_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        // 需要加入音频播放开始功能
        lv_obj_remove_flag(guider_ui.screen_audio_Tag_img_audio_playing, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_audio_Tag_img_audio_play, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_audio_Tag (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_audio_Tag_img_back, screen_audio_Tag_img_back_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_audio_Tag_img_audio_play, screen_audio_Tag_img_audio_play_event_handler, LV_EVENT_ALL, ui);
}

static void screen_mark_err_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

void events_init_screen_mark_err (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_mark_err_img_back, screen_mark_err_img_back_event_handler, LV_EVENT_ALL, ui);
}

static void screen_warn1_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_warn1_img_voic_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_PRESSED:
    {
        //开始录音
        lv_obj_remove_flag(guider_ui.screen_warn1_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    case LV_EVENT_RELEASED:
    {
        //停止录音
        lv_obj_add_flag(guider_ui.screen_warn1_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_warn1 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_warn1_img_back, screen_warn1_img_back_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_warn1_img_voic, screen_warn1_img_voic_event_handler, LV_EVENT_ALL, ui);
}

static void screen_warn2_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_warn2_img_voic_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_PRESSED:
    {
        //开始录音
        lv_obj_remove_flag(guider_ui.screen_warn2_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    case LV_EVENT_RELEASED:
    {
        //停止录音
        lv_obj_add_flag(guider_ui.screen_warn2_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_warn2 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_warn2_img_back, screen_warn2_img_back_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_warn2_img_voic, screen_warn2_img_voic_event_handler, LV_EVENT_ALL, ui);
}

static void screen_warn3_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_warn3_img_voic_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_PRESSED:
    {
        //开始录音
        lv_obj_remove_flag(guider_ui.screen_warn3_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    case LV_EVENT_RELEASED:
    {
        //停止录音
        lv_obj_add_flag(guider_ui.screen_warn3_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_warn3 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_warn3_img_back, screen_warn3_img_back_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_warn3_img_voic, screen_warn3_img_voic_event_handler, LV_EVENT_ALL, ui);
}

static void screen_warn4_img_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        nav_back(
            &guider_ui,
            LV_SCR_LOAD_ANIM_NONE,
            0,
            0
        );
        break;
    }
    default:
        break;
    }
}

static void screen_warn4_img_voic_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_PRESSED:
    {
        //开始录音
        lv_obj_remove_flag(guider_ui.screen_warn4_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    case LV_EVENT_RELEASED:
    {
        //停止录音
        lv_obj_add_flag(guider_ui.screen_warn4_label_tip, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_warn4 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_warn4_img_back, screen_warn4_img_back_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_warn4_img_voic, screen_warn4_img_voic_event_handler, LV_EVENT_ALL, ui);
}

static void screen_volume_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            nav_back(
                &guider_ui,
                LV_SCR_LOAD_ANIM_NONE,
                0,
                0
            );
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            nav_back(
                &guider_ui,
                LV_SCR_LOAD_ANIM_NONE,
                0,
                0
            );
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_volume (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_volume, screen_volume_event_handler, LV_EVENT_ALL, ui);
}

static void screen_bledata_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            nav_back(
                &guider_ui,
                LV_SCR_LOAD_ANIM_NONE,
                0,
                0
            );
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            nav_back(
                &guider_ui,
                LV_SCR_LOAD_ANIM_NONE,
                0,
                0
            );
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_bledata (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_bledata, screen_bledata_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
