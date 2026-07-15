/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *screen_main;
	bool screen_main_del;
	lv_obj_t *screen_main_img_bg;
	lv_obj_t *screen_main_img_bledisc;
	lv_obj_t *screen_main_img_blecon;
	lv_obj_t *screen_main_label_time;
	lv_obj_t *screen_main_label_tip;
	lv_obj_t *screen_main_label_linename;
	lv_obj_t *screen_main_label_linenum_mileage;
	lv_obj_t *screen_main_label_speed;
	lv_obj_t *screen_main_label_mask1;
	lv_obj_t *screen_main_img_battery100;
	lv_obj_t *screen_main_img_battery80;
	lv_obj_t *screen_main_img_battery60;
	lv_obj_t *screen_main_img_battert50;
	lv_obj_t *screen_main_img_battery40;
	lv_obj_t *screen_main_img_battery20;
	lv_obj_t *screen_main_img_battery10;
	lv_obj_t *screen_main_img_voickey;
	lv_obj_t *screen_main_img_play_run;
	lv_obj_t *screen_main_img_play_stop;
	lv_obj_t *screen_main_label_1;
	lv_obj_t *screen_main_label_tip2;
	lv_obj_t *screen_main_label_2;
	lv_obj_t *screen_main_img_kjjm;
	lv_obj_t *screen_main_img_voickey2;
	lv_obj_t *screen_main_label_audio_toast;
	lv_obj_t *screen_mileage_reminder;
	bool screen_mileage_reminder_del;
	lv_obj_t *screen_mileage_reminder_img_bg;
	lv_obj_t *screen_mileage_reminder_label_3;
	lv_obj_t *screen_mileage_reminder_cont_list;
	lv_obj_t *screen_set;
	bool screen_set_del;
	lv_obj_t *screen_set_img_bg;
	lv_obj_t *screen_set_cont_6;
	lv_obj_t *screen_set_cont_7;
	lv_obj_t *screen_set_img_1;
	lv_obj_t *screen_set_label_8;
	lv_obj_t *screen_set_cont_8;
	lv_obj_t *screen_set_img_2;
	lv_obj_t *screen_set_label_9;
	lv_obj_t *screen_set_cont_9;
	lv_obj_t *screen_set_img_3;
	lv_obj_t *screen_set_label_10;
	lv_obj_t *screen_set_cont_10;
	lv_obj_t *screen_set_img_4;
	lv_obj_t *screen_set_label_11;
	lv_obj_t *screen_set_cont_11;
	lv_obj_t *screen_set_img_5;
	lv_obj_t *screen_set_label_12;
	lv_obj_t *screen_set_label_7;
	lv_obj_t *screen_log;
	bool screen_log_del;
	lv_obj_t *screen_log_img_bg;
	lv_obj_t *screen_log_cont_list;
	lv_obj_t *screen_log_img_list_bg;
	lv_obj_t *screen_log_label_14;
	lv_obj_t *screen_log_label_15;
	lv_obj_t *screen_log_label_16;
	lv_obj_t *screen_overlimt;
	bool screen_overlimt_del;
	lv_obj_t *screen_overlimt_img_bg;
	lv_obj_t *screen_overlimt_cont_list;
	lv_obj_t *screen_overlimt_btn_overlimt;
	lv_obj_t *screen_overlimt_btn_overlimt_label;
	lv_obj_t *screen_overlimt_btn_mark;
	lv_obj_t *screen_overlimt_btn_mark_label;
	lv_obj_t *screen_overlimt_btn_audiomark;
	lv_obj_t *screen_overlimt_btn_audiomark_label;
	lv_obj_t *screen_mark;
	bool screen_mark_del;
	lv_obj_t *screen_mark_img_bg;
	lv_obj_t *screen_mark_cont_list;
	lv_obj_t *screen_mark_label_14;
	lv_obj_t *screen_mark_btn_overlimt;
	lv_obj_t *screen_mark_btn_overlimt_label;
	lv_obj_t *screen_mark_btn_mark;
	lv_obj_t *screen_mark_btn_mark_label;
	lv_obj_t *screen_mark_btn_audio;
	lv_obj_t *screen_mark_btn_audio_label;
	lv_obj_t *screen_audiomark;
	bool screen_audiomark_del;
	lv_obj_t *screen_audiomark_img_bg;
	lv_obj_t *screen_audiomark_cont_list;
	lv_obj_t *screen_audiomark_cont_1;
	lv_obj_t *screen_audiomark_img_1;
	lv_obj_t *screen_audiomark_img_2;
	lv_obj_t *screen_audiomark_label_14;
	lv_obj_t *screen_audiomark_btn_overlimt;
	lv_obj_t *screen_audiomark_btn_overlimt_label;
	lv_obj_t *screen_audiomark_btn_mark;
	lv_obj_t *screen_audiomark_btn_mark_label;
	lv_obj_t *screen_audiomark_btn_audio;
	lv_obj_t *screen_audiomark_btn_audio_label;
	lv_obj_t *screen_about;
	bool screen_about_del;
	lv_obj_t *screen_about_img_bg;
	lv_obj_t *screen_about_cont_6;
	lv_obj_t *screen_about_label_hardnum;
	lv_obj_t *screen_about_label_8;
	lv_obj_t *screen_about_cont_5;
	lv_obj_t *screen_about_label_softnum;
	lv_obj_t *screen_about_label_6;
	lv_obj_t *screen_about_cont_4;
	lv_obj_t *screen_about_label_devnum;
	lv_obj_t *screen_about_label_4;
	lv_obj_t *screen_about_label_3;
	lv_obj_t *screen_Tag;
	bool screen_Tag_del;
	lv_obj_t *screen_Tag_img_bg;
	lv_obj_t *screen_Tag_img_back;
	lv_obj_t *screen_Tag_label_type;
	lv_obj_t *screen_Tag_label_time;
	lv_obj_t *screen_Tag_label_speed;
	lv_obj_t *screen_Tag_label_linenum_mileage;
	lv_obj_t *screen_Tag_label_1;
	lv_obj_t *screen_Tag_img_voic;
	lv_obj_t *screen_Tag_img_voicing;
	lv_obj_t *screen_audio_Tag;
	bool screen_audio_Tag_del;
	lv_obj_t *screen_audio_Tag_img_bg;
	lv_obj_t *screen_audio_Tag_img_back;
	lv_obj_t *screen_audio_Tag_img_audio_playing;
	lv_obj_t *screen_audio_Tag_label_speed;
	lv_obj_t *screen_audio_Tag_label_num_mileage;
	lv_obj_t *screen_audio_Tag_label_time;
	lv_obj_t *screen_audio_Tag_label_type;
	lv_obj_t *screen_audio_Tag_img_audio_play;
	lv_obj_t *screen_mark_err;
	bool screen_mark_err_del;
	lv_obj_t *screen_mark_err_img_bg;
	lv_obj_t *screen_mark_err_img_back;
	lv_obj_t *screen_mark_err_label_mark_name;
	lv_obj_t *screen_mark_err_label_time;
	lv_obj_t *screen_mark_err_label_2;
	lv_obj_t *screen_mark_err_label_3;
	lv_obj_t *screen_mark_err_label_4;
	lv_obj_t *screen_warn1;
	bool screen_warn1_del;
	lv_obj_t *screen_warn1_img_bg;
	lv_obj_t *screen_warn1_img_back;
	lv_obj_t *screen_warn1_label_speed;
	lv_obj_t *screen_warn1_label_num_mileage;
	lv_obj_t *screen_warn1_label_time;
	lv_obj_t *screen_warn1_label_type;
	lv_obj_t *screen_warn1_label_typev;
	lv_obj_t *screen_warn1_label_V_level;
	lv_obj_t *screen_warn1_label_V_data;
	lv_obj_t *screen_warn1_label_L_level;
	lv_obj_t *screen_warn1_label_typel;
	lv_obj_t *screen_warn1_label_L_data;
	lv_obj_t *screen_warn1_label_1;
	lv_obj_t *screen_warn1_label_2;
	lv_obj_t *screen_warn1_label_3;
	lv_obj_t *screen_warn1_img_voic;
	lv_obj_t *screen_warn1_label_tip;
	lv_obj_t *screen_warn1_img_voicing;
	lv_obj_t *screen_warn2;
	bool screen_warn2_del;
	lv_obj_t *screen_warn2_img_bg;
	lv_obj_t *screen_warn2_img_back;
	lv_obj_t *screen_warn2_label_speed;
	lv_obj_t *screen_warn2_label_num_mileage;
	lv_obj_t *screen_warn2_label_L_data;
	lv_obj_t *screen_warn2_label_L_level;
	lv_obj_t *screen_warn2_label_typel;
	lv_obj_t *screen_warn2_label_V_data;
	lv_obj_t *screen_warn2_label_V_level;
	lv_obj_t *screen_warn2_label_typev;
	lv_obj_t *screen_warn2_label_time;
	lv_obj_t *screen_warn2_label_type;
	lv_obj_t *screen_warn2_img_voic;
	lv_obj_t *screen_warn2_label_tip;
	lv_obj_t *screen_warn2_img_voicing;
	lv_obj_t *screen_warn3;
	bool screen_warn3_del;
	lv_obj_t *screen_warn3_img_bg;
	lv_obj_t *screen_warn3_img_back;
	lv_obj_t *screen_warn3_label_speed;
	lv_obj_t *screen_warn3_label_num_mileage;
	lv_obj_t *screen_warn3_label_L_data;
	lv_obj_t *screen_warn3_label_L_level;
	lv_obj_t *screen_warn3_label_typel;
	lv_obj_t *screen_warn3_label_V_data;
	lv_obj_t *screen_warn3_label_V_level;
	lv_obj_t *screen_warn3_label_typev;
	lv_obj_t *screen_warn3_label_time;
	lv_obj_t *screen_warn3_label_type;
	lv_obj_t *screen_warn3_img_voic;
	lv_obj_t *screen_warn3_label_tip;
	lv_obj_t *screen_warn3_img_voicing;
	lv_obj_t *screen_warn4;
	bool screen_warn4_del;
	lv_obj_t *screen_warn4_img_bg;
	lv_obj_t *screen_warn4_img_back;
	lv_obj_t *screen_warn4_label_speed;
	lv_obj_t *screen_warn4_label_num_mileage;
	lv_obj_t *screen_warn4_label_L_data;
	lv_obj_t *screen_warn4_label_L_level;
	lv_obj_t *screen_warn4_label_typel;
	lv_obj_t *screen_warn4_label_V_data;
	lv_obj_t *screen_warn4_label_V_level;
	lv_obj_t *screen_warn4_label_typev;
	lv_obj_t *screen_warn4_label_time;
	lv_obj_t *screen_warn4_label_type;
	lv_obj_t *screen_warn4_img_voic;
	lv_obj_t *screen_warn4_label_tip;
	lv_obj_t *screen_warn4_img_voicing;
	lv_obj_t *screen_volume;
	bool screen_volume_del;
	lv_obj_t *screen_volume_img_bg;
	lv_obj_t *screen_volume_cont_list;
	lv_obj_t *screen_volume_slider_1;
	lv_obj_t *screen_volume_label_vol_num;
	lv_obj_t *screen_volume_label_1;
	lv_obj_t *screen_volume_label_title;
	lv_obj_t *screen_bledata;
	bool screen_bledata_del;
	lv_obj_t *screen_bledata_img_bg;
	lv_obj_t *screen_bledata_cont_ble_data;
	lv_obj_t *screen_bledata_label_blemac;
	lv_obj_t *screen_bledata_label_blename;
	lv_obj_t *screen_bledata_label_blenamedata;
	lv_obj_t *screen_bledata_label_blemacdata;
	lv_obj_t *screen_bledata_label_title;
	lv_obj_t *screen_record;
	bool screen_record_del;
	lv_obj_t *screen_record_img_bg;
	lv_obj_t *screen_record_animimg_1;
	lv_obj_t *screen_record_label_5;
	lv_obj_t *screen_record_animimg_2;
	lv_obj_t *screen_record_label_6;
	lv_obj_t *screen_ota;
	bool screen_ota_del;
	lv_obj_t *screen_ota_img_bg;
	lv_obj_t *screen_ota_bar_ota;
	lv_obj_t *screen_ota_label_1;
	lv_obj_t *screen_ota_label_ota;
	lv_obj_t *screen_ota_label_3;
	lv_obj_t *screen_ota_label_4;
	lv_obj_t *screen_ota_label_5;
	lv_obj_t *screen_sleep_set;
	bool screen_sleep_set_del;
	lv_obj_t *screen_sleep_set_img_bg;
	lv_obj_t *screen_sleep_set_cont_data;
	lv_obj_t *screen_sleep_set_label_1;
	lv_obj_t *screen_sleep_set_label_title;
	lv_obj_t *screen_sleep_set_ddlist_1;
	lv_obj_t *screen_Instructions;
	bool screen_Instructions_del;
	lv_obj_t *screen_Instructions_img_bg;
	lv_obj_t *screen_Instructions_cont_data;
	lv_obj_t *screen_Instructions_label_title;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_main(lv_ui *ui);
