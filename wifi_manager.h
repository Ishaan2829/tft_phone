#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
 #include "ui_helpers.h"
#include <Preferences.h>
#include "events.h"
#include <time.h>
extern void wifi_save_network(String ssid, String pass);

#define WIFI_AP_NAME     "esp_setup"
#define WIFI_AP_PASS     "12345678"
#define WIFI_CONNECT_TIMEOUT 10000

static bool wifi_connected = false;
static WebServer portal_server(80);
static Preferences prefs;


// WEBPAGE
static const char PORTAL_HTML[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>BT Phone Setup</title>
  <style>
    body { font-family: sans-serif; background: #0d0d1e; color: white;
           display: flex; justify-content: center; align-items: center;
           height: 100vh; margin: 0; }
    .box { background: #1e1e30; padding: 30px; border-radius: 16px;
           width: 300px; box-shadow: 0 0 20px #8B5CF6; }
    h2   { color: #8B5CF6; text-align: center; margin-bottom: 20px; }
    input { width: 100%; padding: 10px; margin: 8px 0; border-radius: 8px;
            border: 1px solid #333; background: #0d0d0d; color: white;
            box-sizing: border-box; }
    button { width: 100%; padding: 12px; background: #8B5CF6; color: white;
             border: none; border-radius: 8px; font-size: 16px; cursor: pointer;
             margin-top: 10px; }
    p { text-align: center; color: #aaa; font-size: 13px; }
  </style>
</head>
<body>
  <div class='box'>
    <h2>BT Phone Setup</h2>
    <p>Connect to your WiFi network</p>
    <form action='/save' method='POST'>
      <input type='text'     name='ssid'     placeholder='WiFi Name (SSID)' required>
      <input type='password' name='password' placeholder='Password'>
      <button type='submit'>Connect & Save</button>
    </form>
  </div>
</body>
</html>
)";

static const char SUCCESS_HTML[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: sans-serif; background: #0d0d1e; color: white;
           display: flex; justify-content: center; align-items: center;
           height: 100vh; margin: 0; }
    .box { background: #1e1e30; padding: 30px; border-radius: 16px;
           width: 300px; text-align: center; }
    h2 { color: #1DB954; }
    p  { color: #aaa; }
  </style>
</head>
<body>
  <div class='box'>
    <h2>✓ Saved!</h2>
    <p>Device will restart and connect to your network.</p>
    <p>You can close this page.</p>
  </div>
</body>
</html>
)";

// WIP *******C PORTAL********FUTURE WORK********
void handle_root() {
    portal_server.send(200, "text/html", PORTAL_HTML);
}

void handle_save() {
    if (portal_server.hasArg("ssid") && 
        portal_server.arg("ssid").length() > 0) {
        String ssid = portal_server.arg("ssid");
        String pass = portal_server.arg("password");

        prefs.begin("wifi", false);
        prefs.putString("ssid", ssid);
        prefs.putString("pass", pass);
        prefs.end();

        portal_server.send(200, "text/html", SUCCESS_HTML);
        Serial.println("Credentials saved — rebooting");
        delay(2000);
        ESP.restart();
    } else {
        portal_server.send(400, "text/plain", "Missing SSID");
    }
}

void wifi_start_portal() {
    btStop();
    delay(500);
    WiFi.disconnect(true);
    delay(500);
    WiFi.mode(WIFI_AP);
    delay(500);
    WiFi.softAP(WIFI_AP_NAME, WIFI_AP_PASS, 1, 0, 4);
    delay(500);

    Serial.println("Portal started — connect to esp_setup");
    Serial.print("Password: "); Serial.println(WIFI_AP_PASS);
    Serial.println("Then open browser and go to: 192.168.4.1");

    portal_server.on("/",     HTTP_GET,  handle_root);
    portal_server.on("/save", HTTP_POST, handle_save);
    portal_server.begin();

    events_emit({EVT_WIFI_PORTAL_ACTIVE, "esp_setup", "", false});
}

// HARDCODE SSID PASS************************8
#define WIFI_SSID "Ishaan"
#define WIFI_PASS "HOME@12_12"

bool wifi_connect() {
    Serial.print("Connecting to: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > WIFI_CONNECT_TIMEOUT) {
            Serial.println("Connection timed out");
            return false;
        }
        delay(500);
        Serial.print(".");
    }

    wifi_connected = true;
    wifi_save_network(String(WIFI_SSID), String(WIFI_PASS));
    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
    events_emit({EVT_WIFI_CONNECTED, WiFi.localIP().toString(), 
                 String(WIFI_SSID), true});
    return true;
}

//NTP CLOCK************
void ntp_sync() {
    Serial.println("ntp_sync() called");  // ← add this
    Serial.println("Syncing NTP time...");
    delay(1000);
    configTime(19800, 0, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
    // Wait for time to sync
    struct tm timeinfo;
    uint32_t start = millis();
    while (!getLocalTime(&timeinfo)) {
        if (millis() - start > 5000) {
            Serial.println("NTP sync failed");
            return;
        }
        delay(500);
    }
    Serial.println("NTP synced!");
    events_emit({EVT_TIME_SYNCED, "", "", true});
}


void wifi_init() {
    if (!wifi_connect()) {
        wifi_start_portal();
    }
}

void wifi_loop() {
    if (!wifi_connected) {
        portal_server.handleClient();
    }
}