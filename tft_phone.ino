#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>
#include "ui_statusbar.h"
#include "bt_avrcp.h"
#include "ui_clock.h"
#include "ui_calendar.h"
#include "ui_calculator.h"

#include "ui_manager.h"
#include "ui_home.h"
#include "ui_call.h"
#include "ui_music.h"
#include "ui_settings.h"
#include "ui_splash.h"

// ── Hardware ───────────────────────────────────────────────────────────────
#define T_CS  5
#define T_IRQ 27

TFT_eSPI tft;
XPT2046_Touchscreen touch(T_CS, T_IRQ);

// ── LVGL buffer ────────────────────────────────────────────────────────────
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10];

// ── Display flush ──────────────────────────────────────────────────────────
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    // If this is a full screen flush, draw wallpaper first
    

    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// ── Touch read ─────────────────────────────────────────────────────────────
void my_touch_read(lv_indev_drv_t *indev, lv_indev_data_t *data) {
    if (touch.tirqTouched() && touch.touched()) {
        TS_Point p = touch.getPoint();
        data->point.x = map(p.x, 350, 3500, 0, 239);
        data->point.y = map(p.y, 600, 3550, 0, 319);
        data->state   = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void setup() {
    Serial.begin(115200);

    tft.init();
    tft.setRotation(3);// inverse landscape

    touch.begin();
   
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = 320;
    disp_drv.ver_res  = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touch_read;
    lv_indev_drv_register(&indev_drv);

    bt_init();

    //  screens
    create_home_screen();
    create_call_screen();
    create_music_screen();
    create_settings_screen();
    //create_clock_screen();
    //create_calendar_screen();
    //create_calculator_screen();

    create_splash_screen();
    lv_scr_load(scr_splash);
}

void loop() {
    Serial.println(ESP.getFreeHeap());
    uint32_t secs = millis() / 1000;
    update_statusbar(secs, bt_connected);

    lv_label_set_text(lbl_track,  bt_track.c_str());
    lv_label_set_text(lbl_artist, bt_artist.c_str());

    lv_timer_handler();
    delay(5);
}