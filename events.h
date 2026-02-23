#pragma once
#include <Arduino.h>

//EVENTS*************8
enum EventType {
    EVT_BT_CONNECTED,
    EVT_BT_DISCONNECTED,
    EVT_BT_TRACK_CHANGED,
    EVT_BT_PLAYBACK_CHANGED,

    // SYSTEM EVENTS************
    EVT_WIFI_CONNECTED,
    EVT_WIFI_DISCONNECTED,
    EVT_BATTERY_LOW,
    EVT_WIFI_PORTAL_ACTIVE,
    EVT_TIME_SYNCED,

    EVT_MAX  // *********AWLAYS KEEP LAST*********
};

//EVENT DATA 
struct Event {
    EventType type;
    String    str1;   // GP string 
    String    str2;   // GP string 
    bool      flag;   // GP bool 
    int       value;  // GP int 
};

// CALL BACK 
typedef void (*EventCallback)(Event);
static EventCallback _listeners[EVT_MAX][8];
static uint8_t       _listener_count[EVT_MAX] = {0};

// SUB  EVENT
void events_on(EventType type, EventCallback cb) {
    if (_listener_count[type] < 8) {
        _listeners[type][_listener_count[type]++] = cb;
    }
}

// F EVENT
void events_emit(Event e) {
    for (uint8_t i = 0; i < _listener_count[e.type]; i++) {
        if (_listeners[e.type][i]) {
            _listeners[e.type][i](e);
        }
    }
}