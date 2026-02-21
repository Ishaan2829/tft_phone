#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include <WiFi.h>

lv_obj_t *scr_settings_wifi;
static lv_obj_t *lbl_wifi_status;
static lv_obj_t *lbl_wifi_ip;

void create_settings_wifi_screen() {
    scr_settings_wifi = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_settings_wifi, lv_color_hex(0x0d0d0d), 0);
    lv_obj_set_style_bg_grad_color(scr_settings_wifi, lv_color_hex(0x0d0d1e), 0);
    lv_obj_set_style_bg_grad_dir(scr_settings_wifi, LV_GRAD_DIR_VER, 0);
    lv_obj_clear_flag(scr_settings_wifi, LV_OBJ_FLAG_SCROLLABLE);


    //TITLE
    lv_obj_t *title = lv_label_create(scr_settings_wifi);
    lv_label_set_text(title, LV_SYMBOL_WIFI " WiFi");
    lv_obj_set_style_text_color(title, lv_color_hex(0x1DB954), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 25);

    // STATUS************
    lbl_wifi_status = lv_label_create(scr_settings_wifi);
    lv_label_set_text(lbl_wifi_status,
        WiFi.isConnected() ? "Status: Connected" : "Status: Disconnected");
    lv_obj_set_style_text_color(lbl_wifi_status,
        WiFi.isConnected()
            ? lv_color_hex(0x1DB954)
            : lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_wifi_status, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_wifi_status, LV_ALIGN_TOP_MID, 0, 55);

    // IP**********
    lbl_wifi_ip = lv_label_create(scr_settings_wifi);
    String ip = WiFi.isConnected() ? WiFi.localIP().toString() : "IP: N/A";
    lv_label_set_text(lbl_wifi_ip, ip.c_str());
    lv_obj_set_style_text_color(lbl_wifi_ip, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_wifi_ip, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_wifi_ip, LV_ALIGN_TOP_MID, 0, 75);

    // SSID***********
    lv_obj_t *lbl_ssid = lv_label_create(scr_settings_wifi);
    String ssid = WiFi.isConnected() ? "Network: " + WiFi.SSID() : "Network: None";
    lv_label_set_text(lbl_ssid, ssid.c_str());
    lv_obj_set_style_text_color(lbl_ssid, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(lbl_ssid, &lv_font_montserrat_14, 0);
    lv_obj_align(lbl_ssid, LV_ALIGN_TOP_MID, 0, 95);

    // DISSS BUTT************
    lv_obj_t *btn_disc = lv_btn_create(scr_settings_wifi);
    lv_obj_set_size(btn_disc, 200, 50);
    lv_obj_align(btn_disc, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_bg_color(btn_disc, lv_color_hex(0xE53935), 0);
    lv_obj_set_style_radius(btn_disc, 12, 0);
    lv_obj_add_event_cb(btn_disc, [](lv_event_t *e) {
        WiFi.disconnect(true);
        lv_label_set_text(lbl_wifi_status, "Status: Disconnected");
        lv_label_set_text(lbl_wifi_ip, "IP: N/A");
        events_emit({EVT_WIFI_DISCONNECTED, "", "", false});
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl_disc = lv_label_create(btn_disc);
    lv_label_set_text(lbl_disc, LV_SYMBOL_CLOSE " Disconnect");
    lv_obj_set_style_text_font(lbl_disc, &lv_font_montserrat_14, 0);
    lv_obj_center(lbl_disc);

    // BACK BUTT*****************************************
    lv_obj_t *btn_back = lv_btn_create(scr_settings_wifi);
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