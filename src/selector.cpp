#include "main.hpp"

//lvgl docs: https://gcec-2918.github.io/LVGL_v5-3_Documentation_Archive/#Get-started

namespace {
lv_style_t buttonStyleReleased; //relesed style
lv_style_t buttonStylePressed; //pressed style
lv_style_t buttonStyleToggledPressed; //relesed style
lv_style_t buttonStyleToggledReleased; //pressed style
}

void init_button_styles() {
    lv_style_copy(&buttonStyleReleased, &lv_style_plain);
    buttonStyleReleased.body.main_color = LV_COLOR_MAKE(50, 50, 50);
    buttonStyleReleased.body.grad_color = LV_COLOR_MAKE(50, 50, 50);
    buttonStyleReleased.body.radius = 0;
    buttonStyleReleased.text.color = LV_COLOR_WHITE;

    lv_style_copy(&buttonStylePressed, &lv_style_plain);
    buttonStylePressed.body.main_color = LV_COLOR_WHITE;
    buttonStylePressed.body.grad_color = LV_COLOR_WHITE;
    buttonStylePressed.body.radius = 0;
    buttonStylePressed.text.color = LV_COLOR_BLACK;

    lv_style_copy(&buttonStyleToggledPressed, &buttonStylePressed);

    lv_style_copy(&buttonStyleToggledPressed, &lv_style_plain);
    buttonStylePressed.body.main_color = LV_COLOR_LIME;
    buttonStylePressed.body.grad_color = LV_COLOR_LIME;
    buttonStylePressed.body.radius = 0;
    buttonStylePressed.text.color = LV_COLOR_BLACK;
}

// https://team81k.github.io/ProsLVGLTutorial/
lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
    int id, const char * title)
{
    lv_obj_t * btn = lv_btn_create(parent, NULL);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, width, height);
    lv_obj_set_free_num(btn, id);

    lv_obj_t * label = lv_label_create(btn, NULL);
    lv_label_set_text(label, title);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    lv_btn_set_style(btn, LV_BTN_STYLE_REL, &buttonStyleReleased);
    lv_btn_set_style(btn, LV_BTN_STYLE_PR, &buttonStylePressed);
    lv_btn_set_style(btn, LV_BTN_STYLE_TGL_REL, &buttonStyleToggledReleased);
    lv_btn_set_style(btn, LV_BTN_STYLE_TGL_PR, &buttonStyleToggledPressed);

    return btn;
}