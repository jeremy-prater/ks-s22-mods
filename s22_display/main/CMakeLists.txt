idf_component_register(
    SRCS "s22_display.c" "ui/ui.c" "ui/ui_helpers.c"
    INCLUDE_DIRS "." "ui"
    REQUIRES lvgl lvgl_esp32_drivers driver
)
