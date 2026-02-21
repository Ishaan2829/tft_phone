#pragma once
#include <lvgl.h>
#include "ui_manager.h"

void create_statusbar() {
    // VISIBLE ON ALL SCREENS**********
    lv_obj_t *parent = lv_layer_top();

    statusbar = lv_obj_create(parent);
    lv_obj_set_size(statusbar, 320, 20);
    lv_obj_set_pos(statusbar, 0, 0);
    lv_obj_set_style_bg_color(statusbar, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_style_border_width(statusbar, 0, 0);
    lv_obj_set_style_radius(statusbar, 0, 0);
    lv_obj_clear_flag(statusbar, LV_OBJ_FLAG_SCROLLABLE);

    // TIME***************
    lbl_time = lv_label_create(statusbar);
    lv_label_set_text(lbl_time, "00:00");
    lv_obj_set_style_text_color(lbl_time, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_time, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_time, LV_ALIGN_LEFT_MID, 5, 0);

    // WIFI ICON
    lbl_wifi_icon = lv_label_create(statusbar);
    lv_label_set_text(lbl_wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_color(lbl_wifi_icon, lv_color_hex(0x555555), 0);
    lv_obj_align(lbl_wifi_icon, LV_ALIGN_RIGHT_MID, -20, 0);

    // BT ICON
    lbl_bt_icon = lv_label_create(statusbar);
    lv_label_set_text(lbl_bt_icon, LV_SYMBOL_BLUETOOTH);
    lv_obj_set_style_text_color(lbl_bt_icon, lv_color_hex(0x555555), 0);
    lv_obj_align(lbl_bt_icon, LV_ALIGN_RIGHT_MID, -5, 0);
}

// LOOP CALL CLOCK**********
void update_statusbar(uint32_t seconds_since_boot, bool bt_connected) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
   
        char timebuf[6];
        snprintf(timebuf, sizeof(timebuf), "%02d:%02d",
                 timeinfo.tm_hour, timeinfo.tm_min);
        lv_label_set_text(lbl_time, timebuf);
    } else {

        uint32_t mins  = (seconds_since_boot / 60) % 60;
        uint32_t hours = (seconds_since_boot / 3600) % 24;
        char timebuf[6];
        snprintf(timebuf, sizeof(timebuf), "%02d:%02d", hours, mins);
        lv_label_set_text(lbl_time, timebuf);
    }

    lv_color_t bt_color = bt_connected
        ? lv_color_hex(0x1A78C2)
        : lv_color_hex(0x555555);
    lv_obj_set_style_text_color(lbl_bt_icon, bt_color, 0);
}