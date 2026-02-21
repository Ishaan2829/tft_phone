#pragma once
#include <lvgl.h>
#include "ui_manager.h"

lv_obj_t *scr_splash;

void create_splash_screen() {
    scr_splash = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_splash, lv_color_hex(0x080810), 0);
    lv_obj_set_style_bg_grad_color(scr_splash, lv_color_hex(0x1a0a2e), 0);
    lv_obj_set_style_bg_grad_dir(scr_splash, LV_GRAD_DIR_VER, 0);
    lv_obj_clear_flag(scr_splash, LV_OBJ_FLAG_SCROLLABLE);

    //BIG TITLE********
    lv_obj_t *title = lv_label_create(scr_splash);
    lv_label_set_text(title, "BT Phone");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_48, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -20);

    // TITLE
    lv_obj_t *sub = lv_label_create(scr_splash);
    lv_label_set_text(sub, "Bluetooth Companion Device");
    lv_obj_set_style_text_color(sub, lv_color_hex(0x8B5CF6), 0);
    lv_obj_set_style_text_font(sub, &lv_font_montserrat_14, 0);
    lv_obj_align(sub, LV_ALIGN_CENTER, 0, 30);

    // LOADING BAR
    lv_obj_t *bar = lv_bar_create(scr_splash);
    lv_obj_set_size(bar, 200, 4);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 70);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x222222), 0);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x8B5CF6), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar, 2, 0);
    lv_obj_set_style_radius(bar, 2, LV_PART_INDICATOR);
    lv_bar_set_range(bar, 0, 100);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);

    //LOADING ANIMATION*********
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, [](void *obj, int32_t val) {
        lv_bar_set_value((lv_obj_t *)obj, val, LV_ANIM_OFF);
    });
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_ready_cb(&a, [](lv_anim_t *a) {
        ui_switch_screen(&scr_home);
    });
    lv_anim_start(&a);
}