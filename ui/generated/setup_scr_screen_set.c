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



void setup_scr_screen_set(lv_ui *ui)
{
    //Write codes screen_set
    ui->screen_set = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_set, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_set, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_set, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_img_bg
    ui->screen_set_img_bg = lv_image_create(ui->screen_set);
    lv_obj_set_pos(ui->screen_set_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_set_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_set_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_set_img_bg, &_bg_new_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_set_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_set_img_bg, 0);

    //Write style for screen_set_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_set_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_set_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_cont_6
    ui->screen_set_cont_6 = lv_obj_create(ui->screen_set);
    lv_obj_set_pos(ui->screen_set_cont_6, 5, 48);
    lv_obj_set_size(ui->screen_set_cont_6, 230, 210);
    lv_obj_set_scrollbar_mode(ui->screen_set_cont_6, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_cont_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_cont_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_cont_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_cont_6, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_cont_6, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_cont_7
    ui->screen_set_cont_7 = lv_obj_create(ui->screen_set_cont_6);
    lv_obj_set_pos(ui->screen_set_cont_7, 5, 8);
    lv_obj_set_size(ui->screen_set_cont_7, 220, 50);
    lv_obj_set_scrollbar_mode(ui->screen_set_cont_7, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_cont_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_cont_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_cont_7, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_cont_7, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_cont_7, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_cont_7, 43, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_cont_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_cont_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_cont_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_img_1
    ui->screen_set_img_1 = lv_image_create(ui->screen_set_cont_7);
    lv_obj_set_pos(ui->screen_set_img_1, 9, 6);
    lv_obj_set_size(ui->screen_set_img_1, 30, 30);
    lv_obj_add_flag(ui->screen_set_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_set_img_1, &_tb3_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_set_img_1, 50,50);
    lv_image_set_rotation(ui->screen_set_img_1, 0);

    //Write style for screen_set_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_set_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_set_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_label_8
    ui->screen_set_label_8 = lv_label_create(ui->screen_set_cont_7);
    lv_obj_set_pos(ui->screen_set_label_8, 60, 12);
    lv_obj_set_size(ui->screen_set_label_8, 99, 25);
    lv_label_set_text(ui->screen_set_label_8, "蓝牙信息");
    lv_label_set_long_mode(ui->screen_set_label_8, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_8, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_8, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_cont_8
    ui->screen_set_cont_8 = lv_obj_create(ui->screen_set_cont_6);
    lv_obj_set_pos(ui->screen_set_cont_8, 5, 247);
    lv_obj_set_size(ui->screen_set_cont_8, 220, 50);
    lv_obj_set_scrollbar_mode(ui->screen_set_cont_8, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_cont_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_cont_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_cont_8, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_cont_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_cont_8, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_cont_8, 43, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_cont_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_cont_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_img_2
    ui->screen_set_img_2 = lv_image_create(ui->screen_set_cont_8);
    lv_obj_set_pos(ui->screen_set_img_2, 9, 6);
    lv_obj_set_size(ui->screen_set_img_2, 30, 30);
    lv_obj_add_flag(ui->screen_set_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_set_img_2, &_tb1_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_set_img_2, 50,50);
    lv_image_set_rotation(ui->screen_set_img_2, 0);

    //Write style for screen_set_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_set_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_set_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_label_9
    ui->screen_set_label_9 = lv_label_create(ui->screen_set_cont_8);
    lv_obj_set_pos(ui->screen_set_label_9, 60, 12);
    lv_obj_set_size(ui->screen_set_label_9, 99, 25);
    lv_label_set_text(ui->screen_set_label_9, "关于");
    lv_label_set_long_mode(ui->screen_set_label_9, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_9, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_9, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_cont_9
    ui->screen_set_cont_9 = lv_obj_create(ui->screen_set_cont_6);
    lv_obj_set_pos(ui->screen_set_cont_9, 5, 68);
    lv_obj_set_size(ui->screen_set_cont_9, 220, 50);
    lv_obj_set_scrollbar_mode(ui->screen_set_cont_9, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_cont_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_cont_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_cont_9, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_cont_9, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_cont_9, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_cont_9, 43, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_cont_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_cont_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_img_3
    ui->screen_set_img_3 = lv_image_create(ui->screen_set_cont_9);
    lv_obj_set_pos(ui->screen_set_img_3, 9, 6);
    lv_obj_set_size(ui->screen_set_img_3, 30, 30);
    lv_obj_add_flag(ui->screen_set_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_set_img_3, &_tb4_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_set_img_3, 50,50);
    lv_image_set_rotation(ui->screen_set_img_3, 0);

    //Write style for screen_set_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_set_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_set_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_label_10
    ui->screen_set_label_10 = lv_label_create(ui->screen_set_cont_9);
    lv_obj_set_pos(ui->screen_set_label_10, 60, 12);
    lv_obj_set_size(ui->screen_set_label_10, 99, 25);
    lv_label_set_text(ui->screen_set_label_10, "音量设置");
    lv_label_set_long_mode(ui->screen_set_label_10, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_label_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_10, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_10, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_cont_10
    ui->screen_set_cont_10 = lv_obj_create(ui->screen_set_cont_6);
    lv_obj_set_pos(ui->screen_set_cont_10, 5, 188);
    lv_obj_set_size(ui->screen_set_cont_10, 220, 50);
    lv_obj_set_scrollbar_mode(ui->screen_set_cont_10, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_cont_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_cont_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_cont_10, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_cont_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_cont_10, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_cont_10, 43, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_cont_10, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_cont_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_img_4
    ui->screen_set_img_4 = lv_image_create(ui->screen_set_cont_10);
    lv_obj_set_pos(ui->screen_set_img_4, 9, 6);
    lv_obj_set_size(ui->screen_set_img_4, 30, 30);
    lv_obj_add_flag(ui->screen_set_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_set_img_4, &_tb2_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_set_img_4, 50,50);
    lv_image_set_rotation(ui->screen_set_img_4, 0);

    //Write style for screen_set_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_set_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_set_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_label_11
    ui->screen_set_label_11 = lv_label_create(ui->screen_set_cont_10);
    lv_obj_set_pos(ui->screen_set_label_11, 60, 12);
    lv_obj_set_size(ui->screen_set_label_11, 99, 25);
    lv_label_set_text(ui->screen_set_label_11, "使用说明");
    lv_label_set_long_mode(ui->screen_set_label_11, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_11, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_cont_11
    ui->screen_set_cont_11 = lv_obj_create(ui->screen_set_cont_6);
    lv_obj_set_pos(ui->screen_set_cont_11, 5, 128);
    lv_obj_set_size(ui->screen_set_cont_11, 220, 50);
    lv_obj_set_scrollbar_mode(ui->screen_set_cont_11, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_set_cont_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_cont_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_set_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_set_cont_11, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_set_cont_11, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_cont_11, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_cont_11, 43, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_set_cont_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_set_cont_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_cont_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_img_5
    ui->screen_set_img_5 = lv_image_create(ui->screen_set_cont_11);
    lv_obj_set_pos(ui->screen_set_img_5, 9, 6);
    lv_obj_set_size(ui->screen_set_img_5, 30, 30);
    lv_obj_add_flag(ui->screen_set_img_5, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_set_img_5, &_tb5_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_set_img_5, 50,50);
    lv_image_set_rotation(ui->screen_set_img_5, 0);

    //Write style for screen_set_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_set_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_set_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_label_12
    ui->screen_set_label_12 = lv_label_create(ui->screen_set_cont_11);
    lv_obj_set_pos(ui->screen_set_label_12, 60, 12);
    lv_obj_set_size(ui->screen_set_label_12, 99, 25);
    lv_label_set_text(ui->screen_set_label_12, "休眠设置");
    lv_label_set_long_mode(ui->screen_set_label_12, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_12, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_set_label_7
    ui->screen_set_label_7 = lv_label_create(ui->screen_set);
    lv_obj_set_pos(ui->screen_set_label_7, 55, 10);
    lv_obj_set_size(ui->screen_set_label_7, 130, 30);
    lv_label_set_text(ui->screen_set_label_7, "设置");
    lv_label_set_long_mode(ui->screen_set_label_7, LV_LABEL_LONG_WRAP);

    //Write style for screen_set_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_set_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_set_label_7, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_set_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_set_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_set_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_set_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_set.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_set);

    //Init events for screen.
    events_init_screen_set(ui);
}
