SET(UI_SOURCES
    ui/screens/ui_BootConnect.c
    ui/screens/ui_Riding.c
    ui/screens/ui_Charging.c
    ui/ui.c
    ui/ui_helpers.c
    ui/ui_events.c
    ui/images/ui_img_bg_png.c
    ui/images/ui_img_ks_logo_png.c
    ui/fonts/ui_font_Arcade100.c
    ui/fonts/ui_font_FontAwesome35.c
    ui/fonts/ui_font_FontAwesome45.c
)

SET(BLE_CLIENT_SOURCES
    ble/ble.c
    ble/client.c
    ble/utils/peer.c
    ble/utils/misc.c
)

idf_component_register(
    SRCS s22_display.c ${UI_SOURCES} ${BLE_CLIENT_SOURCES}
    INCLUDE_DIRS .
    REQUIRES lvgl lvgl_esp32_drivers driver nvs_flash bt
)