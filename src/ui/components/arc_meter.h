#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_ARC_METER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_ARC_METER_H_

#include "lvgl.h"

class ArcMeter {
public:
  ArcMeter(lv_obj_t *parent, const int32_t min, const int32_t max,
           const lv_palette_t palette, const char *title) {
    meter = lv_meter_create(parent);
    lv_obj_set_size(meter, ARC_SIZE, ARC_SIZE);

    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);

    lv_meter_scale_t *scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 0, 0, 0, lv_color_hex3(0x000000));
    lv_meter_set_scale_range(meter, scale, min, max, 300, 120);

    arc = lv_meter_add_arc(meter, scale, 10, lv_palette_main(palette), 10);
    lv_meter_set_indicator_end_value(meter, arc, (max - min) / 2);

    label = lv_label_create(meter);
    lv_obj_center(label);
    lv_label_set_text(label, "...");

    lv_obj_t *title_label = lv_label_create(meter);
    lv_obj_align(title_label, LV_ALIGN_CENTER, 0, 30);
    lv_label_set_text(title_label, title);
  }

  lv_obj_t *meter;
  lv_meter_indicator_t *arc;
  lv_obj_t *label;

protected:
  void InitLabelStyle(lv_style_t *label_style, const lv_palette_t palette) {
    lv_style_init(label_style);
    lv_style_set_text_font(label_style, &lv_font_montserrat_46);
    lv_style_set_text_color(label_style, lv_palette_darken(palette, 2));
    lv_obj_add_style(label, label_style, LV_PART_MAIN);
  }

private:
  static const uint8_t ARC_SIZE = 130;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_ARC_METER_H_
