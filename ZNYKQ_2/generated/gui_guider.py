# Copyright 2026 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import utime as time
import usys as sys
import lvgl as lv
import ustruct
import fs_driver

lv.init()

# Register display driver.
disp_drv = lv.sdl_window_create(240, 280)
lv.sdl_window_set_resizeable(disp_drv, False)
lv.sdl_window_set_title(disp_drv, "Simulator (MicroPython)")

# Regsiter input driver
mouse = lv.sdl_mouse_create()

# Add default theme for bottom layer
bottom_layer = lv.layer_bottom()
lv.theme_apply(bottom_layer)

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_scale(v)

def anim_img_rotate_cb(obj, v):
    obj.set_rotation(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.binfont_create(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.image_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = lv.calendar.__cast__(e.get_current_target())
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RESULT.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    datetext = lv.label.__cast__(e.get_target())
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = lv.screen_active()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = datetext.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, datetext), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    calendar = lv.calendar.__cast__(e.get_current_target())
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if calendar.get_pressed_date(date) == lv.RESULT.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.remove_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            calendar.delete()

# Create screen_main
screen_main = lv.obj()
screen_main.set_size(240, 280)
screen_main.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_main, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_bg
screen_main_img_bg = lv.image(screen_main)
screen_main_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\mainbg_240_280.png"))
screen_main_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_bg.set_pivot(50,50)
screen_main_img_bg.set_rotation(0)
screen_main_img_bg.set_pos(0, 0)
screen_main_img_bg.set_size(240, 280)
# Set style for screen_main_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_bledisc
screen_main_img_bledisc = lv.image(screen_main)
screen_main_img_bledisc.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\ly_disconnected_23_23.png"))
screen_main_img_bledisc.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_bledisc.set_pivot(50,50)
screen_main_img_bledisc.set_rotation(0)
screen_main_img_bledisc.set_pos(15, 14)
screen_main_img_bledisc.set_size(23, 23)
# Set style for screen_main_img_bledisc, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_bledisc.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_blecon
screen_main_img_blecon = lv.image(screen_main)
screen_main_img_blecon.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\ly_connected_23_23.png"))
screen_main_img_blecon.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_blecon.set_pivot(50,50)
screen_main_img_blecon.set_rotation(0)
screen_main_img_blecon.set_pos(15, 14)
screen_main_img_blecon.set_size(23, 23)
screen_main_img_blecon.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_blecon, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_blecon.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_time
screen_main_label_time = lv.label(screen_main)
screen_main_label_time.set_text("12:20")
screen_main_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_time.set_width(lv.pct(100))
screen_main_label_time.set_pos(68, 14)
screen_main_label_time.set_size(100, 23)
# Set style for screen_main_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_tip
screen_main_label_tip = lv.label(screen_main)
screen_main_label_tip.set_text("蓝牙未连接，请靠近目标设备")
screen_main_label_tip.set_long_mode(lv.label.LONG.SCROLL_CIRCULAR)
screen_main_label_tip.set_width(lv.pct(100))
screen_main_label_tip.set_pos(48, 53)
screen_main_label_tip.set_size(161, 25)
# Set style for screen_main_label_tip, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_tip.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_linename
screen_main_label_linename = lv.label(screen_main)
screen_main_label_linename.set_text("大西客专上下  上")
screen_main_label_linename.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_linename.set_width(lv.pct(100))
screen_main_label_linename.set_pos(16, 98)
screen_main_label_linename.set_size(197, 27)
screen_main_label_linename.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_linename, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_linename.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linename.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_linenum_mileage
screen_main_label_linenum_mileage = lv.label(screen_main)
screen_main_label_linenum_mileage.set_text("K-1111+236A")
screen_main_label_linenum_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_linenum_mileage.set_width(lv.pct(100))
screen_main_label_linenum_mileage.set_pos(16, 131)
screen_main_label_linenum_mileage.set_size(206, 30)
screen_main_label_linenum_mileage.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_linenum_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_linenum_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_linenum_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_speed
screen_main_label_speed = lv.label(screen_main)
screen_main_label_speed.set_text("120 km/h")
screen_main_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_speed.set_width(lv.pct(100))
screen_main_label_speed.set_pos(16, 168)
screen_main_label_speed.set_size(137, 21)
screen_main_label_speed.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_text_font(test_font("PingFangHeavy", 23), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_mask1
screen_main_label_mask1 = lv.label(screen_main)
screen_main_label_mask1.set_text("无数据")
screen_main_label_mask1.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_mask1.set_width(lv.pct(100))
screen_main_label_mask1.set_pos(25, 131)
screen_main_label_mask1.set_size(180, 44)
# Set style for screen_main_label_mask1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_mask1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_text_font(test_font("PingFangMedium", 23), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_text_opa(193, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_mask1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battery100
screen_main_img_battery100 = lv.image(screen_main)
screen_main_img_battery100.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\100_30_20.png"))
screen_main_img_battery100.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battery100.set_pivot(50,50)
screen_main_img_battery100.set_rotation(0)
screen_main_img_battery100.set_pos(195, 14)
screen_main_img_battery100.set_size(30, 20)
# Set style for screen_main_img_battery100, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battery100.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battery80
screen_main_img_battery80 = lv.image(screen_main)
screen_main_img_battery80.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\80_30_20.png"))
screen_main_img_battery80.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battery80.set_pivot(50,50)
screen_main_img_battery80.set_rotation(0)
screen_main_img_battery80.set_pos(195, 14)
screen_main_img_battery80.set_size(30, 20)
screen_main_img_battery80.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_battery80, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battery80.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battery60
screen_main_img_battery60 = lv.image(screen_main)
screen_main_img_battery60.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\60_30_20.png"))
screen_main_img_battery60.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battery60.set_pivot(50,50)
screen_main_img_battery60.set_rotation(0)
screen_main_img_battery60.set_pos(195, 14)
screen_main_img_battery60.set_size(30, 20)
screen_main_img_battery60.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_battery60, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battery60.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battert50
screen_main_img_battert50 = lv.image(screen_main)
screen_main_img_battert50.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\50_30_20.png"))
screen_main_img_battert50.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battert50.set_pivot(50,50)
screen_main_img_battert50.set_rotation(0)
screen_main_img_battert50.set_pos(195, 14)
screen_main_img_battert50.set_size(30, 20)
screen_main_img_battert50.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_battert50, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battert50.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battery40
screen_main_img_battery40 = lv.image(screen_main)
screen_main_img_battery40.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\40_30_20.png"))
screen_main_img_battery40.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battery40.set_pivot(50,50)
screen_main_img_battery40.set_rotation(0)
screen_main_img_battery40.set_pos(195, 14)
screen_main_img_battery40.set_size(30, 20)
screen_main_img_battery40.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_battery40, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battery40.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battery20
screen_main_img_battery20 = lv.image(screen_main)
screen_main_img_battery20.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\20_30_20.png"))
screen_main_img_battery20.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battery20.set_pivot(50,50)
screen_main_img_battery20.set_rotation(0)
screen_main_img_battery20.set_pos(195, 14)
screen_main_img_battery20.set_size(30, 20)
screen_main_img_battery20.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_battery20, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battery20.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_battery10
screen_main_img_battery10 = lv.image(screen_main)
screen_main_img_battery10.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\10_30_20.png"))
screen_main_img_battery10.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_battery10.set_pivot(50,50)
screen_main_img_battery10.set_rotation(0)
screen_main_img_battery10.set_pos(195, 14)
screen_main_img_battery10.set_size(30, 20)
screen_main_img_battery10.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_battery10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_battery10.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_voickey
screen_main_img_voickey = lv.image(screen_main)
screen_main_img_voickey.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum_75_75.png"))
screen_main_img_voickey.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_voickey.set_pivot(50,50)
screen_main_img_voickey.set_rotation(0)
screen_main_img_voickey.set_pos(134, 197)
screen_main_img_voickey.set_size(75, 75)
# Set style for screen_main_img_voickey, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_voickey.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_play_run
screen_main_img_play_run = lv.image(screen_main)
screen_main_img_play_run.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\play_75_75.png"))
screen_main_img_play_run.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_play_run.set_pivot(50,50)
screen_main_img_play_run.set_rotation(0)
screen_main_img_play_run.set_pos(32, 197)
screen_main_img_play_run.set_size(75, 75)
# Set style for screen_main_img_play_run, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_play_run.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_play_stop
screen_main_img_play_stop = lv.image(screen_main)
screen_main_img_play_stop.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\pause_75_75.png"))
screen_main_img_play_stop.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_play_stop.set_pivot(50,50)
screen_main_img_play_stop.set_rotation(0)
screen_main_img_play_stop.set_pos(32, 197)
screen_main_img_play_stop.set_size(75, 75)
screen_main_img_play_stop.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_play_stop, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_play_stop.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_1
screen_main_label_1 = lv.label(screen_main)
screen_main_label_1.set_text("前方：1234567890+设备未运行蓝牙已连接")
screen_main_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_1.set_width(lv.pct(100))
screen_main_label_1.set_pos(32, 93)
screen_main_label_1.set_size(100, 32)
screen_main_label_1.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_tip2
screen_main_label_tip2 = lv.label(screen_main)
screen_main_label_tip2.set_text("录音标记中，请讲话")
screen_main_label_tip2.set_long_mode(lv.label.LONG.SCROLL)
screen_main_label_tip2.set_width(lv.pct(100))
screen_main_label_tip2.set_pos(48, 53)
screen_main_label_tip2.set_size(161, 25)
screen_main_label_tip2.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_tip2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_tip2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_tip2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_2
screen_main_label_2 = lv.label(screen_main)
screen_main_label_2.set_text("设备未运行")
screen_main_label_2.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_2.set_width(lv.pct(100))
screen_main_label_2.set_pos(25, 98)
screen_main_label_2.set_size(171, 32)
screen_main_label_2.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_kjjm
screen_main_img_kjjm = lv.image(screen_main)
screen_main_img_kjjm.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\kjjm_240_280.png"))
screen_main_img_kjjm.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_kjjm.set_pivot(50,50)
screen_main_img_kjjm.set_rotation(0)
screen_main_img_kjjm.set_pos(0, 0)
screen_main_img_kjjm.set_size(240, 280)
screen_main_img_kjjm.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_kjjm, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_kjjm.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_img_voickey2
screen_main_img_voickey2 = lv.image(screen_main)
screen_main_img_voickey2.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum2_75_75.png"))
screen_main_img_voickey2.add_flag(lv.obj.FLAG.CLICKABLE)
screen_main_img_voickey2.set_pivot(50,50)
screen_main_img_voickey2.set_rotation(0)
screen_main_img_voickey2.set_pos(134, 197)
screen_main_img_voickey2.set_size(75, 75)
screen_main_img_voickey2.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_img_voickey2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_img_voickey2.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_main_label_audio_toast
screen_main_label_audio_toast = lv.label(screen_main)
screen_main_label_audio_toast.set_text("上一次录音处理中请稍后")
screen_main_label_audio_toast.set_long_mode(lv.label.LONG.WRAP)
screen_main_label_audio_toast.set_width(lv.pct(100))
screen_main_label_audio_toast.set_pos(0, 0)
screen_main_label_audio_toast.set_size(240, 34)
screen_main_label_audio_toast.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_main_label_audio_toast, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_main_label_audio_toast.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_bg_color(lv.color_hex(0xfdd000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_pad_top(9, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_main_label_audio_toast.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main.update_layout()
# Create screen_mileage_reminder
screen_mileage_reminder = lv.obj()
screen_mileage_reminder.set_size(240, 280)
screen_mileage_reminder.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_mileage_reminder, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mileage_reminder_img_bg
screen_mileage_reminder_img_bg = lv.image(screen_mileage_reminder)
screen_mileage_reminder_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_mileage_reminder_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_mileage_reminder_img_bg.set_pivot(50,50)
screen_mileage_reminder_img_bg.set_rotation(0)
screen_mileage_reminder_img_bg.set_pos(0, 0)
screen_mileage_reminder_img_bg.set_size(240, 280)
# Set style for screen_mileage_reminder_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mileage_reminder_label_3
screen_mileage_reminder_label_3 = lv.label(screen_mileage_reminder)
screen_mileage_reminder_label_3.set_text("提醒里程")
screen_mileage_reminder_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_mileage_reminder_label_3.set_width(lv.pct(100))
screen_mileage_reminder_label_3.set_pos(55, 10)
screen_mileage_reminder_label_3.set_size(130, 30)
# Set style for screen_mileage_reminder_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mileage_reminder_cont_list
screen_mileage_reminder_cont_list = lv.obj(screen_mileage_reminder)
screen_mileage_reminder_cont_list.set_pos(4, 44)
screen_mileage_reminder_cont_list.set_size(230, 227)
screen_mileage_reminder_cont_list.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_mileage_reminder_cont_list, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_cont_list.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_list.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_mileage_reminder_cont_demo
screen_mileage_reminder_cont_demo = lv.obj(screen_mileage_reminder_cont_list)
screen_mileage_reminder_cont_demo.set_pos(3, 1)
screen_mileage_reminder_cont_demo.set_size(224, 62)
screen_mileage_reminder_cont_demo.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_mileage_reminder_cont_demo, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_cont_demo.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_bg_opa(48, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_cont_demo.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_mileage_reminder_label_5
screen_mileage_reminder_label_5 = lv.label(screen_mileage_reminder_cont_demo)
screen_mileage_reminder_label_5.set_text("介西线介西线")
screen_mileage_reminder_label_5.set_long_mode(lv.label.LONG.WRAP)
screen_mileage_reminder_label_5.set_width(lv.pct(100))
screen_mileage_reminder_label_5.set_pos(12, 34)
screen_mileage_reminder_label_5.set_size(101, 21)
# Set style for screen_mileage_reminder_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mileage_reminder_label_4
screen_mileage_reminder_label_4 = lv.label(screen_mileage_reminder_cont_demo)
screen_mileage_reminder_label_4.set_text("K-1234+999A")
screen_mileage_reminder_label_4.set_long_mode(lv.label.LONG.WRAP)
screen_mileage_reminder_label_4.set_width(lv.pct(100))
screen_mileage_reminder_label_4.set_pos(11, 4)
screen_mileage_reminder_label_4.set_size(191, 21)
# Set style for screen_mileage_reminder_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_text_font(test_font("PingFangHeavy", 22), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mileage_reminder_label_6
screen_mileage_reminder_label_6 = lv.label(screen_mileage_reminder_cont_demo)
screen_mileage_reminder_label_6.set_text("已提醒")
screen_mileage_reminder_label_6.set_long_mode(lv.label.LONG.WRAP)
screen_mileage_reminder_label_6.set_width(lv.pct(100))
screen_mileage_reminder_label_6.set_pos(162, 34)
screen_mileage_reminder_label_6.set_size(53, 21)
# Set style for screen_mileage_reminder_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mileage_reminder_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_text_color(lv.color_hex(0xffd200), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mileage_reminder_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_mileage_reminder.update_layout()
# Create screen_set
screen_set = lv.obj()
screen_set.set_size(240, 280)
screen_set.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_img_bg
screen_set_img_bg = lv.image(screen_set)
screen_set_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_set_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_set_img_bg.set_pivot(50,50)
screen_set_img_bg.set_rotation(0)
screen_set_img_bg.set_pos(0, 0)
screen_set_img_bg.set_size(240, 280)
# Set style for screen_set_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_cont_6
screen_set_cont_6 = lv.obj(screen_set)
screen_set_cont_6.set_pos(5, 48)
screen_set_cont_6.set_size(230, 210)
screen_set_cont_6.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set_cont_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_cont_6.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_set_cont_7
screen_set_cont_7 = lv.obj(screen_set_cont_6)
screen_set_cont_7.set_pos(5, 8)
screen_set_cont_7.set_size(220, 50)
screen_set_cont_7.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set_cont_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_cont_7.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_bg_opa(43, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_set_img_1
screen_set_img_1 = lv.image(screen_set_cont_7)
screen_set_img_1.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\tb3_30_30.png"))
screen_set_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_set_img_1.set_pivot(50,50)
screen_set_img_1.set_rotation(0)
screen_set_img_1.set_pos(9, 6)
screen_set_img_1.set_size(30, 30)
# Set style for screen_set_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_img_1.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_label_8
screen_set_label_8 = lv.label(screen_set_cont_7)
screen_set_label_8.set_text("蓝牙信息")
screen_set_label_8.set_long_mode(lv.label.LONG.WRAP)
screen_set_label_8.set_width(lv.pct(100))
screen_set_label_8.set_pos(60, 12)
screen_set_label_8.set_size(99, 25)
# Set style for screen_set_label_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_label_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_cont_8
screen_set_cont_8 = lv.obj(screen_set_cont_6)
screen_set_cont_8.set_pos(5, 247)
screen_set_cont_8.set_size(220, 50)
screen_set_cont_8.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set_cont_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_cont_8.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_bg_opa(43, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_set_img_2
screen_set_img_2 = lv.image(screen_set_cont_8)
screen_set_img_2.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\tb1_30_30.png"))
screen_set_img_2.add_flag(lv.obj.FLAG.CLICKABLE)
screen_set_img_2.set_pivot(50,50)
screen_set_img_2.set_rotation(0)
screen_set_img_2.set_pos(9, 6)
screen_set_img_2.set_size(30, 30)
# Set style for screen_set_img_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_img_2.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_label_9
screen_set_label_9 = lv.label(screen_set_cont_8)
screen_set_label_9.set_text("关于")
screen_set_label_9.set_long_mode(lv.label.LONG.WRAP)
screen_set_label_9.set_width(lv.pct(100))
screen_set_label_9.set_pos(60, 12)
screen_set_label_9.set_size(99, 25)
# Set style for screen_set_label_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_label_9.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_cont_9
screen_set_cont_9 = lv.obj(screen_set_cont_6)
screen_set_cont_9.set_pos(5, 68)
screen_set_cont_9.set_size(220, 50)
screen_set_cont_9.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set_cont_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_cont_9.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_bg_opa(43, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_set_img_3
screen_set_img_3 = lv.image(screen_set_cont_9)
screen_set_img_3.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\tb4_30_30.png"))
screen_set_img_3.add_flag(lv.obj.FLAG.CLICKABLE)
screen_set_img_3.set_pivot(50,50)
screen_set_img_3.set_rotation(0)
screen_set_img_3.set_pos(9, 6)
screen_set_img_3.set_size(30, 30)
# Set style for screen_set_img_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_img_3.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_label_10
screen_set_label_10 = lv.label(screen_set_cont_9)
screen_set_label_10.set_text("音量设置")
screen_set_label_10.set_long_mode(lv.label.LONG.WRAP)
screen_set_label_10.set_width(lv.pct(100))
screen_set_label_10.set_pos(60, 12)
screen_set_label_10.set_size(99, 25)
# Set style for screen_set_label_10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_label_10.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_cont_10
screen_set_cont_10 = lv.obj(screen_set_cont_6)
screen_set_cont_10.set_pos(5, 188)
screen_set_cont_10.set_size(220, 50)
screen_set_cont_10.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set_cont_10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_cont_10.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_bg_opa(43, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_set_img_4
screen_set_img_4 = lv.image(screen_set_cont_10)
screen_set_img_4.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\tb2_30_30.png"))
screen_set_img_4.add_flag(lv.obj.FLAG.CLICKABLE)
screen_set_img_4.set_pivot(50,50)
screen_set_img_4.set_rotation(0)
screen_set_img_4.set_pos(9, 6)
screen_set_img_4.set_size(30, 30)
# Set style for screen_set_img_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_img_4.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_label_11
screen_set_label_11 = lv.label(screen_set_cont_10)
screen_set_label_11.set_text("使用说明")
screen_set_label_11.set_long_mode(lv.label.LONG.WRAP)
screen_set_label_11.set_width(lv.pct(100))
screen_set_label_11.set_pos(60, 12)
screen_set_label_11.set_size(99, 25)
# Set style for screen_set_label_11, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_label_11.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_cont_11
screen_set_cont_11 = lv.obj(screen_set_cont_6)
screen_set_cont_11.set_pos(5, 128)
screen_set_cont_11.set_size(220, 50)
screen_set_cont_11.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_set_cont_11, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_cont_11.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_bg_opa(43, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_cont_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_set_img_5
screen_set_img_5 = lv.image(screen_set_cont_11)
screen_set_img_5.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\tb5_30_30.png"))
screen_set_img_5.add_flag(lv.obj.FLAG.CLICKABLE)
screen_set_img_5.set_pivot(50,50)
screen_set_img_5.set_rotation(0)
screen_set_img_5.set_pos(9, 6)
screen_set_img_5.set_size(30, 30)
# Set style for screen_set_img_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_img_5.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_label_12
screen_set_label_12 = lv.label(screen_set_cont_11)
screen_set_label_12.set_text("休眠设置")
screen_set_label_12.set_long_mode(lv.label.LONG.WRAP)
screen_set_label_12.set_width(lv.pct(100))
screen_set_label_12.set_pos(60, 12)
screen_set_label_12.set_size(99, 25)
# Set style for screen_set_label_12, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_label_12.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_set_label_7
screen_set_label_7 = lv.label(screen_set)
screen_set_label_7.set_text("设置")
screen_set_label_7.set_long_mode(lv.label.LONG.WRAP)
screen_set_label_7.set_width(lv.pct(100))
screen_set_label_7.set_pos(55, 10)
screen_set_label_7.set_size(130, 30)
# Set style for screen_set_label_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_set_label_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_pad_top(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_set_label_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set.update_layout()
# Create screen_log
screen_log = lv.obj()
screen_log.set_size(240, 280)
screen_log.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_log, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_log_img_bg
screen_log_img_bg = lv.image(screen_log)
screen_log_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_log_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_log_img_bg.set_pivot(50,50)
screen_log_img_bg.set_rotation(0)
screen_log_img_bg.set_pos(0, 0)
screen_log_img_bg.set_size(240, 280)
# Set style for screen_log_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_log_cont_list
screen_log_cont_list = lv.obj(screen_log)
screen_log_cont_list.set_pos(5, 48)
screen_log_cont_list.set_size(230, 227)
screen_log_cont_list.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_log_cont_list, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log_cont_list.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_cont_list.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_log_img_list_bg
screen_log_img_list_bg = lv.image(screen_log_cont_list)
screen_log_img_list_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\tccard_224_200.png"))
screen_log_img_list_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_log_img_list_bg.set_pivot(50,50)
screen_log_img_list_bg.set_rotation(0)
screen_log_img_list_bg.set_pos(3, 1)
screen_log_img_list_bg.set_size(224, 200)
screen_log_img_list_bg.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_log_img_list_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log_img_list_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_log_label_14
screen_log_label_14 = lv.label(screen_log)
screen_log_label_14.set_text("暂无录音标记数据超限")
screen_log_label_14.set_long_mode(lv.label.LONG.WRAP)
screen_log_label_14.set_width(lv.pct(100))
screen_log_label_14.set_pos(56, 119)
screen_log_label_14.set_size(131, 44)
screen_log_label_14.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_log_label_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log_label_14.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_pad_top(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_log_label_15
screen_log_label_15 = lv.label(screen_log)
screen_log_label_15.set_text("提醒里程")
screen_log_label_15.set_long_mode(lv.label.LONG.WRAP)
screen_log_label_15.set_width(lv.pct(100))
screen_log_label_15.set_pos(55, 10)
screen_log_label_15.set_size(130, 30)
screen_log_label_15.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_log_label_15, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log_label_15.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_text_font(test_font("PingFangMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_log_label_16
screen_log_label_16 = lv.label(screen_log)
screen_log_label_16.set_text("添乘记录")
screen_log_label_16.set_long_mode(lv.label.LONG.WRAP)
screen_log_label_16.set_width(lv.pct(100))
screen_log_label_16.set_pos(55, 10)
screen_log_label_16.set_size(130, 30)
# Set style for screen_log_label_16, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_log_label_16.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_log_label_16.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_log.update_layout()
# Create screen_overlimt
screen_overlimt = lv.obj()
screen_overlimt.set_size(240, 280)
screen_overlimt.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_overlimt, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overlimt.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overlimt_img_bg
screen_overlimt_img_bg = lv.image(screen_overlimt)
screen_overlimt_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\chaoxian_240_280.png"))
screen_overlimt_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_overlimt_img_bg.set_pivot(50,50)
screen_overlimt_img_bg.set_rotation(0)
screen_overlimt_img_bg.set_pos(0, 0)
screen_overlimt_img_bg.set_size(240, 280)
# Set style for screen_overlimt_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overlimt_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overlimt_cont_list
screen_overlimt_cont_list = lv.obj(screen_overlimt)
screen_overlimt_cont_list.set_pos(5, 48)
screen_overlimt_cont_list.set_size(230, 227)
screen_overlimt_cont_list.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_overlimt_cont_list, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overlimt_cont_list.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_cont_list.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overlimt_btn_overlimt
screen_overlimt_btn_overlimt = lv.button(screen_overlimt)
screen_overlimt_btn_overlimt_label = lv.label(screen_overlimt_btn_overlimt)
screen_overlimt_btn_overlimt_label.set_text("")
screen_overlimt_btn_overlimt_label.set_long_mode(lv.label.LONG.WRAP)
screen_overlimt_btn_overlimt_label.set_width(lv.pct(100))
screen_overlimt_btn_overlimt_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overlimt_btn_overlimt.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_pos(4, 5)
screen_overlimt_btn_overlimt.set_size(72, 40)
# Set style for screen_overlimt_btn_overlimt, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overlimt_btn_overlimt.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_overlimt.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overlimt_btn_mark
screen_overlimt_btn_mark = lv.button(screen_overlimt)
screen_overlimt_btn_mark_label = lv.label(screen_overlimt_btn_mark)
screen_overlimt_btn_mark_label.set_text("")
screen_overlimt_btn_mark_label.set_long_mode(lv.label.LONG.WRAP)
screen_overlimt_btn_mark_label.set_width(lv.pct(100))
screen_overlimt_btn_mark_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overlimt_btn_mark.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_pos(85, 5)
screen_overlimt_btn_mark.set_size(72, 40)
# Set style for screen_overlimt_btn_mark, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overlimt_btn_mark.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_mark.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overlimt_btn_audiomark
screen_overlimt_btn_audiomark = lv.button(screen_overlimt)
screen_overlimt_btn_audiomark_label = lv.label(screen_overlimt_btn_audiomark)
screen_overlimt_btn_audiomark_label.set_text("")
screen_overlimt_btn_audiomark_label.set_long_mode(lv.label.LONG.WRAP)
screen_overlimt_btn_audiomark_label.set_width(lv.pct(100))
screen_overlimt_btn_audiomark_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overlimt_btn_audiomark.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_pos(164, 5)
screen_overlimt_btn_audiomark.set_size(72, 40)
# Set style for screen_overlimt_btn_audiomark, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overlimt_btn_audiomark.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overlimt_btn_audiomark.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_overlimt.update_layout()
# Create screen_mark
screen_mark = lv.obj()
screen_mark.set_size(240, 280)
screen_mark.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_mark, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_img_bg
screen_mark_img_bg = lv.image(screen_mark)
screen_mark_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\biaoji_240_280.png"))
screen_mark_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_mark_img_bg.set_pivot(50,50)
screen_mark_img_bg.set_rotation(0)
screen_mark_img_bg.set_pos(0, 0)
screen_mark_img_bg.set_size(240, 280)
# Set style for screen_mark_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_cont_list
screen_mark_cont_list = lv.obj(screen_mark)
screen_mark_cont_list.set_pos(5, 47)
screen_mark_cont_list.set_size(230, 227)
screen_mark_cont_list.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_mark_cont_list, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_cont_list.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_cont_list.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_label_14
screen_mark_label_14 = lv.label(screen_mark)
screen_mark_label_14.set_text("暂无录音标记数据超限为空")
screen_mark_label_14.set_long_mode(lv.label.LONG.WRAP)
screen_mark_label_14.set_width(lv.pct(100))
screen_mark_label_14.set_pos(55, 127)
screen_mark_label_14.set_size(131, 44)
screen_mark_label_14.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_mark_label_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_label_14.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_pad_top(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_label_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_btn_overlimt
screen_mark_btn_overlimt = lv.button(screen_mark)
screen_mark_btn_overlimt_label = lv.label(screen_mark_btn_overlimt)
screen_mark_btn_overlimt_label.set_text("")
screen_mark_btn_overlimt_label.set_long_mode(lv.label.LONG.WRAP)
screen_mark_btn_overlimt_label.set_width(lv.pct(100))
screen_mark_btn_overlimt_label.align(lv.ALIGN.CENTER, 0, 0)
screen_mark_btn_overlimt.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_pos(4, 5)
screen_mark_btn_overlimt.set_size(72, 40)
# Set style for screen_mark_btn_overlimt, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_btn_overlimt.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_overlimt.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_btn_mark
screen_mark_btn_mark = lv.button(screen_mark)
screen_mark_btn_mark_label = lv.label(screen_mark_btn_mark)
screen_mark_btn_mark_label.set_text("")
screen_mark_btn_mark_label.set_long_mode(lv.label.LONG.WRAP)
screen_mark_btn_mark_label.set_width(lv.pct(100))
screen_mark_btn_mark_label.align(lv.ALIGN.CENTER, 0, 0)
screen_mark_btn_mark.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_mark_btn_mark.set_pos(85, 5)
screen_mark_btn_mark.set_size(72, 40)
# Set style for screen_mark_btn_mark, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_btn_mark.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_mark.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_btn_audio
screen_mark_btn_audio = lv.button(screen_mark)
screen_mark_btn_audio_label = lv.label(screen_mark_btn_audio)
screen_mark_btn_audio_label.set_text("")
screen_mark_btn_audio_label.set_long_mode(lv.label.LONG.WRAP)
screen_mark_btn_audio_label.set_width(lv.pct(100))
screen_mark_btn_audio_label.align(lv.ALIGN.CENTER, 0, 0)
screen_mark_btn_audio.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_mark_btn_audio.set_pos(164, 5)
screen_mark_btn_audio.set_size(72, 40)
# Set style for screen_mark_btn_audio, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_btn_audio.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_btn_audio.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_mark.update_layout()
# Create screen_audiomark
screen_audiomark = lv.obj()
screen_audiomark.set_size(240, 280)
screen_audiomark.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_audiomark, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_img_bg
screen_audiomark_img_bg = lv.image(screen_audiomark)
screen_audiomark_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\luying_240_280.png"))
screen_audiomark_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audiomark_img_bg.set_pivot(50,50)
screen_audiomark_img_bg.set_rotation(0)
screen_audiomark_img_bg.set_pos(0, 0)
screen_audiomark_img_bg.set_size(240, 280)
# Set style for screen_audiomark_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_cont_list
screen_audiomark_cont_list = lv.obj(screen_audiomark)
screen_audiomark_cont_list.set_pos(5, 47)
screen_audiomark_cont_list.set_size(230, 227)
screen_audiomark_cont_list.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_audiomark_cont_list, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_cont_list.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_list.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_audiomark_cont_1
screen_audiomark_cont_1 = lv.obj(screen_audiomark_cont_list)
screen_audiomark_cont_1.set_pos(5, 12)
screen_audiomark_cont_1.set_size(220, 88)
screen_audiomark_cont_1.add_flag(lv.obj.FLAG.HIDDEN)
screen_audiomark_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_audiomark_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_cont_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_bg_opa(54, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_audiomark_img_1
screen_audiomark_img_1 = lv.image(screen_audiomark_cont_1)
screen_audiomark_img_1.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\voc_50_50.png"))
screen_audiomark_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audiomark_img_1.set_pivot(50,50)
screen_audiomark_img_1.set_rotation(0)
screen_audiomark_img_1.set_pos(142, 27)
screen_audiomark_img_1.set_size(50, 50)
# Set style for screen_audiomark_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_img_1.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_img_2
screen_audiomark_img_2 = lv.image(screen_audiomark_cont_1)
screen_audiomark_img_2.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\voc2_50_50.png"))
screen_audiomark_img_2.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audiomark_img_2.set_pivot(50,50)
screen_audiomark_img_2.set_rotation(0)
screen_audiomark_img_2.set_pos(73, 27)
screen_audiomark_img_2.set_size(50, 50)
# Set style for screen_audiomark_img_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_img_2.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_label_14
screen_audiomark_label_14 = lv.label(screen_audiomark)
screen_audiomark_label_14.set_text("暂无录音标记数据超限")
screen_audiomark_label_14.set_long_mode(lv.label.LONG.WRAP)
screen_audiomark_label_14.set_width(lv.pct(100))
screen_audiomark_label_14.set_pos(55, 127)
screen_audiomark_label_14.set_size(131, 44)
screen_audiomark_label_14.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_audiomark_label_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_label_14.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_pad_top(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_label_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_btn_overlimt
screen_audiomark_btn_overlimt = lv.button(screen_audiomark)
screen_audiomark_btn_overlimt_label = lv.label(screen_audiomark_btn_overlimt)
screen_audiomark_btn_overlimt_label.set_text("")
screen_audiomark_btn_overlimt_label.set_long_mode(lv.label.LONG.WRAP)
screen_audiomark_btn_overlimt_label.set_width(lv.pct(100))
screen_audiomark_btn_overlimt_label.align(lv.ALIGN.CENTER, 0, 0)
screen_audiomark_btn_overlimt.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_pos(4, 5)
screen_audiomark_btn_overlimt.set_size(72, 40)
# Set style for screen_audiomark_btn_overlimt, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_btn_overlimt.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_overlimt.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_btn_mark
screen_audiomark_btn_mark = lv.button(screen_audiomark)
screen_audiomark_btn_mark_label = lv.label(screen_audiomark_btn_mark)
screen_audiomark_btn_mark_label.set_text("")
screen_audiomark_btn_mark_label.set_long_mode(lv.label.LONG.WRAP)
screen_audiomark_btn_mark_label.set_width(lv.pct(100))
screen_audiomark_btn_mark_label.align(lv.ALIGN.CENTER, 0, 0)
screen_audiomark_btn_mark.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_pos(85, 5)
screen_audiomark_btn_mark.set_size(72, 40)
# Set style for screen_audiomark_btn_mark, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_btn_mark.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_mark.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audiomark_btn_audio
screen_audiomark_btn_audio = lv.button(screen_audiomark)
screen_audiomark_btn_audio_label = lv.label(screen_audiomark_btn_audio)
screen_audiomark_btn_audio_label.set_text("")
screen_audiomark_btn_audio_label.set_long_mode(lv.label.LONG.WRAP)
screen_audiomark_btn_audio_label.set_width(lv.pct(100))
screen_audiomark_btn_audio_label.align(lv.ALIGN.CENTER, 0, 0)
screen_audiomark_btn_audio.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_pos(164, 5)
screen_audiomark_btn_audio.set_size(72, 40)
# Set style for screen_audiomark_btn_audio, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audiomark_btn_audio.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audiomark_btn_audio.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_audiomark.update_layout()
# Create screen_about
screen_about = lv.obj()
screen_about.set_size(240, 280)
screen_about.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_about, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_img_bg
screen_about_img_bg = lv.image(screen_about)
screen_about_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_about_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_about_img_bg.set_pivot(50,50)
screen_about_img_bg.set_rotation(0)
screen_about_img_bg.set_pos(0, 0)
screen_about_img_bg.set_size(240, 280)
# Set style for screen_about_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_cont_6
screen_about_cont_6 = lv.obj(screen_about)
screen_about_cont_6.set_pos(14, 188)
screen_about_cont_6.set_size(212, 67)
screen_about_cont_6.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_about_cont_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_cont_6.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_border_opa(108, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_radius(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_about_label_hardnum
screen_about_label_hardnum = lv.label(screen_about_cont_6)
screen_about_label_hardnum.set_text("XXXXXXXXXXXXX")
screen_about_label_hardnum.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_hardnum.set_width(lv.pct(100))
screen_about_label_hardnum.set_pos(20, 33)
screen_about_label_hardnum.set_size(176, 24)
# Set style for screen_about_label_hardnum, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_hardnum.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_hardnum.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_label_8
screen_about_label_8 = lv.label(screen_about_cont_6)
screen_about_label_8.set_text("硬件版本")
screen_about_label_8.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_8.set_width(lv.pct(100))
screen_about_label_8.set_pos(20, 7)
screen_about_label_8.set_size(66, 19)
# Set style for screen_about_label_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_cont_5
screen_about_cont_5 = lv.obj(screen_about)
screen_about_cont_5.set_pos(14, 119)
screen_about_cont_5.set_size(212, 67)
screen_about_cont_5.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_about_cont_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_cont_5.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_border_opa(108, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_radius(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_about_label_softnum
screen_about_label_softnum = lv.label(screen_about_cont_5)
screen_about_label_softnum.set_text("XXXXXXXXXXXXX")
screen_about_label_softnum.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_softnum.set_width(lv.pct(100))
screen_about_label_softnum.set_pos(20, 33)
screen_about_label_softnum.set_size(176, 24)
# Set style for screen_about_label_softnum, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_softnum.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_softnum.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_label_6
screen_about_label_6 = lv.label(screen_about_cont_5)
screen_about_label_6.set_text("软件版本")
screen_about_label_6.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_6.set_width(lv.pct(100))
screen_about_label_6.set_pos(20, 7)
screen_about_label_6.set_size(66, 19)
# Set style for screen_about_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_cont_4
screen_about_cont_4 = lv.obj(screen_about)
screen_about_cont_4.set_pos(14, 50)
screen_about_cont_4.set_size(212, 67)
screen_about_cont_4.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_about_cont_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_cont_4.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_border_opa(108, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_radius(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_cont_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_about_label_devnum
screen_about_label_devnum = lv.label(screen_about_cont_4)
screen_about_label_devnum.set_text("XXXXXXXXXXXXX")
screen_about_label_devnum.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_devnum.set_width(lv.pct(100))
screen_about_label_devnum.set_pos(20, 33)
screen_about_label_devnum.set_size(176, 24)
# Set style for screen_about_label_devnum, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_devnum.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_devnum.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_label_4
screen_about_label_4 = lv.label(screen_about_cont_4)
screen_about_label_4.set_text("设备编号")
screen_about_label_4.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_4.set_width(lv.pct(100))
screen_about_label_4.set_pos(20, 7)
screen_about_label_4.set_size(66, 19)
# Set style for screen_about_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_about_label_3
screen_about_label_3 = lv.label(screen_about)
screen_about_label_3.set_text("关于")
screen_about_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_about_label_3.set_width(lv.pct(100))
screen_about_label_3.set_pos(55, 10)
screen_about_label_3.set_size(130, 30)
# Set style for screen_about_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_about_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_about_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_about.update_layout()
# Create screen_Tag
screen_Tag = lv.obj()
screen_Tag.set_size(240, 280)
screen_Tag.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_Tag, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_img_bg
screen_Tag_img_bg = lv.image(screen_Tag)
screen_Tag_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\markbg_240_280.png"))
screen_Tag_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_Tag_img_bg.set_pivot(50,50)
screen_Tag_img_bg.set_rotation(0)
screen_Tag_img_bg.set_pos(0, 0)
screen_Tag_img_bg.set_size(240, 280)
# Set style for screen_Tag_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_img_back
screen_Tag_img_back = lv.image(screen_Tag)
screen_Tag_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_Tag_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_Tag_img_back.set_pivot(50,50)
screen_Tag_img_back.set_rotation(0)
screen_Tag_img_back.set_pos(136, 216)
screen_Tag_img_back.set_size(50, 50)
# Set style for screen_Tag_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_type1
screen_Tag_label_type1 = lv.label(screen_Tag)
screen_Tag_label_type1.set_text("人工标记")
screen_Tag_label_type1.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_type1.set_width(lv.pct(100))
screen_Tag_label_type1.set_pos(34, 49)
screen_Tag_label_type1.set_size(104, 27)
# Set style for screen_Tag_label_type1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_type1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_text_font(test_font("PingFangHeavy", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_time
screen_Tag_label_time = lv.label(screen_Tag)
screen_Tag_label_time.set_text("12 : 20")
screen_Tag_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_time.set_width(lv.pct(100))
screen_Tag_label_time.set_pos(154, 52)
screen_Tag_label_time.set_size(61, 26)
# Set style for screen_Tag_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_speed
screen_Tag_label_speed = lv.label(screen_Tag)
screen_Tag_label_speed.set_text("120 km/h")
screen_Tag_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_speed.set_width(lv.pct(100))
screen_Tag_label_speed.set_pos(34, 168)
screen_Tag_label_speed.set_size(153, 18)
# Set style for screen_Tag_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_linenum_mileage
screen_Tag_label_linenum_mileage = lv.label(screen_Tag)
screen_Tag_label_linenum_mileage.set_text("K1111+236")
screen_Tag_label_linenum_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_linenum_mileage.set_width(lv.pct(100))
screen_Tag_label_linenum_mileage.set_pos(34, 132)
screen_Tag_label_linenum_mileage.set_size(153, 30)
# Set style for screen_Tag_label_linenum_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_linenum_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_text_font(test_font("PingFangHeavy", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_linenum_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_1
screen_Tag_label_1 = lv.label(screen_Tag)
screen_Tag_label_1.set_text("人工标记")
screen_Tag_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_1.set_width(lv.pct(100))
screen_Tag_label_1.set_pos(70, 124)
screen_Tag_label_1.set_size(100, 32)
screen_Tag_label_1.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_Tag_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_text_font(test_font("PingFangHeavy", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_img_voic
screen_Tag_img_voic = lv.image(screen_Tag)
screen_Tag_img_voic.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum_60_60.png"))
screen_Tag_img_voic.add_flag(lv.obj.FLAG.CLICKABLE)
screen_Tag_img_voic.set_pivot(50,50)
screen_Tag_img_voic.set_rotation(0)
screen_Tag_img_voic.set_pos(45, 205)
screen_Tag_img_voic.set_size(60, 60)
# Set style for screen_Tag_img_voic, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_img_voic.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_img_voicing
screen_Tag_img_voicing = lv.image(screen_Tag)
screen_Tag_img_voicing.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum2_60_60.png"))
screen_Tag_img_voicing.add_flag(lv.obj.FLAG.CLICKABLE)
screen_Tag_img_voicing.set_pivot(50,50)
screen_Tag_img_voicing.set_rotation(0)
screen_Tag_img_voicing.set_pos(45, 205)
screen_Tag_img_voicing.set_size(60, 60)
screen_Tag_img_voicing.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_Tag_img_voicing, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_img_voicing.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_tip
screen_Tag_label_tip = lv.label(screen_Tag)
screen_Tag_label_tip.set_text("录音标记中...")
screen_Tag_label_tip.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_tip.set_width(lv.pct(100))
screen_Tag_label_tip.set_pos(38, 7)
screen_Tag_label_tip.set_size(168, 21)
screen_Tag_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_Tag_label_tip, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_tip.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_bg_opa(75, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_tip.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Tag_label_type
screen_Tag_label_type = lv.label(screen_Tag)
screen_Tag_label_type.set_text("类型: 晃车")
screen_Tag_label_type.set_long_mode(lv.label.LONG.WRAP)
screen_Tag_label_type.set_width(lv.pct(100))
screen_Tag_label_type.set_pos(34, 98)
screen_Tag_label_type.set_size(182, 24)
# Set style for screen_Tag_label_type, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Tag_label_type.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_pad_top(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Tag_label_type.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_Tag.update_layout()
# Create screen_audio_Tag
screen_audio_Tag = lv.obj()
screen_audio_Tag.set_size(240, 280)
screen_audio_Tag.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_audio_Tag, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_img_bg
screen_audio_Tag_img_bg = lv.image(screen_audio_Tag)
screen_audio_Tag_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\markbg_240_280.png"))
screen_audio_Tag_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audio_Tag_img_bg.set_pivot(50,50)
screen_audio_Tag_img_bg.set_rotation(0)
screen_audio_Tag_img_bg.set_pos(0, 0)
screen_audio_Tag_img_bg.set_size(240, 280)
# Set style for screen_audio_Tag_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_img_back
screen_audio_Tag_img_back = lv.image(screen_audio_Tag)
screen_audio_Tag_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_audio_Tag_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audio_Tag_img_back.set_pivot(50,50)
screen_audio_Tag_img_back.set_rotation(0)
screen_audio_Tag_img_back.set_pos(136, 216)
screen_audio_Tag_img_back.set_size(50, 50)
# Set style for screen_audio_Tag_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_img_audio_playing
screen_audio_Tag_img_audio_playing = lv.image(screen_audio_Tag)
screen_audio_Tag_img_audio_playing.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\voc_55_55.png"))
screen_audio_Tag_img_audio_playing.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audio_Tag_img_audio_playing.set_pivot(50,50)
screen_audio_Tag_img_audio_playing.set_rotation(0)
screen_audio_Tag_img_audio_playing.set_pos(45, 211)
screen_audio_Tag_img_audio_playing.set_size(55, 55)
screen_audio_Tag_img_audio_playing.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_audio_Tag_img_audio_playing, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_img_audio_playing.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_label_speed
screen_audio_Tag_label_speed = lv.label(screen_audio_Tag)
screen_audio_Tag_label_speed.set_text("120 km/h")
screen_audio_Tag_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_audio_Tag_label_speed.set_width(lv.pct(100))
screen_audio_Tag_label_speed.set_pos(34, 168)
screen_audio_Tag_label_speed.set_size(121, 18)
# Set style for screen_audio_Tag_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_label_num_mileage
screen_audio_Tag_label_num_mileage = lv.label(screen_audio_Tag)
screen_audio_Tag_label_num_mileage.set_text("K1111+236")
screen_audio_Tag_label_num_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_audio_Tag_label_num_mileage.set_width(lv.pct(100))
screen_audio_Tag_label_num_mileage.set_pos(34, 132)
screen_audio_Tag_label_num_mileage.set_size(153, 30)
# Set style for screen_audio_Tag_label_num_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_label_num_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_text_font(test_font("PingFangHeavy", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_num_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_label_time
screen_audio_Tag_label_time = lv.label(screen_audio_Tag)
screen_audio_Tag_label_time.set_text("12 : 20")
screen_audio_Tag_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_audio_Tag_label_time.set_width(lv.pct(100))
screen_audio_Tag_label_time.set_pos(154, 52)
screen_audio_Tag_label_time.set_size(61, 26)
# Set style for screen_audio_Tag_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_label_type1
screen_audio_Tag_label_type1 = lv.label(screen_audio_Tag)
screen_audio_Tag_label_type1.set_text("录音标记")
screen_audio_Tag_label_type1.set_long_mode(lv.label.LONG.WRAP)
screen_audio_Tag_label_type1.set_width(lv.pct(100))
screen_audio_Tag_label_type1.set_pos(34, 49)
screen_audio_Tag_label_type1.set_size(109, 28)
# Set style for screen_audio_Tag_label_type1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_label_type1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_text_font(test_font("PingFangHeavy", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_pad_bottom(, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_img_audio_play
screen_audio_Tag_img_audio_play = lv.image(screen_audio_Tag)
screen_audio_Tag_img_audio_play.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\voc2_55_55.png"))
screen_audio_Tag_img_audio_play.add_flag(lv.obj.FLAG.CLICKABLE)
screen_audio_Tag_img_audio_play.set_pivot(50,50)
screen_audio_Tag_img_audio_play.set_rotation(0)
screen_audio_Tag_img_audio_play.set_pos(45, 211)
screen_audio_Tag_img_audio_play.set_size(55, 55)
# Set style for screen_audio_Tag_img_audio_play, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_img_audio_play.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_audio_Tag_label_type
screen_audio_Tag_label_type = lv.label(screen_audio_Tag)
screen_audio_Tag_label_type.set_text("类型: 录音")
screen_audio_Tag_label_type.set_long_mode(lv.label.LONG.WRAP)
screen_audio_Tag_label_type.set_width(lv.pct(100))
screen_audio_Tag_label_type.set_pos(34, 98)
screen_audio_Tag_label_type.set_size(182, 24)
# Set style for screen_audio_Tag_label_type, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_audio_Tag_label_type.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_text_font(test_font("PingFangHeavy", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_pad_top(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_audio_Tag_label_type.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_audio_Tag.update_layout()
# Create screen_mark_err
screen_mark_err = lv.obj()
screen_mark_err.set_size(240, 280)
screen_mark_err.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_mark_err, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_img_bg
screen_mark_err_img_bg = lv.image(screen_mark_err)
screen_mark_err_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\markbg_240_280.png"))
screen_mark_err_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_mark_err_img_bg.set_pivot(50,50)
screen_mark_err_img_bg.set_rotation(0)
screen_mark_err_img_bg.set_pos(0, 0)
screen_mark_err_img_bg.set_size(240, 280)
# Set style for screen_mark_err_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_img_back
screen_mark_err_img_back = lv.image(screen_mark_err)
screen_mark_err_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_mark_err_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_mark_err_img_back.set_pivot(50,50)
screen_mark_err_img_back.set_rotation(0)
screen_mark_err_img_back.set_pos(93, 213)
screen_mark_err_img_back.set_size(50, 50)
# Set style for screen_mark_err_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_label_mark_name
screen_mark_err_label_mark_name = lv.label(screen_mark_err)
screen_mark_err_label_mark_name.set_text("标记失败")
screen_mark_err_label_mark_name.set_long_mode(lv.label.LONG.WRAP)
screen_mark_err_label_mark_name.set_width(lv.pct(100))
screen_mark_err_label_mark_name.set_pos(34, 49)
screen_mark_err_label_mark_name.set_size(104, 28)
# Set style for screen_mark_err_label_mark_name, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_label_mark_name.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_text_font(test_font("PingFangHeavy", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_mark_name.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_label_time
screen_mark_err_label_time = lv.label(screen_mark_err)
screen_mark_err_label_time.set_text("12 : 20")
screen_mark_err_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_mark_err_label_time.set_width(lv.pct(100))
screen_mark_err_label_time.set_pos(154, 52)
screen_mark_err_label_time.set_size(61, 26)
# Set style for screen_mark_err_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_label_2
screen_mark_err_label_2 = lv.label(screen_mark_err)
screen_mark_err_label_2.set_text("错误码")
screen_mark_err_label_2.set_long_mode(lv.label.LONG.WRAP)
screen_mark_err_label_2.set_width(lv.pct(100))
screen_mark_err_label_2.set_pos(34, 169)
screen_mark_err_label_2.set_size(66, 28)
# Set style for screen_mark_err_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_label_3
screen_mark_err_label_3 = lv.label(screen_mark_err)
screen_mark_err_label_3.set_text("-1")
screen_mark_err_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_mark_err_label_3.set_width(lv.pct(100))
screen_mark_err_label_3.set_pos(125, 169)
screen_mark_err_label_3.set_size(63, 22)
# Set style for screen_mark_err_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_mark_err_label_4
screen_mark_err_label_4 = lv.label(screen_mark_err)
screen_mark_err_label_4.set_text("请重新进行标记")
screen_mark_err_label_4.set_long_mode(lv.label.LONG.WRAP)
screen_mark_err_label_4.set_width(lv.pct(100))
screen_mark_err_label_4.set_pos(34, 106)
screen_mark_err_label_4.set_size(178, 26)
# Set style for screen_mark_err_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_mark_err_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_mark_err_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_mark_err.update_layout()
# Create screen_warn1
screen_warn1 = lv.obj()
screen_warn1.set_size(240, 280)
screen_warn1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_warn1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_img_bg
screen_warn1_img_bg = lv.image(screen_warn1)
screen_warn1_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1bg_240_280.png"))
screen_warn1_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn1_img_bg.set_pivot(50,50)
screen_warn1_img_bg.set_rotation(0)
screen_warn1_img_bg.set_pos(0, 0)
screen_warn1_img_bg.set_size(240, 280)
# Set style for screen_warn1_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_img_back
screen_warn1_img_back = lv.image(screen_warn1)
screen_warn1_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_warn1_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn1_img_back.set_pivot(50,50)
screen_warn1_img_back.set_rotation(0)
screen_warn1_img_back.set_pos(149, 212)
screen_warn1_img_back.set_size(50, 50)
# Set style for screen_warn1_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_speed
screen_warn1_label_speed = lv.label(screen_warn1)
screen_warn1_label_speed.set_text("120 km/h")
screen_warn1_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_speed.set_width(lv.pct(100))
screen_warn1_label_speed.set_pos(135, 174)
screen_warn1_label_speed.set_size(91, 18)
# Set style for screen_warn1_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_num_mileage
screen_warn1_label_num_mileage = lv.label(screen_warn1)
screen_warn1_label_num_mileage.set_text("K1111+236")
screen_warn1_label_num_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_num_mileage.set_width(lv.pct(100))
screen_warn1_label_num_mileage.set_pos(24, 174)
screen_warn1_label_num_mileage.set_size(105, 20)
# Set style for screen_warn1_label_num_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_num_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_num_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_time
screen_warn1_label_time = lv.label(screen_warn1)
screen_warn1_label_time.set_text("12 : 20")
screen_warn1_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_time.set_width(lv.pct(100))
screen_warn1_label_time.set_pos(129, 55)
screen_warn1_label_time.set_size(61, 26)
# Set style for screen_warn1_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_type
screen_warn1_label_type = lv.label(screen_warn1)
screen_warn1_label_type.set_text("一级报警")
screen_warn1_label_type.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_type.set_width(lv.pct(100))
screen_warn1_label_type.set_pos(24, 50)
screen_warn1_label_type.set_size(99, 25)
# Set style for screen_warn1_label_type, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_type.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_type.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_typev
screen_warn1_label_typev = lv.label(screen_warn1)
screen_warn1_label_typev.set_text("垂")
screen_warn1_label_typev.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_typev.set_width(lv.pct(100))
screen_warn1_label_typev.set_pos(59, 100)
screen_warn1_label_typev.set_size(17, 20)
# Set style for screen_warn1_label_typev, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_typev.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typev.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_V_level
screen_warn1_label_V_level = lv.label(screen_warn1)
screen_warn1_label_V_level.set_text("I")
screen_warn1_label_V_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_V_level.set_width(lv.pct(100))
screen_warn1_label_V_level.set_pos(82, 100)
screen_warn1_label_V_level.set_size(15, 17)
# Set style for screen_warn1_label_V_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_V_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_V_data
screen_warn1_label_V_data = lv.label(screen_warn1)
screen_warn1_label_V_data.set_text("5")
screen_warn1_label_V_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_V_data.set_width(lv.pct(100))
screen_warn1_label_V_data.set_pos(45, 127)
screen_warn1_label_V_data.set_size(64, 30)
# Set style for screen_warn1_label_V_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_V_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_V_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_L_level
screen_warn1_label_L_level = lv.label(screen_warn1)
screen_warn1_label_L_level.set_text("I")
screen_warn1_label_L_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_L_level.set_width(lv.pct(100))
screen_warn1_label_L_level.set_pos(162, 100)
screen_warn1_label_L_level.set_size(15, 17)
# Set style for screen_warn1_label_L_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_L_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_typel
screen_warn1_label_typel = lv.label(screen_warn1)
screen_warn1_label_typel.set_text("水")
screen_warn1_label_typel.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_typel.set_width(lv.pct(100))
screen_warn1_label_typel.set_pos(137, 100)
screen_warn1_label_typel.set_size(17, 20)
# Set style for screen_warn1_label_typel, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_typel.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_typel.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_L_data
screen_warn1_label_L_data = lv.label(screen_warn1)
screen_warn1_label_L_data.set_text("3")
screen_warn1_label_L_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_L_data.set_width(lv.pct(100))
screen_warn1_label_L_data.set_pos(125, 127)
screen_warn1_label_L_data.set_size(64, 30)
# Set style for screen_warn1_label_L_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_L_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_L_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_1
screen_warn1_label_1 = lv.label(screen_warn1)
screen_warn1_label_1.set_text("1234567890")
screen_warn1_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_1.set_width(lv.pct(100))
screen_warn1_label_1.set_pos(42, 132)
screen_warn1_label_1.set_size(100, 32)
screen_warn1_label_1.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn1_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_2
screen_warn1_label_2 = lv.label(screen_warn1)
screen_warn1_label_2.set_text("1234567890：K+km/h")
screen_warn1_label_2.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_2.set_width(lv.pct(100))
screen_warn1_label_2.set_pos(70, 124)
screen_warn1_label_2.set_size(100, 32)
screen_warn1_label_2.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn1_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_3
screen_warn1_label_3 = lv.label(screen_warn1)
screen_warn1_label_3.set_text("IIV")
screen_warn1_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_3.set_width(lv.pct(100))
screen_warn1_label_3.set_pos(70, 124)
screen_warn1_label_3.set_size(100, 32)
screen_warn1_label_3.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn1_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_img_voic
screen_warn1_img_voic = lv.image(screen_warn1)
screen_warn1_img_voic.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum_60_60.png"))
screen_warn1_img_voic.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn1_img_voic.set_pivot(50,50)
screen_warn1_img_voic.set_rotation(0)
screen_warn1_img_voic.set_pos(45, 205)
screen_warn1_img_voic.set_size(60, 60)
# Set style for screen_warn1_img_voic, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_img_voic.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_label_tip
screen_warn1_label_tip = lv.label(screen_warn1)
screen_warn1_label_tip.set_text("录音标记中...")
screen_warn1_label_tip.set_long_mode(lv.label.LONG.WRAP)
screen_warn1_label_tip.set_width(lv.pct(100))
screen_warn1_label_tip.set_pos(38, 7)
screen_warn1_label_tip.set_size(168, 21)
screen_warn1_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn1_label_tip, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_label_tip.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_bg_opa(74, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn1_label_tip.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn1_img_voicing
screen_warn1_img_voicing = lv.image(screen_warn1)
screen_warn1_img_voicing.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum2_60_60.png"))
screen_warn1_img_voicing.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn1_img_voicing.set_pivot(50,50)
screen_warn1_img_voicing.set_rotation(0)
screen_warn1_img_voicing.set_pos(45, 205)
screen_warn1_img_voicing.set_size(60, 60)
screen_warn1_img_voicing.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn1_img_voicing, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn1_img_voicing.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_warn1.update_layout()
# Create screen_warn2
screen_warn2 = lv.obj()
screen_warn2.set_size(240, 280)
screen_warn2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_warn2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_img_bg
screen_warn2_img_bg = lv.image(screen_warn2)
screen_warn2_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2bg_240_280.png"))
screen_warn2_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn2_img_bg.set_pivot(50,50)
screen_warn2_img_bg.set_rotation(0)
screen_warn2_img_bg.set_pos(0, 0)
screen_warn2_img_bg.set_size(240, 280)
# Set style for screen_warn2_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_img_back
screen_warn2_img_back = lv.image(screen_warn2)
screen_warn2_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_warn2_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn2_img_back.set_pivot(50,50)
screen_warn2_img_back.set_rotation(0)
screen_warn2_img_back.set_pos(149, 212)
screen_warn2_img_back.set_size(50, 50)
# Set style for screen_warn2_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_speed
screen_warn2_label_speed = lv.label(screen_warn2)
screen_warn2_label_speed.set_text("120 km/h")
screen_warn2_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_speed.set_width(lv.pct(100))
screen_warn2_label_speed.set_pos(135, 174)
screen_warn2_label_speed.set_size(92, 18)
# Set style for screen_warn2_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_num_mileage
screen_warn2_label_num_mileage = lv.label(screen_warn2)
screen_warn2_label_num_mileage.set_text("K1111+236")
screen_warn2_label_num_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_num_mileage.set_width(lv.pct(100))
screen_warn2_label_num_mileage.set_pos(24, 174)
screen_warn2_label_num_mileage.set_size(105, 20)
# Set style for screen_warn2_label_num_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_num_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_num_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_L_data
screen_warn2_label_L_data = lv.label(screen_warn2)
screen_warn2_label_L_data.set_text("3")
screen_warn2_label_L_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_L_data.set_width(lv.pct(100))
screen_warn2_label_L_data.set_pos(125, 127)
screen_warn2_label_L_data.set_size(64, 30)
# Set style for screen_warn2_label_L_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_L_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_L_level
screen_warn2_label_L_level = lv.label(screen_warn2)
screen_warn2_label_L_level.set_text("I")
screen_warn2_label_L_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_L_level.set_width(lv.pct(100))
screen_warn2_label_L_level.set_pos(162, 100)
screen_warn2_label_L_level.set_size(15, 17)
# Set style for screen_warn2_label_L_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_L_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_L_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_typel
screen_warn2_label_typel = lv.label(screen_warn2)
screen_warn2_label_typel.set_text("水")
screen_warn2_label_typel.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_typel.set_width(lv.pct(100))
screen_warn2_label_typel.set_pos(137, 100)
screen_warn2_label_typel.set_size(17, 20)
# Set style for screen_warn2_label_typel, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_typel.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typel.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_V_data
screen_warn2_label_V_data = lv.label(screen_warn2)
screen_warn2_label_V_data.set_text("15")
screen_warn2_label_V_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_V_data.set_width(lv.pct(100))
screen_warn2_label_V_data.set_pos(45, 127)
screen_warn2_label_V_data.set_size(64, 30)
# Set style for screen_warn2_label_V_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_V_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_V_level
screen_warn2_label_V_level = lv.label(screen_warn2)
screen_warn2_label_V_level.set_text("II")
screen_warn2_label_V_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_V_level.set_width(lv.pct(100))
screen_warn2_label_V_level.set_pos(82, 100)
screen_warn2_label_V_level.set_size(15, 17)
# Set style for screen_warn2_label_V_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_V_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_V_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_typev
screen_warn2_label_typev = lv.label(screen_warn2)
screen_warn2_label_typev.set_text("垂")
screen_warn2_label_typev.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_typev.set_width(lv.pct(100))
screen_warn2_label_typev.set_pos(59, 100)
screen_warn2_label_typev.set_size(17, 20)
# Set style for screen_warn2_label_typev, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_typev.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_typev.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_time
screen_warn2_label_time = lv.label(screen_warn2)
screen_warn2_label_time.set_text("12 : 20")
screen_warn2_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_time.set_width(lv.pct(100))
screen_warn2_label_time.set_pos(129, 55)
screen_warn2_label_time.set_size(61, 26)
# Set style for screen_warn2_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_type
screen_warn2_label_type = lv.label(screen_warn2)
screen_warn2_label_type.set_text("二级报警")
screen_warn2_label_type.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_type.set_width(lv.pct(100))
screen_warn2_label_type.set_pos(24, 50)
screen_warn2_label_type.set_size(99, 25)
# Set style for screen_warn2_label_type, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_type.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_type.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_img_voic
screen_warn2_img_voic = lv.image(screen_warn2)
screen_warn2_img_voic.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum_60_60.png"))
screen_warn2_img_voic.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn2_img_voic.set_pivot(50,50)
screen_warn2_img_voic.set_rotation(0)
screen_warn2_img_voic.set_pos(45, 205)
screen_warn2_img_voic.set_size(60, 60)
# Set style for screen_warn2_img_voic, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_img_voic.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_label_tip
screen_warn2_label_tip = lv.label(screen_warn2)
screen_warn2_label_tip.set_text("录音标记中...")
screen_warn2_label_tip.set_long_mode(lv.label.LONG.WRAP)
screen_warn2_label_tip.set_width(lv.pct(100))
screen_warn2_label_tip.set_pos(38, 7)
screen_warn2_label_tip.set_size(168, 21)
screen_warn2_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn2_label_tip, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_label_tip.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_bg_opa(74, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn2_label_tip.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn2_img_voicing
screen_warn2_img_voicing = lv.image(screen_warn2)
screen_warn2_img_voicing.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum2_60_60.png"))
screen_warn2_img_voicing.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn2_img_voicing.set_pivot(50,50)
screen_warn2_img_voicing.set_rotation(0)
screen_warn2_img_voicing.set_pos(45, 205)
screen_warn2_img_voicing.set_size(60, 60)
screen_warn2_img_voicing.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn2_img_voicing, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn2_img_voicing.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_warn2.update_layout()
# Create screen_warn3
screen_warn3 = lv.obj()
screen_warn3.set_size(240, 280)
screen_warn3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_warn3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_img_bg
screen_warn3_img_bg = lv.image(screen_warn3)
screen_warn3_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\3bg_240_280.png"))
screen_warn3_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn3_img_bg.set_pivot(50,50)
screen_warn3_img_bg.set_rotation(0)
screen_warn3_img_bg.set_pos(0, 0)
screen_warn3_img_bg.set_size(240, 280)
# Set style for screen_warn3_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_img_back
screen_warn3_img_back = lv.image(screen_warn3)
screen_warn3_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_warn3_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn3_img_back.set_pivot(50,50)
screen_warn3_img_back.set_rotation(0)
screen_warn3_img_back.set_pos(149, 212)
screen_warn3_img_back.set_size(50, 50)
# Set style for screen_warn3_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_speed
screen_warn3_label_speed = lv.label(screen_warn3)
screen_warn3_label_speed.set_text("120 km/h")
screen_warn3_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_speed.set_width(lv.pct(100))
screen_warn3_label_speed.set_pos(135, 174)
screen_warn3_label_speed.set_size(93, 18)
# Set style for screen_warn3_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_num_mileage
screen_warn3_label_num_mileage = lv.label(screen_warn3)
screen_warn3_label_num_mileage.set_text("K1111+236")
screen_warn3_label_num_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_num_mileage.set_width(lv.pct(100))
screen_warn3_label_num_mileage.set_pos(24, 174)
screen_warn3_label_num_mileage.set_size(111, 20)
# Set style for screen_warn3_label_num_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_num_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_num_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_L_data
screen_warn3_label_L_data = lv.label(screen_warn3)
screen_warn3_label_L_data.set_text("25")
screen_warn3_label_L_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_L_data.set_width(lv.pct(100))
screen_warn3_label_L_data.set_pos(125, 127)
screen_warn3_label_L_data.set_size(64, 30)
# Set style for screen_warn3_label_L_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_L_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_L_level
screen_warn3_label_L_level = lv.label(screen_warn3)
screen_warn3_label_L_level.set_text("III")
screen_warn3_label_L_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_L_level.set_width(lv.pct(100))
screen_warn3_label_L_level.set_pos(162, 100)
screen_warn3_label_L_level.set_size(21, 17)
# Set style for screen_warn3_label_L_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_L_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_L_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_typel
screen_warn3_label_typel = lv.label(screen_warn3)
screen_warn3_label_typel.set_text("水")
screen_warn3_label_typel.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_typel.set_width(lv.pct(100))
screen_warn3_label_typel.set_pos(137, 100)
screen_warn3_label_typel.set_size(17, 20)
# Set style for screen_warn3_label_typel, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_typel.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typel.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_V_data
screen_warn3_label_V_data = lv.label(screen_warn3)
screen_warn3_label_V_data.set_text("15")
screen_warn3_label_V_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_V_data.set_width(lv.pct(100))
screen_warn3_label_V_data.set_pos(45, 127)
screen_warn3_label_V_data.set_size(64, 30)
# Set style for screen_warn3_label_V_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_V_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_V_level
screen_warn3_label_V_level = lv.label(screen_warn3)
screen_warn3_label_V_level.set_text("II")
screen_warn3_label_V_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_V_level.set_width(lv.pct(100))
screen_warn3_label_V_level.set_pos(83, 100)
screen_warn3_label_V_level.set_size(21, 17)
# Set style for screen_warn3_label_V_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_V_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_V_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_typev
screen_warn3_label_typev = lv.label(screen_warn3)
screen_warn3_label_typev.set_text("垂")
screen_warn3_label_typev.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_typev.set_width(lv.pct(100))
screen_warn3_label_typev.set_pos(59, 100)
screen_warn3_label_typev.set_size(17, 20)
# Set style for screen_warn3_label_typev, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_typev.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_typev.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_time
screen_warn3_label_time = lv.label(screen_warn3)
screen_warn3_label_time.set_text("12 : 20")
screen_warn3_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_time.set_width(lv.pct(100))
screen_warn3_label_time.set_pos(129, 55)
screen_warn3_label_time.set_size(61, 26)
# Set style for screen_warn3_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_type
screen_warn3_label_type = lv.label(screen_warn3)
screen_warn3_label_type.set_text("三级报警")
screen_warn3_label_type.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_type.set_width(lv.pct(100))
screen_warn3_label_type.set_pos(24, 50)
screen_warn3_label_type.set_size(99, 25)
# Set style for screen_warn3_label_type, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_type.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_type.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_img_voic
screen_warn3_img_voic = lv.image(screen_warn3)
screen_warn3_img_voic.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum_60_60.png"))
screen_warn3_img_voic.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn3_img_voic.set_pivot(50,50)
screen_warn3_img_voic.set_rotation(0)
screen_warn3_img_voic.set_pos(45, 205)
screen_warn3_img_voic.set_size(60, 60)
# Set style for screen_warn3_img_voic, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_img_voic.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_label_tip
screen_warn3_label_tip = lv.label(screen_warn3)
screen_warn3_label_tip.set_text("录音标记中...")
screen_warn3_label_tip.set_long_mode(lv.label.LONG.WRAP)
screen_warn3_label_tip.set_width(lv.pct(100))
screen_warn3_label_tip.set_pos(38, 7)
screen_warn3_label_tip.set_size(168, 21)
screen_warn3_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn3_label_tip, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_label_tip.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_bg_opa(75, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn3_label_tip.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn3_img_voicing
screen_warn3_img_voicing = lv.image(screen_warn3)
screen_warn3_img_voicing.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum2_60_60.png"))
screen_warn3_img_voicing.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn3_img_voicing.set_pivot(50,50)
screen_warn3_img_voicing.set_rotation(0)
screen_warn3_img_voicing.set_pos(45, 205)
screen_warn3_img_voicing.set_size(60, 60)
screen_warn3_img_voicing.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn3_img_voicing, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn3_img_voicing.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_warn3.update_layout()
# Create screen_warn4
screen_warn4 = lv.obj()
screen_warn4.set_size(240, 280)
screen_warn4.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_warn4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_img_bg
screen_warn4_img_bg = lv.image(screen_warn4)
screen_warn4_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\4bg_240_280.png"))
screen_warn4_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn4_img_bg.set_pivot(50,50)
screen_warn4_img_bg.set_rotation(0)
screen_warn4_img_bg.set_pos(0, 0)
screen_warn4_img_bg.set_size(240, 280)
# Set style for screen_warn4_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_img_back
screen_warn4_img_back = lv.image(screen_warn4)
screen_warn4_img_back.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\close_50_50.png"))
screen_warn4_img_back.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn4_img_back.set_pivot(50,50)
screen_warn4_img_back.set_rotation(0)
screen_warn4_img_back.set_pos(149, 212)
screen_warn4_img_back.set_size(50, 50)
# Set style for screen_warn4_img_back, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_img_back.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_speed
screen_warn4_label_speed = lv.label(screen_warn4)
screen_warn4_label_speed.set_text("120 km/h")
screen_warn4_label_speed.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_speed.set_width(lv.pct(100))
screen_warn4_label_speed.set_pos(135, 174)
screen_warn4_label_speed.set_size(94, 20)
# Set style for screen_warn4_label_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_num_mileage
screen_warn4_label_num_mileage = lv.label(screen_warn4)
screen_warn4_label_num_mileage.set_text("K1111+236")
screen_warn4_label_num_mileage.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_num_mileage.set_width(lv.pct(100))
screen_warn4_label_num_mileage.set_pos(24, 174)
screen_warn4_label_num_mileage.set_size(111, 20)
# Set style for screen_warn4_label_num_mileage, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_num_mileage.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_num_mileage.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_L_data
screen_warn4_label_L_data = lv.label(screen_warn4)
screen_warn4_label_L_data.set_text("22")
screen_warn4_label_L_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_L_data.set_width(lv.pct(100))
screen_warn4_label_L_data.set_pos(125, 127)
screen_warn4_label_L_data.set_size(64, 30)
# Set style for screen_warn4_label_L_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_L_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_L_level
screen_warn4_label_L_level = lv.label(screen_warn4)
screen_warn4_label_L_level.set_text("III")
screen_warn4_label_L_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_L_level.set_width(lv.pct(100))
screen_warn4_label_L_level.set_pos(162, 100)
screen_warn4_label_L_level.set_size(25, 17)
# Set style for screen_warn4_label_L_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_L_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_L_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_typel
screen_warn4_label_typel = lv.label(screen_warn4)
screen_warn4_label_typel.set_text("水")
screen_warn4_label_typel.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_typel.set_width(lv.pct(100))
screen_warn4_label_typel.set_pos(137, 100)
screen_warn4_label_typel.set_size(17, 20)
# Set style for screen_warn4_label_typel, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_typel.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typel.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_V_data
screen_warn4_label_V_data = lv.label(screen_warn4)
screen_warn4_label_V_data.set_text("36")
screen_warn4_label_V_data.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_V_data.set_width(lv.pct(100))
screen_warn4_label_V_data.set_pos(45, 127)
screen_warn4_label_V_data.set_size(64, 30)
# Set style for screen_warn4_label_V_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_V_data.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_text_font(test_font("PingFangHeavy", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_V_level
screen_warn4_label_V_level = lv.label(screen_warn4)
screen_warn4_label_V_level.set_text("IV")
screen_warn4_label_V_level.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_V_level.set_width(lv.pct(100))
screen_warn4_label_V_level.set_pos(82, 100)
screen_warn4_label_V_level.set_size(27, 17)
# Set style for screen_warn4_label_V_level, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_V_level.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_V_level.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_typev
screen_warn4_label_typev = lv.label(screen_warn4)
screen_warn4_label_typev.set_text("垂")
screen_warn4_label_typev.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_typev.set_width(lv.pct(100))
screen_warn4_label_typev.set_pos(59, 100)
screen_warn4_label_typev.set_size(17, 20)
# Set style for screen_warn4_label_typev, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_typev.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_typev.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_time
screen_warn4_label_time = lv.label(screen_warn4)
screen_warn4_label_time.set_text("12 : 20")
screen_warn4_label_time.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_time.set_width(lv.pct(100))
screen_warn4_label_time.set_pos(129, 55)
screen_warn4_label_time.set_size(61, 26)
# Set style for screen_warn4_label_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_time.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_type
screen_warn4_label_type = lv.label(screen_warn4)
screen_warn4_label_type.set_text("四级报警")
screen_warn4_label_type.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_type.set_width(lv.pct(100))
screen_warn4_label_type.set_pos(24, 50)
screen_warn4_label_type.set_size(99, 25)
# Set style for screen_warn4_label_type, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_type.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_type.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_img_voic
screen_warn4_img_voic = lv.image(screen_warn4)
screen_warn4_img_voic.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum_60_60.png"))
screen_warn4_img_voic.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn4_img_voic.set_pivot(50,50)
screen_warn4_img_voic.set_rotation(0)
screen_warn4_img_voic.set_pos(45, 205)
screen_warn4_img_voic.set_size(60, 60)
# Set style for screen_warn4_img_voic, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_img_voic.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_label_tip
screen_warn4_label_tip = lv.label(screen_warn4)
screen_warn4_label_tip.set_text("录音标记中...")
screen_warn4_label_tip.set_long_mode(lv.label.LONG.WRAP)
screen_warn4_label_tip.set_width(lv.pct(100))
screen_warn4_label_tip.set_pos(38, 7)
screen_warn4_label_tip.set_size(168, 21)
screen_warn4_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn4_label_tip, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_label_tip.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_text_color(lv.color_hex(0xfbfd46), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_bg_opa(74, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_warn4_label_tip.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_warn4_img_voicing
screen_warn4_img_voicing = lv.image(screen_warn4)
screen_warn4_img_voicing.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\volum2_60_60.png"))
screen_warn4_img_voicing.add_flag(lv.obj.FLAG.CLICKABLE)
screen_warn4_img_voicing.set_pivot(50,50)
screen_warn4_img_voicing.set_rotation(0)
screen_warn4_img_voicing.set_pos(45, 205)
screen_warn4_img_voicing.set_size(60, 60)
screen_warn4_img_voicing.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_warn4_img_voicing, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_warn4_img_voicing.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_warn4.update_layout()
# Create screen_volume
screen_volume = lv.obj()
screen_volume.set_size(240, 280)
screen_volume.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_volume, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_volume_img_bg
screen_volume_img_bg = lv.image(screen_volume)
screen_volume_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_volume_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_volume_img_bg.set_pivot(50,50)
screen_volume_img_bg.set_rotation(0)
screen_volume_img_bg.set_pos(0, 0)
screen_volume_img_bg.set_size(240, 280)
# Set style for screen_volume_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_volume_cont_list
screen_volume_cont_list = lv.obj(screen_volume)
screen_volume_cont_list.set_pos(11, 61)
screen_volume_cont_list.set_size(219, 210)
screen_volume_cont_list.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_volume_cont_list, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume_cont_list.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_cont_list.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_volume_slider_1
screen_volume_slider_1 = lv.slider(screen_volume_cont_list)
screen_volume_slider_1.set_range(0, 100)
screen_volume_slider_1.set_mode(lv.slider.MODE.NORMAL)
screen_volume_slider_1.set_value(100, lv.ANIM.OFF)
screen_volume_slider_1.set_pos(28, 30)
screen_volume_slider_1.set_size(154, 40)
# Set style for screen_volume_slider_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume_slider_1.set_style_bg_opa(60, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_volume_slider_1, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_volume_slider_1.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_radius(8, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_volume_slider_1, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_volume_slider_1.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_volume_slider_1.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_volume_label_vol_num
screen_volume_label_vol_num = lv.label(screen_volume_cont_list)
screen_volume_label_vol_num.set_text("100")
screen_volume_label_vol_num.set_long_mode(lv.label.LONG.WRAP)
screen_volume_label_vol_num.set_width(lv.pct(100))
screen_volume_label_vol_num.set_pos(133, 113)
screen_volume_label_vol_num.set_size(54, 16)
# Set style for screen_volume_label_vol_num, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume_label_vol_num.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_vol_num.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_volume_label_1
screen_volume_label_1 = lv.label(screen_volume_cont_list)
screen_volume_label_1.set_text("音量 ")
screen_volume_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_volume_label_1.set_width(lv.pct(100))
screen_volume_label_1.set_pos(29, 113)
screen_volume_label_1.set_size(54, 20)
# Set style for screen_volume_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_volume_label_title
screen_volume_label_title = lv.label(screen_volume)
screen_volume_label_title.set_text("音量设置")
screen_volume_label_title.set_long_mode(lv.label.LONG.WRAP)
screen_volume_label_title.set_width(lv.pct(100))
screen_volume_label_title.set_pos(55, 10)
screen_volume_label_title.set_size(130, 30)
# Set style for screen_volume_label_title, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_volume_label_title.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_volume_label_title.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_volume.update_layout()
# Create screen_bledata
screen_bledata = lv.obj()
screen_bledata.set_size(240, 280)
screen_bledata.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_bledata, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bledata_img_bg
screen_bledata_img_bg = lv.image(screen_bledata)
screen_bledata_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_bledata_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bledata_img_bg.set_pivot(50,50)
screen_bledata_img_bg.set_rotation(0)
screen_bledata_img_bg.set_pos(0, 0)
screen_bledata_img_bg.set_size(240, 280)
# Set style for screen_bledata_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bledata_cont_ble_data
screen_bledata_cont_ble_data = lv.obj(screen_bledata)
screen_bledata_cont_ble_data.set_pos(9, 54)
screen_bledata_cont_ble_data.set_size(219, 216)
screen_bledata_cont_ble_data.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_bledata_cont_ble_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_cont_ble_data.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_cont_ble_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_bledata_label_blemac
screen_bledata_label_blemac = lv.label(screen_bledata_cont_ble_data)
screen_bledata_label_blemac.set_text("蓝牙MAC\n")
screen_bledata_label_blemac.set_long_mode(lv.label.LONG.WRAP)
screen_bledata_label_blemac.set_width(lv.pct(100))
screen_bledata_label_blemac.set_pos(25, 76)
screen_bledata_label_blemac.set_size(75, 20)
# Set style for screen_bledata_label_blemac, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_label_blemac.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemac.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bledata_label_blename
screen_bledata_label_blename = lv.label(screen_bledata_cont_ble_data)
screen_bledata_label_blename.set_text("蓝牙名称\n")
screen_bledata_label_blename.set_long_mode(lv.label.LONG.WRAP)
screen_bledata_label_blename.set_width(lv.pct(100))
screen_bledata_label_blename.set_pos(25, 10)
screen_bledata_label_blename.set_size(75, 20)
# Set style for screen_bledata_label_blename, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_label_blename.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blename.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bledata_label_blenamedata
screen_bledata_label_blenamedata = lv.label(screen_bledata_cont_ble_data)
screen_bledata_label_blenamedata.set_text("xxxxxxxxxxxx")
screen_bledata_label_blenamedata.set_long_mode(lv.label.LONG.WRAP)
screen_bledata_label_blenamedata.set_width(lv.pct(100))
screen_bledata_label_blenamedata.set_pos(25, 39)
screen_bledata_label_blenamedata.set_size(184, 23)
# Set style for screen_bledata_label_blenamedata, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_label_blenamedata.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blenamedata.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bledata_label_blemacdata
screen_bledata_label_blemacdata = lv.label(screen_bledata_cont_ble_data)
screen_bledata_label_blemacdata.set_text("xxxxxxxxxxxx\n")
screen_bledata_label_blemacdata.set_long_mode(lv.label.LONG.WRAP)
screen_bledata_label_blemacdata.set_width(lv.pct(100))
screen_bledata_label_blemacdata.set_pos(25, 104)
screen_bledata_label_blemacdata.set_size(185, 24)
# Set style for screen_bledata_label_blemacdata, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_label_blemacdata.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_text_font(test_font("PingFangHeavy", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_blemacdata.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bledata_label_title
screen_bledata_label_title = lv.label(screen_bledata)
screen_bledata_label_title.set_text("蓝牙信息")
screen_bledata_label_title.set_long_mode(lv.label.LONG.WRAP)
screen_bledata_label_title.set_width(lv.pct(100))
screen_bledata_label_title.set_pos(55, 10)
screen_bledata_label_title.set_size(130, 30)
# Set style for screen_bledata_label_title, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bledata_label_title.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bledata_label_title.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_bledata.update_layout()
# Create screen_record
screen_record = lv.obj()
screen_record.set_size(240, 280)
screen_record.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_record, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_record.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_record_img_bg
screen_record_img_bg = lv.image(screen_record)
screen_record_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_record_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_record_img_bg.set_pivot(50,50)
screen_record_img_bg.set_rotation(0)
screen_record_img_bg.set_pos(0, 0)
screen_record_img_bg.set_size(240, 280)
# Set style for screen_record_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_record_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_record_animimg_1
screen_record_animimg_1 = lv.animimg(screen_record)
screen_record_animimg_1_imgs = [None]*6
screen_record_animimg_1_imgs[0] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1_0_100_100.png")
screen_record_animimg_1_imgs[1] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1_1_100_100.png")
screen_record_animimg_1_imgs[2] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1_2_100_100.png")
screen_record_animimg_1_imgs[3] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1_3_100_100.png")
screen_record_animimg_1_imgs[4] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1_4_100_100.png")
screen_record_animimg_1_imgs[5] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\1_5_100_100.png")
screen_record_animimg_1.set_src(screen_record_animimg_1_imgs, 6, False)
screen_record_animimg_1.set_duration(100*6)
screen_record_animimg_1.set_repeat_count(lv.ANIM_REPEAT_INFINITE)
screen_record_animimg_1.start()
screen_record_animimg_1.set_pos(70, 144)
screen_record_animimg_1.set_size(100, 100)

# Create screen_record_label_5
screen_record_label_5 = lv.label(screen_record)
screen_record_label_5.set_text("录音标记中...")
screen_record_label_5.set_long_mode(lv.label.LONG.WRAP)
screen_record_label_5.set_width(lv.pct(100))
screen_record_label_5.set_pos(55, 10)
screen_record_label_5.set_size(157, 30)
# Set style for screen_record_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_record_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_record_animimg_2
screen_record_animimg_2 = lv.animimg(screen_record)
screen_record_animimg_2_imgs = [None]*10
screen_record_animimg_2_imgs[0] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_18_207_92.png")
screen_record_animimg_2_imgs[1] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_19_207_92.png")
screen_record_animimg_2_imgs[2] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_2_207_92.png")
screen_record_animimg_2_imgs[3] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_3_207_92.png")
screen_record_animimg_2_imgs[4] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_4_207_92.png")
screen_record_animimg_2_imgs[5] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_5_207_92.png")
screen_record_animimg_2_imgs[6] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_6_207_92.png")
screen_record_animimg_2_imgs[7] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_7_207_92.png")
screen_record_animimg_2_imgs[8] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_8_207_92.png")
screen_record_animimg_2_imgs[9] = load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\2_9_207_92.png")
screen_record_animimg_2.set_src(screen_record_animimg_2_imgs, 10, False)
screen_record_animimg_2.set_duration(80*10)
screen_record_animimg_2.set_repeat_count(lv.ANIM_REPEAT_INFINITE)
screen_record_animimg_2.start()
screen_record_animimg_2.set_pos(16, 43)
screen_record_animimg_2.set_size(207, 92)

# Create screen_record_label_6
screen_record_label_6 = lv.label(screen_record)
screen_record_label_6.set_text("正在保存录音...")
screen_record_label_6.set_long_mode(lv.label.LONG.WRAP)
screen_record_label_6.set_width(lv.pct(100))
screen_record_label_6.set_pos(70, 124)
screen_record_label_6.set_size(100, 32)
screen_record_label_6.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_record_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_record_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_record_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_record.update_layout()
# Create screen_ota
screen_ota = lv.obj()
screen_ota.set_size(240, 280)
screen_ota.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_ota, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ota_img_bg
screen_ota_img_bg = lv.image(screen_ota)
screen_ota_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_ota_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_ota_img_bg.set_pivot(50,50)
screen_ota_img_bg.set_rotation(0)
screen_ota_img_bg.set_pos(0, 0)
screen_ota_img_bg.set_size(240, 280)
# Set style for screen_ota_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ota_bar_ota
screen_ota_bar_ota = lv.bar(screen_ota)
screen_ota_bar_ota.set_style_anim_duration(500, 0)
screen_ota_bar_ota.set_mode(lv.bar.MODE.NORMAL)
screen_ota_bar_ota.set_range(0, 100)
screen_ota_bar_ota.set_value(1, lv.ANIM.OFF)
screen_ota_bar_ota.set_pos(20, 127)
screen_ota_bar_ota.set_size(201, 30)
# Set style for screen_ota_bar_ota, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_bar_ota.set_style_bg_opa(60, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_ota_bar_ota, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_ota_bar_ota.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_ota_bar_ota.set_style_radius(10, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Create screen_ota_label_1
screen_ota_label_1 = lv.label(screen_ota)
screen_ota_label_1.set_text("程序升级中")
screen_ota_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_ota_label_1.set_width(lv.pct(100))
screen_ota_label_1.set_pos(42, 58)
screen_ota_label_1.set_size(157, 32)
# Set style for screen_ota_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ota_label_ota
screen_ota_label_ota = lv.label(screen_ota)
screen_ota_label_ota.set_text("(1/10KB) 已完成")
screen_ota_label_ota.set_long_mode(lv.label.LONG.WRAP)
screen_ota_label_ota.set_width(lv.pct(100))
screen_ota_label_ota.set_pos(17, 175)
screen_ota_label_ota.set_size(209, 28)
# Set style for screen_ota_label_ota, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_label_ota.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_ota.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ota_label_3
screen_ota_label_3 = lv.label(screen_ota)
screen_ota_label_3.set_text("0123456789% 已完成正在重启")
screen_ota_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_ota_label_3.set_width(lv.pct(100))
screen_ota_label_3.set_pos(51, 108)
screen_ota_label_3.set_size(134, 32)
screen_ota_label_3.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_ota_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ota_label_4
screen_ota_label_4 = lv.label(screen_ota)
screen_ota_label_4.set_text("1%")
screen_ota_label_4.set_long_mode(lv.label.LONG.WRAP)
screen_ota_label_4.set_width(lv.pct(100))
screen_ota_label_4.set_pos(68, 93)
screen_ota_label_4.set_size(100, 25)
# Set style for screen_ota_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ota_label_5
screen_ota_label_5 = lv.label(screen_ota)
screen_ota_label_5.set_text("正在重启")
screen_ota_label_5.set_long_mode(lv.label.LONG.WRAP)
screen_ota_label_5.set_width(lv.pct(100))
screen_ota_label_5.set_pos(68, 208)
screen_ota_label_5.set_size(100, 32)
# Set style for screen_ota_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ota_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_text_font(test_font("PingFangHeavy", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ota_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_ota.update_layout()
# Create screen_sleep_set
screen_sleep_set = lv.obj()
screen_sleep_set.set_size(240, 280)
screen_sleep_set.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_sleep_set, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sleep_set.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sleep_set_img_bg
screen_sleep_set_img_bg = lv.image(screen_sleep_set)
screen_sleep_set_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_sleep_set_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_sleep_set_img_bg.set_pivot(50,50)
screen_sleep_set_img_bg.set_rotation(0)
screen_sleep_set_img_bg.set_pos(0, 0)
screen_sleep_set_img_bg.set_size(240, 280)
# Set style for screen_sleep_set_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sleep_set_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sleep_set_cont_data
screen_sleep_set_cont_data = lv.obj(screen_sleep_set)
screen_sleep_set_cont_data.set_pos(6, 48)
screen_sleep_set_cont_data.set_size(230, 227)
screen_sleep_set_cont_data.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_sleep_set_cont_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sleep_set_cont_data.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_cont_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_sleep_set_label_1
screen_sleep_set_label_1 = lv.label(screen_sleep_set_cont_data)
screen_sleep_set_label_1.set_text("无操作间隔时间")
screen_sleep_set_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_sleep_set_label_1.set_width(lv.pct(100))
screen_sleep_set_label_1.set_pos(49, 132)
screen_sleep_set_label_1.set_size(144, 21)
# Set style for screen_sleep_set_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sleep_set_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sleep_set_label_title
screen_sleep_set_label_title = lv.label(screen_sleep_set)
screen_sleep_set_label_title.set_text("休眠设置")
screen_sleep_set_label_title.set_long_mode(lv.label.LONG.WRAP)
screen_sleep_set_label_title.set_width(lv.pct(100))
screen_sleep_set_label_title.set_pos(55, 10)
screen_sleep_set_label_title.set_size(130, 30)
# Set style for screen_sleep_set_label_title, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sleep_set_label_title.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_label_title.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sleep_set_ddlist_1
screen_sleep_set_ddlist_1 = lv.dropdown(screen_sleep_set)
screen_sleep_set_ddlist_1.set_options("不休眠\n15秒\n30秒\n1分钟\n2分钟\n10分钟")
screen_sleep_set_ddlist_1.set_pos(66, 93)
screen_sleep_set_ddlist_1.set_size(124, 31)
# Set style for screen_sleep_set_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sleep_set_ddlist_1.set_style_text_color(lv.color_hex(0x0D3055), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_border_color(lv.color_hex(0xe1e6ee), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_radius(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sleep_set_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sleep_set_ddlist_1, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_screen_sleep_set_ddlist_1_extra_list_selected_checked = lv.style_t()
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.init()
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_border_width(1)
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_border_opa(255)
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_border_color(lv.color_hex(0xe1e6ee))
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_radius(3)
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_bg_opa(255)
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_bg_color(lv.color_hex(0x00a1b5))
style_screen_sleep_set_ddlist_1_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_sleep_set_ddlist_1.get_list().add_style(style_screen_sleep_set_ddlist_1_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for screen_sleep_set_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_sleep_set_ddlist_1_extra_list_main_default = lv.style_t()
style_screen_sleep_set_ddlist_1_extra_list_main_default.init()
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_max_height(90)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_text_color(lv.color_hex(0x0D3055))
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_text_font(test_font("PingFangHeavy", 18))
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_text_opa(255)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_border_width(1)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_border_opa(255)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_border_color(lv.color_hex(0xe1e6ee))
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_radius(3)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_bg_opa(255)
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_bg_color(lv.color_hex(0xffffff))
style_screen_sleep_set_ddlist_1_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_sleep_set_ddlist_1.get_list().add_style(style_screen_sleep_set_ddlist_1_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_sleep_set_ddlist_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default = lv.style_t()
style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default.init()
style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default.set_radius(3)
style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default.set_bg_opa(255)
style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x00ff00))
style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_sleep_set_ddlist_1.get_list().add_style(style_screen_sleep_set_ddlist_1_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

screen_sleep_set.update_layout()
# Create screen_Instructions
screen_Instructions = lv.obj()
screen_Instructions.set_size(240, 280)
screen_Instructions.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_Instructions, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Instructions.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Instructions_img_bg
screen_Instructions_img_bg = lv.image(screen_Instructions)
screen_Instructions_img_bg.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\bg_new_240_280.png"))
screen_Instructions_img_bg.add_flag(lv.obj.FLAG.CLICKABLE)
screen_Instructions_img_bg.set_pivot(50,50)
screen_Instructions_img_bg.set_rotation(0)
screen_Instructions_img_bg.set_pos(0, 0)
screen_Instructions_img_bg.set_size(240, 280)
screen_Instructions_img_bg.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_Instructions_img_bg, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Instructions_img_bg.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Instructions_cont_data
screen_Instructions_cont_data = lv.obj(screen_Instructions)
screen_Instructions_cont_data.set_pos(5, 48)
screen_Instructions_cont_data.set_size(230, 227)
screen_Instructions_cont_data.add_flag(lv.obj.FLAG.HIDDEN)
screen_Instructions_cont_data.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_Instructions_cont_data, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Instructions_cont_data.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_cont_data.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Instructions_label_title
screen_Instructions_label_title = lv.label(screen_Instructions)
screen_Instructions_label_title.set_text("使用说明")
screen_Instructions_label_title.set_long_mode(lv.label.LONG.WRAP)
screen_Instructions_label_title.set_width(lv.pct(100))
screen_Instructions_label_title.set_pos(55, 10)
screen_Instructions_label_title.set_size(130, 30)
screen_Instructions_label_title.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_Instructions_label_title, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Instructions_label_title.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_text_font(test_font("PingFangHeavy", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_Instructions_label_title.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_Instructions_img_1
screen_Instructions_img_1 = lv.image(screen_Instructions)
screen_Instructions_img_1.set_src(load_image(r"C:\Users\Administrator\Desktop\ZNYKQ_LVGL_SL\ZNYKQ_2\generated\MicroPython\shiyongshuoming_240_1547.png"))
screen_Instructions_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_Instructions_img_1.set_pivot(50,50)
screen_Instructions_img_1.set_rotation(0)
screen_Instructions_img_1.set_pos(0, 0)
screen_Instructions_img_1.set_size(240, 1547)
# Set style for screen_Instructions_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_Instructions_img_1.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_Instructions.update_layout()

def screen_main_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_log, lv.SCR_LOAD_ANIM.MOVE_LEFT, 200, 200, False)
        

    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.BOTTOM == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_set, lv.SCR_LOAD_ANIM.MOVE_BOTTOM, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_mileage_reminder, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
        

screen_main.add_event_cb(lambda e: screen_main_event_handler(e), lv.EVENT.ALL, None)

def screen_main_img_voickey_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.LONG_PRESSED):
        pass
        

        screen_main_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_label_tip2.remove_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_img_voickey2.remove_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_img_voickey.add_flag(lv.obj.FLAG.HIDDEN)
        
screen_main_img_voickey.add_event_cb(lambda e: screen_main_img_voickey_event_handler(e), lv.EVENT.ALL, None)

def screen_main_img_play_run_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

        screen_main_img_play_run.add_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_img_play_stop.remove_flag(lv.obj.FLAG.HIDDEN)
        
screen_main_img_play_run.add_event_cb(lambda e: screen_main_img_play_run_event_handler(e), lv.EVENT.ALL, None)

def screen_main_img_play_stop_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        screen_main_img_play_stop.add_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_img_play_run.remove_flag(lv.obj.FLAG.HIDDEN)
        
        

screen_main_img_play_stop.add_event_cb(lambda e: screen_main_img_play_stop_event_handler(e), lv.EVENT.ALL, None)

def screen_main_img_voickey2_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        screen_main_img_voickey2.add_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_img_voickey.remove_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_label_tip.remove_flag(lv.obj.FLAG.HIDDEN)
        
        screen_main_label_tip2.add_flag(lv.obj.FLAG.HIDDEN)
        
        

screen_main_img_voickey2.add_event_cb(lambda e: screen_main_img_voickey2_event_handler(e), lv.EVENT.ALL, None)

def screen_mileage_reminder_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_log, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
        

    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_main, lv.SCR_LOAD_ANIM.MOVE_LEFT, 200, 200, False)
        

screen_mileage_reminder.add_event_cb(lambda e: screen_mileage_reminder_event_handler(e), lv.EVENT.ALL, None)

def screen_set_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_main, lv.SCR_LOAD_ANIM.MOVE_TOP, 200, 200, False)
screen_set.add_event_cb(lambda e: screen_set_event_handler(e), lv.EVENT.ALL, None)

def screen_set_cont_7_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_set_cont_7.add_event_cb(lambda e: screen_set_cont_7_event_handler(e), lv.EVENT.ALL, None)

def screen_set_cont_8_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_set_cont_8.add_event_cb(lambda e: screen_set_cont_8_event_handler(e), lv.EVENT.ALL, None)

def screen_set_cont_9_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_set_cont_9.add_event_cb(lambda e: screen_set_cont_9_event_handler(e), lv.EVENT.ALL, None)

def screen_set_cont_10_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_set_cont_10.add_event_cb(lambda e: screen_set_cont_10_event_handler(e), lv.EVENT.ALL, None)

def screen_set_cont_11_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_set_cont_11.add_event_cb(lambda e: screen_set_cont_11_event_handler(e), lv.EVENT.ALL, None)

def screen_log_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_mileage_reminder, lv.SCR_LOAD_ANIM.MOVE_LEFT, 200, 200, False)
        

    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_main, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
        

screen_log.add_event_cb(lambda e: screen_log_event_handler(e), lv.EVENT.ALL, None)

def screen_overlimt_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_log, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
        

screen_overlimt.add_event_cb(lambda e: screen_overlimt_event_handler(e), lv.EVENT.ALL, None)

def screen_overlimt_btn_overlimt_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_overlimt_btn_overlimt.add_event_cb(lambda e: screen_overlimt_btn_overlimt_event_handler(e), lv.EVENT.ALL, None)

def screen_overlimt_btn_mark_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_overlimt_btn_mark.add_event_cb(lambda e: screen_overlimt_btn_mark_event_handler(e), lv.EVENT.ALL, None)

def screen_overlimt_btn_audiomark_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_overlimt_btn_audiomark.add_event_cb(lambda e: screen_overlimt_btn_audiomark_event_handler(e), lv.EVENT.ALL, None)

def screen_mark_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_log, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
        

screen_mark.add_event_cb(lambda e: screen_mark_event_handler(e), lv.EVENT.ALL, None)

def screen_mark_btn_overlimt_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_mark_btn_overlimt.add_event_cb(lambda e: screen_mark_btn_overlimt_event_handler(e), lv.EVENT.ALL, None)

def screen_mark_btn_mark_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_mark_btn_mark.add_event_cb(lambda e: screen_mark_btn_mark_event_handler(e), lv.EVENT.ALL, None)

def screen_mark_btn_audio_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_mark_btn_audio.add_event_cb(lambda e: screen_mark_btn_audio_event_handler(e), lv.EVENT.ALL, None)

def screen_audiomark_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_log, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
        

screen_audiomark.add_event_cb(lambda e: screen_audiomark_event_handler(e), lv.EVENT.ALL, None)

def screen_audiomark_btn_overlimt_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_audiomark_btn_overlimt.add_event_cb(lambda e: screen_audiomark_btn_overlimt_event_handler(e), lv.EVENT.ALL, None)

def screen_audiomark_btn_mark_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_audiomark_btn_mark.add_event_cb(lambda e: screen_audiomark_btn_mark_event_handler(e), lv.EVENT.ALL, None)

def screen_audiomark_btn_audio_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_audiomark_btn_audio.add_event_cb(lambda e: screen_audiomark_btn_audio_event_handler(e), lv.EVENT.ALL, None)

def screen_about_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

screen_about.add_event_cb(lambda e: screen_about_event_handler(e), lv.EVENT.ALL, None)

def screen_Tag_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_Tag_img_back.add_event_cb(lambda e: screen_Tag_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_audio_Tag_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_audio_Tag_img_back.add_event_cb(lambda e: screen_audio_Tag_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_audio_Tag_img_audio_playing_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

        screen_audio_Tag_img_audio_play.remove_flag(lv.obj.FLAG.HIDDEN)
        
        screen_audio_Tag_img_audio_playing.add_flag(lv.obj.FLAG.HIDDEN)
        
screen_audio_Tag_img_audio_playing.add_event_cb(lambda e: screen_audio_Tag_img_audio_playing_event_handler(e), lv.EVENT.ALL, None)

def screen_mark_err_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_mark_err_img_back.add_event_cb(lambda e: screen_mark_err_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_warn1_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_warn1_img_back.add_event_cb(lambda e: screen_warn1_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_warn1_img_voic_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.PRESSED):
        pass
        

        screen_warn1_label_tip.remove_flag(lv.obj.FLAG.HIDDEN)
        
    if (code == lv.EVENT.RELEASED):
        pass
        

        screen_warn1_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
        
screen_warn1_img_voic.add_event_cb(lambda e: screen_warn1_img_voic_event_handler(e), lv.EVENT.ALL, None)

def screen_warn2_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_warn2_img_back.add_event_cb(lambda e: screen_warn2_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_warn2_img_voic_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.PRESSED):
        pass
        

        screen_warn2_label_tip.remove_flag(lv.obj.FLAG.HIDDEN)
        
    if (code == lv.EVENT.RELEASED):
        pass
        

        screen_warn2_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
        
screen_warn2_img_voic.add_event_cb(lambda e: screen_warn2_img_voic_event_handler(e), lv.EVENT.ALL, None)

def screen_warn3_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_warn3_img_back.add_event_cb(lambda e: screen_warn3_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_warn3_img_voic_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.PRESSED):
        pass
        

        screen_warn3_label_tip.remove_flag(lv.obj.FLAG.HIDDEN)
        
    if (code == lv.EVENT.RELEASED):
        pass
        

        screen_warn3_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
        
screen_warn3_img_voic.add_event_cb(lambda e: screen_warn3_img_voic_event_handler(e), lv.EVENT.ALL, None)

def screen_warn4_img_back_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

screen_warn4_img_back.add_event_cb(lambda e: screen_warn4_img_back_event_handler(e), lv.EVENT.ALL, None)

def screen_warn4_img_voic_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.PRESSED):
        pass
        

        screen_warn4_label_tip.remove_flag(lv.obj.FLAG.HIDDEN)
        
    if (code == lv.EVENT.RELEASED):
        pass
        

        screen_warn4_label_tip.add_flag(lv.obj.FLAG.HIDDEN)
        
screen_warn4_img_voic.add_event_cb(lambda e: screen_warn4_img_voic_event_handler(e), lv.EVENT.ALL, None)

def screen_volume_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

screen_volume.add_event_cb(lambda e: screen_volume_event_handler(e), lv.EVENT.ALL, None)

def screen_bledata_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

screen_bledata.add_event_cb(lambda e: screen_bledata_event_handler(e), lv.EVENT.ALL, None)

def screen_sleep_set_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

screen_sleep_set.add_event_cb(lambda e: screen_sleep_set_event_handler(e), lv.EVENT.ALL, None)

def screen_Instructions_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        

screen_Instructions.add_event_cb(lambda e: screen_Instructions_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.screen_load(screen_mileage_reminder)

if __name__ == '__main__':
    while True:
        lv.task_handler()
        time.sleep_ms(5)
