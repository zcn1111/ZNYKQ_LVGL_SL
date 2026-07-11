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



void setup_scr_screen_log(lv_ui *ui)
{
    //Write codes screen_log
    ui->screen_log = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_log, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_log, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_log, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_log, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_log, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_log, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_log_img_bg
    ui->screen_log_img_bg = lv_image_create(ui->screen_log);
    lv_obj_set_pos(ui->screen_log_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_log_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_log_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_log_img_bg, &_bg_new_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_log_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_log_img_bg, 0);

    //Write style for screen_log_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_log_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_log_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_log_cont_list
    ui->screen_log_cont_list = lv_obj_create(ui->screen_log);
    lv_obj_set_pos(ui->screen_log_cont_list, 5, 48);
    lv_obj_set_size(ui->screen_log_cont_list, 230, 227);
    lv_obj_set_scrollbar_mode(ui->screen_log_cont_list, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_log_cont_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_log_cont_list, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_log_cont_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_log_cont_list, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_log_cont_list, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_log_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_log_img_list_bg
    ui->screen_log_img_list_bg = lv_image_create(ui->screen_log_cont_list);
    lv_obj_set_pos(ui->screen_log_img_list_bg, 3, 1);
    lv_obj_set_size(ui->screen_log_img_list_bg, 224, 200);
    lv_obj_add_flag(ui->screen_log_img_list_bg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_log_img_list_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_log_img_list_bg, &_tccard_RGB565A8_224x200);
    lv_image_set_pivot(ui->screen_log_img_list_bg, 50,50);
    lv_image_set_rotation(ui->screen_log_img_list_bg, 0);

    //Write style for screen_log_img_list_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_log_img_list_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_log_img_list_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_log_label_14
    ui->screen_log_label_14 = lv_label_create(ui->screen_log);
    lv_obj_set_pos(ui->screen_log_label_14, 56, 119);
    lv_obj_set_size(ui->screen_log_label_14, 131, 44);
    lv_obj_add_flag(ui->screen_log_label_14, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_log_label_14, "暂无录音标记数据超限");
    lv_label_set_long_mode(ui->screen_log_label_14, LV_LABEL_LONG_WRAP);

    //Write style for screen_log_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_log_label_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_log_label_14, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_log_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_log_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_log_label_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_log_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_log_label_15
    ui->screen_log_label_15 = lv_label_create(ui->screen_log);
    lv_obj_set_pos(ui->screen_log_label_15, 55, 10);
    lv_obj_set_size(ui->screen_log_label_15, 130, 30);
    lv_obj_add_flag(ui->screen_log_label_15, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_log_label_15, "提醒里程");
    lv_label_set_long_mode(ui->screen_log_label_15, LV_LABEL_LONG_WRAP);

    //Write style for screen_log_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_log_label_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_log_label_15, &lv_font_PingFangMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_log_label_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_log_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_log_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_log_label_16
    ui->screen_log_label_16 = lv_label_create(ui->screen_log);
    lv_obj_set_pos(ui->screen_log_label_16, 55, 10);
    lv_obj_set_size(ui->screen_log_label_16, 130, 30);
    lv_label_set_text(ui->screen_log_label_16, "添乘记录");
    lv_label_set_long_mode(ui->screen_log_label_16, LV_LABEL_LONG_WRAP);

    //Write style for screen_log_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_log_label_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_log_label_16, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_log_label_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_log_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_log_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_log.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_log);

    //Init events for screen.
    events_init_screen_log(ui);
}
