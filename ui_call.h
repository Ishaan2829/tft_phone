#pragma once
#include <lvgl.h>
#include "ui_manager.h"

lv_obj_t *scr_call;

// ── Call status label (updated by Bluetooth later) ────────────────────────
lv_obj_t *lbl_caller;
lv_obj_t *lbl_call_status;

void create_call_screen() {
    scr_call = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_call, lv_color_hex(0x0a0a0a), 0);

    // ── Caller name/number ─────────────────────────────────────────────────
    lbl_caller = lv_label_create(scr_call);
    lv_label_set_text(lbl_caller, "Unknown");
    lv_obj_set_style_text_color(lbl_caller, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_caller, &lv_font_montserrat_20, 0);
    lv_obj_align(lbl_caller, LV_ALIGN_TOP_MID, 0, 30);

    // ── Call status ────────────────────────────────────────────────────────
    lbl_call_status = lv_label_create(scr_call);
    lv_label_set_text(lbl_call_status, "Incoming Call...");
    lv_obj_set_style_text_color(lbl_call_status, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_font(lbl_call_status, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_call_status, LV_ALIGN_TOP_MID, 0, 65);

    // ── Accept Button ──────────────────────────────────────────────────────
    lv_obj_t *btn_accept = lv_btn_create(scr_call);
    lv_obj_set_size(btn_accept, 120, 55);
    lv_obj_align(btn_accept, LV_ALIGN_BOTTOM_LEFT, 30, -30);
    lv_obj_set_style_bg_color(btn_accept, lv_color_hex(0x1DB954), 0);
    lv_obj_add_event_cb(btn_accept, [](lv_event_t *e) {
        lv_label_set_text(lbl_call_status, "On Call");
        // bt_accept_call(); // ← will be hooked in Phase 3
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_accept = lv_label_create(btn_accept);
    lv_label_set_text(lbl_accept, LV_SYMBOL_CALL " Accept");
    lv_obj_center(lbl_accept);

    // ── Reject Button ──────────────────────────────────────────────────────
    lv_obj_t *btn_reject = lv_btn_create(scr_call);
    lv_obj_set_size(btn_reject, 120, 55);
    lv_obj_align(btn_reject, LV_ALIGN_BOTTOM_RIGHT, -30, -30);
    lv_obj_set_style_bg_color(btn_reject, lv_color_hex(0xE53935), 0);
    lv_obj_add_event_cb(btn_reject, [](lv_event_t *e) {
        // bt_reject_call(); // ← will be hooked in Phase 3
        ui_switch_screen(&scr_home);
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_reject = lv_label_create(btn_reject);
    lv_label_set_text(lbl_reject, LV_SYMBOL_CLOSE " Reject");
    lv_obj_center(lbl_reject);

    // BACK BUTTON ***************************************
    lv_obj_t *btn_back = lv_btn_create(scr_call);
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