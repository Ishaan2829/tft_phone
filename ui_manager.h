#pragma once
#include <lvgl.h>

extern lv_obj_t *scr_home;
extern lv_obj_t *scr_call;
extern lv_obj_t *scr_music;
extern lv_obj_t *scr_settings;
extern lv_obj_t *scr_calculator;
extern lv_obj_t *scr_calendar;
extern lv_obj_t *scr_clock;


// ── Shared UI labels updated by events ────────────────────────────────────
lv_obj_t *statusbar;
lv_obj_t *lbl_time;
lv_obj_t *lbl_bt_icon;
lv_obj_t *lbl_track;
lv_obj_t *lbl_artist;
lv_obj_t *lbl_bt_status;
lv_obj_t *lbl_bt_device;

// ── Forward declarations of create functions ───────────────────────────────
void create_calculator_screen();
void create_calendar_screen();
void create_clock_screen();

// ── Lazy load flags ────────────────────────────────────────────────────────
static bool scr_calculator_created = false;
static bool scr_calendar_created   = false;
static bool scr_clock_created      = false;

// ── Switch screen with lazy loading ───────────────────────────────────────
void ui_switch_screen(lv_obj_t **target_ptr) {
    // Create screen if first time opening
    if (target_ptr == &scr_calculator && !scr_calculator_created) {
        create_calculator_screen();
        scr_calculator_created = true;
    } else if (target_ptr == &scr_calendar && !scr_calendar_created) {
        create_calendar_screen();
        scr_calendar_created = true;
    } else if (target_ptr == &scr_clock && !scr_clock_created) {
        create_clock_screen();
        scr_clock_created = true;
    }
    lv_scr_load_anim(*target_ptr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false);
}