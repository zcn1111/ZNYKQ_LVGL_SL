/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_overlimt(lv_ui *ui)
{
    //Write codes screen_overlimt
    ui->screen_overlimt = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_overlimt, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_overlimt, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_overlimt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overlimt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overlimt, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overlimt, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overlimt_img_bg
    ui->screen_overlimt_img_bg = lv_image_create(ui->screen_overlimt);
    lv_obj_set_pos(ui->screen_overlimt_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_overlimt_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_overlimt_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_overlimt_img_bg, &_sanjibg_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_overlimt_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_overlimt_img_bg, 0);

    //Write style for screen_overlimt_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_overlimt_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_overlimt_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overlimt_cont_list
    ui->screen_overlimt_cont_list = lv_obj_create(ui->screen_overlimt);
    lv_obj_set_pos(ui->screen_overlimt_cont_list, 5, 54);
    lv_obj_set_size(ui->screen_overlimt_cont_list, 230, 222);
    lv_obj_set_scrollbar_mode(ui->screen_overlimt_cont_list, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_overlimt_cont_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overlimt_cont_list, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_overlimt_cont_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_overlimt_cont_list, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_overlimt_cont_list, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overlimt_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overlimt_btn_overlimt
    ui->screen_overlimt_btn_overlimt = lv_button_create(ui->screen_overlimt);
    lv_obj_set_pos(ui->screen_overlimt_btn_overlimt, 4, 5);
    lv_obj_set_size(ui->screen_overlimt_btn_overlimt, 72, 34);
    ui->screen_overlimt_btn_overlimt_label = lv_label_create(ui->screen_overlimt_btn_overlimt);
    lv_label_set_text(ui->screen_overlimt_btn_overlimt_label, "");
    lv_label_set_long_mode(ui->screen_overlimt_btn_overlimt_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overlimt_btn_overlimt_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overlimt_btn_overlimt, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overlimt_btn_overlimt_label, LV_PCT(100));

    //Write style for screen_overlimt_btn_overlimt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overlimt_btn_overlimt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overlimt_btn_overlimt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overlimt_btn_overlimt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overlimt_btn_overlimt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overlimt_btn_overlimt, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overlimt_btn_overlimt, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overlimt_btn_overlimt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overlimt_btn_overlimt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overlimt_btn_mark
    ui->screen_overlimt_btn_mark = lv_button_create(ui->screen_overlimt);
    lv_obj_set_pos(ui->screen_overlimt_btn_mark, 85, 5);
    lv_obj_set_size(ui->screen_overlimt_btn_mark, 72, 34);
    ui->screen_overlimt_btn_mark_label = lv_label_create(ui->screen_overlimt_btn_mark);
    lv_label_set_text(ui->screen_overlimt_btn_mark_label, "");
    lv_label_set_long_mode(ui->screen_overlimt_btn_mark_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overlimt_btn_mark_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overlimt_btn_mark, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overlimt_btn_mark_label, LV_PCT(100));

    //Write style for screen_overlimt_btn_mark, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overlimt_btn_mark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overlimt_btn_mark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overlimt_btn_mark, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overlimt_btn_mark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overlimt_btn_mark, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overlimt_btn_mark, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overlimt_btn_mark, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overlimt_btn_mark, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overlimt_btn_audiomark
    ui->screen_overlimt_btn_audiomark = lv_button_create(ui->screen_overlimt);
    lv_obj_set_pos(ui->screen_overlimt_btn_audiomark, 164, 5);
    lv_obj_set_size(ui->screen_overlimt_btn_audiomark, 72, 34);
    ui->screen_overlimt_btn_audiomark_label = lv_label_create(ui->screen_overlimt_btn_audiomark);
    lv_label_set_text(ui->screen_overlimt_btn_audiomark_label, "");
    lv_label_set_long_mode(ui->screen_overlimt_btn_audiomark_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overlimt_btn_audiomark_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overlimt_btn_audiomark, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overlimt_btn_audiomark_label, LV_PCT(100));

    //Write style for screen_overlimt_btn_audiomark, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overlimt_btn_audiomark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overlimt_btn_audiomark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overlimt_btn_audiomark, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overlimt_btn_audiomark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overlimt_btn_audiomark, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overlimt_btn_audiomark, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overlimt_btn_audiomark, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overlimt_btn_audiomark, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_overlimt.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_overlimt);

    //Init events for screen.
    events_init_screen_overlimt(ui);
}
