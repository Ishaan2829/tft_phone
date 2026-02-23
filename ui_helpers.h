#pragma once
#include <lvgl.h>
#include "theme.h"

// Forward declare ui_switch_screen
void ui_switch_screen(lv_obj_t **target_ptr);

void create_back_btn(lv_obj_t *parent, lv_obj_t **target_scr) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 60, 28);
    lv_obj_set_pos(btn, 4, CONTENT_Y);
    lv_obj_set_style_bg_color(btn, lv_color_hex(SD_SURFACE), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(SD_SURFACE_HI), LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn, 4, 0);
    lv_obj_set_style_border_width(btn, 0, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(btn, target_scr);
    lv_obj_add_event_cb(btn, [](lv_event_t *e) {
        lv_obj_t **t = (lv_obj_t **)lv_obj_get_user_data(lv_event_get_target(e));
        ui_switch_screen(t);
    }, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, LV_SYMBOL_LEFT " Back");
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
    lv_obj_center(lbl);
}