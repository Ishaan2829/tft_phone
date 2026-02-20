#pragma once
#include <Arduino.h>
#include <BluetoothA2DPSink.h>

// Forward declare UI labels
extern lv_obj_t *lbl_bt_status;
extern lv_obj_t *lbl_bt_device;

BluetoothA2DPSink a2dp_sink;


//Global bool for sharing bt status
bool bt_connected = false;

// ── Track info ─────────────────────────────────────────────────────────────
String bt_track  = "No Track";
String bt_artist = "Unknown Artist";
bool   bt_playing = false;

// ── AVRCP metadata callback ────────────────────────────────────────────────
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
    switch (id) {
        case ESP_AVRC_MD_ATTR_TITLE:
            bt_track = String((char *)text);
            break;
        case ESP_AVRC_MD_ATTR_ARTIST:
            bt_artist = String((char *)text);
            break;
    }
}

// ── Connection state callback ──────────────────────────────────────────────
void connection_state_callback(esp_a2d_connection_state_t state, void *ptr) {
    if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
        bt_connected = true;
        lv_label_set_text(lbl_bt_status, "Status: Connected");
    } else if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {
        bt_connected = false;
        lv_label_set_text(lbl_bt_status, "Status: Disconnected");
        lv_label_set_text(lbl_bt_device, "Device: None");
    }
}

// ── Init Bluetooth ─────────────────────────────────────────────────────────
void bt_init() {
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_on_connection_state_changed(connection_state_callback);
    a2dp_sink.start("BT-Phone");
    Serial.println("Bluetooth started — search for BT-Phone");
}

// ── Music controls ─────────────────────────────────────────────────────────
void bt_play_pause() {
    if (bt_playing) {
        a2dp_sink.pause();
        bt_playing = false;
    } else {
        a2dp_sink.play();
        bt_playing = true;
    }
}

void bt_next_track() {
    a2dp_sink.next();
}

void bt_prev_track() {
    a2dp_sink.previous();
}