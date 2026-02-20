#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include "image_data.h"
extern TFT_eSPI tft;

lv_obj_t *scr_home;

typedef struct {
    const char *icon;
    lv_obj_t  **target;
    lv_color_t  color;
} app_item_t;

void create_home_screen() {
    scr_home = lv_obj_create(NULL);
    lv_obj_set_style_bg_opa(scr_home, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_opa(scr_home, LV_OPA_TRANSP, 0);

    lv_disp_t *disp = lv_disp_get_default();
    lv_obj_set_style_bg_color(scr_home, lv_color_hex(0x0a0a1a), 0);
    lv_obj_set_style_bg_grad_color(scr_home, lv_color_hex(0x1a0a2e), 0);
    lv_obj_set_style_bg_grad_dir(scr_home, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_bg_opa(scr_home, LV_OPA_COVER, 0);

    create_statusbar(scr_home);

   

    // APP DEFINATIONS ************************
    app_item_t apps[] = {
        { LV_SYMBOL_CALL,     &scr_call,       lv_color_hex(0x1DB954) },
        { LV_SYMBOL_AUDIO,    &scr_music,      lv_color_hex(0x1A78C2) },
        { LV_SYMBOL_SETTINGS, &scr_settings,   lv_color_hex(0x8B5CF6) },
        { LV_SYMBOL_EDIT,     &scr_calculator, lv_color_hex(0xE53935) },
        { LV_SYMBOL_LIST,     &scr_calendar,   lv_color_hex(0xF59E0B) },
        { LV_SYMBOL_BELL,     &scr_clock,      lv_color_hex(0x06B6D4) },
    };

    // ── Icon grid (2 x 3, centered)*************
    uint8_t cols    = 3;
    uint8_t icon_sz = 72;
    uint8_t gap     = 18;
    uint8_t total_w = cols * icon_sz + (cols - 1) * gap; // 250px
    uint8_t start_x = (320 - total_w) / 2;               // 35px
    uint8_t start_y = 45;

    for (uint8_t i = 0; i < 6; i++) {
        uint8_t col = i % cols;
        uint8_t row = i / cols;

        uint8_t x = start_x + col * (icon_sz + gap);
        uint8_t y = start_y + row * (icon_sz + gap);

        // ICON CONTAINER************************
        lv_obj_t *btn = lv_obj_create(scr_home);
        lv_obj_set_size(btn, icon_sz, icon_sz);
        lv_obj_set_pos(btn, x, y);
        lv_obj_set_style_radius(btn, 20, 0);
        lv_obj_set_style_border_width(btn, 2, 0);
        lv_obj_set_style_border_color(btn, apps[i].color, 0);
        lv_obj_set_style_border_opa(btn, 80, 0);
        lv_obj_set_style_bg_opa(btn, LV_OPA_70, 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x1e1e30), LV_STATE_PRESSED);
        lv_obj_set_style_shadow_width(btn, 12, 0);
        lv_obj_set_style_shadow_color(btn, apps[i].color, 0);
        lv_obj_set_style_shadow_opa(btn, 60, 0);
        lv_obj_set_style_pad_all(btn, 0, 0);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);

        // ICON SYMBOL**************************
        lv_obj_t *icon = lv_label_create(btn);
        lv_label_set_text(icon, apps[i].icon);
        lv_obj_set_style_text_color(icon, apps[i].color, 0);
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_20, 0);
        lv_obj_center(icon);

        // ── Touch handler ──────────────────────────────────────────────────
        lv_obj_set_user_data(btn, apps[i].target);
        lv_obj_add_event_cb(btn, [](lv_event_t *e) {
            lv_obj_t **t = (lv_obj_t **)lv_obj_get_user_data(lv_event_get_target(e));
            ui_switch_screen(t);
        }, LV_EVENT_CLICKED, NULL);
    }
}