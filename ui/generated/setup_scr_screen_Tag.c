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



void setup_scr_screen_Tag(lv_ui *ui)
{
    //Write codes screen_Tag
    ui->screen_Tag = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_Tag, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_Tag, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_Tag, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_Tag, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_Tag, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_Tag, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_img_bg
    ui->screen_Tag_img_bg = lv_image_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_Tag_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_Tag_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_Tag_img_bg, &_markbg_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_Tag_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_Tag_img_bg, 0);

    //Write style for screen_Tag_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_Tag_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_Tag_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_img_back
    ui->screen_Tag_img_back = lv_image_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_img_back, 92, 214);
    lv_obj_set_size(ui->screen_Tag_img_back, 50, 50);
    lv_obj_add_flag(ui->screen_Tag_img_back, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_Tag_img_back, &_close_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_Tag_img_back, 50,50);
    lv_image_set_rotation(ui->screen_Tag_img_back, 0);

    //Write style for screen_Tag_img_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_Tag_img_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_Tag_img_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_label_type
    ui->screen_Tag_label_type = lv_label_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_label_type, 34, 49);
    lv_obj_set_size(ui->screen_Tag_label_type, 104, 27);
    lv_label_set_text(ui->screen_Tag_label_type, "A键标记");
    lv_label_set_long_mode(ui->screen_Tag_label_type, LV_LABEL_LONG_WRAP);

    //Write style for screen_Tag_label_type, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Tag_label_type, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Tag_label_type, &lv_font_PingFangHeavy_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Tag_label_type, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Tag_label_type, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Tag_label_type, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_label_time
    ui->screen_Tag_label_time = lv_label_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_label_time, 154, 52);
    lv_obj_set_size(ui->screen_Tag_label_time, 61, 26);
    lv_label_set_text(ui->screen_Tag_label_time, "12 : 20");
    lv_label_set_long_mode(ui->screen_Tag_label_time, LV_LABEL_LONG_WRAP);

    //Write style for screen_Tag_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Tag_label_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Tag_label_time, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Tag_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Tag_label_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Tag_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_label_speed
    ui->screen_Tag_label_speed = lv_label_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_label_speed, 34, 160);
    lv_obj_set_size(ui->screen_Tag_label_speed, 153, 18);
    lv_label_set_text(ui->screen_Tag_label_speed, "120 km/h");
    lv_label_set_long_mode(ui->screen_Tag_label_speed, LV_LABEL_LONG_WRAP);

    //Write style for screen_Tag_label_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Tag_label_speed, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Tag_label_speed, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Tag_label_speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Tag_label_speed, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Tag_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_label_linenum_mileage
    ui->screen_Tag_label_linenum_mileage = lv_label_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_label_linenum_mileage, 34, 109);
    lv_obj_set_size(ui->screen_Tag_label_linenum_mileage, 153, 30);
    lv_label_set_text(ui->screen_Tag_label_linenum_mileage, "K1111+236");
    lv_label_set_long_mode(ui->screen_Tag_label_linenum_mileage, LV_LABEL_LONG_WRAP);

    //Write style for screen_Tag_label_linenum_mileage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Tag_label_linenum_mileage, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Tag_label_linenum_mileage, &lv_font_PingFangHeavy_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Tag_label_linenum_mileage, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Tag_label_linenum_mileage, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Tag_label_linenum_mileage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Tag_label_1
    ui->screen_Tag_label_1 = lv_label_create(ui->screen_Tag);
    lv_obj_set_pos(ui->screen_Tag_label_1, 70, 124);
    lv_obj_set_size(ui->screen_Tag_label_1, 100, 32);
    lv_obj_add_flag(ui->screen_Tag_label_1, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_Tag_label_1, "人工标记");
    lv_label_set_long_mode(ui->screen_Tag_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_Tag_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Tag_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Tag_label_1, &lv_font_PingFangHeavy_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Tag_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Tag_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Tag_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_Tag.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_Tag);

    //Init events for screen.
    events_init_screen_Tag(ui);
}
