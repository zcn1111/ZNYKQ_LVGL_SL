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



void setup_scr_screen_main(lv_ui *ui)
{
    //Write codes screen_main
    ui->screen_main = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_main, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_bg
    ui->screen_main_img_bg = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_main_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_main_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_bg, &_mainbg_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_main_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_main_img_bg, 0);

    //Write style for screen_main_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_bledisc
    ui->screen_main_img_bledisc = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_bledisc, 15, 14);
    lv_obj_set_size(ui->screen_main_img_bledisc, 23, 23);
    lv_obj_add_flag(ui->screen_main_img_bledisc, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_bledisc, &_ly_disconnected_RGB565A8_23x23);
    lv_image_set_pivot(ui->screen_main_img_bledisc, 50,50);
    lv_image_set_rotation(ui->screen_main_img_bledisc, 0);

    //Write style for screen_main_img_bledisc, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_bledisc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_bledisc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_blecon
    ui->screen_main_img_blecon = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_blecon, 15, 14);
    lv_obj_set_size(ui->screen_main_img_blecon, 23, 23);
    lv_obj_add_flag(ui->screen_main_img_blecon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_blecon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_blecon, &_ly_connected_RGB565A8_23x23);
    lv_image_set_pivot(ui->screen_main_img_blecon, 50,50);
    lv_image_set_rotation(ui->screen_main_img_blecon, 0);

    //Write style for screen_main_img_blecon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_blecon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_blecon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_time
    ui->screen_main_label_time = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_time, 68, 14);
    lv_obj_set_size(ui->screen_main_label_time, 100, 23);
    lv_label_set_text(ui->screen_main_label_time, "12:20");
    lv_label_set_long_mode(ui->screen_main_label_time, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_time, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_time, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_tip
    ui->screen_main_label_tip = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_tip, 48, 53);
    lv_obj_set_size(ui->screen_main_label_tip, 161, 25);
    lv_label_set_text(ui->screen_main_label_tip, "蓝牙未连接，请靠近目标设备");
    lv_label_set_long_mode(ui->screen_main_label_tip, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for screen_main_label_tip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_tip, lv_color_hex(0xfbfd46), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_tip, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_tip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_tip, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_tip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_linename
    ui->screen_main_label_linename = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_linename, 16, 98);
    lv_obj_set_size(ui->screen_main_label_linename, 197, 27);
    lv_obj_add_flag(ui->screen_main_label_linename, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_linename, "大西客专上下  上");
    lv_label_set_long_mode(ui->screen_main_label_linename, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_linename, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_linename, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_linename, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_linename, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_linename, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_linename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_linenum_mileage
    ui->screen_main_label_linenum_mileage = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_linenum_mileage, 16, 131);
    lv_obj_set_size(ui->screen_main_label_linenum_mileage, 206, 30);
    lv_obj_add_flag(ui->screen_main_label_linenum_mileage, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_linenum_mileage, "K-1111+236A");
    lv_label_set_long_mode(ui->screen_main_label_linenum_mileage, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_linenum_mileage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_linenum_mileage, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_linenum_mileage, &lv_font_PingFangHeavy_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_linenum_mileage, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_linenum_mileage, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_speed
    ui->screen_main_label_speed = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_speed, 16, 168);
    lv_obj_set_size(ui->screen_main_label_speed, 137, 21);
    lv_obj_add_flag(ui->screen_main_label_speed, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_speed, "120 km/h");
    lv_label_set_long_mode(ui->screen_main_label_speed, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_speed, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_speed, &lv_font_PingFangHeavy_23, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_speed, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_mask1
    ui->screen_main_label_mask1 = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_mask1, 25, 131);
    lv_obj_set_size(ui->screen_main_label_mask1, 180, 44);
    lv_label_set_text(ui->screen_main_label_mask1, "无数据");
    lv_label_set_long_mode(ui->screen_main_label_mask1, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_mask1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_mask1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_mask1, &lv_font_PingFangMedium_23, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_mask1, 193, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_mask1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_mask1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battery100
    ui->screen_main_img_battery100 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battery100, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battery100, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battery100, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battery100, &_100_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battery100, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battery100, 0);

    //Write style for screen_main_img_battery100, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battery100, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battery100, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battery80
    ui->screen_main_img_battery80 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battery80, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battery80, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battery80, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_battery80, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battery80, &_80_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battery80, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battery80, 0);

    //Write style for screen_main_img_battery80, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battery80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battery80, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battery60
    ui->screen_main_img_battery60 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battery60, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battery60, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battery60, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_battery60, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battery60, &_60_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battery60, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battery60, 0);

    //Write style for screen_main_img_battery60, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battery60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battery60, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battert50
    ui->screen_main_img_battert50 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battert50, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battert50, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battert50, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_battert50, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battert50, &_50_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battert50, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battert50, 0);

    //Write style for screen_main_img_battert50, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battert50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battert50, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battery40
    ui->screen_main_img_battery40 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battery40, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battery40, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battery40, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_battery40, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battery40, &_40_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battery40, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battery40, 0);

    //Write style for screen_main_img_battery40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battery40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battery40, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battery20
    ui->screen_main_img_battery20 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battery20, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battery20, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battery20, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_battery20, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battery20, &_20_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battery20, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battery20, 0);

    //Write style for screen_main_img_battery20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battery20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battery20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_battery10
    ui->screen_main_img_battery10 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_battery10, 195, 14);
    lv_obj_set_size(ui->screen_main_img_battery10, 30, 20);
    lv_obj_add_flag(ui->screen_main_img_battery10, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_battery10, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_battery10, &_10_RGB565A8_30x20);
    lv_image_set_pivot(ui->screen_main_img_battery10, 50,50);
    lv_image_set_rotation(ui->screen_main_img_battery10, 0);

    //Write style for screen_main_img_battery10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_battery10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_battery10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_voickey
    ui->screen_main_img_voickey = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_voickey, 134, 197);
    lv_obj_set_size(ui->screen_main_img_voickey, 75, 75);
    lv_obj_add_flag(ui->screen_main_img_voickey, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_voickey, &_volum_RGB565A8_75x75);
    lv_image_set_pivot(ui->screen_main_img_voickey, 50,50);
    lv_image_set_rotation(ui->screen_main_img_voickey, 0);

    //Write style for screen_main_img_voickey, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_voickey, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_voickey, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_play_run
    ui->screen_main_img_play_run = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_play_run, 32, 197);
    lv_obj_set_size(ui->screen_main_img_play_run, 75, 75);
    lv_obj_add_flag(ui->screen_main_img_play_run, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_play_run, &_play_RGB565A8_75x75);
    lv_image_set_pivot(ui->screen_main_img_play_run, 50,50);
    lv_image_set_rotation(ui->screen_main_img_play_run, 0);

    //Write style for screen_main_img_play_run, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_play_run, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_play_run, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_play_stop
    ui->screen_main_img_play_stop = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_play_stop, 32, 197);
    lv_obj_set_size(ui->screen_main_img_play_stop, 75, 75);
    lv_obj_add_flag(ui->screen_main_img_play_stop, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_play_stop, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_play_stop, &_pause_RGB565A8_75x75);
    lv_image_set_pivot(ui->screen_main_img_play_stop, 50,50);
    lv_image_set_rotation(ui->screen_main_img_play_stop, 0);

    //Write style for screen_main_img_play_stop, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_play_stop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_play_stop, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_1
    ui->screen_main_label_1 = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_1, 32, 93);
    lv_obj_set_size(ui->screen_main_label_1, 100, 32);
    lv_obj_add_flag(ui->screen_main_label_1, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_1, "前方：1234567890+设备未运行蓝牙已连接");
    lv_label_set_long_mode(ui->screen_main_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_1, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_tip2
    ui->screen_main_label_tip2 = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_tip2, 48, 53);
    lv_obj_set_size(ui->screen_main_label_tip2, 161, 25);
    lv_obj_add_flag(ui->screen_main_label_tip2, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_tip2, "录音标记中，请讲话");
    lv_label_set_long_mode(ui->screen_main_label_tip2, LV_LABEL_LONG_SCROLL);

    //Write style for screen_main_label_tip2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_tip2, lv_color_hex(0xfbfd46), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_tip2, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_tip2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_tip2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_tip2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_2
    ui->screen_main_label_2 = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_2, 25, 98);
    lv_obj_set_size(ui->screen_main_label_2, 171, 32);
    lv_obj_add_flag(ui->screen_main_label_2, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_2, "设备未运行");
    lv_label_set_long_mode(ui->screen_main_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_2, &lv_font_PingFangHeavy_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_kjjm
    ui->screen_main_img_kjjm = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_kjjm, 0, 0);
    lv_obj_set_size(ui->screen_main_img_kjjm, 240, 280);
    lv_obj_add_flag(ui->screen_main_img_kjjm, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_kjjm, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_kjjm, &_kjjm_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_main_img_kjjm, 50,50);
    lv_image_set_rotation(ui->screen_main_img_kjjm, 0);

    //Write style for screen_main_img_kjjm, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_kjjm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_kjjm, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_img_voickey2
    ui->screen_main_img_voickey2 = lv_image_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_img_voickey2, 134, 197);
    lv_obj_set_size(ui->screen_main_img_voickey2, 75, 75);
    lv_obj_add_flag(ui->screen_main_img_voickey2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_main_img_voickey2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_main_img_voickey2, &_volum2_RGB565A8_75x75);
    lv_image_set_pivot(ui->screen_main_img_voickey2, 50,50);
    lv_image_set_rotation(ui->screen_main_img_voickey2, 0);

    //Write style for screen_main_img_voickey2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_main_img_voickey2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_main_img_voickey2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main_label_audio_toast
    ui->screen_main_label_audio_toast = lv_label_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_main_label_audio_toast, 0, 0);
    lv_obj_set_size(ui->screen_main_label_audio_toast, 240, 34);
    lv_obj_add_flag(ui->screen_main_label_audio_toast, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_main_label_audio_toast, "上一次录音处理中请稍后");
    lv_label_set_long_mode(ui->screen_main_label_audio_toast, LV_LABEL_LONG_WRAP);

    //Write style for screen_main_label_audio_toast, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main_label_audio_toast, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_main_label_audio_toast, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_main_label_audio_toast, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_main_label_audio_toast, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_main_label_audio_toast, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main_label_audio_toast, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_main_label_audio_toast, lv_color_hex(0xfdd000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main_label_audio_toast, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main_label_audio_toast, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main_label_audio_toast, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_main.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_main);

    //Init events for screen.
    events_init_screen_main(ui);
}
