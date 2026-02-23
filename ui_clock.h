#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include "ui_helpers.h"
lv_obj_t *scr_clock;

void create_clock_screen() {
    scr_clock = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_clock, lv_color_hex(0x0d0d0d), 0);
    lv_obj_clear_flag(scr_clock, LV_OBJ_FLAG_SCROLLABLE);
    
    //DISP**********
    lv_obj_t *lbl_clock = lv_label_create(scr_clock);
    lv_label_set_text(lbl_clock, "00:00");
    lv_obj_set_style_text_color(lbl_clock, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_clock, &lv_font_montserrat_48, 0);
    lv_obj_align(lbl_clock, LV_ALIGN_CENTER, 0, -20);

    // S************
    lv_obj_t *lbl_secs = lv_label_create(scr_clock);
    lv_label_set_text(lbl_secs, "00s");
    lv_obj_set_style_text_color(lbl_secs, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_secs, &lv_font_montserrat_20, 0);
    lv_obj_align(lbl_secs, LV_ALIGN_CENTER, 0, 30);


static lv_obj_t *clock_labels[2];
clock_labels[0] = lbl_clock;
clock_labels[1] = lbl_secs;

//NTP


lv_timer_create([](lv_timer_t *t) {

    lv_obj_t **labels = (lv_obj_t **)t->user_data;
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        
        char timebuf[6];
        char secbuf[5];
        snprintf(timebuf, sizeof(timebuf), "%02d:%02d",
                 timeinfo.tm_hour, timeinfo.tm_min);
        snprintf(secbuf, sizeof(secbuf), "%02ds",
                 timeinfo.tm_sec);
        lv_label_set_text(labels[0], timebuf);
        lv_label_set_text(labels[1], secbuf);
    }
}, 1000, clock_labels);

    // BB
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