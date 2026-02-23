#pragma once
#include "theme.h"
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include <WiFi.h>
#include <Preferences.h>
#include <utility>
#include"wifi_manager.h"

#include "ui_helpers.h"
lv_obj_t *scr_settings_wifi;
static lv_obj_t *lbl_wifi_status;
static lv_obj_t *lbl_wifi_ip;
static lv_obj_t *networks_cont;

#define MAX_SAVED_NETWORKS 5

// ── Save a network to flash ────────────────────────────────────────────────
void wifi_save_network(String ssid, String pass) {
    Preferences prefs;
    prefs.begin("wifi_nets", false);
    
    // Check if already saved
    for (int i = 0; i < MAX_SAVED_NETWORKS; i++) {
        String key = "ssid_" + String(i);
        if (prefs.getString(key.c_str(), "") == ssid) {
            prefs.end();
            return; // already exists
        }
    }
    
    // Find empty slot
    for (int i = 0; i < MAX_SAVED_NETWORKS; i++) {
        String ssid_key = "ssid_" + String(i);
        if (prefs.getString(ssid_key.c_str(), "") == "") {
            prefs.putString(ssid_key.c_str(), ssid);
            prefs.putString(("pass_" + String(i)).c_str(), pass);
            Serial.println("Saved network: " + ssid);
            break;
        }
    }
    prefs.end();
}

// ── Connect to a saved network ─────────────────────────────────────────────
void wifi_connect_to(String ssid, String pass) {
    WiFi.disconnect(true);
    delay(500);
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("Connecting to: " + ssid);
    
    lv_label_set_text(lbl_wifi_status, "Connecting...");
    
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > 10000) {
            lv_label_set_text(lbl_wifi_status, "Failed to connect");
            return;
        }
        delay(500);
    }
    
    lv_label_set_text(lbl_wifi_status, "Status: Connected");
    lv_label_set_text(lbl_wifi_ip, WiFi.localIP().toString().c_str());
    events_emit({EVT_WIFI_CONNECTED, WiFi.localIP().toString(), ssid, true});
    Serial.println("Connected: " + WiFi.localIP().toString());
}

// ── Delete a saved network ─────────────────────────────────────────────────
void wifi_delete_network(int index) {
    Preferences prefs;
    prefs.begin("wifi_nets", false);
    prefs.putString(("ssid_" + String(index)).c_str(), "");
    prefs.putString(("pass_" + String(index)).c_str(), "");
    prefs.end();
}

