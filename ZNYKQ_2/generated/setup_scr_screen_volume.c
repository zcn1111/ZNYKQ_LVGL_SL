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



void setup_scr_screen_volume(lv_ui *ui)
{
    //Write codes screen_volume
    ui->screen_volume = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_volume, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_volume, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_volume, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_volume_img_1
    ui->screen_volume_img_1 = lv_image_create(ui->screen_volume);
    lv_obj_set_pos(ui->screen_volume_img_1, 0, 0);
    lv_obj_set_size(ui->screen_volume_img_1, 240, 280);
    lv_obj_add_flag(ui->screen_volume_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_volume_img_1, &_bg_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_volume_img_1, 50,50);
    lv_image_set_rotation(ui->screen_volume_img_1, 0);

    //Write style for screen_volume_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_volume_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_volume_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_volume_cont_list
    ui->screen_volume_cont_list = lv_obj_create(ui->screen_volume);
    lv_obj_set_pos(ui->screen_volume_cont_list, 11, 61);
    lv_obj_set_size(ui->screen_volume_cont_list, 219, 210);
    lv_obj_set_scrollbar_mode(ui->screen_volume_cont_list, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_volume_cont_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_volume_cont_list, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_volume_cont_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_volume_cont_list, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_volume_cont_list, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_volume_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_volume_slider_1
    ui->screen_volume_slider_1 = lv_slider_create(ui->screen_volume_cont_list);
    lv_obj_set_pos(ui->screen_volume_slider_1, 28, 30);
    lv_obj_set_size(ui->screen_volume_slider_1, 154, 40);
    lv_slider_set_range(ui->screen_volume_slider_1, 0, 100);
    lv_slider_set_mode(ui->screen_volume_slider_1, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_volume_slider_1, 100, LV_ANIM_OFF);

    //Write style for screen_volume_slider_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_volume_slider_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_volume_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_volume_slider_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_slider_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_volume_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_volume_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_volume_slider_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_volume_slider_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_volume_slider_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_volume_slider_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_slider_1, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_volume_slider_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_volume_slider_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_volume_slider_1, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_volume_slider_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_slider_1, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_volume_label_vol_num
    ui->screen_volume_label_vol_num = lv_label_create(ui->screen_volume_cont_list);
    lv_obj_set_pos(ui->screen_volume_label_vol_num, 133, 113);
    lv_obj_set_size(ui->screen_volume_label_vol_num, 54, 16);
    lv_label_set_text(ui->screen_volume_label_vol_num, "100");
    lv_label_set_long_mode(ui->screen_volume_label_vol_num, LV_LABEL_LONG_WRAP);

    //Write style for screen_volume_label_vol_num, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_volume_label_vol_num, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_volume_label_vol_num, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_volume_label_vol_num, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_volume_label_vol_num, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_volume_label_vol_num, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_volume_label_1
    ui->screen_volume_label_1 = lv_label_create(ui->screen_volume_cont_list);
    lv_obj_set_pos(ui->screen_volume_label_1, 29, 113);
    lv_obj_set_size(ui->screen_volume_label_1, 54, 20);
    lv_label_set_text(ui->screen_volume_label_1, "音量 ：");
    lv_label_set_long_mode(ui->screen_volume_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_volume_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_volume_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_volume_label_1, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_volume_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_volume_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_volume_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_volume_label_title
    ui->screen_volume_label_title = lv_label_create(ui->screen_volume);
    lv_obj_set_pos(ui->screen_volume_label_title, 55, 13);
    lv_obj_set_size(ui->screen_volume_label_title, 130, 30);
    lv_label_set_text(ui->screen_volume_label_title, "音量设置");
    lv_label_set_long_mode(ui->screen_volume_label_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_volume_label_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_volume_label_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_volume_label_title, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_volume_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_volume_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_volume_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_volume.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_volume);

    //Init events for screen.
    events_init_screen_volume(ui);
}
