SET(UI_SOURCES
    ui/screens/ui_Screen1.c
    ui/ui.c
    ui/ui_helpers.c
    ui/fonts/ui_font_Arcade80.c
    ui/fonts/ui_font_FontAwesome35.c
)

idf_component_register(
    SRCS s22_display.c ${UI_SOURCES}
    INCLUDE_DIRS .
    REQUIRES lvgl lvgl_esp32_drivers driver
)