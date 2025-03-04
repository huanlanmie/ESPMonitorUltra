// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: page2_weather

#ifndef _PAGE2_WEATHER_UI_H
#define _PAGE2_WEATHER_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_page2_screen_weather
void ui_page2_screen_weather_screen_init(void);
extern lv_obj_t * ui_page2_screen_weather;
extern lv_obj_t * ui_page2_panel_main;
extern lv_obj_t * ui_page2_panel_main_label_date;
extern lv_obj_t * ui_page2_panel_main_label_week;
extern lv_obj_t * ui_page2_panel_main_label_lunar;
extern lv_obj_t * ui_page2_panel_main_label_time;
extern lv_obj_t * ui_page2_panel_main_label_suggest;
extern lv_obj_t * ui_page2_image_current_weather;
extern lv_obj_t * ui_page2_panel_main_label_current_temp;
extern lv_obj_t * ui_page2_panel_main_label_current_text;
extern lv_obj_t * ui_page2_panel_main_panel_temphub;
extern lv_obj_t * ui_page2_panel_main_label_inroom;
extern lv_obj_t * ui_page2_panel_main_label_inroom_temp;
extern lv_obj_t * ui_page2_panel_main_label_inroom_hub;
extern lv_obj_t * ui_page2_panel_main_image_temp;
extern lv_obj_t * ui_page2_panel_main_image_temp2;
extern lv_obj_t * ui_page2_panel_main_warn;
extern lv_obj_t * ui_page2_panel_main_label_current_temp2;
extern lv_obj_t * ui_page2_image_current_weather_day1;
extern lv_obj_t * ui_page2_image_current_weather_day2;
extern lv_obj_t * ui_page2_image_current_weather_day3;
extern lv_obj_t * ui_page2_panel_main_label_current_day1;
extern lv_obj_t * ui_page2_panel_main_label_current_day2;
extern lv_obj_t * ui_page2_panel_main_label_current_day3;
extern lv_obj_t * ui_page2_panel_main_label_current_day1_temp;
extern lv_obj_t * ui_page2_panel_main_label_current_day2_temp;
extern lv_obj_t * ui_page2_panel_main_label_current_day3_temp;
extern lv_obj_t * ui_page2_image_localtion;
extern lv_obj_t * ui_page2_label_location;
extern lv_obj_t * ui_page2_label_update;
// CUSTOM VARIABLES

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_images_bizhi_png);    // assets/images/bizhi.png
LV_IMG_DECLARE(ui_img_images_m00_png);    // assets/images/m00.png
LV_IMG_DECLARE(ui_img_images_temp_png);    // assets/images/temp.png
LV_IMG_DECLARE(ui_img_images_hub_png);    // assets/images/hub.png
LV_IMG_DECLARE(ui_img_images_m01_png);    // assets/images/m01.png
LV_IMG_DECLARE(ui_img_images_m02_png);    // assets/images/m02.png
LV_IMG_DECLARE(ui_img_images_m08_png);    // assets/images/m08.png
LV_IMG_DECLARE(ui_img_images_location_png);    // assets/images/location.png
LV_IMG_DECLARE(ui_img_images_m03_png);    // assets/images/m03.png
LV_IMG_DECLARE(ui_img_images_m04_png);    // assets/images/m04.png
LV_IMG_DECLARE(ui_img_images_m05_png);    // assets/images/m05.png
LV_IMG_DECLARE(ui_img_images_m06_png);    // assets/images/m06.png
LV_IMG_DECLARE(ui_img_images_m07_png);    // assets/images/m07.png
LV_IMG_DECLARE(ui_img_images_m09_png);    // assets/images/m09.png
LV_IMG_DECLARE(ui_img_images_m10_png);    // assets/images/m10.png
LV_IMG_DECLARE(ui_img_images_m11_png);    // assets/images/m11.png
LV_IMG_DECLARE(ui_img_images_m12_png);    // assets/images/m12.png
LV_IMG_DECLARE(ui_img_images_m13_png);    // assets/images/m13.png
LV_IMG_DECLARE(ui_img_images_m14_png);    // assets/images/m14.png
LV_IMG_DECLARE(ui_img_images_m15_png);    // assets/images/m15.png
LV_IMG_DECLARE(ui_img_images_m16_png);    // assets/images/m16.png
LV_IMG_DECLARE(ui_img_images_m17_png);    // assets/images/m17.png

// FONTS
LV_FONT_DECLARE(ui_font_Font102px);
LV_FONT_DECLARE(ui_font_Font18px);
LV_FONT_DECLARE(ui_font_Font38px);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
