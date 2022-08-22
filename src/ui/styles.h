#ifndef ENVIRO_PE_SRC_UI_STYLES_H_
#define ENVIRO_PE_SRC_UI_STYLES_H_

#include "lvgl.h"

LV_FONT_DECLARE(montserrat_semibold_70);
LV_FONT_DECLARE(trend_symbols);

static lv_style_t big_number_digital_clock_style;
static lv_style_t big_number_container_style;
static lv_style_t big_number_content_style;
static lv_style_t big_number_style;
static lv_style_t big_number_trend_style;
static lv_style_t big_number_divider_style;

static lv_style_t temp_color_style;
static lv_style_t humid_color_style;
static lv_style_t aqi_color_style;

static lv_style_t graph_title_style;
static lv_style_t graph_point_style;

void InitStyles() {
  lv_style_init(&big_number_digital_clock_style);
  lv_style_set_text_align(&big_number_digital_clock_style, LV_TEXT_ALIGN_RIGHT);
  lv_style_set_text_font(&big_number_digital_clock_style,
                         &lv_font_montserrat_32);
  lv_style_set_text_color(&big_number_digital_clock_style,
                          lv_palette_darken(LV_PALETTE_GREY, 2));

  lv_style_init(&big_number_container_style);
  lv_style_set_width(&big_number_container_style, 104);
  lv_style_set_height(&big_number_container_style, lv_pct(98));
  lv_style_set_outline_width(&big_number_container_style, 0);
  lv_style_set_outline_pad(&big_number_container_style, 0);
  lv_style_set_border_width(&big_number_container_style, 1);
  lv_style_set_border_color(&big_number_container_style,
                            lv_palette_lighten(LV_PALETTE_GREY, 1));
  lv_style_set_pad_all(&big_number_container_style, 0);

  lv_style_init(&big_number_content_style);
  lv_style_set_pad_all(&big_number_content_style, 0);

  lv_style_init(&big_number_style);
  lv_style_set_text_font(&big_number_style, &montserrat_semibold_70);

  lv_style_init(&big_number_trend_style);
  lv_style_set_text_font(&big_number_trend_style, &trend_symbols);

  lv_style_init(&big_number_divider_style);
  lv_style_set_text_color(&big_number_divider_style,
                          lv_palette_lighten(LV_PALETTE_GREY, 1));

  lv_style_init(&temp_color_style);
  lv_style_set_text_color(&temp_color_style,
                          lv_palette_darken(LV_PALETTE_RED, 2));

  lv_style_init(&humid_color_style);
  lv_style_set_text_color(&humid_color_style,
                          lv_palette_darken(LV_PALETTE_BLUE, 2));

  lv_style_init(&aqi_color_style);
  lv_style_set_text_color(&aqi_color_style,
                          lv_palette_darken(LV_PALETTE_GREEN, 2));

  lv_style_init(&graph_title_style);
  lv_style_set_text_align(&graph_title_style, LV_TEXT_ALIGN_CENTER);

  lv_style_init(&graph_point_style);
  lv_style_set_width(&graph_point_style, 0);
  lv_style_set_height(&graph_point_style, 0);
  lv_style_set_radius(&graph_point_style, 0);
}

#endif // ENVIRO_PE_SRC_UI_STYLES_H_
