#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_BIG_NUMBER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_BIG_NUMBER_H_

#include "ring_buffer.h"
#include "ui/components/big_number.h"
#include "ui/styles.h"
#include "util.h"

class TempBigNumber : public BigNumber {
public:
  TempBigNumber(lv_obj_t *parent, RingBuffer<float> *const temp_c_values)
      : BigNumber(parent, "Temp (°F)"), temp_c_values_(temp_c_values) {
    lv_obj_add_style(big_number_, &temp_color_style, LV_PART_MAIN);
    Update();
  }

  void Update() {
    const float temp_f =
        CToF(temp_c_values_->Average(minutes(MOVING_AVERAGE_MINUTES)));
    lv_label_set_text_fmt(big_number_, "%.0f", temp_f);
  }

private:
  static const uint8_t MOVING_AVERAGE_MINUTES = 2;

  RingBuffer<float> *const temp_c_values_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_BIG_NUMBER_H_
