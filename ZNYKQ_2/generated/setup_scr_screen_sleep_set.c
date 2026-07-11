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



void setup_scr_screen_sleep_set(lv_ui *ui)
{
    //Write codes screen_sleep_set
    ui->screen_sleep_set = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_sleep_set, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_sleep_set, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_sleep_set, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_sleep_set, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_sleep_set_img_bg
    ui->screen_sleep_set_img_bg = lv_image_create(ui->screen_sleep_set);
    lv_obj_set_pos(ui->screen_sleep_set_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_sleep_set_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_sleep_set_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_sleep_set_img_bg, &_bg_new_RGB565_240x280_tresh);
    lv_image_set_pivot(ui->screen_sleep_set_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_sleep_set_img_bg, 0);

    //Write style for screen_sleep_set_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_sleep_set_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_sleep_set_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_sleep_set_cont_data
    ui->screen_sleep_set_cont_data = lv_obj_create(ui->screen_sleep_set);
    lv_obj_set_pos(ui->screen_sleep_set_cont_data, 6, 48);
    lv_obj_set_size(ui->screen_sleep_set_cont_data, 230, 227);
    lv_obj_set_scrollbar_mode(ui->screen_sleep_set_cont_data, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_sleep_set_cont_data, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_sleep_set_cont_data, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_sleep_set_cont_data, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_sleep_set_cont_data, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_sleep_set_cont_data, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sleep_set_cont_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_sleep_set_label_1
    ui->screen_sleep_set_label_1 = lv_label_create(ui->screen_sleep_set_cont_data);
    lv_obj_set_pos(ui->screen_sleep_set_label_1, 49, 132);
    lv_obj_set_size(ui->screen_sleep_set_label_1, 144, 21);
    lv_label_set_text(ui->screen_sleep_set_label_1, "无操作间隔时间");
    lv_label_set_long_mode(ui->screen_sleep_set_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_sleep_set_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_sleep_set_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sleep_set_label_1, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_sleep_set_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_sleep_set_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sleep_set_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_sleep_set_label_title
    ui->screen_sleep_set_label_title = lv_label_create(ui->screen_sleep_set);
    lv_obj_set_pos(ui->screen_sleep_set_label_title, 55, 10);
    lv_obj_set_size(ui->screen_sleep_set_label_title, 130, 30);
    lv_label_set_text(ui->screen_sleep_set_label_title, "休眠设置");
    lv_label_set_long_mode(ui->screen_sleep_set_label_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_sleep_set_label_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_sleep_set_label_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sleep_set_label_title, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_sleep_set_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_sleep_set_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sleep_set_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_sleep_set_ddlist_1
    ui->screen_sleep_set_ddlist_1 = lv_dropdown_create(ui->screen_sleep_set);
    lv_obj_set_pos(ui->screen_sleep_set_ddlist_1, 66, 93);
    lv_obj_set_size(ui->screen_sleep_set_ddlist_1, 124, 31);
    lv_dropdown_set_options(ui->screen_sleep_set_ddlist_1, "不休眠\n15秒\n30秒\n1分钟\n2分钟\n10分钟");

    //Write style for screen_sleep_set_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_sleep_set_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sleep_set_ddlist_1, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_sleep_set_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sleep_set_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_sleep_set_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_sleep_set_ddlist_1, lv_color_hex(0xe1e6ee), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_sleep_set_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_sleep_set_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_sleep_set_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_sleep_set_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sleep_set_ddlist_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sleep_set_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sleep_set_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sleep_set_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sleep_set_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_sleep_set_ddlist_1_extra_list_selected_checked
    static lv_style_t style_screen_sleep_set_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_sleep_set_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_sleep_set_ddlist_1), &style_screen_sleep_set_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style for screen_sleep_set_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_sleep_set_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sleep_set_ddlist_1, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_sleep_set_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sleep_set_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_sleep_set_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_sleep_set_ddlist_1, lv_color_hex(0xe1e6ee), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_sleep_set_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_sleep_set_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_sleep_set_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_sleep_set_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sleep_set_ddlist_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sleep_set_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sleep_set_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sleep_set_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sleep_set_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_sleep_set_ddlist_1), &style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //The custom code of screen_sleep_set.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_sleep_set);

    //Init events for screen.
    events_init_screen_sleep_set(ui);
}