void setup_scr_screen_mileage_reminder(lv_ui *ui);
void setup_scr_screen_set(lv_ui *ui);
void setup_scr_screen_log(lv_ui *ui);
void setup_scr_screen_overlimt(lv_ui *ui);
void setup_scr_screen_mark(lv_ui *ui);
void setup_scr_screen_audiomark(lv_ui *ui);
void setup_scr_screen_about(lv_ui *ui);
void setup_scr_screen_Tag(lv_ui *ui);
void setup_scr_screen_audio_Tag(lv_ui *ui);
void setup_scr_screen_mark_err(lv_ui *ui);
void setup_scr_screen_warn1(lv_ui *ui);
void setup_scr_screen_warn2(lv_ui *ui);
void setup_scr_screen_warn3(lv_ui *ui);
void setup_scr_screen_warn4(lv_ui *ui);
void setup_scr_screen_volume(lv_ui *ui);
void setup_scr_screen_bledata(lv_ui *ui);
void setup_scr_screen_record(lv_ui *ui);
void setup_scr_screen_ota(lv_ui *ui);
void setup_scr_screen_sleep_set(lv_ui *ui);
void setup_scr_screen_Instructions(lv_ui *ui);
LV_IMAGE_DECLARE(_mainbg_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_ly_disconnected_RGB565A8_23x23);
LV_IMAGE_DECLARE(_ly_connected_RGB565A8_23x23);
LV_IMAGE_DECLARE(_100_RGB565A8_30x20);
LV_IMAGE_DECLARE(_80_RGB565A8_30x20);
LV_IMAGE_DECLARE(_60_RGB565A8_30x20);
LV_IMAGE_DECLARE(_50_RGB565A8_30x20);
LV_IMAGE_DECLARE(_40_RGB565A8_30x20);
LV_IMAGE_DECLARE(_20_RGB565A8_30x20);
LV_IMAGE_DECLARE(_10_RGB565A8_30x20);
LV_IMAGE_DECLARE(_volum_RGB565A8_75x75);
LV_IMAGE_DECLARE(_play_RGB565A8_75x75);
LV_IMAGE_DECLARE(_pause_RGB565A8_75x75);
LV_IMAGE_DECLARE(_kjjm_RGB565A8_240x280);
LV_IMAGE_DECLARE(_volum2_RGB565A8_75x75);
LV_IMAGE_DECLARE(_bg_new_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_tb3_RGB565A8_30x30);
LV_IMAGE_DECLARE(_tb1_RGB565A8_30x30);
LV_IMAGE_DECLARE(_tb4_RGB565A8_30x30);
LV_IMAGE_DECLARE(_tb2_RGB565A8_30x30);
LV_IMAGE_DECLARE(_tb5_RGB565A8_30x30);
LV_IMAGE_DECLARE(_tccard_RGB565A8_224x200);
LV_IMAGE_DECLARE(_chaoxian_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_biaoji_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_luying_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_voc_RGB565A8_50x50);
LV_IMAGE_DECLARE(_voc2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_markbg_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_close_RGB565A8_50x50);
LV_IMAGE_DECLARE(_volum_RGB565A8_60x60);
LV_IMAGE_DECLARE(_volum2_RGB565A8_60x60);
LV_IMAGE_DECLARE(_voc_RGB565A8_55x55);
LV_IMAGE_DECLARE(_voc2_RGB565A8_55x55);
LV_IMAGE_DECLARE(_1bg_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_2bg_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_3bg_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(_4bg_RGB565_240x280_tresh);
LV_IMAGE_DECLARE(screen_record_animimg_11_0);
LV_IMAGE_DECLARE(screen_record_animimg_11_1);
LV_IMAGE_DECLARE(screen_record_animimg_11_2);
LV_IMAGE_DECLARE(screen_record_animimg_11_3);
LV_IMAGE_DECLARE(screen_record_animimg_11_4);
LV_IMAGE_DECLARE(screen_record_animimg_11_5);
LV_IMAGE_DECLARE(screen_record_animimg_22_18);
LV_IMAGE_DECLARE(screen_record_animimg_22_19);
LV_IMAGE_DECLARE(screen_record_animimg_22_2);
LV_IMAGE_DECLARE(screen_record_animimg_22_3);
LV_IMAGE_DECLARE(screen_record_animimg_22_4);
LV_IMAGE_DECLARE(screen_record_animimg_22_5);
LV_IMAGE_DECLARE(screen_record_animimg_22_6);
LV_IMAGE_DECLARE(screen_record_animimg_22_7);
LV_IMAGE_DECLARE(screen_record_animimg_22_8);
LV_IMAGE_DECLARE(screen_record_animimg_22_9);

LV_FONT_DECLARE(lv_font_PingFangHeavy_20)
LV_FONT_DECLARE(lv_font_MicrosoftYaHei_16)
LV_FONT_DECLARE(lv_font_PingFangHeavy_24)
LV_FONT_DECLARE(lv_font_PingFangHeavy_30)
LV_FONT_DECLARE(lv_font_PingFangHeavy_23)
LV_FONT_DECLARE(lv_font_PingFangMedium_23)
LV_FONT_DECLARE(lv_font_PingFangHeavy_16)
LV_FONT_DECLARE(lv_font_PingFangHeavy_18)
LV_FONT_DECLARE(lv_font_PingFangMedium_18)
LV_FONT_DECLARE(lv_font_PingFangHeavy_12)
LV_FONT_DECLARE(lv_font_PingFangHeavy_26)


#ifdef __cplusplus
}
#endif
#endif
