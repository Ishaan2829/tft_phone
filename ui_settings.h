#pragma once
#include <lvgl.h>
#include "ui_manager.h"

lv_obj_t *scr_settings;

// ── Status label (updated by Bluetooth later) ─────────────────────────────
lv_obj_t *lbl_bt_status;
lv_obj_t *lbl_bt_device;

void create_settings_screen() {
    scr_settings = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_settings, lv_color_hex(0x0f0f1a), 0);

    // ── Title ──────────────────────────────────────────────────────────────
    lv_obj_t *title = lv_label_create(scr_settings);
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 15);

    // ── Bluetooth status ───────────────────────────────────────────────────
    lv_obj_t *lbl_bt_title = lv_label_create(scr_settings);
    lv_label_set_text(lbl_bt_title, "Bluetooth");
    lv_obj_set_style_text_color(lbl_bt_title, lv_color_hex(0x8B5CF6), 0);
    lv_obj_set_style_text_font(lbl_bt_title, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_title, LV_ALIGN_TOP_LEFT, 20, 60);

    lbl_bt_status = lv_label_create(scr_settings);
    lv_label_set_text(lbl_bt_status, "Status: Disconnected");
    lv_obj_set_style_text_color(lbl_bt_status, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_font(lbl_bt_status, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_status, LV_ALIGN_TOP_LEFT, 20, 85);

    lbl_bt_device = lv_label_create(scr_settings);
    lv_label_set_text(lbl_bt_device, "Device: None");
    lv_obj_set_style_text_color(lbl_bt_device, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_font(lbl_bt_device, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_bt_device, LV_ALIGN_TOP_LEFT, 20, 110);

    // ── Pair Button ────────────────────────────────────────────────────────
    lv_obj_t *btn_pair = lv_btn_create(scr_settings);
    lv_obj_set_size(btn_pair, 150, 30);
    lv_obj_align(btn_pair, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_style_bg_color(btn_pair, lv_color_hex(0x8B5CF6), 0);
    lv_obj_add_event_cb(btn_pair, [](lv_event_t *e) {
        lv_label_set_text(lbl_bt_status, "Status: Pairing...");
        // bt_start_pairing(); // ← Phase 3
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_pair = lv_label_create(btn_pair);
    lv_label_set_text(lbl_pair, LV_SYMBOL_BLUETOOTH " Start Pairing");
    lv_obj_center(lbl_pair);

    // ── Back Button ────────────────────────────────────────────────────────
    lv_obj_t *btn_back = lv_btn_create(scr_settings);
    lv_obj_set_size(btn_back, 80, 35);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_back, [](lv_event_t *e) {
        ui_switch_screen(&scr_home);
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_back = lv_label_create(btn_back);
    lv_label_set_text(lbl_back, LV_SYMBOL_LEFT " Back");
    lv_obj_center(lbl_back);
}