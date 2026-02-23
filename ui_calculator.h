#pragma once
#include <lvgl.h>
#include "ui_manager.h"
#include "ui_statusbar.h"
#include "ui_helpers.h"
lv_obj_t *scr_calculator;

// STATE
static double  calc_a       = 0;
static double  calc_b       = 0;
static char    calc_op      = 0;
static bool    calc_new_num = true;
static lv_obj_t *lbl_display;

//DISP UPDATE AFTER PRESS**************
void calc_update_display(double val) {
    char buf[20];
    if (val == (int64_t)val)
        snprintf(buf, sizeof(buf), "%lld", (int64_t)val);
    else
        snprintf(buf, sizeof(buf), "%.4g", val);
    lv_label_set_text(lbl_display, buf);
}

// PRESS HANDLER************************
void calc_handle(const char *txt) {
    // Clear
    if (strcmp(txt, "C") == 0) {
        calc_a = calc_b = 0;
        calc_op = 0;
        calc_new_num = true;
        lv_label_set_text(lbl_display, "0");
        return;
    }

  
    if (strcmp(txt, "=") == 0) {
        double cur = atof(lv_label_get_text(lbl_display));
        switch (calc_op) {
            case '+': calc_a += cur; break;
            case '-': calc_a -= cur; break;
            case '*': calc_a *= cur; break;
            case '/': calc_a = cur != 0 ? calc_a / cur : 0; break;
        }
        calc_update_display(calc_a);
        calc_op = 0;
        calc_new_num = true;
        return;
    }

    
    if (strcmp(txt,"+") == 0 || strcmp(txt,"-") == 0 ||
        strcmp(txt,"*") == 0 || strcmp(txt,"/") == 0) {
        calc_a   = atof(lv_label_get_text(lbl_display));
        calc_op  = txt[0];
        calc_new_num = true;
        return;
    }

    const char *cur = lv_label_get_text(lbl_display);
    char buf[20];
    if (calc_new_num) {
        snprintf(buf, sizeof(buf), "%s", txt);
        calc_new_num = false;
    } else {
        if (strlen(cur) >= 12) return; // limit length
        snprintf(buf, sizeof(buf), "%s%s", cur, txt);
    }
    lv_label_set_text(lbl_display, buf);
}

void create_calculator_screen() {
    scr_calculator = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_calculator, lv_color_hex(0x0d0d0d), 0);
    lv_obj_clear_flag(scr_calculator, LV_OBJ_FLAG_SCROLLABLE);
;

    lv_obj_t *display = lv_obj_create(scr_calculator);
    lv_obj_set_size(display, 300, 40);
    lv_obj_align(display, LV_ALIGN_TOP_MID, 0, 25);
    lv_obj_set_style_bg_color(display, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_style_border_width(display, 0, 0);
    lv_obj_set_style_radius(display, 8, 0);

    lbl_display = lv_label_create(display);
    lv_label_set_text(lbl_display, "0");
    lv_obj_set_style_text_color(lbl_display, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_display, &lv_font_montserrat_20, 0);
    lv_obj_align(lbl_display, LV_ALIGN_RIGHT_MID, -8, 0);

    // GRID******************
    const char *btns[4][4] = {
    { "C", "/", "*", "-" },
    { "7", "8", "9", "+" },
    { "4", "5", "6", "=" },
    { "1", "2", "3", "0" },
    };
    
    for (uint8_t r = 0; r < 4; r++) {
        for (uint8_t c = 0; c < 4; c++) {
            const char *label = btns[r][c];
            lv_obj_t *btn = lv_btn_create(scr_calculator);
            lv_obj_set_size(btn, 75, 45);  // wider and taller
            lv_obj_set_pos(btn, 4 + c * 79, 65 + r * 48);
    lv_color_t op_color  = lv_color_hex(0xF59E0B);
    lv_color_t num_color = lv_color_hex(0x2a2a2a);
    lv_color_t clr_color = lv_color_hex(0xE53935);

    

            

            // COLOUR
            if (strcmp(label, "C") == 0)
                lv_obj_set_style_bg_color(btn, clr_color, 0);
            else if (strcmp(label,"+") == 0 || strcmp(label,"-") == 0 ||
                     strcmp(label,"*") == 0 || strcmp(label,"/") == 0 ||
                     strcmp(label,"=") == 0)
                lv_obj_set_style_bg_color(btn, op_color, 0);
            else
                lv_obj_set_style_bg_color(btn, num_color, 0);

            lv_obj_t *lbl = lv_label_create(btn);
            lv_label_set_text(lbl, label);
            lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
            lv_obj_center(lbl);

           
            lv_obj_add_event_cb(btn, [](lv_event_t *e) {
                calc_handle((const char *)lv_event_get_user_data(e));
            }, LV_EVENT_CLICKED, (void *)label);
        }
    }

    lv_obj_t *btn_back = lv_btn_create(scr_calculator);
    lv_obj_set_size(btn_back, 80, 30);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 4, 22);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn_back, [](lv_event_t *e) {
        ui_switch_screen(&scr_home);
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_t *lbl_back = lv_label_create(btn_back);
    lv_label_set_text(lbl_back, LV_SYMBOL_LEFT " Back");
    lv_obj_center(lbl_back);
}