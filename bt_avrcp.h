#pragma once
#include <Arduino.h>
#include <BluetoothA2DPSink.h>
#include "events.h"

// Forward declare UI labels
extern lv_obj_t *lbl_bt_status;
extern lv_obj_t *lbl_bt_device;

BluetoothA2DPSink a2dp_sink;
bool bt_connected = false;

// ── AVRCP metadata callback ────────────────────────────────────────────────
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
    static String title, artist;
    switch (id) {
        case ESP_AVRC_MD_ATTR_TITLE:
            title = String((char *)text);
            break;
        case ESP_AVRC_MD_ATTR_ARTIST:
            artist = String((char *)text);
            events_emit({EVT_BT_TRACK_CHANGED, title, artist});
            break;
    }
}

// ── Connection state callback ──────────────────────────────────────────────
void connection_state_callback(esp_a2d_connection_state_t state, void *ptr) {
    if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
        bt_connected = true;
        events_emit({EVT_BT_CONNECTED, "BT-Phone", "", true});
        Serial.println("A2DP Connected!");
    } else if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {
        bt_connected = false;
        events_emit({EVT_BT_DISCONNECTED, "", "", false});
        Serial.println("A2DP Disconnected");
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
void bt_play_pause() { a2dp_sink.pause();    }
void bt_next_track() { a2dp_sink.next();     }
void bt_prev_track() { a2dp_sink.previous(); }