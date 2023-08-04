// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: screen1

#include "../ui.h"

void ui_Riding_screen_init(void)
{
ui_Riding = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Riding, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_img_src( ui_Riding, &ui_img_bg_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_tiled(ui_Riding, true, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SpeedMeter = lv_arc_create(ui_Riding);
lv_obj_set_width( ui_SpeedMeter, 320);
lv_obj_set_height( ui_SpeedMeter, 320);
lv_obj_set_x( ui_SpeedMeter, 0 );
lv_obj_set_y( ui_SpeedMeter, 40 );
lv_obj_set_align( ui_SpeedMeter, LV_ALIGN_CENTER );
lv_arc_set_range(ui_SpeedMeter, 0,44);
lv_arc_set_bg_angles(ui_SpeedMeter,180,270);
lv_obj_set_style_arc_width(ui_SpeedMeter, 30, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_arc_rounded(ui_SpeedMeter, false, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_outline_color(ui_SpeedMeter, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_outline_opa(ui_SpeedMeter, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_arc_color(ui_SpeedMeter, lv_color_hex(0x29A93B), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_SpeedMeter, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_arc_width(ui_SpeedMeter, 30, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_arc_rounded(ui_SpeedMeter, false, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_SpeedMeter, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SpeedMeter, 0, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_PWMMeter = lv_arc_create(ui_Riding);
lv_obj_set_width( ui_PWMMeter, 320);
lv_obj_set_height( ui_PWMMeter, 320);
lv_obj_set_x( ui_PWMMeter, 0 );
lv_obj_set_y( ui_PWMMeter, 40 );
lv_obj_set_align( ui_PWMMeter, LV_ALIGN_CENTER );
lv_arc_set_value(ui_PWMMeter, 50);
lv_arc_set_bg_angles(ui_PWMMeter,270,0);
lv_obj_set_style_arc_color(ui_PWMMeter, lv_color_hex(0xFF8940), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_PWMMeter, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_arc_width(ui_PWMMeter, 30, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_arc_rounded(ui_PWMMeter, false, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_outline_color(ui_PWMMeter, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_outline_opa(ui_PWMMeter, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_arc_color(ui_PWMMeter, lv_color_hex(0x293031), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_arc_opa(ui_PWMMeter, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_arc_width(ui_PWMMeter, 30, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_arc_rounded(ui_PWMMeter, false, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_PWMMeter, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_PWMMeter, 0, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_SpeedValue = lv_label_create(ui_Riding);
lv_obj_set_width( ui_SpeedValue, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_SpeedValue, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_SpeedValue, -60 );
lv_obj_set_y( ui_SpeedValue, -20 );
lv_obj_set_align( ui_SpeedValue, LV_ALIGN_CENTER );
lv_label_set_text(ui_SpeedValue,"0");
lv_obj_set_style_text_font(ui_SpeedValue, &lv_font_montserrat_48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_PWMValue = lv_label_create(ui_Riding);
lv_obj_set_width( ui_PWMValue, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_PWMValue, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_PWMValue, 50 );
lv_obj_set_y( ui_PWMValue, -20 );
lv_obj_set_align( ui_PWMValue, LV_ALIGN_CENTER );
lv_label_set_text(ui_PWMValue,"0");
lv_obj_set_style_text_font(ui_PWMValue, &lv_font_montserrat_48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SpeedLabel = lv_label_create(ui_Riding);
lv_obj_set_width( ui_SpeedLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_SpeedLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_SpeedLabel, -60 );
lv_obj_set_y( ui_SpeedLabel, 20 );
lv_obj_set_align( ui_SpeedLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_SpeedLabel,"Speed");
lv_obj_set_style_text_font(ui_SpeedLabel, &lv_font_montserrat_30, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_PWMLabel = lv_label_create(ui_Riding);
lv_obj_set_width( ui_PWMLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_PWMLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_PWMLabel, 60 );
lv_obj_set_y( ui_PWMLabel, 20 );
lv_obj_set_align( ui_PWMLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_PWMLabel,"Power");
lv_obj_set_style_text_font(ui_PWMLabel, &lv_font_montserrat_30, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_BatteryValue = lv_label_create(ui_Riding);
lv_obj_set_width( ui_BatteryValue, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_BatteryValue, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_BatteryValue, -130 );
lv_obj_set_y( ui_BatteryValue, 105 );
lv_obj_set_align( ui_BatteryValue, LV_ALIGN_CENTER );
lv_label_set_text(ui_BatteryValue,"0%");
lv_obj_set_style_text_font(ui_BatteryValue, &lv_font_montserrat_24, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_BatteryIcon = lv_label_create(ui_Riding);
lv_obj_set_width( ui_BatteryIcon, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_BatteryIcon, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_BatteryIcon, -130 );
lv_obj_set_y( ui_BatteryIcon, 70 );
lv_obj_set_align( ui_BatteryIcon, LV_ALIGN_CENTER );
lv_label_set_text(ui_BatteryIcon,"");
lv_obj_set_style_text_color(ui_BatteryIcon, lv_color_hex(0x40BC30), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_BatteryIcon, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_BatteryIcon, &ui_font_FontAwesome35, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TempIcon = lv_label_create(ui_Riding);
lv_obj_set_width( ui_TempIcon, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_TempIcon, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_TempIcon, 130 );
lv_obj_set_y( ui_TempIcon, 70 );
lv_obj_set_align( ui_TempIcon, LV_ALIGN_CENTER );
lv_label_set_text(ui_TempIcon,"");
lv_obj_set_style_text_color(ui_TempIcon, lv_color_hex(0xBCC300), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_TempIcon, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_TempIcon, &ui_font_FontAwesome35, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TempValue = lv_label_create(ui_Riding);
lv_obj_set_width( ui_TempValue, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_TempValue, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_TempValue, 130 );
lv_obj_set_y( ui_TempValue, 105 );
lv_obj_set_align( ui_TempValue, LV_ALIGN_CENTER );
lv_label_set_text(ui_TempValue,"0");
lv_obj_set_style_text_font(ui_TempValue, &lv_font_montserrat_24, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_CurrentIcon = lv_label_create(ui_Riding);
lv_obj_set_width( ui_CurrentIcon, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_CurrentIcon, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_CurrentIcon, 45 );
lv_obj_set_y( ui_CurrentIcon, 70 );
lv_obj_set_align( ui_CurrentIcon, LV_ALIGN_CENTER );
lv_label_set_text(ui_CurrentIcon,"");
lv_obj_set_style_text_color(ui_CurrentIcon, lv_color_hex(0xB2BC30), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_CurrentIcon, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_CurrentIcon, &ui_font_FontAwesome35, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_CurrentValue = lv_label_create(ui_Riding);
lv_obj_set_width( ui_CurrentValue, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_CurrentValue, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_CurrentValue, 45 );
lv_obj_set_y( ui_CurrentValue, 105 );
lv_obj_set_align( ui_CurrentValue, LV_ALIGN_CENTER );
lv_label_set_text(ui_CurrentValue,"0A");
lv_obj_set_style_text_font(ui_CurrentValue, &lv_font_montserrat_24, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_VoltIcon = lv_label_create(ui_Riding);
lv_obj_set_width( ui_VoltIcon, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_VoltIcon, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_VoltIcon, -45 );
lv_obj_set_y( ui_VoltIcon, 70 );
lv_obj_set_align( ui_VoltIcon, LV_ALIGN_CENTER );
lv_label_set_text(ui_VoltIcon,"");
lv_obj_set_style_text_color(ui_VoltIcon, lv_color_hex(0x9DBC30), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_VoltIcon, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_VoltIcon, &ui_font_FontAwesome35, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_VoltLabel = lv_label_create(ui_Riding);
lv_obj_set_width( ui_VoltLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_VoltLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_VoltLabel, -45 );
lv_obj_set_y( ui_VoltLabel, 105 );
lv_obj_set_align( ui_VoltLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_VoltLabel,"0V");
lv_obj_set_style_text_font(ui_VoltLabel, &lv_font_montserrat_24, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_M5Battery1Charging = lv_label_create(ui_Riding);
lv_obj_set_width( ui_M5Battery1Charging, 40);
lv_obj_set_height( ui_M5Battery1Charging, 35);
lv_obj_set_x( ui_M5Battery1Charging, 135 );
lv_obj_set_y( ui_M5Battery1Charging, -105 );
lv_obj_set_align( ui_M5Battery1Charging, LV_ALIGN_CENTER );
lv_label_set_text(ui_M5Battery1Charging,"");
lv_obj_set_style_text_color(ui_M5Battery1Charging, lv_color_hex(0x00FF08), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_M5Battery1Charging, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_M5Battery1Charging, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_M5Battery1Charging, &ui_font_FontAwesome35, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_M5Battery1 = lv_label_create(ui_Riding);
lv_obj_set_width( ui_M5Battery1, 40);
lv_obj_set_height( ui_M5Battery1, 35);
lv_obj_set_x( ui_M5Battery1, 135 );
lv_obj_set_y( ui_M5Battery1, -105 );
lv_obj_set_align( ui_M5Battery1, LV_ALIGN_CENTER );
lv_label_set_text(ui_M5Battery1,"");
lv_obj_set_style_text_align(ui_M5Battery1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_M5Battery1, &ui_font_FontAwesome35, LV_PART_MAIN| LV_STATE_DEFAULT);

}
