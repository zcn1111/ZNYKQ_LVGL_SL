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



void setup_scr_screen_audiomark(lv_ui *ui)
{
    //Write codes screen_audiomark
    ui->screen_audiomark = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_audiomark, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_audiomark, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_audiomark, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_audiomark, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_audiomark, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_audiomark, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_img_bg
    ui->screen_audiomark_img_bg = lv_image_create(ui->screen_audiomark);
    lv_obj_set_pos(ui->screen_audiomark_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_audiomark_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_audiomark_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_audiomark_img_bg, &_luying_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_audiomark_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_audiomark_img_bg, 0);

    //Write style for screen_audiomark_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_audiomark_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_audiomark_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_cont_list
    ui->screen_audiomark_cont_list = lv_obj_create(ui->screen_audiomark);
    lv_obj_set_pos(ui->screen_audiomark_cont_list, 5, 47);
    lv_obj_set_size(ui->screen_audiomark_cont_list, 230, 227);
    lv_obj_set_scrollbar_mode(ui->screen_audiomark_cont_list, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_audiomark_cont_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_audiomark_cont_list, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_audiomark_cont_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_audiomark_cont_list, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_audiomark_cont_list, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_audiomark_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_cont_1
    ui->screen_audiomark_cont_1 = lv_obj_create(ui->screen_audiomark_cont_list);
    lv_obj_set_pos(ui->screen_audiomark_cont_1, 5, 12);
    lv_obj_set_size(ui->screen_audiomark_cont_1, 220, 88);
    lv_obj_set_scrollbar_mode(ui->screen_audiomark_cont_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_audiomark_cont_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_audiomark_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_audiomark_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_audiomark_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_audiomark_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_audiomark_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_audiomark_cont_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_audiomark_cont_1, 54, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_audiomark_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_audiomark_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_audiomark_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_audiomark_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_audiomark_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_audiomark_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_audiomark_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_img_1
    ui->screen_audiomark_img_1 = lv_image_create(ui->screen_audiomark_cont_1);
    lv_obj_set_pos(ui->screen_audiomark_img_1, 142, 27);
    lv_obj_set_size(ui->screen_audiomark_img_1, 50, 50);
    lv_obj_add_flag(ui->screen_audiomark_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_audiomark_img_1, &_voc_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_audiomark_img_1, 50,50);
    lv_image_set_rotation(ui->screen_audiomark_img_1, 0);

    //Write style for screen_audiomark_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_audiomark_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_audiomark_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_img_2
    ui->screen_audiomark_img_2 = lv_image_create(ui->screen_audiomark_cont_1);
    lv_obj_set_pos(ui->screen_audiomark_img_2, 73, 27);
    lv_obj_set_size(ui->screen_audiomark_img_2, 50, 50);
    lv_obj_add_flag(ui->screen_audiomark_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_audiomark_img_2, &_voc2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_audiomark_img_2, 50,50);
    lv_image_set_rotation(ui->screen_audiomark_img_2, 0);

    //Write style for screen_audiomark_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_audiomark_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_audiomark_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_label_14
    ui->screen_audiomark_label_14 = lv_label_create(ui->screen_audiomark);
    lv_obj_set_pos(ui->screen_audiomark_label_14, 55, 127);
    lv_obj_set_size(ui->screen_audiomark_label_14, 131, 44);
    lv_obj_add_flag(ui->screen_audiomark_label_14, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_audiomark_label_14, "暂无录音标记数据超限");
    lv_label_set_long_mode(ui->screen_audiomark_label_14, LV_LABEL_LONG_WRAP);

    //Write style for screen_audiomark_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_audiomark_label_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_audiomark_label_14, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_audiomark_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_audiomark_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_audiomark_label_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_audiomark_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_btn_overlimt
    ui->screen_audiomark_btn_overlimt = lv_button_create(ui->screen_audiomark);
    lv_obj_set_pos(ui->screen_audiomark_btn_overlimt, 4, 5);
    lv_obj_set_size(ui->screen_audiomark_btn_overlimt, 72, 40);
    ui->screen_audiomark_btn_overlimt_label = lv_label_create(ui->screen_audiomark_btn_overlimt);
    lv_label_set_text(ui->screen_audiomark_btn_overlimt_label, "");
    lv_label_set_long_mode(ui->screen_audiomark_btn_overlimt_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_audiomark_btn_overlimt_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_audiomark_btn_overlimt, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_audiomark_btn_overlimt_label, LV_PCT(100));

    //Write style for screen_audiomark_btn_overlimt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_audiomark_btn_overlimt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_audiomark_btn_overlimt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_audiomark_btn_overlimt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_audiomark_btn_overlimt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_audiomark_btn_overlimt, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_audiomark_btn_overlimt, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_audiomark_btn_overlimt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_audiomark_btn_overlimt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_btn_mark
    ui->screen_audiomark_btn_mark = lv_button_create(ui->screen_audiomark);
    lv_obj_set_pos(ui->screen_audiomark_btn_mark, 85, 5);
    lv_obj_set_size(ui->screen_audiomark_btn_mark, 72, 40);
    ui->screen_audiomark_btn_mark_label = lv_label_create(ui->screen_audiomark_btn_mark);
    lv_label_set_text(ui->screen_audiomark_btn_mark_label, "");
    lv_label_set_long_mode(ui->screen_audiomark_btn_mark_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_audiomark_btn_mark_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_audiomark_btn_mark, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_audiomark_btn_mark_label, LV_PCT(100));

    //Write style for screen_audiomark_btn_mark, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_audiomark_btn_mark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_audiomark_btn_mark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_audiomark_btn_mark, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_audiomark_btn_mark, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_audiomark_btn_mark, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_audiomark_btn_mark, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_audiomark_btn_mark, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_audiomark_btn_mark, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_audiomark_btn_audio
    ui->screen_audiomark_btn_audio = lv_button_create(ui->screen_audiomark);
    lv_obj_set_pos(ui->screen_audiomark_btn_audio, 164, 5);
    lv_obj_set_size(ui->screen_audiomark_btn_audio, 72, 40);
    ui->screen_audiomark_btn_audio_label = lv_label_create(ui->screen_audiomark_btn_audio);
    lv_label_set_text(ui->screen_audiomark_btn_audio_label, "");
    lv_label_set_long_mode(ui->screen_audiomark_btn_audio_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_audiomark_btn_audio_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_audiomark_btn_audio, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_audiomark_btn_audio_label, LV_PCT(100));

    //Write style for screen_audiomark_btn_audio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_audiomark_btn_audio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_audiomark_btn_audio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_audiomark_btn_audio, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_audiomark_btn_audio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_audiomark_btn_audio, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_audiomark_btn_audio, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_audiomark_btn_audio, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_audiomark_btn_audio, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_audiomark.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_audiomark);

    //Init events for screen.
    events_init_screen_audiomark(ui);
}
