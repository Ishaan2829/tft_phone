#pragma once
#include <Arduino.h>

// ── Event types ────────────────────────────────────────────────────────────
enum EventType {
    // Bluetooth events
    EVT_BT_CONNECTED,
    EVT_BT_DISCONNECTED,
    EVT_BT_TRACK_CHANGED,
    EVT_BT_PLAYBACK_CHANGED,

    // System events (add more as needed)
    EVT_WIFI_CONNECTED,
    EVT_WIFI_DISCONNECTED,
    EVT_BATTERY_LOW,

    EVT_MAX  // always keep last
};

// ── Event data ─────────────────────────────────────────────────────────────
struct Event {
    EventType type;
    String    str1;   // general purpose string (track title, device name etc)
    String    str2;   // general purpose string (artist etc)
    bool      flag;   // general purpose bool (playing, connected etc)
    int       value;  // general purpose int  (battery %, volume etc)
};

// ── Callback type ──────────────────────────────────────────────────────────
typedef void (*EventCallback)(Event);

// ── Registry — up to 8 listeners per event type ───────────────────────────
static EventCallback _listeners[EVT_MAX][8];
static uint8_t       _listener_count[EVT_MAX] = {0};

// ── Subscribe to an event ──────────────────────────────────────────────────
void events_on(EventType type, EventCallback cb) {
    if (_listener_count[type] < 8) {
        _listeners[type][_listener_count[type]++] = cb;
    }
}

// ── Fire an event ──────────────────────────────────────────────────────────
void events_emit(Event e) {
    for (uint8_t i = 0; i < _listener_count[e.type]; i++) {
        if (_listeners[e.type][i]) {
            _listeners[e.type][i](e);
        }
    }
}