// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: page4_set&about

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_page4_screen_set_about
void ui_page4_screen_set_about_screen_init(void);
lv_obj_t * ui_page4_screen_set_about;
lv_obj_t * ui_page4_panel_about;
lv_obj_t * ui_page4_label_about_version;
lv_obj_t * ui_page4_label_about_name;
lv_obj_t * ui_page4_label_about_designed_by;
lv_obj_t * ui_page4_panel_wifi_info;
lv_obj_t * ui_page4_label_wifi_info_title;
lv_obj_t * ui_page4_panel_wifi_connection_status;
lv_obj_t * ui_page4_label_wifi_connection_status;
lv_obj_t * ui_page4_panel_wifi_ip_address;
lv_obj_t * ui_page4_label_wifi_ip_address;
lv_obj_t * ui_page4_panel_wifi_signal_strength;
lv_obj_t * ui_page4_label_wifi_signal_strength;
lv_obj_t * ui_page4_panel_comm;
lv_obj_t * ui_page4_label_comm_titile;
lv_obj_t * ui_page4_panel_bilibili_name;
lv_obj_t * ui_page4_label_bilibili_name;
lv_obj_t * ui_page4_panel_qq_num;
lv_obj_t * ui_page4_label_qq_num;
lv_obj_t * ui_page4_panel_communication_group_;
lv_obj_t * ui_page4_label_communication_group_;
lv_obj_t * ui_page4_panel_set_mode;
lv_obj_t * ui_page4_label_set_mode_title;
lv_obj_t * ui_page4_panel_set_mode_info;
lv_obj_t * ui_page4_label_set_mode_info;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_page4_screen_set_about_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_page4_screen_set_about);
}
