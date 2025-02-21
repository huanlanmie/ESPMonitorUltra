SET(SOURCES ui_page2_screen_weather.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_images_bizhi_png.c
    ui_img_images_m00_png.c
    ui_img_images_temp_png.c
    ui_img_images_hub_png.c
    ui_img_images_m01_png.c
    ui_img_images_m02_png.c
    ui_img_images_m08_png.c
    ui_img_images_location_png.c
    ui_img_images_m03_png.c
    ui_img_images_m04_png.c
    ui_img_images_m05_png.c
    ui_img_images_m06_png.c
    ui_img_images_m07_png.c
    ui_img_images_m09_png.c
    ui_img_images_m10_png.c
    ui_img_images_m11_png.c
    ui_img_images_m12_png.c
    ui_img_images_m13_png.c
    ui_img_images_m14_png.c
    ui_img_images_m15_png.c
    ui_img_images_m16_png.c
    ui_img_images_m17_png.c
    fonts/ui_font_Font102px.c
    fonts/ui_font_Font18px.c
    fonts/ui_font_Font38px.c)

add_library(ui ${SOURCES})
