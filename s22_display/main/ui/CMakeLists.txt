SET(SOURCES screens/ui_Screen1.c
    ui.c
    ui_helpers.c
    fonts/ui_font_Arcade100.c
    fonts/ui_font_FontAwesome35.c)

add_library(ui ${SOURCES})
