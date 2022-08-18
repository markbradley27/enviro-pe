#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_BIG_NUMBER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_BIG_NUMBER_H_

#include "ring_buffer.h"
#include "ui/components/big_number.h"
#include "ui/styles.h"
#include "util.h"

class HumidBigNumber : public BigNumber {
public:
  HumidBigNumber(lv_obj_t *parent, RingBuffer<float> *const humid_values)
      : BigNumber(parent, "Humid (%)"), humid_values_(humid_values) {
    lv_obj_add_style(big_number_, &humid_color_style, LV_PART_MAIN);
    lv_obj_add_style(trend_icon_, &humid_color_style, LV_PART_MAIN);
    lv_obj_add_style(min_number_, &humid_color_style, LV_PART_MAIN);
    lv_obj_add_style(max_number_, &humid_color_style, LV_PART_MAIN);
    Update();
  }

  void Update() {
    const float humid = humid_values_->Average(minutes(MOVING_AVERAGE_MINUTES));
    lv_label_set_text_fmt(big_number_, "%.0f", humid);

    const float diff = humid - humid_values_->Average(
                                   minutes(TREND_COMP_MOVING_AVERAGE_MINUTES));
    lv_label_set_text(trend_icon_,
                      abs(diff) < TREND_THRESHOLD
                          ? TREND_SYMBOL_FLAT
                          : diff > 0 ? TREND_SYMBOL_UP : TREND_SYMBOL_DOWN);

    const auto min_max = humid_values_->MinMax(hours(24));
    lv_label_set_text_fmt(min_number_, "%.0f", min_max.first);
    lv_label_set_text_fmt(max_number_, "%.0f", min_max.second);
  }

private:
  // Displayed moving average must differ from trend comparison moving average
  // by this much to show a trend symbol.
  static constexpr float TREND_THRESHOLD = 2;

  RingBuffer<float> *const humid_values_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_BIG_NUMBER_H_
