#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"

lv_obj_t *scr_clock;

void create_clock_screen() {
    scr_clock = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_clock, lv_color_hex(0x0d0d0d), 0);
    lv_obj_clear_flag(scr_clock, LV_OBJ_FLAG_SCROLLABLE);

    // ── Status bar ─────────────────────────────────────────────────────────
    create_statusbar(scr_clock);

    // ── Big clock display ──────────────────────────────────────────────────
    lv_obj_t *lbl_clock = lv_label_create(scr_clock);
    lv_label_set_text(lbl_clock, "00:00");
    lv_obj_set_style_text_color(lbl_clock, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_clock, &lv_font_montserrat_48, 0);
    lv_obj_align(lbl_clock, LV_ALIGN_CENTER, 0, -20);

    // ── Seconds label ──────────────────────────────────────────────────────
    lv_obj_t *lbl_secs = lv_label_create(scr_clock);
    lv_label_set_text(lbl_secs, "00s");
    lv_obj_set_style_text_color(lbl_secs, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_secs, &lv_font_montserrat_20, 0);
    lv_obj_align(lbl_secs, LV_ALIGN_CENTER, 0, 30);

    // ── Update timer ───────────────────────────────────────────────────────
    // ── Update timer ───────────────────────────────────────────────────────────
static lv_obj_t *clock_labels[2];
clock_labels[0] = lbl_clock;
clock_labels[1] = lbl_secs;

lv_timer_create([](lv_timer_t *t) {
    lv_obj_t **labels = (lv_obj_t **)t->user_data;
    uint32_t secs  = millis() / 1000;
    uint32_t mins  = (secs / 60) % 60;
    uint32_t hours = (secs / 3600) % 24;
    uint32_t s     = secs % 60;

    char timebuf[6];
    char secbuf[5];
    snprintf(timebuf, sizeof(timebuf), "%02d:%02d", hours, mins);
    snprintf(secbuf,  sizeof(secbuf),  "%02ds", s);

    lv_label_set_text(labels[0], timebuf);
    lv_label_set_text(labels[1], secbuf);
}, 1000, clock_labels);

    // ── Back button ────────────────────────────────────────────────────────
    lv_obj_t *btn_back = lv_btn_create(scr_clock);
    lv_obj_set_size(btn_back, 80, 35);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 25);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_back, [](lv_event_t *e) {
        ui_switch_screen(&scr_home);
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_back = lv_label_create(btn_back);
    lv_label_set_text(lbl_back, LV_SYMBOL_LEFT " Back");
    lv_obj_center(lbl_back);
}