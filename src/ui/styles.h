#ifndef ENVIRO_PE_SRC_UI_STYLES_H_
#define ENVIRO_PE_SRC_UI_STYLES_H_

#include "lvgl.h"

#include "montserrat_semibold_70.h"

static lv_style_t big_number_container_style;
static lv_style_t big_number_content_style;
static lv_style_t big_number_style;

static lv_style_t temp_color_style;
static lv_style_t humid_color_style;
static lv_style_t aqi_color_style;

void InitStyles() {
  lv_style_init(&big_number_container_style);
  /*
  lv_style_set_width(&big_number_container_style, lv_pct(33));
  */
  lv_style_set_height(&big_number_container_style, lv_pct(100));
  lv_style_set_outline_width(&big_number_container_style, 0);
  lv_style_set_outline_pad(&big_number_container_style, 0);
  lv_style_set_border_width(&big_number_container_style, 0);
  lv_style_set_pad_all(&big_number_container_style, 0);

  lv_style_init(&big_number_content_style);
  lv_style_set_pad_all(&big_number_content_style, 0);

  lv_style_init(&big_number_style);
  lv_style_set_text_font(&big_number_style, &montserrat_semibold_70);

  lv_style_init(&temp_color_style);
  lv_style_set_text_color(&temp_color_style,
                          lv_palette_darken(LV_PALETTE_RED, 2));

  lv_style_init(&humid_color_style);
  lv_style_set_text_color(&humid_color_style,
                          lv_palette_darken(LV_PALETTE_BLUE, 2));

  lv_style_init(&aqi_color_style);
  lv_style_set_text_color(&aqi_color_style,
                          lv_palette_darken(LV_PALETTE_GREEN, 2));
}

#endif // ENVIRO_PE_SRC_UI_STYLES_H_
