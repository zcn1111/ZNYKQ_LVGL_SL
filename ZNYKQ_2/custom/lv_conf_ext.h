/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

/*
 * lv_conf_ext.h for custom lvconf file.
 * example :
 *	#undef LV_FONT_FMT_TXT_LARGE
 *  #define LV_FONT_FMT_TXT_LARGE 1
 */

#ifndef LV_CONF_EXT_H
#define LV_CONF_EXT_H


/* common code  begin  */


/* common code end */


#if LV_USE_GUIDER_SIMULATOR
/* code for simulator begin  */

/* Enable LVGL FS on simulator (stdio backend) */
#undef  LV_USE_FS_POSIX
#define LV_USE_FS_POSIX 1

/* Use same drive letter as ESP32 config */
#undef  LV_FS_POSIX_LETTER
#define LV_FS_POSIX_LETTER 'S'

/* Map S:/... to <cwd> */
#undef  LV_FS_POSIX_PATH
#define LV_FS_POSIX_PATH "C:/Users/Administrator/Desktop/SH_LVGL_PRJ/ZNYKQ_2/lvgl-simulator"

#undef  LV_FS_POSIX_CACHE_SIZE
#define LV_FS_POSIX_CACHE_SIZE 0

#undef  LV_FONT_FMT_TXT_LARGE
#define LV_FONT_FMT_TXT_LARGE 1

/* code for simulator end */
#else
/* code for board begin */


/* code for board end */	
#endif



#endif  /* LV_CONF_EXT_H */	