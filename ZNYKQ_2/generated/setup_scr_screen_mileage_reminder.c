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
#include "app_mileage_reminder_list.h"



void setup_scr_screen_mileage_reminder(lv_ui *ui)
{
    //Write codes screen_mileage_reminder
    ui->screen_mileage_reminder = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_mileage_reminder, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_mileage_reminder, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_mileage_reminder, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mileage_reminder, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mileage_reminder, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mileage_reminder, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mileage_reminder_img_bg
    ui->screen_mileage_reminder_img_bg = lv_image_create(ui->screen_mileage_reminder);
    lv_obj_set_pos(ui->screen_mileage_reminder_img_bg, 0, 0);
    lv_obj_set_size(ui->screen_mileage_reminder_img_bg, 240, 280);
    lv_obj_add_flag(ui->screen_mileage_reminder_img_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_mileage_reminder_img_bg, &_bg_RGB565A8_240x280);
    lv_image_set_pivot(ui->screen_mileage_reminder_img_bg, 50,50);
    lv_image_set_rotation(ui->screen_mileage_reminder_img_bg, 0);

    //Write style for screen_mileage_reminder_img_bg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_mileage_reminder_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_mileage_reminder_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mileage_reminder_label_3
    ui->screen_mileage_reminder_label_3 = lv_label_create(ui->screen_mileage_reminder);
    lv_obj_set_pos(ui->screen_mileage_reminder_label_3, 55, 10);
    lv_obj_set_size(ui->screen_mileage_reminder_label_3, 130, 30);
    lv_label_set_text(ui->screen_mileage_reminder_label_3, "提醒里程");
    lv_label_set_long_mode(ui->screen_mileage_reminder_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_mileage_reminder_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mileage_reminder_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mileage_reminder_label_3, &lv_font_PingFangHeavy_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mileage_reminder_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mileage_reminder_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mileage_reminder_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mileage_reminder_cont_list
    ui->screen_mileage_reminder_cont_list = lv_obj_create(ui->screen_mileage_reminder);
    lv_obj_set_pos(ui->screen_mileage_reminder_cont_list, 5, 47);
    lv_obj_set_size(ui->screen_mileage_reminder_cont_list, 230, 227);
    lv_obj_set_scrollbar_mode(ui->screen_mileage_reminder_cont_list, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_mileage_reminder_cont_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_mileage_reminder_cont_list, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_mileage_reminder_cont_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_mileage_reminder_cont_list, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_mileage_reminder_cont_list, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mileage_reminder_cont_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_mileage_reminder.
    ui_mileage_reminder_render_default(ui);

    //Update current screen layout.
    lv_obj_update_layout(ui->screen_mileage_reminder);

    //Init events for screen.
    events_init_screen_mileage_reminder(ui);
}
