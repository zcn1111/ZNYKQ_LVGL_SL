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



void setup_scr_screen_warn2(lv_ui *ui)
{
    //Write codes screen_warn2
    ui->screen_warn2 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_warn2, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_warn2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_warn2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_warn2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_warn2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_warn2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_img_bg
    ui->screen_warn2_img_bg = lv_image_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_warn2_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_warn2_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_warn2_img_bg, &_2bg_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_warn2_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_warn2_img_bg, 0);

    //Write style for screen_warn2_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_warn2_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_warn2_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_img_back
    ui->screen_warn2_img_back = lv_image_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_img_back, 149, 212);
    lv_obj_set_size(ui->screen_warn2_img_back, 50, 50);
    lv_obj_add_flag(ui->screen_warn2_img_back, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_warn2_img_back, &_close_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_warn2_img_back, 50,50);
    lv_image_set_rotation(ui->screen_warn2_img_back, 0);

    //Write style for screen_warn2_img_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_warn2_img_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_warn2_img_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_speed
    ui->screen_warn2_label_speed = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_speed, 135, 174);
    lv_obj_set_size(ui->screen_warn2_label_speed, 92, 18);
    lv_label_set_text(ui->screen_warn2_label_speed, "120 km/h");
    lv_label_set_long_mode(ui->screen_warn2_label_speed, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_speed, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_speed, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_speed, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_num_mileage
    ui->screen_warn2_label_num_mileage = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_num_mileage, 24, 174);
    lv_obj_set_size(ui->screen_warn2_label_num_mileage, 105, 20);
    lv_label_set_text(ui->screen_warn2_label_num_mileage, "K1111+236");
    lv_label_set_long_mode(ui->screen_warn2_label_num_mileage, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_num_mileage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_num_mileage, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_num_mileage, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_num_mileage, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_num_mileage, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_num_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_L_data
    ui->screen_warn2_label_L_data = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_L_data, 125, 127);
    lv_obj_set_size(ui->screen_warn2_label_L_data, 64, 30);
    lv_label_set_text(ui->screen_warn2_label_L_data, "3");
    lv_label_set_long_mode(ui->screen_warn2_label_L_data, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_L_data, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_L_data, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_L_data, &lv_font_PingFangHeavy_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_L_data, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_L_data, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_L_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_L_level
    ui->screen_warn2_label_L_level = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_L_level, 162, 100);
    lv_obj_set_size(ui->screen_warn2_label_L_level, 15, 17);
    lv_label_set_text(ui->screen_warn2_label_L_level, "I");
    lv_label_set_long_mode(ui->screen_warn2_label_L_level, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_L_level, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_L_level, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_L_level, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_L_level, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_L_level, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_L_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_typel
    ui->screen_warn2_label_typel = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_typel, 137, 100);
    lv_obj_set_size(ui->screen_warn2_label_typel, 17, 20);
    lv_label_set_text(ui->screen_warn2_label_typel, "水");
    lv_label_set_long_mode(ui->screen_warn2_label_typel, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_typel, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_typel, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_typel, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_typel, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_typel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_typel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_V_data
    ui->screen_warn2_label_V_data = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_V_data, 45, 127);
    lv_obj_set_size(ui->screen_warn2_label_V_data, 64, 30);
    lv_label_set_text(ui->screen_warn2_label_V_data, "15");
    lv_label_set_long_mode(ui->screen_warn2_label_V_data, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_V_data, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_V_data, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_V_data, &lv_font_PingFangHeavy_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_V_data, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_V_data, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_V_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_V_level
    ui->screen_warn2_label_V_level = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_V_level, 82, 100);
    lv_obj_set_size(ui->screen_warn2_label_V_level, 15, 17);
    lv_label_set_text(ui->screen_warn2_label_V_level, "II");
    lv_label_set_long_mode(ui->screen_warn2_label_V_level, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_V_level, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_V_level, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_V_level, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_V_level, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_V_level, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_V_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_typev
    ui->screen_warn2_label_typev = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_typev, 59, 100);
    lv_obj_set_size(ui->screen_warn2_label_typev, 17, 20);
    lv_label_set_text(ui->screen_warn2_label_typev, "垂");
    lv_label_set_long_mode(ui->screen_warn2_label_typev, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_typev, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_typev, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_typev, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_typev, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_typev, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_typev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_time
    ui->screen_warn2_label_time = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_time, 129, 55);
    lv_obj_set_size(ui->screen_warn2_label_time, 61, 26);
    lv_label_set_text(ui->screen_warn2_label_time, "12 : 20");
    lv_label_set_long_mode(ui->screen_warn2_label_time, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_time, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_type
    ui->screen_warn2_label_type = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_type, 24, 50);
    lv_obj_set_size(ui->screen_warn2_label_type, 99, 25);
    lv_label_set_text(ui->screen_warn2_label_type, "二级报警");
    lv_label_set_long_mode(ui->screen_warn2_label_type, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_type, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_type, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_type, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_type, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_type, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_img_voic
    ui->screen_warn2_img_voic = lv_image_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_img_voic, 45, 205);
    lv_obj_set_size(ui->screen_warn2_img_voic, 60, 60);
    lv_obj_add_flag(ui->screen_warn2_img_voic, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_warn2_img_voic, &_volum_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_warn2_img_voic, 50,50);
    lv_image_set_rotation(ui->screen_warn2_img_voic, 0);

    //Write style for screen_warn2_img_voic, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_warn2_img_voic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_warn2_img_voic, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_label_tip
    ui->screen_warn2_label_tip = lv_label_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_label_tip, 38, 7);
    lv_obj_set_size(ui->screen_warn2_label_tip, 168, 21);
    lv_obj_add_flag(ui->screen_warn2_label_tip, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_warn2_label_tip, "录音标记中...");
    lv_label_set_long_mode(ui->screen_warn2_label_tip, LV_LABEL_LONG_WRAP);

    //Write style for screen_warn2_label_tip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_warn2_label_tip, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_warn2_label_tip, lv_color_hex(0xfbfd46), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_warn2_label_tip, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_warn2_label_tip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_warn2_label_tip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_warn2_label_tip, 74, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_warn2_label_tip, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_warn2_label_tip, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_warn2_label_tip, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_warn2_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_warn2_img_voicing
    ui->screen_warn2_img_voicing = lv_image_create(ui->screen_warn2);
    lv_obj_set_pos(ui->screen_warn2_img_voicing, 45, 205);
    lv_obj_set_size(ui->screen_warn2_img_voicing, 60, 60);
    lv_obj_add_flag(ui->screen_warn2_img_voicing, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_warn2_img_voicing, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_warn2_img_voicing, &_volum2_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_warn2_img_voicing, 50,50);
    lv_image_set_rotation(ui->screen_warn2_img_voicing, 0);

    //Write style for screen_warn2_img_voicing, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_warn2_img_voicing, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_warn2_img_voicing, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_warn2.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_warn2);

    //Init events for screen.
    events_init_screen_warn2(ui);
}
