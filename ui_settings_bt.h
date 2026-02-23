#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include "ui_helpers.h"

lv_obj_t *scr_settings_bt;

void create_settings_bt_screen() {
    scr_settings_bt = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_settings_bt, lv_color_hex(0x0d0d0d), 0);
    lv_obj_set_style_bg_grad_color(scr_settings_bt, lv_color_hex(0x0d0d1e), 0);
    lv_obj_set_style_bg_grad_dir(scr_settings_bt, LV_GRAD_DIR_VER, 0);
    lv_obj_clear_flag(scr_settings_bt, LV_OBJ_FLAG_SCROLLABLE);


    // TITLE
    lv_obj_t *title = lv_label_create(scr_settings_bt);
    lv_label_set_text(title, LV_SYMBOL_BLUETOOTH " Bluetooth");
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A78C2), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 25);

    // STATUS
    lbl_bt_status = lv_label_create(scr_settings_bt);
    lv_label_set_text(lbl_bt_status, "Status: Disconnected");
    lv_obj_set_style_text_color(lbl_bt_status, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_bt_status, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_status, LV_ALIGN_TOP_MID, 0, 55);

    //DEVICE
    lbl_bt_device = lv_label_create(scr_settings_bt);
    lv_label_set_text(lbl_bt_device, "Device: None");
    lv_obj_set_style_text_color(lbl_bt_device, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_bt_device, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_device, LV_ALIGN_TOP_MID, 0, 75);

    //PAIR
    lv_obj_t *btn_pair = lv_btn_create(scr_settings_bt);
    lv_obj_set_size(btn_pair, 200, 50);
    lv_obj_align(btn_pair, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_bg_color(btn_pair, lv_color_hex(0x1A78C2), 0);
    lv_obj_set_style_radius(btn_pair, 12, 0);
    lv_obj_add_event_cb(btn_pair, [](lv_event_t *e) {
        lv_label_set_text(lbl_bt_status, "Status: Pairing...");
        // bt_start_pairing(); // ← Phase 3
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_pair = lv_label_create(btn_pair);
    lv_label_set_text(lbl_pair, LV_SYMBOL_BLUETOOTH " Start Pairing");
    lv_obj_set_style_text_font(lbl_pair, &lv_font_montserrat_14, 0);
    lv_obj_center(lbl_pair);

    // BACK
    lv_obj_t *btn_back = lv_btn_create(scr_settings_bt);
    lv_obj_set_size(btn_back, 80, 30);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 25);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_back, [](lv_event_t *e) {
        ui_switch_screen(&scr_settings);
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_t *lbl_back = lv_label_create(btn_back);
    lv_label_set_text(lbl_back, LV_SYMBOL_LEFT " Back");
    lv_obj_center(lbl_back);
}