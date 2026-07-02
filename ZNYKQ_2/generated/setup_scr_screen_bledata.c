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



void setup_scr_screen_bledata(lv_ui *ui)
{
    //Write codes screen_bledata
    ui->screen_bledata = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_bledata, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_bledata, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_bledata, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bledata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_img_1
    ui->screen_bledata_img_1 = lv_image_create(ui->screen_bledata);
    lv_obj_set_pos(ui->screen_bledata_img_1, 0, 0);
    lv_obj_set_size(ui->screen_bledata_img_1, 240, 280);
    lv_obj_add_flag(ui->screen_bledata_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_bledata_img_1, &_bg_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_bledata_img_1, 50,50);
    lv_image_set_rotation(ui->screen_bledata_img_1, 0);

    //Write style for screen_bledata_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_bledata_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_bledata_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_cont_ble_data
    ui->screen_bledata_cont_ble_data = lv_obj_create(ui->screen_bledata);
    lv_obj_set_pos(ui->screen_bledata_cont_ble_data, 10, 60);
    lv_obj_set_size(ui->screen_bledata_cont_ble_data, 219, 210);
    lv_obj_set_scrollbar_mode(ui->screen_bledata_cont_ble_data, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_bledata_cont_ble_data, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_bledata_cont_ble_data, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_bledata_cont_ble_data, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_bledata_cont_ble_data, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_bledata_cont_ble_data, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bledata_cont_ble_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_label_blemac
    ui->screen_bledata_label_blemac = lv_label_create(ui->screen_bledata_cont_ble_data);
    lv_obj_set_pos(ui->screen_bledata_label_blemac, 25, 76);
    lv_obj_set_size(ui->screen_bledata_label_blemac, 75, 20);
    lv_label_set_text(ui->screen_bledata_label_blemac, "蓝牙MAC\n");
    lv_label_set_long_mode(ui->screen_bledata_label_blemac, LV_LABEL_LONG_WRAP);

    //Write style for screen_bledata_label_blemac, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_bledata_label_blemac, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_bledata_label_blemac, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_bledata_label_blemac, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_bledata_label_blemac, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bledata_label_blemac, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_label_blename
    ui->screen_bledata_label_blename = lv_label_create(ui->screen_bledata_cont_ble_data);
    lv_obj_set_pos(ui->screen_bledata_label_blename, 25, 10);
    lv_obj_set_size(ui->screen_bledata_label_blename, 75, 20);
    lv_label_set_text(ui->screen_bledata_label_blename, "蓝牙名称\n");
    lv_label_set_long_mode(ui->screen_bledata_label_blename, LV_LABEL_LONG_WRAP);

    //Write style for screen_bledata_label_blename, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_bledata_label_blename, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_bledata_label_blename, &lv_font_PingFangHeavy_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_bledata_label_blename, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_bledata_label_blename, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bledata_label_blename, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_label_blenamedata
    ui->screen_bledata_label_blenamedata = lv_label_create(ui->screen_bledata_cont_ble_data);
    lv_obj_set_pos(ui->screen_bledata_label_blenamedata, 25, 39);
    lv_obj_set_size(ui->screen_bledata_label_blenamedata, 184, 20);
    lv_label_set_text(ui->screen_bledata_label_blenamedata, "xxxxxxxxxxxx");
    lv_label_set_long_mode(ui->screen_bledata_label_blenamedata, LV_LABEL_LONG_WRAP);

    //Write style for screen_bledata_label_blenamedata, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_bledata_label_blenamedata, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_bledata_label_blenamedata, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_bledata_label_blenamedata, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_bledata_label_blenamedata, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bledata_label_blenamedata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_label_blemacdata
    ui->screen_bledata_label_blemacdata = lv_label_create(ui->screen_bledata_cont_ble_data);
    lv_obj_set_pos(ui->screen_bledata_label_blemacdata, 25, 104);
    lv_obj_set_size(ui->screen_bledata_label_blemacdata, 185, 20);
    lv_label_set_text(ui->screen_bledata_label_blemacdata, "xxxxxxxxxxxx\n");
    lv_label_set_long_mode(ui->screen_bledata_label_blemacdata, LV_LABEL_LONG_WRAP);

    //Write style for screen_bledata_label_blemacdata, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_bledata_label_blemacdata, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_bledata_label_blemacdata, &lv_font_PingFangHeavy_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_bledata_label_blemacdata, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_bledata_label_blemacdata, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bledata_label_blemacdata, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bledata_label_title
    ui->screen_bledata_label_title = lv_label_create(ui->screen_bledata);
    lv_obj_set_pos(ui->screen_bledata_label_title, 55, 10);
    lv_obj_set_size(ui->screen_bledata_label_title, 130, 30);
    lv_label_set_text(ui->screen_bledata_label_title, "蓝牙信息");
    lv_label_set_long_mode(ui->screen_bledata_label_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_bledata_label_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_bledata_label_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_bledata_label_title, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_bledata_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_bledata_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bledata_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_bledata.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_bledata);

    //Init events for screen.
    events_init_screen_bledata(ui);
}
