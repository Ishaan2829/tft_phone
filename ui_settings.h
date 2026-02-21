#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include <WiFi.h>

lv_obj_t *scr_settings;

//TOGGLE STATES*******************
static bool sw_bt_state   = true;
static bool sw_wifi_state = true;
//MENU ROW*************************************
lv_obj_t* create_toggle_row(lv_obj_t *parent, const char *icon,
                              lv_color_t icon_color, const char *label,
                              bool initial_state, lv_event_cb_t cb) {
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, 280, 48);
    lv_obj_set_style_bg_color(row, lv_color_hex(0x1e1e30), 0);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_radius(row, 10, 0);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *ico = lv_label_create(row);
    lv_label_set_text(ico, icon);
    lv_obj_set_style_text_color(ico, icon_color, 0);
    lv_obj_set_style_text_font(ico, &lv_font_montserrat_20, 0);
    lv_obj_align(ico, LV_ALIGN_LEFT_MID, 8, 0);

    lv_obj_t *lbl = lv_label_create(row);
    lv_label_set_text(lbl, label);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 40, 0);

    lv_obj_t *sw = lv_switch_create(row);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_size(sw, 50, 26);
    lv_obj_set_style_bg_color(sw, lv_color_hex(0x333333), 0);
    lv_obj_set_style_bg_color(sw, icon_color, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_flag(sw, LV_OBJ_FLAG_CLICKABLE);
    if (initial_state) lv_obj_add_state(sw, LV_STATE_CHECKED);
    lv_obj_add_event_cb(sw, cb, LV_EVENT_VALUE_CHANGED, NULL);

    return row;
}

// MENU ROW -> NEXT SCREEN*******************
lv_obj_t* create_nav_row(lv_obj_t *parent, const char *icon,
                          lv_color_t icon_color, const char *label,
                          const char *subtitle, lv_obj_t **target) {
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, 280, 48);
    lv_obj_set_style_bg_color(row, lv_color_hex(0x1e1e30), 0);
    lv_obj_set_style_bg_color(row, lv_color_hex(0x2a2a40), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_radius(row, 10, 0);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);

    // ICON
    lv_obj_t *ico = lv_label_create(row);
    lv_label_set_text(ico, icon);
    lv_obj_set_style_text_color(ico, icon_color, 0);
    lv_obj_set_style_text_font(ico, &lv_font_montserrat_20, 0);
    lv_obj_align(ico, LV_ALIGN_LEFT_MID, 8, 0);

    // LABLE
    lv_obj_t *lbl = lv_label_create(row);
    lv_label_set_text(lbl, label);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 40, 0);

    // SUBTITLE
    if (subtitle) {
        lv_obj_t *sub = lv_label_create(row);
        lv_label_set_text(sub, subtitle);
        lv_obj_set_style_text_color(sub, lv_color_hex(0x666666), 0);
        lv_obj_set_style_text_font(sub, &lv_font_montserrat_14, 0);
        lv_obj_align(sub, LV_ALIGN_RIGHT_MID, -25, 0);
    }

    // ARROW
    lv_obj_t *arr = lv_label_create(row);
    lv_label_set_text(arr, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(arr, lv_color_hex(0x444444), 0);
    lv_obj_align(arr, LV_ALIGN_RIGHT_MID, -5, 0);

    lv_obj_set_user_data(row, target);
    lv_obj_add_event_cb(row, [](lv_event_t *e) {
        lv_obj_t **t = (lv_obj_t **)lv_obj_get_user_data(lv_event_get_target(e));
        ui_switch_screen(t);
    }, LV_EVENT_CLICKED, NULL);

    return row;
}

void create_settings_screen() {
    scr_settings = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_settings, lv_color_hex(0x0d0d0d), 0);
    lv_obj_set_style_bg_grad_color(scr_settings, lv_color_hex(0x0d0d1e), 0);
    lv_obj_set_style_bg_grad_dir(scr_settings, LV_GRAD_DIR_VER, 0);
    lv_obj_clear_flag(scr_settings, LV_OBJ_FLAG_SCROLLABLE);


    // TITLE
    lv_obj_t *title = lv_label_create(scr_settings);
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 25);

    // SCROLABLE ROW
    lv_obj_t *cont = lv_obj_create(scr_settings);
    lv_obj_set_size(cont, 300, 175);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x0d0d0d), 0);
    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(cont, 6, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);

    //BLUETOOTH TOGGLE SWITHC********************************************8
create_nav_row(cont,
    LV_SYMBOL_BLUETOOTH, lv_color_hex(0x1A78C2),
    "Bluetooth", "Tap to manage", &scr_settings_bt);
    //WIFI ROW
create_nav_row(cont,
    LV_SYMBOL_WIFI, lv_color_hex(0x1DB954),
    "WiFi", "Tap to manage", &scr_settings_wifi);

    // STATUS BAR UPDATE BLE****************************
    lbl_bt_status = lv_label_create(scr_settings);
    lv_label_set_text(lbl_bt_status, "BT: Disconnected");
    lv_obj_set_style_text_color(lbl_bt_status, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_bt_status, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_status, LV_ALIGN_BOTTOM_LEFT, 15, -25);

    lbl_bt_device = lv_label_create(scr_settings);
    lv_label_set_text(lbl_bt_device, "");
    lv_obj_set_style_text_color(lbl_bt_device, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_bt_device, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_device, LV_ALIGN_BOTTOM_LEFT, 15, -10);


    //BACK BUTTON********
lv_obj_t *btn_back = lv_btn_create(scr_settings);
lv_obj_set_size(btn_back, 80, 30);
lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 25);
lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x333333), 0);
lv_obj_add_event_cb(btn_back, [](lv_event_t *e) {
    ui_switch_screen(&scr_home);
}, LV_EVENT_CLICKED, NULL);
lv_obj_t *lbl_back = lv_label_create(btn_back);
lv_label_set_text(lbl_back, LV_SYMBOL_LEFT " Back");
lv_obj_center(lbl_back);}