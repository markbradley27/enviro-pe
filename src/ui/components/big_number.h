#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_BIG_NUMBER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_BIG_NUMBER_H_

#include "lvgl.h"

#include "ui/styles.h"

#define TREND_SYMBOL_DOWN "\xEE\x82\x97"
#define TREND_SYMBOL_UP "\xEE\x82\x98"
#define TREND_SYMBOL_FLAT "\xEF\x81\xA8"

class BigNumber {
public:
  BigNumber(lv_obj_t *const parent, const char *title) {
    lv_obj_t *title_label = lv_label_create(parent);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    big_number_ = lv_label_create(parent);
    lv_obj_add_style(big_number_, &big_number_style, LV_PART_MAIN);
    lv_obj_align(big_number_, LV_ALIGN_TOP_MID, 0, 35);

    trend_icon_ = lv_label_create(parent);
    lv_obj_add_style(trend_icon_, &big_number_trend_style, LV_PART_MAIN);
    lv_obj_align(trend_icon_, LV_ALIGN_TOP_MID, 0, 99);
    lv_label_set_text(trend_icon_, TREND_SYMBOL_FLAT);

    lv_obj_t *divider_label = lv_label_create(parent);
    lv_label_set_text(divider_label, "-----------");
    lv_obj_align(divider_label, LV_ALIGN_TOP_MID, 0, 118);
    lv_obj_add_style(divider_label, &big_number_divider_style, LV_PART_MAIN);

    lv_obj_t *twenty_four_hr_label = lv_label_create(parent);
    lv_label_set_text(twenty_four_hr_label, "24hr");
    lv_obj_align(twenty_four_hr_label, LV_ALIGN_TOP_MID, 0, 135);

    lv_obj_t *min_label = lv_label_create(parent);
    lv_label_set_text(min_label, "min");
    lv_obj_align(min_label, LV_ALIGN_TOP_MID, -MIN_MAX_X_OFFSET, 153);

    lv_obj_t *max_label = lv_label_create(parent);
    lv_label_set_text(max_label, "max");
    lv_obj_align(max_label, LV_ALIGN_TOP_MID, MIN_MAX_X_OFFSET, 153);

    min_number_ = lv_label_create(parent);
    lv_label_set_text(min_number_, "-");
    lv_obj_align(min_number_, LV_ALIGN_TOP_MID, -MIN_MAX_X_OFFSET, 172);

    max_number_ = lv_label_create(parent);
    lv_label_set_text(max_number_, "-");
    lv_obj_align(max_number_, LV_ALIGN_TOP_MID, MIN_MAX_X_OFFSET, 172);
  }

protected:
  static const uint8_t MIN_MAX_X_OFFSET = 20;

  // Moving average period to display as the big number.
  static const uint8_t MOVING_AVERAGE_MINUTES = 2;
  // Moving average period to compare against when deciding the trend.
  static const uint8_t TREND_COMP_MOVING_AVERAGE_MINUTES = 10;

  lv_obj_t *big_number_;
  lv_obj_t *trend_icon_;
  lv_obj_t *min_number_;
  lv_obj_t *max_number_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_BIG_NUMBER_H_
