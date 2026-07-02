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



void setup_scr_screen_ota(lv_ui *ui)
{
    //Write codes screen_ota
    ui->screen_ota = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_ota, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_ota, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_ota, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ota_img_1
    ui->screen_ota_img_1 = lv_image_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_img_1, 0, 0);
    lv_obj_set_size(ui->screen_ota_img_1, 240, 280);
    lv_obj_add_flag(ui->screen_ota_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_ota_img_1, &_bg_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_ota_img_1, 50,50);
    lv_image_set_rotation(ui->screen_ota_img_1, 0);

    //Write style for screen_ota_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_ota_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_ota_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ota_bar_ota
    ui->screen_ota_bar_ota = lv_bar_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_bar_ota, 20, 127);
    lv_obj_set_size(ui->screen_ota_bar_ota, 201, 30);
    lv_obj_set_style_anim_duration(ui->screen_ota_bar_ota, 500, 0);
    lv_bar_set_mode(ui->screen_ota_bar_ota, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_ota_bar_ota, 0, 100);
    lv_bar_set_value(ui->screen_ota_bar_ota, 1, LV_ANIM_OFF);

    //Write style for screen_ota_bar_ota, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_ota_bar_ota, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_ota_bar_ota, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_ota_bar_ota, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_bar_ota, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ota_bar_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_ota_bar_ota, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_ota_bar_ota, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_ota_bar_ota, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_ota_bar_ota, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_bar_ota, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_ota_label_1
    ui->screen_ota_label_1 = lv_label_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_label_1, 42, 58);
    lv_obj_set_size(ui->screen_ota_label_1, 157, 32);
    lv_label_set_text(ui->screen_ota_label_1, "程序升级中");
    lv_label_set_long_mode(ui->screen_ota_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_ota_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ota_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ota_label_1, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ota_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ota_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ota_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ota_label_ota
    ui->screen_ota_label_ota = lv_label_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_label_ota, 17, 175);
    lv_obj_set_size(ui->screen_ota_label_ota, 209, 28);
    lv_label_set_text(ui->screen_ota_label_ota, "(1/10KB) 已完成");
    lv_label_set_long_mode(ui->screen_ota_label_ota, LV_LABEL_LONG_WRAP);

    //Write style for screen_ota_label_ota, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ota_label_ota, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ota_label_ota, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ota_label_ota, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ota_label_ota, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ota_label_ota, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ota_label_3
    ui->screen_ota_label_3 = lv_label_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_label_3, 51, 108);
    lv_obj_set_size(ui->screen_ota_label_3, 134, 32);
    lv_obj_add_flag(ui->screen_ota_label_3, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->screen_ota_label_3, "0123456789% 已完成正在重启");
    lv_label_set_long_mode(ui->screen_ota_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_ota_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ota_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ota_label_3, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ota_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ota_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ota_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ota_label_4
    ui->screen_ota_label_4 = lv_label_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_label_4, 68, 93);
    lv_obj_set_size(ui->screen_ota_label_4, 100, 25);
    lv_label_set_text(ui->screen_ota_label_4, "1%");
    lv_label_set_long_mode(ui->screen_ota_label_4, LV_LABEL_LONG_WRAP);

    //Write style for screen_ota_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ota_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ota_label_4, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ota_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ota_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ota_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ota_label_5
    ui->screen_ota_label_5 = lv_label_create(ui->screen_ota);
    lv_obj_set_pos(ui->screen_ota_label_5, 68, 208);
    lv_obj_set_size(ui->screen_ota_label_5, 100, 32);
    lv_label_set_text(ui->screen_ota_label_5, "正在重启");
    lv_label_set_long_mode(ui->screen_ota_label_5, LV_LABEL_LONG_WRAP);

    //Write style for screen_ota_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ota_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ota_label_5, &lv_font_PingFangHeavy_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ota_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ota_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ota_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_ota.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_ota);

}
