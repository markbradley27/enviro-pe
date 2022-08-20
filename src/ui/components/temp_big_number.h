#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_BIG_NUMBER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_BIG_NUMBER_H_

#include "ring_buffer.h"
#include "ui/components/big_number.h"
#include "ui/styles.h"
#include "util.h"

class TempBigNumber : public BigNumber {
public:
  TempBigNumber(lv_obj_t *parent, RingBuffer<float> *const temp_c_5s_values,
                RingBuffer<float> *const temp_c_5m_avgs)
      : BigNumber(parent, "Temp (°F)"), temp_c_5s_values_(temp_c_5s_values),
        temp_c_5m_avgs_(temp_c_5m_avgs) {
    lv_obj_add_style(big_number_, &temp_color_style, LV_PART_MAIN);
    lv_obj_add_style(trend_icon_, &temp_color_style, LV_PART_MAIN);
    lv_obj_add_style(min_number_, &temp_color_style, LV_PART_MAIN);
    lv_obj_add_style(max_number_, &temp_color_style, LV_PART_MAIN);
    Update();
  }

  void Update() {
    const float temp_c =
        temp_c_5s_values_->Average(minutes(MOVING_AVERAGE_MINUTES));
    lv_label_set_text_fmt(big_number_, "%.0f", CToF(temp_c));

    const float diff = temp_c - temp_c_5s_values_->Average(
                                    minutes(TREND_COMP_MOVING_AVERAGE_MINUTES));
    lv_label_set_text(trend_icon_,
                      abs(diff) < TREND_THRESHOLD_C
                          ? TREND_SYMBOL_FLAT
                          : diff > 0 ? TREND_SYMBOL_UP : TREND_SYMBOL_DOWN);

    if (temp_c_5m_avgs_->rbegin() != temp_c_5m_avgs_->rend()) {
      const auto min_max = temp_c_5m_avgs_->MinMax(hours(24));
      lv_label_set_text_fmt(min_number_, "%.0f", CToF(min_max.first));
      lv_label_set_text_fmt(max_number_, "%.0f", CToF(min_max.second));
    } else {
      lv_label_set_text(min_number_, "-");
      lv_label_set_text(max_number_, "-");
    }
  }

private:
  // Displayed moving average must differ from trend comparison moving average
  // by this much to show a trend symbol.
  static constexpr float TREND_THRESHOLD_C = 0.139; // ~1/4 degree Fahrenheit

  RingBuffer<float> *const temp_c_5s_values_;
  RingBuffer<float> *const temp_c_5m_avgs_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_BIG_NUMBER_H_
