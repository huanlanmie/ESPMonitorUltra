SET(SOURCES ui_page3_screen_monitor.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_images_cpu_png.c
    ui_img_1298787339.c
    ui_img_images_ram_png.c
    ui_img_images_upload_png.c
    ui_img_images_download_png.c
    fonts/ui_font_Font18mkzpx.c
    fonts/ui_font_Font18px.c
    fonts/ui_font_Font24px.c
    fonts/ui_font_Font48px.c)

add_library(ui ${SOURCES})
