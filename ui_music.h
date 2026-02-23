#pragma once
#include <lvgl.h>
#include "ui_manager.h" 
#include "ui_helpers.h"

lv_obj_t *scr_music;



void create_music_screen() {
    scr_music = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_music, lv_color_hex(0x0d1117), 0);

    // TITLE******
    lv_obj_t *title = lv_label_create(scr_music);
    lv_label_set_text(title, "Music");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 15);

    // TRACK***********
    lbl_track = lv_label_create(scr_music);
    lv_label_set_text(lbl_track, "No Track Playing");
    lv_obj_set_style_text_color(lbl_track, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_track, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_track, LV_ALIGN_CENTER, 0, -40);

    // ARTIST***********
    lbl_artist = lv_label_create(scr_music);
    lv_label_set_text(lbl_artist, "Unknown Artist");
    lv_obj_set_style_text_color(lbl_artist, lv_color_hex(0x888888), 0);
    lv_obj_set_style_text_font(lbl_artist, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_artist, LV_ALIGN_CENTER, 0, -15);

    // ── Previous Button ────────────────────────────────────────────────────
    lv_obj_t *btn_prev = lv_btn_create(scr_music);
    lv_obj_set_size(btn_prev, 75, 50);
    lv_obj_align(btn_prev, LV_ALIGN_CENTER, -110, 40);
    lv_obj_set_style_bg_color(btn_prev, lv_color_hex(0x1A78C2), 0);
    lv_obj_add_event_cb(btn_prev, [](lv_event_t *e) {
        bt_prev_track();
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_prev = lv_label_create(btn_prev);
    lv_label_set_text(lbl_prev, LV_SYMBOL_PREV);
    lv_obj_center(lbl_prev);

    //PLAY/PAUSE BUTTON***************************
    lv_obj_t *btn_play = lv_btn_create(scr_music);
    lv_obj_set_size(btn_play, 75, 50);
    lv_obj_align(btn_play, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_style_bg_color(btn_play, lv_color_hex(0x1DB954), 0);
    lv_obj_add_event_cb(btn_play, [](lv_event_t *e) {
        bt_play_pause();
        lv_obj_t *lbl = lv_obj_get_child(lv_event_get_target(e), 0);
        const char *cur = lv_label_get_text(lbl);
        if (strcmp(cur, LV_SYMBOL_PLAY) == 0)
            lv_label_set_text(lbl, LV_SYMBOL_PAUSE);
        else
            lv_label_set_text(lbl, LV_SYMBOL_PLAY);
        }, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *lbl_play = lv_label_create(btn_play);
    lv_label_set_text(lbl_play, LV_SYMBOL_PLAY);
    lv_obj_center(lbl_play);

    // NEXT*********
    lv_obj_t *btn_next = lv_btn_create(scr_music);
    lv_obj_set_size(btn_next, 75, 50);
    lv_obj_align(btn_next, LV_ALIGN_CENTER, 110, 40);
    lv_obj_set_style_bg_color(btn_next, lv_color_hex(0x1A78C2), 0);
    lv_obj_add_event_cb(btn_next, [](lv_event_t *e) {
        bt_next_track();
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_next = lv_label_create(btn_next);
    lv_label_set_text(lbl_next, LV_SYMBOL_NEXT);
    lv_obj_center(lbl_next);

    // BACK***************
    lv_obj_t *btn_back = lv_btn_create(scr_music);
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