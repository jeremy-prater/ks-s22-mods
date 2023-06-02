SET(SOURCES screens/ui_BootConnect.c
    screens/ui_Riding.c
    screens/ui_Charging.c
    ui.c
    ui_helpers.c
    images/ui_img_bg_png.c
    images/ui_img_ks_logo_png.c
    fonts/ui_font_Arcade100.c
    fonts/ui_font_FontAwesome35.c
    fonts/ui_font_FontAwesome45.c)

add_library(ui ${SOURCES})
