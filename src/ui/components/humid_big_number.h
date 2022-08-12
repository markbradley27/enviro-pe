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
    Update();
  }

  void Update() {
    const float humid = humid_values_->Average(minutes(MOVING_AVERAGE_MINUTES));
    lv_label_set_text_fmt(big_number_, "%.0f", humid);
  }

private:
  static const uint8_t MOVING_AVERAGE_MINUTES = 2;

  RingBuffer<float> *const humid_values_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_BIG_NUMBER_H_
