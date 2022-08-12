#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_AQI_BIG_NUMBER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_AQI_BIG_NUMBER_H_

#include "ring_buffer.h"
#include "ui/components/big_number.h"
#include "ui/styles.h"
#include "util.h"

class AqiBigNumber : public BigNumber {
public:
  AqiBigNumber(lv_obj_t *parent, RingBuffer<uint16_t> *const pm25_values)
      : BigNumber(parent, "AQI"), pm25_values_(pm25_values) {
    lv_obj_add_style(big_number_, &aqi_color_style, LV_PART_MAIN);
    Update();
  }

  void Update() {
    const uint16_t pm25 =
        pm25_values_->Average(minutes(MOVING_AVERAGE_MINUTES));
    lv_label_set_text_fmt(big_number_, "%d", pm25);
  }

private:
  static const uint8_t MOVING_AVERAGE_MINUTES = 2;

  RingBuffer<uint16_t> *const pm25_values_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_AQI_BIG_NUMBER_H_