// ── Rebuild the saved networks list ───────────────────────────────────────
void wifi_rebuild_list() {
    lv_obj_clean(networks_cont);
    
    Preferences prefs;
    prefs.begin("wifi_nets", true);
    
    bool any = false;
    for (int i = 0; i < MAX_SAVED_NETWORKS; i++) {
        String ssid = prefs.getString(("ssid_" + String(i)).c_str(), "");
        if (ssid == "") continue;
        any = true;
        
        String pass = prefs.getString(("pass_" + String(i)).c_str(), "");
        
        // Network row
        lv_obj_t *row = lv_obj_create(networks_cont);
        lv_obj_set_size(row, 270, 36);
        lv_obj_set_style_bg_color(row, lv_color_hex(SD_SURFACE), 0);
        lv_obj_set_style_bg_color(row, lv_color_hex(SD_SURFACE_HI), LV_STATE_PRESSED);
        lv_obj_set_style_border_width(row, 1, 0);
        lv_obj_set_style_border_color(row, lv_color_hex(SD_BORDER), 0);
        lv_obj_set_style_radius(row, 4, 0);
        lv_obj_set_style_pad_all(row, 0, 0);
        lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(row, LV_OBJ_FLAG_CLICKABLE);

        // SSID label
        lv_obj_t *lbl = lv_label_create(row);
        lv_label_set_text(lbl, ssid.c_str());
        lv_obj_set_style_text_color(lbl, lv_color_hex(SD_TEXT), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 8, 0);

        // Connected indicator
        if (WiFi.isConnected() && WiFi.SSID() == ssid) {
            lv_obj_t *dot = lv_label_create(row);
            lv_label_set_text(dot, LV_SYMBOL_OK);
            lv_obj_set_style_text_color(dot, lv_color_hex(SD_RED), 0);
            lv_obj_set_style_text_font(dot, &lv_font_montserrat_14, 0);
            lv_obj_align(dot, LV_ALIGN_RIGHT_MID, -24, 0);
        }

        // Delete button
        lv_obj_t *btn_del = lv_btn_create(row);
        lv_obj_set_size(btn_del, 20, 20);
        lv_obj_align(btn_del, LV_ALIGN_RIGHT_MID, -4, 0);
        lv_obj_set_style_bg_color(btn_del, lv_color_hex(SD_RED_DARK), 0);
        lv_obj_set_style_radius(btn_del, 2, 0);
        lv_obj_set_style_border_width(btn_del, 0, 0);
        lv_obj_add_flag(btn_del, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(btn_del, [](lv_event_t *e) {
            int idx = (int)(intptr_t)lv_event_get_user_data(e);
            wifi_delete_network(idx);
            wifi_rebuild_list();
        }, LV_EVENT_CLICKED, (void *)(intptr_t)i);

        lv_obj_t *x = lv_label_create(btn_del);
        lv_label_set_text(x, LV_SYMBOL_CLOSE);
        lv_obj_set_style_text_font(x, &lv_font_montserrat_14, 0);
        lv_obj_center(x);

        // Tap row to connect
        lv_obj_set_user_data(row, new std::pair<String,String>(ssid, pass));
        lv_obj_add_event_cb(row, [](lv_event_t *e) {
            auto *p = (std::pair<String,String>*)lv_obj_get_user_data(lv_event_get_target(e));
            wifi_connect_to(p->first, p->second);
        }, LV_EVENT_CLICKED, NULL);
    }
    prefs.end();

    if (!any) {
        lv_obj_t *empty = lv_label_create(networks_cont);
        lv_label_set_text(empty, "No saved networks");
        lv_obj_set_style_text_color(empty, lv_color_hex(SD_TEXT_MUTED), 0);
        lv_obj_set_style_text_font(empty, &lv_font_montserrat_14, 0);
    }
}

void create_settings_wifi_screen() {
    scr_settings_wifi = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_settings_wifi, lv_color_hex(SD_BG), 0);
    lv_obj_clear_flag(scr_settings_wifi, LV_OBJ_FLAG_SCROLLABLE);

    create_back_btn(scr_settings_wifi, &scr_settings);

    // ── Title ──────────────────────────────────────────────────────────────
    lv_obj_t *title = lv_label_create(scr_settings_wifi);
    lv_label_set_text(title, LV_SYMBOL_WIFI " WiFi");
    lv_obj_set_style_text_color(title, lv_color_hex(SD_RED), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(title, 130, CONTENT_Y + 4);

    // ── Status ─────────────────────────────────────────────────────────────
    lbl_wifi_status = lv_label_create(scr_settings_wifi);
    lv_label_set_text(lbl_wifi_status,
        WiFi.isConnected() ? "Status: Connected" : "Status: Disconnected");
    lv_obj_set_style_text_color(lbl_wifi_status,
        WiFi.isConnected() ? lv_color_hex(SD_RED) : lv_color_hex(SD_TEXT_MUTED), 0);
    lv_obj_set_style_text_font(lbl_wifi_status, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(lbl_wifi_status, 15, 38);

    // ── IP ─────────────────────────────────────────────────────────────────
    lbl_wifi_ip = lv_label_create(scr_settings_wifi);
    String ip = WiFi.isConnected() ? WiFi.localIP().toString() : "IP: N/A";
    lv_label_set_text(lbl_wifi_ip, ip.c_str());
    lv_obj_set_style_text_color(lbl_wifi_ip, lv_color_hex(SD_TEXT_MUTED), 0);
    lv_obj_set_style_text_font(lbl_wifi_ip, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(lbl_wifi_ip, 15, 55);

    // ── Saved networks label ───────────────────────────────────────────────
    lv_obj_t *lbl_saved = lv_label_create(scr_settings_wifi);
    lv_label_set_text(lbl_saved, "SAVED NETWORKS");
    lv_obj_set_style_text_color(lbl_saved, lv_color_hex(SD_TEXT_MUTED), 0);
    lv_obj_set_style_text_font(lbl_saved, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(lbl_saved, 15, 75);

    // ── Networks container ─────────────────────────────────────────────────
    networks_cont = lv_obj_create(scr_settings_wifi);
    lv_obj_set_size(networks_cont, 290, 130);
    lv_obj_set_pos(networks_cont, 15, 88);
    lv_obj_set_style_bg_color(networks_cont, lv_color_hex(SD_BG), 0);
    lv_obj_set_style_border_width(networks_cont, 0, 0);
    lv_obj_set_flex_flow(networks_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(networks_cont, 4, 0);
    lv_obj_set_style_pad_all(networks_cont, 0, 0);

    wifi_rebuild_list();

    // ── Disconnect button ──────────────────────────────────────────────────
    lv_obj_t *btn_disc = lv_btn_create(scr_settings_wifi);
    lv_obj_set_size(btn_disc, 120, 32);
    lv_obj_set_pos(btn_disc, 170, 200);
    lv_obj_set_style_bg_color(btn_disc, lv_color_hex(SD_RED_DARK), 0);
    lv_obj_set_style_radius(btn_disc, 4, 0);
    lv_obj_set_style_border_width(btn_disc, 0, 0);
    lv_obj_add_flag(btn_disc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_disc, [](lv_event_t *e) {
        WiFi.disconnect(true);
        lv_label_set_text(lbl_wifi_status, "Status: Disconnected");
        lv_label_set_text(lbl_wifi_ip, "IP: N/A");
        events_emit({EVT_WIFI_DISCONNECTED, "", "", false});
        wifi_rebuild_list();
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_t *ld = lv_label_create(btn_disc);
    lv_label_set_text(ld, LV_SYMBOL_CLOSE " Disconnect");
    lv_obj_set_style_text_font(ld, &lv_font_montserrat_14, 0);
    lv_obj_center(ld);
}