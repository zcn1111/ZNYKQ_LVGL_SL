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



void setup_scr_screen_mark_err(lv_ui *ui)
{
    //Write codes screen_mark_err
    ui->screen_mark_err = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_mark_err, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_mark_err, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_mark_err, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mark_err, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mark_err, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mark_err, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_img_bg
    ui->screen_mark_err_img_bg = lv_image_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_mark_err_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_mark_err_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_mark_err_img_bg, &_markbg_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_mark_err_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_mark_err_img_bg, 0);

    //Write style for screen_mark_err_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_mark_err_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_mark_err_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_img_back
    ui->screen_mark_err_img_back = lv_image_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_img_back, 93, 213);
    lv_obj_set_size(ui->screen_mark_err_img_back, 50, 50);
    lv_obj_add_flag(ui->screen_mark_err_img_back, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_mark_err_img_back, &_close_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_mark_err_img_back, 50,50);
    lv_image_set_rotation(ui->screen_mark_err_img_back, 0);

    //Write style for screen_mark_err_img_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_mark_err_img_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_mark_err_img_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_label_mark_name
    ui->screen_mark_err_label_mark_name = lv_label_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_label_mark_name, 34, 49);
    lv_obj_set_size(ui->screen_mark_err_label_mark_name, 104, 28);
    lv_label_set_text(ui->screen_mark_err_label_mark_name, "标记失败");
    lv_label_set_long_mode(ui->screen_mark_err_label_mark_name, LV_LABEL_LONG_WRAP);

    //Write style for screen_mark_err_label_mark_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mark_err_label_mark_name, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mark_err_label_mark_name, &lv_font_PingFangHeavy_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mark_err_label_mark_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mark_err_label_mark_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mark_err_label_mark_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_label_time
    ui->screen_mark_err_label_time = lv_label_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_label_time, 154, 52);
    lv_obj_set_size(ui->screen_mark_err_label_time, 61, 26);
    lv_label_set_text(ui->screen_mark_err_label_time, "12 : 20");
    lv_label_set_long_mode(ui->screen_mark_err_label_time, LV_LABEL_LONG_WRAP);

    //Write style for screen_mark_err_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mark_err_label_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mark_err_label_time, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mark_err_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mark_err_label_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mark_err_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_label_2
    ui->screen_mark_err_label_2 = lv_label_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_label_2, 34, 169);
    lv_obj_set_size(ui->screen_mark_err_label_2, 66, 28);
    lv_label_set_text(ui->screen_mark_err_label_2, "错误码");
    lv_label_set_long_mode(ui->screen_mark_err_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_mark_err_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mark_err_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mark_err_label_2, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mark_err_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mark_err_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mark_err_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_label_3
    ui->screen_mark_err_label_3 = lv_label_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_label_3, 125, 169);
    lv_obj_set_size(ui->screen_mark_err_label_3, 63, 22);
    lv_label_set_text(ui->screen_mark_err_label_3, "-1");
    lv_label_set_long_mode(ui->screen_mark_err_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_mark_err_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mark_err_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mark_err_label_3, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mark_err_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mark_err_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mark_err_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mark_err_label_4
    ui->screen_mark_err_label_4 = lv_label_create(ui->screen_mark_err);
    lv_obj_set_pos(ui->screen_mark_err_label_4, 34, 106);
    lv_obj_set_size(ui->screen_mark_err_label_4, 178, 26);
    lv_label_set_text(ui->screen_mark_err_label_4, "请重新进行标记");
    lv_label_set_long_mode(ui->screen_mark_err_label_4, LV_LABEL_LONG_WRAP);

    //Write style for screen_mark_err_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mark_err_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mark_err_label_4, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mark_err_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mark_err_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mark_err_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_mark_err.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_mark_err);

    //Init events for screen.
    events_init_screen_mark_err(ui);
}
