#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"

lv_obj_t *scr_calendar;

// ── Month names ────────────────────────────────────────────────────────────
static const char *month_names[] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};

// ── Days in each month (non-leap year) ────────────────────────────────────
static const uint8_t days_in_month[] = {
    31,28,31,30,31,30,31,31,30,31,30,31
};

static int8_t  cal_month = 0;   // 0 = January
static int8_t  cal_day   = 1;
static lv_obj_t *lbl_month_year;
static lv_obj_t *day_labels[42]; // 6 rows x 7 cols max

// ── Redraw day grid ────────────────────────────────────────────────────────
void cal_redraw() {
    char buf[20];
    snprintf(buf, sizeof(buf), "%s", month_names[cal_month]);
    lv_label_set_text(lbl_month_year, buf);

    // First day offset (simplified — starts on Monday=0)
    uint8_t total_days = days_in_month[cal_month];
    uint8_t day = 1;

    for (uint8_t i = 0; i < 42; i++) {
        if (i < total_days) {
            char d[3];
            snprintf(d, sizeof(d), "%d", day++);
            lv_label_set_text(day_labels[i], d);
            lv_obj_set_style_text_color(day_labels[i],
                (i == cal_day - 1)
                    ? lv_color_hex(0x06B6D4)
                    : lv_color_hex(0xCCCCCC), 0);
        } else {
            lv_label_set_text(day_labels[i], "");
        }
    }
}

void create_calendar_screen() {
    scr_calendar = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_calendar, lv_color_hex(0x0d0d0d), 0);
    lv_obj_clear_flag(scr_calendar, LV_OBJ_FLAG_SCROLLABLE);

    // ── Status bar ─────────────────────────────────────────────────────────
    create_statusbar(scr_calendar);

    // ── Month title ────────────────────────────────────────────────────────
    lbl_month_year = lv_label_create(scr_calendar);
    lv_label_set_text(lbl_month_year, "January");
    lv_obj_set_style_text_color(lbl_month_year, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_month_year, &lv_font_montserrat_20, 0);
    lv_obj_align(lbl_month_year, LV_ALIGN_TOP_MID, 0, 25);

    // ── Prev/Next month buttons ────────────────────────────────────────────
    lv_obj_t *btn_prev = lv_btn_create(scr_calendar);
    lv_obj_set_size(btn_prev, 40, 30);
    lv_obj_align(btn_prev, LV_ALIGN_TOP_LEFT, 10, 22);
    lv_obj_set_style_bg_color(btn_prev, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_prev, [](lv_event_t *e) {
        cal_month = (cal_month - 1 + 12) % 12;
        cal_redraw();
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_t *lp = lv_label_create(btn_prev);
    lv_label_set_text(lp, LV_SYMBOL_LEFT);
    lv_obj_center(lp);

    lv_obj_t *btn_next = lv_btn_create(scr_calendar);
    lv_obj_set_size(btn_next, 40, 30);
    lv_obj_align(btn_next, LV_ALIGN_TOP_RIGHT, -10, 22);
    lv_obj_set_style_bg_color(btn_next, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_next, [](lv_event_t *e) {
        cal_month = (cal_month + 1) % 12;
        cal_redraw();
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_t *ln = lv_label_create(btn_next);
    lv_label_set_text(ln, LV_SYMBOL_RIGHT);
    lv_obj_center(ln);

    // ── Day headers ────────────────────────────────────────────────────────
    const char *days[] = {"M","T","W","T","F","S","S"};
    for (uint8_t i = 0; i < 7; i++) {
        lv_obj_t *lbl = lv_label_create(scr_calendar);
        lv_label_set_text(lbl, days[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xF59E0B), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_set_pos(lbl, 12 + i * 43, 58);
    }

    // ── Day grid ───────────────────────────────────────────────────────────
    for (uint8_t i = 0; i < 42; i++) {
        day_labels[i] = lv_label_create(scr_calendar);
        lv_obj_set_style_text_font(day_labels[i], &lv_font_montserrat_14, 0);
        lv_obj_set_pos(day_labels[i],
            14 + (i % 7) * 43,
            75 + (i / 7) * 26);
    }

    cal_redraw();

    // ── Back button ────────────────────────────────────────────────────────
    lv_obj_t *btn_back = lv_btn_create(scr_calendar);
    lv_obj_set_size(btn_back, 80, 30);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_back, [](lv_event_t *e) {
        ui_switch_screen(&scr_home);
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_t *lbl_back = lv_label_create(btn_back);
    lv_label_set_text(lbl_back, LV_SYMBOL_LEFT " Back");
    lv_obj_center(lbl_back);
}